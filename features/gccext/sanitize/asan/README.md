# GCC extensions: AddressSanitizer trip demos

_Folder: `features/gccext/sanitize/asan/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [gcc-sanitize](#gcc-sanitize)

## gcc-sanitize

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_asan_double_free.cpp](gccext_asan_double_free.cpp) | c++17 | 13 | ASan demo: freeing the same pointer twice. Runs only under -fsanitize=address; ASan reports 'attempting double-free'. |
| [gccext_asan_heap_buffer_overflow.cpp](gccext_asan_heap_buffer_overflow.cpp) | c++17 | 13 | ASan demo: writing one past the end of a heap buffer. Runs only under -fsanitize=address; ASan reports 'heap-buffer-overflow'. |
| [gccext_asan_heap_use_after_free.cpp](gccext_asan_heap_use_after_free.cpp) | c++17 | 13 | ASan demo: read after free. Runs only under -fsanitize=address; ASan reports 'heap-use-after-free' and aborts. |
| [gccext_asan_stack_buffer_overflow.cpp](gccext_asan_stack_buffer_overflow.cpp) | c++17 | 13 | ASan demo: writing past the end of a stack array. Runs only under -fsanitize=address; ASan reports 'stack-buffer-overflow'. |
