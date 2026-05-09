# GCC extensions: ThreadSanitizer trip demos

_Folder: `features/gccext/sanitize/tsan/`. 1 topic(s). Index of examples; build metadata lives in `CMakeLists.txt` next to each `.cpp`. Update by hand when adding new examples._

## gcc-sanitize

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_tsan_data_race.cpp](gccext_tsan_data_race.cpp) | c++17 | 13 | TSan demo: two threads writing the same plain int without synchronisation. Runs only under -fsanitize=thread; reports a data race. |
