# GCC extensions: diagnostic pragmas

_Folder: `features/gccext/pragmas/`. 1 topic(s). Index of examples; build metadata lives in `CMakeLists.txt` next to each `.cpp`. Update by hand when adding new examples._

## gcc-pragmas

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_diagnostic_pragma.cpp](gccext_diagnostic_pragma.cpp) | c++17 | 13 | '#pragma GCC diagnostic push/pop/ignored' silences a warning locally without -Wno-* polluting the rest of the file. |
