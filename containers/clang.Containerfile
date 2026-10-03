# Clang cross-check image. Layers clang-${CLANG_VERSION} from apt.llvm.org
# onto the official gcc:${GCC_VERSION} Docker image, so the C++ standard
# library stays the same pinned libstdc++ the GCC lanes use and only the
# compiler front-end changes. The recipe lives in containers/install-clang.sh,
# which CI runs inside the same base image, so local and CI behaviour match.
#
# Build:
#   podman build -f containers/clang.Containerfile -t gcc-updates:clang23 .
ARG GCC_VERSION=16
FROM gcc:${GCC_VERSION}
ARG CLANG_VERSION=23
COPY containers/install-clang.sh /tmp/install-clang.sh
RUN bash /tmp/install-clang.sh "${CLANG_VERSION}" \
 && rm -rf /var/lib/apt/lists/* /tmp/install-clang.sh
WORKDIR /work
