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
#
#   # ctest filters / extra args (everything after a `--` is forwarded to ctest):
#   ./scripts/container-dev.sh 15 -- -R cpp23_       # run tests matching cpp23_
#   ./scripts/container-dev.sh 15 -- -L ranges       # run all 'ranges' topic tests
#   ./scripts/container-dev.sh 15 -- -j              # run in parallel when speed matters
#   ./scripts/container-dev.sh 15 -- -N              # list tests, don't run
#
#   # Engine and architecture selection:
#   ./scripts/container-dev.sh 15 engine=docker      # docker instead of podman
#   ./scripts/container-dev.sh 15 arch=arm64         # emulate linux/arm64
#   ./scripts/container-dev.sh 15 arch=amd64 engine=docker
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

GCC_VERSION="${1:-15}"
shift || true

case "${GCC_VERSION}" in
    13|14|15|16) ;;
    *)
        echo "error: unsupported GCC version '${GCC_VERSION}' (expected 13|14|15|16)" >&2
        exit 2
        ;;
esac

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
ENGINE="${CONTAINER_ENGINE:-podman}"
ARCH=""

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
        sanitize=*)      CMAKE_ARGS+=("-DGCC_FEATURE_SANITIZE=${arg#sanitize=}") ;;
        analyzer)        CMAKE_ARGS+=("-DGCC_FEATURE_ANALYZER=ON") ;;
        readme)          README_MODE=1 ;;
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
            echo "error: unknown argument '$arg' (expected engine=podman|docker / arch=… / sanitize=… / analyzer / readme / list / run NAME / show NAME / -- <ctest-args>)" >&2
            exit 2
            ;;
    esac
done

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

TAG="localhost/gcc-updates:gcc${GCC_VERSION}${ARCH_SUFFIX}"
BUILD_VOL="gcc-updates-build-gcc${GCC_VERSION}${ARCH_SUFFIX}"

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
    "${ENGINE}" build \
        ${PLATFORM_ARGS[@]:+"${PLATFORM_ARGS[@]}"} \
        --build-arg="GCC_VERSION=${GCC_VERSION}" \
        -f "${REPO_ROOT}/containers/gcc.Containerfile" \
        -t "${TAG}" \
        "${REPO_ROOT}"
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
        -v "${REPO_ROOT}:/work:${RO_OPTS}" \
        -v "${BUILD_VOL}:/build${VOL_OPTS}" \
        -w /work \
        "${TAG}" "$@"
}

echo "==> configuring (cmake -S /work -B /build ${CMAKE_ARGS[*]:-})"
# Reset every mode before applying this invocation's arguments. Build volumes
# persist between runs, so cached sanitizer/analyzer/readme modes must never
# leak from an earlier command.
# The ${arr[@]:+…} form keeps bash 3.2 (macOS) from treating the empty
# array as unbound under `set -u`.
run cmake -S /work -B /build \
    -DGCC_FEATURE_README=check \
    -DGCC_FEATURE_ANALYZER=OFF \
    -DGCC_FEATURE_SANITIZE= \
    ${CMAKE_ARGS[@]:+"${CMAKE_ARGS[@]}"}

if [ "${ACTION}" = list ]; then
    exec "${ENGINE}" run --rm \
        ${PLATFORM_ARGS[@]:+"${PLATFORM_ARGS[@]}"} \
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
        echo "==> building ${TARGET_NAME}"
        run cmake --build /build --target "${TARGET_NAME}" --parallel
    else
        echo "==> ${TARGET_NAME} compiles inside CTest (no executable target)"
    fi
else
    echo "==> building"
    run cmake --build /build --parallel
fi

echo "==> running ctest ${CTEST_ARGS[*]}"
exec "${ENGINE}" run --rm \
    ${PLATFORM_ARGS[@]:+"${PLATFORM_ARGS[@]}"} \
    -v "${REPO_ROOT}:/work:${RO_OPTS}" \
    -v "${BUILD_VOL}:/build${VOL_OPTS}" \
    -w /build \
    "${TAG}" \
    ctest "${CTEST_ARGS[@]}"
