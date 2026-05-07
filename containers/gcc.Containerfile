# Builds a small image with gcc-${GCC_VERSION} and python3 installed from the
# ubuntu-toolchain-r/test PPA. Same toolchain source as CI, so local and CI
# results are bit-identical.
#
# Build:
#   podman build --build-arg=GCC_VERSION=15 -t gcc-updates:gcc15 .
FROM ubuntu:24.04
ARG GCC_VERSION=15
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
      software-properties-common \
      gnupg \
      ca-certificates \
      python3 \
 && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
 && apt-get update \
 && apt-get install -y --no-install-recommends \
      "gcc-${GCC_VERSION}" \
      "g++-${GCC_VERSION}" \
      libtbb-dev \
 && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 100 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION} 100 \
 && rm -rf /var/lib/apt/lists/*
WORKDIR /work
