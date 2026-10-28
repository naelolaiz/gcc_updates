#!/usr/bin/env bash
# Local-dev entrypoint. Builds (or reuses) a container image based on the
# official gcc:${1:-15} Docker image, then runs cmake + ctest inside it
# against a read-only mount of the working tree (build dir lives in a
# named volume so it persists across runs and is writable).
#
# Works with podman (default) or docker; select with engine=docker or the
# CONTAINER_ENGINE environment variable.
#
# Usage:
#   ./scripts/container-dev.sh                       # default gcc 15, run all tests
#   ./scripts/container-dev.sh 14
#   ./scripts/container-dev.sh 16
#   ./scripts/container-dev.sh clang23               # clang-23 cross-check lane
#
#   # ctest filters / extra args (everything after a `--` is forwarded to ctest):
#   ./scripts/container-dev.sh 15 -- -R cpp23_       # run tests matching cpp23_
#   ./scripts/container-dev.sh 15 -- -L ranges       # run all 'ranges' topic tests
#   ./scripts/container-dev.sh 15 -- -j4             # run CTest with 4 parallel jobs
#   ./scripts/container-dev.sh 15 -- -N              # list tests, don't run
#
#   # Engine, architecture, and resource limits:
#   ./scripts/container-dev.sh 15 engine=docker      # docker instead of podman
#   ./scripts/container-dev.sh 15 arch=arm64         # emulate linux/arm64
#   ./scripts/container-dev.sh 15 jobs=4             # build parallelism (default:JOBS=2)
#   ./scripts/container-dev.sh 15 cpus=2 memory=4g   # cap container CPU/RAM
#   ./scripts/container-dev.sh 15 clean              # remove cached build volume
#
#   A non-native arch needs an engine able to emulate it (qemu binfmt;
#   Docker Desktop and podman machine ship it, bare-Linux hosts need
#   qemu-user-static registered). Images and build volumes are suffixed
#   per arch, so native and emulated builds never share object files.
#
#   # CMake configure modes:
#   ./scripts/container-dev.sh 15 sanitize=address,undefined
#   ./scripts/container-dev.sh 15 sanitize=thread
#   ./scripts/container-dev.sh 16 analyzer
#   ./scripts/container-dev.sh 16 list
#   ./scripts/container-dev.sh 16 run cpp23_expected
#   ./scripts/container-dev.sh 16 show cpp23_expected
#
#   # Regenerate the per-bucket README.md indexes (configure-only, then
#   # copies the generated files back into features/):
#   ./scripts/container-dev.sh 15 readme
#
# Per project policy, this is the ONLY supported way to invoke the toolchain
# locally — never run g++/cmake directly on the host.
set -euo pipefail

TOOLCHAIN="${1:-15}"
shift || true

case "${TOOLCHAIN}" in
    13|14|15|16)
        COMPILER=gcc
        GCC_VERSION="${TOOLCHAIN}"
        LANE="gcc${TOOLCHAIN}"
        ;;
    clang23)
        # Cross-check lane: clang front-end over the gcc:16 image's pinned
        # libstdc++ (see containers/clang.Containerfile).
        COMPILER=clang
        CLANG_VERSION="${TOOLCHAIN#clang}"
        LANE="${TOOLCHAIN}"
        ;;
    *)
        echo "error: unsupported toolchain '${TOOLCHAIN}' (expected 13|14|15|16|clang23)" >&2
        exit 2
        ;;
esac

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
ENGINE="${CONTAINER_ENGINE:-podman}"
ARCH=""
JOBS="${JOBS:-2}"
CPUS="${CONTAINER_CPUS:-}"
MEMORY="${CONTAINER_MEMORY:-}"
SANITIZE_SPEC=""

