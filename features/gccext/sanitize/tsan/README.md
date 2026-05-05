# GCC extensions: attributes, builtins, OpenMP, vectorization, target multi-versioning

_Folder: `features/gccext/sanitize/tsan/`. 1 example(s) across 1 topic(s). Auto-generated from each file's `// gcc-test:` header — do not edit by hand. Run `python3 scripts/discover.py --emit-docs` to refresh._

## gcc-sanitize

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_tsan_data_race.cpp](gccext_tsan_data_race.cpp) | c++17 | 13 | TSan demo: two threads writing the same plain int without synchronisation. Runs only under -fsanitize=thread; reports a data race. |
