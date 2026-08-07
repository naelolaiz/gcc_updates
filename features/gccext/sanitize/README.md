# `features/gccext/sanitize/` — sanitizer interaction & demos

Two kinds of files live here:

| Subfolder | Contents | Runs in default matrix? |
|-----------|----------|-------------------------|
| [`integration/`](integration/) | Files showing how to *interact* with sanitizers without triggering them: `[[gnu::no_sanitize]]` opt-out, `-D_GLIBCXX_DEBUG` mode, `[[assume]]` semantics under UBSan. | yes |
| [`ubsan/`](ubsan/)             | Programs that DELIBERATELY trip a UBSan check (signed overflow, null deref, oversized shift). | no — only under `sanitize=undefined` |
| [`asan/`](asan/)               | Programs that DELIBERATELY trip an AddressSanitizer check (heap-use-after-free, heap- and stack-buffer-overflow, double-free). | no — only under `sanitize=address` |
| [`leak/`](leak/)               | Programs that intentionally leak memory; LeakSanitizer reports them at exit. | no — only under `sanitize=address` (leak detection is on by default) or `sanitize=leak` |
| [`tsan/`](tsan/)               | Programs with a deliberate data race; ThreadSanitizer reports it. | no — only under `sanitize=thread`. **Cannot share a build with ASan** — runs in its own CI job. |

The demo files use `REQUIRES_SANITIZER` in their `gcc_feature_test()` calls, so
CMake skips them in regular runs and only registers them in matching sanitizer
runs. They use `WILL_FAIL` plus `EXPECT_OUTPUT`, so the test asserts the
expected sanitizer report text rather than accepting any failure.

## Running them

```bash
# UBSan + ASan (the default sanitizer profile in CI)
./scripts/container-dev.sh 15 sanitize=undefined,address

# TSan (separate, since it can't co-exist with ASan)
./scripts/container-dev.sh 15 sanitize=thread

# Pure leak detection (lighter than full ASan)
./scripts/container-dev.sh 15 sanitize=leak
```

The narrative reference is in [../../../docs/sanitizers.md](../../../docs/sanitizers.md).