# Parse args. Anything before `--` is an option (engine=… / arch=… /
# sanitize=… / analyzer / …); anything after `--` is forwarded as-is to ctest.
CMAKE_ARGS=()
CTEST_ARGS=(--verbose)
README_MODE=0
ACTION=test
TARGET_NAME=""
seen_dashdash=0
while [ "$#" -gt 0 ]; do
    arg="$1"
    shift
    if [ "$seen_dashdash" -eq 1 ]; then
        CTEST_ARGS+=("$arg")
        continue
    fi
    case "$arg" in
        --)              seen_dashdash=1 ;;
        engine=*)        ENGINE="${arg#engine=}" ;;
        arch=*)          ARCH="${arg#arch=}" ;;
        jobs=*)          JOBS="${arg#jobs=}" ;;
        cpus=*)          CPUS="${arg#cpus=}" ;;
        memory=*)        MEMORY="${arg#memory=}" ;;
        sanitize=*)
            SANITIZE_SPEC="${arg#sanitize=}"
            CMAKE_ARGS+=("-DGCC_FEATURE_SANITIZE=${SANITIZE_SPEC}")
            ;;
        analyzer)        CMAKE_ARGS+=("-DGCC_FEATURE_ANALYZER=ON") ;;
        readme)          README_MODE=1 ;;
        clean)           ACTION=clean ;;
        list)            ACTION=list ;;
        run|show)
            if [ "$#" -eq 0 ]; then
                echo "error: '${arg}' requires an example name" >&2
                exit 2
            fi
            ACTION="$arg"
            TARGET_NAME="$1"
            shift
            ;;
        *)
            echo "error: unknown argument '$arg' (expected engine=podman|docker / arch=… / jobs=N / cpus=N / memory=… / sanitize=… / analyzer / readme / clean / list / run NAME / show NAME / -- <ctest-args>)" >&2
            exit 2
            ;;
    esac
done

case "${JOBS}" in
    ''|0|*[!0-9]*)
        echo "error: invalid jobs='${JOBS}' (expected a positive integer)" >&2
        exit 2
        ;;
esac

case "${ENGINE}" in
    podman|docker) ;;
    *)
        echo "error: unsupported engine '${ENGINE}' (expected podman or docker)" >&2
        exit 2
        ;;
esac

# SELinux relabel flags are podman idiom; docker (notably Docker Desktop)
# does not want them on named volumes.
if [ "${ENGINE}" = podman ]; then
    RO_OPTS="ro,Z"
    VOL_OPTS=":Z"
    OUT_OPTS="rw,Z"
else
    RO_OPTS="ro"
    VOL_OPTS=""
    OUT_OPTS="rw"
fi

# Non-native arch: pass --platform through, and keep per-arch images and
# build volumes apart (native names stay unchanged for compatibility).
PLATFORM_ARGS=()
ARCH_SUFFIX=""
if [ -n "${ARCH}" ]; then
    case "${ARCH}" in
        *[!a-z0-9/]*)
            echo "error: invalid arch '${ARCH}' (e.g. amd64, arm64, riscv64)" >&2
            exit 2
            ;;
    esac
    PLATFORM_ARGS=(--platform "linux/${ARCH}")
    ARCH_SUFFIX="-$(printf '%s' "${ARCH}" | tr '/' '-')"
fi

TAG="localhost/gcc-updates:${LANE}${ARCH_SUFFIX}"
BUILD_VOL="gcc-updates-build-${LANE}${ARCH_SUFFIX}"

RESOURCE_ARGS=()
if [ -n "${CPUS}" ]; then
    RESOURCE_ARGS+=(--cpus "${CPUS}")
fi
if [ -n "${MEMORY}" ]; then
    RESOURCE_ARGS+=(--memory "${MEMORY}")
fi

SANITIZE_ENV_ARGS=()
if [ -n "${SANITIZE_SPEC}" ]; then
    SANITIZE_ENV_ARGS=(
        -e "ASAN_OPTIONS=${ASAN_OPTIONS:-halt_on_error=1:abort_on_error=0:detect_leaks=1:strict_string_checks=1}"
        -e "UBSAN_OPTIONS=${UBSAN_OPTIONS:-halt_on_error=1:print_stacktrace=1}"
        -e "LSAN_OPTIONS=${LSAN_OPTIONS:-exitcode=1}"
        -e "TSAN_OPTIONS=${TSAN_OPTIONS:-halt_on_error=1:exitcode=66}"
    )
fi

