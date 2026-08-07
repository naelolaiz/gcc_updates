# GCC extensions: UndefinedBehaviorSanitizer trip demos

_Folder: `features/gccext/sanitize/ubsan/`. 5 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [diagnostics](#diagnostics)
- [gcc-sanitize](#gcc-sanitize)
- [memory-safety](#memory-safety)
- [numerics](#numerics)
- [object-model](#object-model)

## diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_ubsan_divide_by_zero.cpp](gccext_ubsan_divide_by_zero.cpp) | c++17 | GCC >= 13 | covered | UndefinedBehaviorSanitizer diagnoses integer division by zero before the machine instruction traps. |
| [gccext_ubsan_invalid_vptr.cpp](gccext_ubsan_invalid_vptr.cpp) | c++17 | GCC >= 13 | covered | UBSan's vptr check diagnoses a virtual call through storage that does not contain an object of the claimed dynamic type. |
| [gccext_ubsan_misaligned_access.cpp](gccext_ubsan_misaligned_access.cpp) | c++17 | GCC >= 13 | covered | UndefinedBehaviorSanitizer diagnoses a typed load through an address that does not meet the type's alignment requirement. |

## gcc-sanitize

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_ubsan_divide_by_zero.cpp](gccext_ubsan_divide_by_zero.cpp) | c++17 | GCC >= 13 | covered | UndefinedBehaviorSanitizer diagnoses integer division by zero before the machine instruction traps. |
| [gccext_ubsan_invalid_vptr.cpp](gccext_ubsan_invalid_vptr.cpp) | c++17 | GCC >= 13 | covered | UBSan's vptr check diagnoses a virtual call through storage that does not contain an object of the claimed dynamic type. |
| [gccext_ubsan_misaligned_access.cpp](gccext_ubsan_misaligned_access.cpp) | c++17 | GCC >= 13 | covered | UndefinedBehaviorSanitizer diagnoses a typed load through an address that does not meet the type's alignment requirement. |
| [gccext_ubsan_null_deref.cpp](gccext_ubsan_null_deref.cpp) | c++17 | GCC >= 13 | covered | UBSan demo: dereferencing a null pointer. Runs only under -fsanitize=undefined; reports the null deref and aborts. |
| [gccext_ubsan_shift.cpp](gccext_ubsan_shift.cpp) | c++17 | GCC >= 13 | covered | UBSan demo: shifting an int by a count >= the type's width is UB. Runs only under -fsanitize=undefined. |
| [gccext_ubsan_signed_overflow.cpp](gccext_ubsan_signed_overflow.cpp) | c++17 | GCC >= 13 | covered | UBSan demo: signed integer overflow. Runs only under -fsanitize=undefined; -fno-sanitize-recover=undefined makes the handler fatal (exit 1). |

## memory-safety

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_ubsan_misaligned_access.cpp](gccext_ubsan_misaligned_access.cpp) | c++17 | GCC >= 13 | covered | UndefinedBehaviorSanitizer diagnoses a typed load through an address that does not meet the type's alignment requirement. |

## numerics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_ubsan_divide_by_zero.cpp](gccext_ubsan_divide_by_zero.cpp) | c++17 | GCC >= 13 | covered | UndefinedBehaviorSanitizer diagnoses integer division by zero before the machine instruction traps. |

## object-model

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_ubsan_invalid_vptr.cpp](gccext_ubsan_invalid_vptr.cpp) | c++17 | GCC >= 13 | covered | UBSan's vptr check diagnoses a virtual call through storage that does not contain an object of the claimed dynamic type. |
