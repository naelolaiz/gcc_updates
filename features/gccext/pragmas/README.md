# GCC extensions: diagnostic pragmas

_Folder: `features/gccext/pragmas/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [gcc-pragmas](#gcc-pragmas)

## gcc-pragmas

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_diagnostic_pragma.cpp](gccext_diagnostic_pragma.cpp) | c++17 | GCC >= 13 | covered | '#pragma GCC diagnostic push/pop/ignored' silences a warning locally without -Wno-* polluting the rest of the file. |
