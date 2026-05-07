#!/usr/bin/env bash
# Thin wrapper around scripts/discover.py.
# Used by CI and by scripts/podman-dev.sh.
set -euo pipefail
cd "$(dirname "$0")/.."
exec python3 scripts/discover.py "$@"
