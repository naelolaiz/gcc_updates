# GCC extensions: UndefinedBehaviorSanitizer trip demos

_Folder: `features/gccext/sanitize/ubsan/`. 3 example(s) across 1 topic(s). Index of examples; build metadata lives in `CMakeLists.txt` next to each `.cpp`. Update by hand when adding new examples._

## gcc-sanitize

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_ubsan_null_deref.cpp](gccext_ubsan_null_deref.cpp) | c++17 | 13 | UBSan demo: dereferencing a null pointer. Runs only under -fsanitize=undefined; reports the null deref and aborts. |
| [gccext_ubsan_shift.cpp](gccext_ubsan_shift.cpp) | c++17 | 13 | UBSan demo: shifting an int by a count >= the type's width is UB. Runs only under -fsanitize=undefined. |
| [gccext_ubsan_signed_overflow.cpp](gccext_ubsan_signed_overflow.cpp) | c++17 | 13 | UBSan demo: signed integer overflow. Runs only under -fsanitize=undefined; -fno-sanitize-recover=undefined makes the handler fatal (exit 1). |
