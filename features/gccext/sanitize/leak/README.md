# GCC extensions: LeakSanitizer trip demos

_Folder: `features/gccext/sanitize/leak/`. 1 topic(s). Index of examples; build metadata lives in `CMakeLists.txt` next to each `.cpp`. Update by hand when adding new examples._

## gcc-sanitize

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_lsan_simple_leak.cpp](gccext_lsan_simple_leak.cpp) | c++17 | 13 | LSan demo: heap allocation with no matching free. Runs only when ASan or LSan is active; reports leaks at exit. |
