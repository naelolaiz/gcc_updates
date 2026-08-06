# GCC extensions: LeakSanitizer trip demos

_Folder: `features/gccext/sanitize/leak/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [gcc-sanitize](#gcc-sanitize)

## gcc-sanitize

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_lsan_simple_leak.cpp](gccext_lsan_simple_leak.cpp) | c++17 | GCC >= 13 | covered | LSan demo: heap allocation with no matching free. Runs only when ASan or LSan is active; reports leaks at exit. |
