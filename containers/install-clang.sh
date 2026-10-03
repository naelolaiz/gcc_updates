#!/usr/bin/env bash
# Installs clang-<major> from apt.llvm.org into an official gcc:N image and
# pins it to that image's upstream libstdc++, so the C++ standard library
# stays the one the GCC lanes use and only the compiler front-end changes.
#
# This is the single install recipe for the clang cross-check lane:
# containers/clang.Containerfile and the clang jobs in
# .github/workflows/ci.yml both run it, so local and CI behaviour match.
#
# Usage (as root, inside a gcc:N container):
#   bash containers/install-clang.sh <clang-major>
set -euo pipefail

CLANG_VERSION="${1:?usage: install-clang.sh <clang-major>}"

export DEBIAN_FRONTEND=noninteractive
apt-get update
apt-get install -y --no-install-recommends cmake libtbb-dev gnupg
curl -fsSL https://apt.llvm.org/llvm-snapshot.gpg.key \
    | gpg --dearmor --yes -o /usr/share/keyrings/llvm.gpg
. /etc/os-release
echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] https://apt.llvm.org/${VERSION_CODENAME}/ llvm-toolchain-${VERSION_CODENAME}-${CLANG_VERSION} main" \
    > /etc/apt/sources.list.d/llvm.list
apt-get update
apt-get install -y --no-install-recommends \
    "clang-${CLANG_VERSION}" \
    "libomp-${CLANG_VERSION}-dev" \
    "libclang-rt-${CLANG_VERSION}-dev"

# Clang's GCC-toolchain autodetection only scans /usr, so it would pair with
# the Debian base's libstdc++ instead of the image's upstream one under
# /usr/local. The default config file pins the /usr/local toolchain for every
# clang++ invocation (headers and runtime alike).
gcc_dirs=(/usr/local/lib/gcc/*/*)
if [ "${#gcc_dirs[@]}" -ne 1 ] || [ ! -d "${gcc_dirs[0]}" ]; then
    echo "error: expected exactly one GCC installation under /usr/local/lib/gcc, found: ${gcc_dirs[*]}" >&2
    exit 1
fi
echo "--gcc-install-dir=${gcc_dirs[0]}" \
    > "/usr/lib/llvm-${CLANG_VERSION}/bin/clang++.cfg"

# The pin is the lane's whole premise, so prove it: clang must resolve the
# same libstdc++ release as the image's own g++. Falling back to the Debian
# library would not fail the build -- it would only un-register every
# MIN_LIBSTDCXX-gated example and leave the lane green.
libstdcxx_release() {
    printf '#include <version>\n_GLIBCXX_RELEASE\n' | "$1" -x c++ -E -P - | tail -n 1
}
expected="$(libstdcxx_release g++)"
actual="$(libstdcxx_release "clang++-${CLANG_VERSION}")"
"clang++-${CLANG_VERSION}" --version
echo "libstdc++ release: g++ ${expected}, clang++-${CLANG_VERSION} ${actual}"
if [ -z "${expected}" ] || [ "${actual}" != "${expected}" ]; then
    echo "error: clang++-${CLANG_VERSION} is not using the image's libstdc++ ${expected}" >&2
    exit 1
fi
