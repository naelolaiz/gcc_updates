# Clang cross-check image. Layers clang-${CLANG_VERSION} from apt.llvm.org
# onto the official gcc:${GCC_VERSION} Docker image, so the C++ standard
# library stays the same pinned libstdc++ the GCC lanes use and only the
# compiler front-end changes. CI installs the same packages into the same
# base image, so local and CI behaviour match.
#
# Build:
#   podman build -f containers/clang.Containerfile -t gcc-updates:clang22 .
ARG GCC_VERSION=16
FROM gcc:${GCC_VERSION}
ARG CLANG_VERSION=22
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
 && apt-get install -y --no-install-recommends cmake libtbb-dev gnupg \
 && curl -fsSL https://apt.llvm.org/llvm-snapshot.gpg.key \
    | gpg --dearmor -o /usr/share/keyrings/llvm.gpg \
 && . /etc/os-release \
 && echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] https://apt.llvm.org/${VERSION_CODENAME}/ llvm-toolchain-${VERSION_CODENAME}-${CLANG_VERSION} main" \
    > /etc/apt/sources.list.d/llvm.list \
 && apt-get update \
 && apt-get install -y --no-install-recommends \
      clang-${CLANG_VERSION} \
      libomp-${CLANG_VERSION}-dev \
 && rm -rf /var/lib/apt/lists/* \
 # Clang's GCC-toolchain autodetection only scans /usr, so it would pair with
 # the Debian base's libstdc++ instead of the image's upstream one under
 # /usr/local. The default config file pins the /usr/local toolchain for every
 # clang++ invocation (headers and runtime alike).
 && echo "--gcc-install-dir=$(ls -d /usr/local/lib/gcc/*/*)" \
    > /usr/lib/llvm-${CLANG_VERSION}/bin/clang++.cfg
WORKDIR /work
