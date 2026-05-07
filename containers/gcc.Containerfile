# Local-dev image. Layers libtbb-dev + python3 onto the official gcc:N
# Docker image (Debian-based) so behaviour is bit-identical to CI, which
# runs every job inside the same gcc:${GCC_VERSION} image.
#
# Build:
#   podman build --build-arg=GCC_VERSION=15 -t gcc-updates:gcc15 .
ARG GCC_VERSION=15
FROM gcc:${GCC_VERSION}
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
      libtbb-dev \
      python3 \
 && rm -rf /var/lib/apt/lists/*
WORKDIR /work