if [ "${ACTION}" = clean ]; then
    echo "==> removing build volume ${BUILD_VOL}"
    "${ENGINE}" volume rm -f "${BUILD_VOL}" >/dev/null
    exit 0
fi

# The clang lane picks its compiler explicitly; gcc lanes use the image default.
CMAKE_COMPILER_ARGS=()
if [ "${COMPILER}" = clang ]; then
    CMAKE_COMPILER_ARGS=("-DCMAKE_CXX_COMPILER=clang++-${CLANG_VERSION}")
fi

if [ "${ACTION}" = show ]; then
    # No mapfile: macOS ships bash 3.2, which lacks it.
    matches=()
    while IFS= read -r match; do
        matches+=("${match}")
    done < <(find "${REPO_ROOT}/features" -type f -name "${TARGET_NAME}.cpp" -print)
    if [ "${#matches[@]}" -ne 1 ]; then
        echo "error: expected one source named '${TARGET_NAME}.cpp', found ${#matches[@]}" >&2
        exit 2
    fi
    source_file="${matches[0]}"
    cmake_file="$(dirname "${source_file}")/CMakeLists.txt"
    echo "==> ${source_file#${REPO_ROOT}/}"
    sed -n '1,220p' "${source_file}"
    echo
    echo "==> registration in ${cmake_file#${REPO_ROOT}/}"
    grep -n -A8 -F "gcc_feature_test(${TARGET_NAME}" "${cmake_file}" || true
    exit 0
fi

if [ "${ACTION}" = run ]; then
    case "${TARGET_NAME}" in
        *[!A-Za-z0-9_]*)
            echo "error: invalid example name '${TARGET_NAME}'" >&2
            exit 2
            ;;
    esac
    CTEST_ARGS+=(--no-tests=error -R "^${TARGET_NAME}(_compile_diagnostic)?$")
fi

# `image exists` is podman-only; `image inspect` works on both engines.
if ! "${ENGINE}" image inspect "${TAG}" >/dev/null 2>&1; then
    echo "==> building ${TAG} (one-time, ~2-3 min)"
    if [ "${COMPILER}" = clang ]; then
        "${ENGINE}" build \
            ${PLATFORM_ARGS[@]:+"${PLATFORM_ARGS[@]}"} \
            --build-arg="CLANG_VERSION=${CLANG_VERSION}" \
            -f "${REPO_ROOT}/containers/clang.Containerfile" \
            -t "${TAG}" \
            "${REPO_ROOT}"
    else
        "${ENGINE}" build \
            ${PLATFORM_ARGS[@]:+"${PLATFORM_ARGS[@]}"} \
            --build-arg="GCC_VERSION=${GCC_VERSION}" \
            -f "${REPO_ROOT}/containers/gcc.Containerfile" \
            -t "${TAG}" \
            "${REPO_ROOT}"
    fi
fi

if [ "${README_MODE}" -eq 1 ]; then
    OUT_DIR="$(mktemp -d)"
    trap 'rm -rf "${OUT_DIR}"' EXIT
    echo "==> regenerating README indexes"
    "${ENGINE}" run --rm \
        ${PLATFORM_ARGS[@]:+"${PLATFORM_ARGS[@]}"} \
        -v "${REPO_ROOT}:/work:${RO_OPTS}" \
        -v "${BUILD_VOL}:/build${VOL_OPTS}" \
        -v "${OUT_DIR}:/readme-out:${OUT_OPTS}" \
        -w /work \
        "${TAG}" cmake -S /work -B /build \
        ${CMAKE_COMPILER_ARGS[@]:+"${CMAKE_COMPILER_ARGS[@]}"} \
        -DGCC_FEATURE_README=write -DGCC_FEATURE_README_OUT=/readme-out \
        -DGCC_FEATURE_ANALYZER=OFF -DGCC_FEATURE_SANITIZE=
    cp -R "${OUT_DIR}/features/." "${REPO_ROOT}/features/"
    cp "${OUT_DIR}/coverage.yml" "${REPO_ROOT}/coverage.yml"
    echo "==> README indexes updated under features/"
    exit 0
fi

