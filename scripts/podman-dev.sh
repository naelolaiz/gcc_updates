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

if ! podman image exists "${TAG}"; then
    echo "==> building ${TAG} (one-time, ~2-3 min)"
    podman build \
        --build-arg="GCC_VERSION=${GCC_VERSION}" \
        -f "${REPO_ROOT}/containers/gcc.Containerfile" \
        -t "${TAG}" \
        "${REPO_ROOT}"
fi

# Parse args. Anything before `--` is a configure mode (sanitize=… / analyzer);
# anything after `--` is forwarded as-is to ctest.
CMAKE_ARGS=()
CTEST_ARGS=(--verbose)
README_MODE=0
seen_dashdash=0
for arg in "$@"; do
    if [ "$seen_dashdash" -eq 1 ]; then
        CTEST_ARGS+=("$arg")
        continue
    fi
    case "$arg" in
        --)              seen_dashdash=1 ;;
        sanitize=*)      CMAKE_ARGS+=("-DGCC_FEATURE_SANITIZE=${arg#sanitize=}") ;;
        analyzer)        CMAKE_ARGS+=("-DGCC_FEATURE_ANALYZER=ON") ;;
        readme)          README_MODE=1 ;;
        *)
            echo "error: unknown argument '$arg' (expected sanitize=… / analyzer / readme / -- <ctest-args>)" >&2
            exit 2
            ;;
    esac
done

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
        -DGCC_FEATURE_README=write -DGCC_FEATURE_README_OUT=/readme-out
    cp -R "${OUT_DIR}/features/." "${REPO_ROOT}/features/"
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
# -DGCC_FEATURE_README=check explicitly, so a cached =write from a previous
# `readme` run can never leak into a normal build (the tree is mounted ro).
run cmake -S /work -B /build -DGCC_FEATURE_README=check "${CMAKE_ARGS[@]}"

echo "==> building"
run cmake --build /build --parallel

echo "==> running ctest ${CTEST_ARGS[*]}"
exec podman run --rm \
    -v "${REPO_ROOT}:/work:ro,Z" \
    -v "${BUILD_VOL}:/build:Z" \
    -w /build \
    "${TAG}" \
    ctest "${CTEST_ARGS[@]}"
