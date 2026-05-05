#!/usr/bin/env bash
# Local-dev entrypoint. Builds (or reuses) a podman image with gcc-${1:-15}
# from ppa:ubuntu-toolchain-r/test, then runs scripts/discover.py inside it
# against a read-only mount of the working tree.
#
# Usage:
#   ./scripts/podman-dev.sh              # default GCC 15
#   ./scripts/podman-dev.sh 14
#   ./scripts/podman-dev.sh 16 --dry-run
#   ./scripts/podman-dev.sh 15 --filter=ranges
#
# Per project policy, this is the ONLY supported way to invoke the toolchain
# locally — never run g++ directly on the host.
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

if ! podman image exists "${TAG}"; then
    echo "==> building ${TAG} (one-time, ~2-3 min)"
    podman build \
        --build-arg="GCC_VERSION=${GCC_VERSION}" \
        -f "${REPO_ROOT}/containers/gcc.Containerfile" \
        -t "${TAG}" \
        "${REPO_ROOT}"
fi

echo "==> running discover.py inside ${TAG}"
exec podman run --rm \
    -v "${REPO_ROOT}:/work:ro,Z" \
    -w /work \
    "${TAG}" \
    python3 scripts/discover.py --gcc-version="${GCC_VERSION}" "$@"
