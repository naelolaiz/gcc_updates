#!/usr/bin/env bash
# Local-dev entrypoint. Builds (or reuses) a podman image based on the
# official gcc:${1:-15} Docker image, then runs cmake + ctest inside it
# against a read-only mount of the working tree (build dir lives in a
# named podman volume so it persists across runs and is writable).
#
# Usage:
#   ./scripts/podman-dev.sh                       # default gcc 15, run all tests
#   ./scripts/podman-dev.sh 14
#   ./scripts/podman-dev.sh 16
#
#   # ctest filters / extra args (everything after a `--` is forwarded to ctest):
#   ./scripts/podman-dev.sh 15 -- -R cpp23_       # run tests matching cpp23_
#   ./scripts/podman-dev.sh 15 -- -L ranges       # run all 'ranges' topic tests
#   ./scripts/podman-dev.sh 15 -- -j              # run in parallel when speed matters
#   ./scripts/podman-dev.sh 15 -- -N              # list tests, don't run
#
#   # CMake configure modes:
#   ./scripts/podman-dev.sh 15 sanitize=address,undefined
#   ./scripts/podman-dev.sh 15 sanitize=thread
#   ./scripts/podman-dev.sh 16 analyzer
#   ./scripts/podman-dev.sh 16 list
#   ./scripts/podman-dev.sh 16 run cpp23_expected
#   ./scripts/podman-dev.sh 16 show cpp23_expected
#
#   # Regenerate the per-bucket README.md indexes (configure-only, then
#   # copies the generated files back into features/):
#   ./scripts/podman-dev.sh 15 readme
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
TAG="localhost/gcc-updates:gcc${GCC_VERSION}"
BUILD_VOL="gcc-updates-build-gcc${GCC_VERSION}"

# Parse args. Anything before `--` is a configure mode (sanitize=… / analyzer);
# anything after `--` is forwarded as-is to ctest.
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
            echo "error: unknown argument '$arg' (expected sanitize=… / analyzer / readme / list / run NAME / show NAME / -- <ctest-args>)" >&2
            exit 2
            ;;
    esac
done

if [ "${ACTION}" = show ]; then
    mapfile -t matches < <(find "${REPO_ROOT}/features" -type f -name "${TARGET_NAME}.cpp" -print)
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

if ! podman image exists "${TAG}"; then
    echo "==> building ${TAG} (one-time, ~2-3 min)"
    podman build \
        --build-arg="GCC_VERSION=${GCC_VERSION}" \
        -f "${REPO_ROOT}/containers/gcc.Containerfile" \
        -t "${TAG}" \
        "${REPO_ROOT}"
fi

if [ "${README_MODE}" -eq 1 ]; then
    OUT_DIR="$(mktemp -d)"
    trap 'rm -rf "${OUT_DIR}"' EXIT
    echo "==> regenerating README indexes"
    podman run --rm \
        -v "${REPO_ROOT}:/work:ro,Z" \
        -v "${BUILD_VOL}:/build:Z" \
        -v "${OUT_DIR}:/readme-out:rw,Z" \
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
    podman run --rm \
        -v "${REPO_ROOT}:/work:ro,Z" \
        -v "${BUILD_VOL}:/build:Z" \
        -w /work \
        "${TAG}" "$@"
}

echo "==> configuring (cmake -S /work -B /build ${CMAKE_ARGS[*]:-})"
# Reset every mode before applying this invocation's arguments. Build volumes
# persist between runs, so cached sanitizer/analyzer/readme modes must never
# leak from an earlier command.
run cmake -S /work -B /build \
    -DGCC_FEATURE_README=check \
    -DGCC_FEATURE_ANALYZER=OFF \
    -DGCC_FEATURE_SANITIZE= \
    "${CMAKE_ARGS[@]}"

if [ "${ACTION}" = list ]; then
    exec podman run --rm \
        -v "${REPO_ROOT}:/work:ro,Z" \
        -v "${BUILD_VOL}:/build:Z" \
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
exec podman run --rm \
    -v "${REPO_ROOT}:/work:ro,Z" \
    -v "${BUILD_VOL}:/build:Z" \
    -w /build \
    "${TAG}" \
    ctest "${CTEST_ARGS[@]}"