run() {
    "${ENGINE}" run --rm \
        ${PLATFORM_ARGS[@]:+"${PLATFORM_ARGS[@]}"} \
        ${RESOURCE_ARGS[@]:+"${RESOURCE_ARGS[@]}"} \
        ${SANITIZE_ENV_ARGS[@]:+"${SANITIZE_ENV_ARGS[@]}"} \
        -e "CMAKE_BUILD_PARALLEL_LEVEL=${JOBS}" \
        -v "${REPO_ROOT}:/work:${RO_OPTS}" \
        -v "${BUILD_VOL}:/build${VOL_OPTS}" \
        -w /work \
        "${TAG}" "$@"
}

# If the persistent build volume holds a CMakeCache.txt whose compiler path no
# longer exists inside the container (e.g. after a base-image upgrade), clear
# the cache before configuring.
run bash -c '
    if [ -f /build/CMakeCache.txt ]; then
        cached_cxx="$(sed -n "s/^CMAKE_CXX_COMPILER:FILEPATH=//p" /build/CMakeCache.txt | head -n 1)"
        if [ -n "${cached_cxx}" ] && [ ! -x "${cached_cxx}" ]; then
            echo "==> clearing stale CMake cache (missing ${cached_cxx})"
            rm -rf /build/CMakeCache.txt /build/CMakeFiles
        fi
    fi
'

echo "==> configuring (cmake -S /work -B /build ${CMAKE_ARGS[*]:-})"
# Reset every mode before applying this invocation's arguments. Build volumes
# persist between runs, so cached sanitizer/analyzer/readme modes must never
# leak from an earlier command.
# The ${arr[@]:+…} form keeps bash 3.2 (macOS) from treating the empty
# array as unbound under `set -u`.
run cmake -S /work -B /build \
    ${CMAKE_COMPILER_ARGS[@]:+"${CMAKE_COMPILER_ARGS[@]}"} \
    -DGCC_FEATURE_README=check \
    -DGCC_FEATURE_ANALYZER=OFF \
    -DGCC_FEATURE_SANITIZE= \
    ${CMAKE_ARGS[@]:+"${CMAKE_ARGS[@]}"}

if [ "${ACTION}" = list ]; then
    exec "${ENGINE}" run --rm \
        ${PLATFORM_ARGS[@]:+"${PLATFORM_ARGS[@]}"} \
        ${RESOURCE_ARGS[@]:+"${RESOURCE_ARGS[@]}"} \
        -v "${REPO_ROOT}:/work:${RO_OPTS}" \
        -v "${BUILD_VOL}:/build${VOL_OPTS}" \
        -w /build \
        "${TAG}" \
        ctest -N "${CTEST_ARGS[@]:1}"
fi

if [ "${ACTION}" = run ]; then
    # Runtime examples have a build target with the example name. Diagnostic,
    # module, analyzer, and compile-only modes compile inside CTest instead.
    target_help="$(run cmake --build /build --target help)"
    has_target=0
    while IFS= read -r target_line; do
        if [ "${target_line}" = "... ${TARGET_NAME}" ]; then
            has_target=1
            break
        fi
    done <<< "${target_help}"
    if [ "${has_target}" -eq 1 ]; then
        echo "==> building ${TARGET_NAME} (jobs=${JOBS})"
        run cmake --build /build --target "${TARGET_NAME}" --parallel "${JOBS}"
    else
        echo "==> ${TARGET_NAME} compiles inside CTest (no executable target)"
    fi
else
    echo "==> building (jobs=${JOBS})"
    run cmake --build /build --parallel "${JOBS}"
fi

echo "==> running ctest ${CTEST_ARGS[*]}"
exec "${ENGINE}" run --rm \
    ${PLATFORM_ARGS[@]:+"${PLATFORM_ARGS[@]}"} \
    ${RESOURCE_ARGS[@]:+"${RESOURCE_ARGS[@]}"} \
    ${SANITIZE_ENV_ARGS[@]:+"${SANITIZE_ENV_ARGS[@]}"} \
    -v "${REPO_ROOT}:/work:${RO_OPTS}" \
    -v "${BUILD_VOL}:/build${VOL_OPTS}" \
    -w /build \
    "${TAG}" \
    ctest "${CTEST_ARGS[@]}"
