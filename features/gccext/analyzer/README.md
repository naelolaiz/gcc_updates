# GCC extensions: -fanalyzer compile-time demos

_Folder: `features/gccext/analyzer/`. 3 example(s) across 1 topic(s). Index of examples; build metadata lives in `CMakeLists.txt` next to each `.cpp`. Update by hand when adding new examples._

## gcc-analyzer

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_analyzer_double_free.cpp](gccext_analyzer_double_free.cpp) | c++17 | 16 | -fanalyzer flags a double-delete that only fires on a control-flow branch the runtime might never hit; runtime sanitizers can miss this if the test inputs don't reach that path. |
| [gccext_analyzer_null_deref.cpp](gccext_analyzer_null_deref.cpp) | c++17 | 16 | -fanalyzer follows the path where a function may return nullptr and is then dereferenced unconditionally; demonstrates path-sensitivity that lint checkers miss. |
| [gccext_analyzer_use_after_free.cpp](gccext_analyzer_use_after_free.cpp) | c++17 | 16 | -fanalyzer warns at compile time about use-after-delete on a path through main(). GCC 16 is the first version where this works on C++ (NRVO + initial exception support). |
