# GCC extensions: AddressSanitizer trip demos

_Folder: `features/gccext/sanitize/asan/`. 5 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [diagnostics](#diagnostics)
- [essential](#essential)
- [gcc-sanitize](#gcc-sanitize)
- [lifetimes](#lifetimes)
- [memory-safety](#memory-safety)

## diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_asan_global_buffer_overflow.cpp](gccext_asan_global_buffer_overflow.cpp) | c++17 | GCC >= 13 | covered | AddressSanitizer detects an out-of-bounds write past a global array and identifies the global-buffer-overflow class. |
| [gccext_asan_heap_use_after_free.cpp](gccext_asan_heap_use_after_free.cpp) | c++17 | GCC >= 13 | covered | ASan demo: read after free. Runs only under -fsanitize=address; ASan reports 'heap-use-after-free' and aborts. |
| [gccext_asan_stack_use_after_scope.cpp](gccext_asan_stack_use_after_scope.cpp) | c++17 | GCC >= 13 | covered | AddressSanitizer's use-after-scope instrumentation catches a pointer used after the local object it referred to left scope. |

## essential

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_asan_heap_use_after_free.cpp](gccext_asan_heap_use_after_free.cpp) | c++17 | GCC >= 13 | covered | ASan demo: read after free. Runs only under -fsanitize=address; ASan reports 'heap-use-after-free' and aborts. |

## gcc-sanitize

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_asan_double_free.cpp](gccext_asan_double_free.cpp) | c++17 | GCC >= 13 | covered | ASan demo: freeing the same pointer twice. Runs only under -fsanitize=address; ASan reports 'attempting double-free'. |
| [gccext_asan_global_buffer_overflow.cpp](gccext_asan_global_buffer_overflow.cpp) | c++17 | GCC >= 13 | covered | AddressSanitizer detects an out-of-bounds write past a global array and identifies the global-buffer-overflow class. |
| [gccext_asan_heap_buffer_overflow.cpp](gccext_asan_heap_buffer_overflow.cpp) | c++17 | GCC >= 13 | covered | ASan demo: writing one past the end of a heap buffer. Runs only under -fsanitize=address; ASan reports 'heap-buffer-overflow'. |
| [gccext_asan_heap_use_after_free.cpp](gccext_asan_heap_use_after_free.cpp) | c++17 | GCC >= 13 | covered | ASan demo: read after free. Runs only under -fsanitize=address; ASan reports 'heap-use-after-free' and aborts. |
| [gccext_asan_stack_buffer_overflow.cpp](gccext_asan_stack_buffer_overflow.cpp) | c++17 | GCC >= 13 | covered | ASan demo: writing past the end of a stack array. Runs only under -fsanitize=address; ASan reports 'stack-buffer-overflow'. |
| [gccext_asan_stack_use_after_scope.cpp](gccext_asan_stack_use_after_scope.cpp) | c++17 | GCC >= 13 | covered | AddressSanitizer's use-after-scope instrumentation catches a pointer used after the local object it referred to left scope. |

## lifetimes

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_asan_stack_use_after_scope.cpp](gccext_asan_stack_use_after_scope.cpp) | c++17 | GCC >= 13 | covered | AddressSanitizer's use-after-scope instrumentation catches a pointer used after the local object it referred to left scope. |

## memory-safety

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_asan_global_buffer_overflow.cpp](gccext_asan_global_buffer_overflow.cpp) | c++17 | GCC >= 13 | covered | AddressSanitizer detects an out-of-bounds write past a global array and identifies the global-buffer-overflow class. |
| [gccext_asan_heap_use_after_free.cpp](gccext_asan_heap_use_after_free.cpp) | c++17 | GCC >= 13 | covered | ASan demo: read after free. Runs only under -fsanitize=address; ASan reports 'heap-use-after-free' and aborts. |
| [gccext_asan_stack_use_after_scope.cpp](gccext_asan_stack_use_after_scope.cpp) | c++17 | GCC >= 13 | covered | AddressSanitizer's use-after-scope instrumentation catches a pointer used after the local object it referred to left scope. |
