# GCC extensions: ThreadSanitizer trip demos

_Folder: `features/gccext/sanitize/tsan/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [gcc-sanitize](#gcc-sanitize)

## gcc-sanitize

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_tsan_data_race.cpp](gccext_tsan_data_race.cpp) | c++17 | GCC >= 13 | covered | TSan demo: two threads writing the same plain int without synchronisation. Runs only under -fsanitize=thread; reports a data race. |
