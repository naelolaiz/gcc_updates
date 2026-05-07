# GCC extensions: attributes, builtins, OpenMP, vectorization, target multi-versioning

_Folder: `features/gccext/sanitize/leak/`. 1 example(s) across 1 topic(s). Auto-generated from each file's `// gcc-test:` header — do not edit by hand. Run `python3 scripts/discover.py --emit-docs` to refresh._

## gcc-sanitize

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_lsan_simple_leak.cpp](gccext_lsan_simple_leak.cpp) | c++17 | 13 | LSan demo: heap allocation with no matching free. Runs only when ASan or LSan is active; reports leaks at exit. |
