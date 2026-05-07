#!/usr/bin/env python3
"""
discover.py - the build & test engine for gcc_updates.

Walks `features/**/*.cpp` (recursive), parses each file's `// gcc-test:`
metadata header, then for the active GCC version compiles + runs every
eligible example.

Usage:
    discover.py                            # autodetect g++; build & run all
    discover.py --gcc-version=15           # force matrix-version match
    discover.py --dry-run                  # parse metadata only; do not compile
    discover.py --emit-docs                # regenerate every features/<bucket>/README.md
    discover.py --filter=ranges            # only examples whose topic matches
    discover.py --sanitize=undefined,address  # add -fsanitize=... to every build
    discover.py --analyzer                 # compile-only, only requires-analyzer demos
    discover.py --show-cmds                # dump the build command per example
"""
from __future__ import annotations

import argparse
import dataclasses
import os
import re
import signal
import shlex
import shutil
import subprocess
import sys
import time
import traceback
from pathlib import Path
from typing import Callable, Iterable

REPO_ROOT = Path(__file__).resolve().parent.parent
FEATURES_DIR = REPO_ROOT / "features"
DOCS_DIR = REPO_ROOT / "docs"
BUILD_DIR = Path("/tmp/gcc_updates_build")
ACTIVE_GROUP_CLOSER: Callable[[], None] | None = None

REQUIRED_KEYS = {"std", "min-gcc", "topic", "experimental"}
KNOWN_KEYS = REQUIRED_KEYS | {
    "extra-flags",
    "run-args",
    "expect-exit",
    "max-gcc",
    "requires-sanitizer",
    "requires-analyzer",
    "expect-error",
    "skip-sanitizer",
    "min-libstdcxx",
    "max-libstdcxx",
}
KNOWN_STDS = {"c++11", "c++14", "c++17", "c++20", "c++23", "c++26"}

DEFAULT_FLAGS = ["-Wall", "-Wextra", "-Wpedantic", "-O2", "-pthread"]

ANSI_GREEN = "\033[32m"
ANSI_RED = "\033[31m"
ANSI_YELLOW = "\033[33m"
ANSI_DIM = "\033[2m"
ANSI_RESET = "\033[0m"

def print_captured_output(text: str) -> None:
    for line in text.rstrip().splitlines():
        print(f"    | {line}")


def close_active_group() -> None:
    if ACTIVE_GROUP_CLOSER is not None:
        ACTIVE_GROUP_CLOSER()


def handle_termination(signum: int, _frame: object) -> None:
    close_active_group()
    if os.environ.get("GITHUB_ACTIONS") == "true":
        print(f"received signal {signum}; stopping", flush=True)
    raise SystemExit(128 + signum)


@dataclasses.dataclass
class Example:
    path: Path
    std: str
    min_gcc: int
    max_gcc: int | None
    topic: str
    experimental: bool
    extra_flags: list[str]
    run_args: list[str]
    expect_exit: int
    description: str
    requires_sanitizer: list[str]   # if non-empty, file only runs under matching --sanitize
    requires_analyzer: bool         # if true, file only runs under --analyzer (compile-only)
    expect_error: re.Pattern | None # required when experimental=true; checked on failure
    skip_sanitizer: list[str]       # skip the file when any of these sanitizers is active
    min_libstdcxx: int | None       # require _GLIBCXX_RELEASE >= this (independent of g++ version)
    max_libstdcxx: int | None       # require _GLIBCXX_RELEASE <= this

    @property
    def stem(self) -> str:
        return self.path.stem

    @property
    def standard_bucket(self) -> str:
        # "cpp20_..." -> "cpp20"; otherwise the leading prefix up to first underscore.
        return self.stem.split("_", 1)[0]


def parse_metadata(path: Path) -> Example:
    text = path.read_text(encoding="utf-8")
    head = text.splitlines()[:15]
    meta_line = next((ln for ln in head if ln.startswith("// gcc-test:")), None)
    if not meta_line:
        raise ValueError(f"{path}: missing `// gcc-test:` metadata header")

    payload = meta_line[len("// gcc-test:"):].strip()
    pairs: dict[str, str] = {}
    for tok in shlex.split(payload):
        if "=" not in tok:
            raise ValueError(f"{path}: malformed metadata token {tok!r}")
        k, v = tok.split("=", 1)
        pairs[k] = v

    unknown = set(pairs) - KNOWN_KEYS
    if unknown:
        raise ValueError(f"{path}: unknown metadata keys: {sorted(unknown)}")
    missing = REQUIRED_KEYS - set(pairs)
    if missing:
        raise ValueError(f"{path}: missing required keys: {sorted(missing)}")

    if pairs["std"] not in KNOWN_STDS:
        raise ValueError(f"{path}: std={pairs['std']!r} not in {sorted(KNOWN_STDS)}")
    if pairs["experimental"] not in {"true", "false"}:
        raise ValueError(f"{path}: experimental must be true|false")

    description = ""
    for ln in head:
        m = re.match(r"//\s*description:\s*(.*)", ln)
        if m:
            description = m.group(1).strip()
            break

    extra_flags: list[str] = []
    if "extra-flags" in pairs:
        extra_flags = [f for f in pairs["extra-flags"].split(",") if f]

    run_args: list[str] = []
    if "run-args" in pairs:
        run_args = shlex.split(pairs["run-args"])

    requires_sanitizer: list[str] = []
    if "requires-sanitizer" in pairs:
        requires_sanitizer = [s for s in pairs["requires-sanitizer"].split(",") if s]

    skip_sanitizer: list[str] = []
    if "skip-sanitizer" in pairs:
        skip_sanitizer = [s for s in pairs["skip-sanitizer"].split(",") if s]

    requires_analyzer = pairs.get("requires-analyzer", "false") == "true"
    if "requires-analyzer" in pairs and pairs["requires-analyzer"] not in {"true", "false"}:
        raise ValueError(f"{path}: requires-analyzer must be true|false")

    expect_error: re.Pattern | None = None
    if "expect-error" in pairs:
        try:
            expect_error = re.compile(pairs["expect-error"])
        except re.error as e:
            raise ValueError(f"{path}: expect-error regex is invalid: {e}")

    experimental = pairs["experimental"] == "true"
    # An experimental flag without a declared expected error is the bug class
    # this whole mechanism exists to prevent: any failure -- typo, missing
    # include, real regression -- would otherwise be silently absorbed and
    # indistinguishable from "feature legitimately not yet shipped".
    if experimental and expect_error is None:
        raise ValueError(
            f"{path}: experimental=true requires expect-error=<regex> "
            "(matched against the failing build's stderr). Without it any "
            "kind of failure is silently tolerated, defeating the point.")

    return Example(
        path=path,
        std=pairs["std"],
        min_gcc=int(pairs["min-gcc"]),
        max_gcc=int(pairs["max-gcc"]) if "max-gcc" in pairs else None,
        topic=pairs["topic"],
        experimental=experimental,
        extra_flags=extra_flags,
        run_args=run_args,
        expect_exit=int(pairs.get("expect-exit", "0")),
        description=description,
        requires_sanitizer=requires_sanitizer,
        requires_analyzer=requires_analyzer,
        expect_error=expect_error,
        skip_sanitizer=skip_sanitizer,
        min_libstdcxx=int(pairs["min-libstdcxx"]) if "min-libstdcxx" in pairs else None,
        max_libstdcxx=int(pairs["max-libstdcxx"]) if "max-libstdcxx" in pairs else None,
    )


def discover() -> list[Example]:
    if not FEATURES_DIR.is_dir():
        return []
    examples: list[Example] = []
    errors: list[str] = []
    for path in sorted(FEATURES_DIR.rglob("*.cpp")):
        try:
            examples.append(parse_metadata(path))
        except ValueError as e:
            errors.append(str(e))
    if errors:
        print("Metadata errors:", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        sys.exit(2)
    return examples


def detect_gcc_version() -> int:
    out = subprocess.check_output(["g++", "-dumpversion"], text=True).strip()
    return int(out.split(".")[0])


def detect_libstdcxx_release() -> int | None:
    """Return the active libstdc++ major release (_GLIBCXX_RELEASE) or None.

    libstdc++'s release version is independent of g++'s -dumpversion: a
    binary built against libstdc++ N can be linked at runtime against any
    libstdc++ >= N, so a feature gated on the runtime library has to be
    checked separately from the compiler version.

    The probe asks the preprocessor for the macro after including <version>
    -- no compile, no run. Returns None if the macro is unavailable (very old
    toolchains, non-libstdc++).
    """
    try:
        out = subprocess.check_output(
            ["g++", "-x", "c++", "-E", "-dM", "-"],
            input="#include <version>\n", text=True, stderr=subprocess.DEVNULL,
        )
    except (OSError, subprocess.CalledProcessError):
        return None
    for ln in out.splitlines():
        # `#define _GLIBCXX_RELEASE 15`
        m = re.match(r"#define\s+_GLIBCXX_RELEASE\s+(\d+)", ln)
        if m:
            return int(m.group(1))
    return None


def sanitizer_flags(sanitize: str | None) -> list[str]:
    """Convert --sanitize=ubsan,address into the g++ flag list."""
    if not sanitize:
        return []
    parts = [p.strip() for p in sanitize.split(",") if p.strip()]
    if not parts:
        return []
    return [
        f"-fsanitize={','.join(parts)}",
        "-fno-omit-frame-pointer",
        "-g",
    ]


def build_cmd_for(ex: Example, sanitize: str | None = None,
                  analyzer: bool = False) -> list[str]:
    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    out_bin = BUILD_DIR / ex.stem
    return [
        "g++",
        f"-std={ex.std}",
        *DEFAULT_FLAGS,
        *(["-fanalyzer"] if analyzer else []),
        *sanitizer_flags(sanitize),
        str(ex.path),
        *ex.extra_flags,
        "-o",
        str(out_bin),
    ]


def compile_one(ex: Example, gcc_version: int, verbose: bool = False,
                sanitize: str | None = None,
                analyzer: bool = False) -> tuple[bool, str]:
    cmd = build_cmd_for(ex, sanitize=sanitize, analyzer=analyzer)
    out_bin = cmd[-1]
    if verbose:
        print(f"  $ {' '.join(cmd)}")
    try:
        proc = subprocess.run(cmd, capture_output=True, text=True)
    except OSError as e:
        return False, f"compile command failed to start:\n{e}"
    # In verbose mode, surface compiler diagnostics (warnings, notes) even on
    # success -- otherwise -Wall/-Wextra output is silently discarded.
    if verbose and proc.stderr:
        print_captured_output(proc.stderr)
    if proc.returncode != 0:
        return False, f"compile failed:\n{proc.stderr}"
    return True, out_bin


def run_one(ex: Example, binary: str, verbose: bool = False) -> tuple[bool, str]:
    if verbose:
        run_cmd = " ".join(shlex.quote(p) for p in [binary, *ex.run_args])
        print(f"  $ {run_cmd}")
    try:
        proc = subprocess.run(
            [binary, *ex.run_args],
            capture_output=True,
            text=True,
            timeout=30,
        )
    except subprocess.TimeoutExpired as e:
        stdout = e.stdout or ""
        stderr = e.stderr or ""
        return (
            False,
            f"timeout after 30s expected exit={ex.expect_exit}\n"
            f"stdout:\n{stdout}\nstderr:\n{stderr}",
        )
    except OSError as e:
        return False, f"run command failed to start:\n{e}"
    if verbose and (proc.stdout or proc.stderr):
        print_captured_output(proc.stdout)
        print_captured_output(proc.stderr)
    if proc.returncode != ex.expect_exit:
        return (
            False,
            f"exit={proc.returncode} expected={ex.expect_exit}\n"
            f"stdout:\n{proc.stdout}\nstderr:\n{proc.stderr}",
        )
    return True, ""


def filter_examples(
    examples: Iterable[Example],
    gcc_version: int,
    topic_filter: str | None,
    active_sanitizers: list[str] | None = None,
    analyzer_active: bool = False,
    libstdcxx_release: int | None = None,
) -> tuple[list[Example], list[tuple[Example, str]]]:
    eligible: list[Example] = []
    skipped: list[tuple[Example, str]] = []
    active = set(active_sanitizers or [])
    for ex in examples:
        if topic_filter and topic_filter not in ex.topic and topic_filter not in ex.stem:
            continue
        if gcc_version < ex.min_gcc:
            skipped.append((ex, f"needs gcc {ex.min_gcc}+"))
            continue
        if ex.max_gcc is not None and gcc_version > ex.max_gcc:
            skipped.append((ex, f"limited to gcc {ex.max_gcc}"))
            continue
        # libstdc++ release gate: independent of g++ version. Some C++23
        # library features landed at a different libstdc++ release than the
        # compiler version they appear with (e.g. ranges::starts_with /
        # ends_with require libstdc++ 16, per cppreference). When the probe
        # couldn't read _GLIBCXX_RELEASE we don't gate (treated as
        # "unknown but likely OK").
        if ex.min_libstdcxx is not None and libstdcxx_release is not None \
                and libstdcxx_release < ex.min_libstdcxx:
            skipped.append((ex, f"needs libstdc++ {ex.min_libstdcxx}+ "
                                f"(found {libstdcxx_release})"))
            continue
        if ex.max_libstdcxx is not None and libstdcxx_release is not None \
                and libstdcxx_release > ex.max_libstdcxx:
            skipped.append((ex, f"limited to libstdc++ {ex.max_libstdcxx} "
                                f"(found {libstdcxx_release})"))
            continue
        # Files that *require* a sanitizer only run when one of the requested
        # sanitizers is active. They DELIBERATELY trip a check, so running them
        # in the default matrix would either UB-crash or silently "pass".
        if ex.requires_sanitizer:
            if not active or not (set(ex.requires_sanitizer) & active):
                skipped.append((ex, f"requires --sanitize={','.join(ex.requires_sanitizer)}"))
                continue
        # Files that explicitly opt OUT of a sanitizer (e.g. OpenMP / IFUNC code
        # that data-races inside libgomp's runtime under TSan -- not the
        # example's own bug). Still runs in regular and other-sanitizer modes.
        if ex.skip_sanitizer and active and (set(ex.skip_sanitizer) & active):
            overlap = sorted(set(ex.skip_sanitizer) & active)
            skipped.append((ex, f"skip-sanitizer={','.join(overlap)}"))
            continue
        # Analyzer demos: requires-analyzer=true files only run under --analyzer.
        # Conversely, --analyzer mode runs *only* analyzer demos -- the job is
        # compile-only and the rest of the workspace is exercised by the matrix.
        if ex.requires_analyzer:
            if not analyzer_active:
                skipped.append((ex, "requires --analyzer"))
                continue
        elif analyzer_active:
            # Don't append a SKIP line: in --analyzer mode every non-analyzer
            # file is by definition irrelevant. Reporting 160+ "not an analyzer
            # demo" lines per run is noise that drowns out the 3 real demos.
            continue
        eligible.append(ex)
    return eligible, skipped


def emit_docs(examples: list[Example]) -> None:
    # Per-bucket titles (filename prefix). Used as a fallback when the leaf
    # path doesn't map to a more specific title below.
    bucket_titles = {
        "cpp11":  "C++11 examples",
        "cpp14":  "C++14 examples",
        "cpp17":  "C++17 examples",
        "cpp20":  "C++20 examples",
        "cpp23":  "C++23 examples",
        "cpp26":  "C++26 (experimental) examples",
        "gccext": "GCC extensions",
        "gcc13":  "GCC 13 release-notes examples",
        "gcc14":  "GCC 14 release-notes examples",
        "gcc15":  "GCC 15 release-notes examples",
        "gcc16":  "GCC 16 release-notes examples",
    }
    # Per-leaf-folder titles, keyed by the path under features/. These are the
    # *narrow* titles the user actually sees in each leaf README; without them
    # every gccext/* folder ends up sharing one generic catch-all heading.
    leaf_titles = {
        "gccext/attributes":         "GCC extensions: attributes",
        "gccext/builtins":           "GCC extensions: builtins & vector_size",
        "gccext/codegen":            "GCC extensions: codegen (auto-vectorisation, inline asm)",
        "gccext/openmp":             "GCC extensions: OpenMP",
        "gccext/pragmas":            "GCC extensions: diagnostic pragmas",
        "gccext/sanitize/integration": "GCC extensions: sanitizer integration (clean demos)",
        "gccext/sanitize/asan":      "GCC extensions: AddressSanitizer trip demos",
        "gccext/sanitize/ubsan":     "GCC extensions: UndefinedBehaviorSanitizer trip demos",
        "gccext/sanitize/tsan":      "GCC extensions: ThreadSanitizer trip demos",
        "gccext/sanitize/leak":      "GCC extensions: LeakSanitizer trip demos",
        "gccext/analyzer":           "GCC extensions: -fanalyzer compile-time demos",
    }

    # Group examples by leaf folder; each leaf gets its own README.md.
    by_leaf: dict[Path, list[Example]] = {}
    for ex in examples:
        by_leaf.setdefault(ex.path.parent, []).append(ex)

    for leaf_dir, items in sorted(by_leaf.items()):
        rel_leaf = leaf_dir.relative_to(REPO_ROOT)
        leaf_key = str(rel_leaf.relative_to("features"))
        # Prefer a leaf-specific title; fall back to the bucket title (e.g.
        # cppNN buckets only have one leaf each, so the bucket title is fine).
        bucket = items[0].standard_bucket
        title = leaf_titles.get(leaf_key,
                                bucket_titles.get(bucket, f"{bucket} examples"))
        path = leaf_dir / "README.md"

        items.sort(key=lambda e: (e.topic, e.stem))
        by_topic: dict[str, list[Example]] = {}
        for ex in items:
            by_topic.setdefault(ex.topic, []).append(ex)

        lines = [f"# {title}", ""]
        lines.append(f"_Folder: `{rel_leaf}/`. {len(items)} example(s) across "
                     f"{len(by_topic)} topic(s). Auto-generated from each "
                     f"file's `// gcc-test:` header — do not edit by hand. "
                     f"Run `python3 scripts/discover.py --emit-docs` to refresh._")
        lines.append("")
        if len(by_topic) > 1:
            lines.append("## Topics")
            lines.append("")
            for topic, _ in sorted(by_topic.items()):
                anchor = topic.replace(" ", "-").replace("/", "-")
                lines.append(f"- [{topic}](#{anchor})")
            lines.append("")
        for topic, ex_list in sorted(by_topic.items()):
            lines.append(f"## {topic}")
            lines.append("")
            lines.append("| File | std | min-gcc | Description |")
            lines.append("| ---- | --- | ------- | ----------- |")
            for ex in ex_list:
                exp = " *(experimental)*" if ex.experimental else ""
                lines.append(
                    f"| [{ex.path.name}]({ex.path.name}) | {ex.std} | "
                    f"{ex.min_gcc} | {ex.description}{exp} |"
                )
            lines.append("")
        path.write_text("\n".join(lines), encoding="utf-8")
        print(f"wrote {path.relative_to(REPO_ROOT)} "
              f"({len(items)} entries, {len(by_topic)} topics)")


def report(eligible: list[Example],
           skipped: list[tuple[Example, str]],
           total: int) -> None:
    print(f"discovered: {total} examples")
    print(f"  eligible: {len(eligible)}")
    print(f"  skipped : {len(skipped)}")
    silent = total - len(eligible) - len(skipped)
    if silent:
        # Filter and analyzer-mode "not an analyzer demo" drops are deliberately
        # not surfaced as SKIPs (they'd just be noise) -- account for them here
        # so 'discovered' still reconciles with the full corpus.
        print(f"  filtered: {silent}")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--gcc-version", type=int, default=None,
                    help="Override autodetected major version of g++")
    ap.add_argument("--dry-run", action="store_true",
                    help="Parse metadata only; do not compile or run")
    ap.add_argument("--emit-docs", action="store_true",
                    help="(Re)generate every features/<bucket>/README.md leaf index from metadata")
    ap.add_argument("--filter", default=None,
                    help="Only process examples whose topic or filename contains this substring")
    ap.add_argument("--show-cmds", action="store_true",
                    help="Print the exact g++ build command for each example, then exit")
    ap.add_argument("--verbose", action="store_true",
                    help="Print the full g++ command before each compile")
    ap.add_argument("--sanitize", default=None,
                    help="Comma-separated sanitizer list, e.g. 'undefined,address'. "
                         "Adds -fsanitize=… -fno-omit-frame-pointer -g to every build.")
    ap.add_argument("--analyzer", action="store_true",
                    help="Enable -fanalyzer; runs only requires-analyzer=true demos, "
                         "compile-only (binaries are not executed).")
    args = ap.parse_args()

    examples = discover()

    if args.emit_docs:
        emit_docs(examples)
        return 0

    if args.dry_run:
        for ex in examples:
            tag = " [experimental]" if ex.experimental else ""
            print(f"{ex.path.name}: std={ex.std} min-gcc={ex.min_gcc} "
                  f"topic={ex.topic}{tag}")
        san_list = [s for s in (args.sanitize or "").split(",") if s]
        eligible_dr, skipped_dr = filter_examples(
            examples, args.gcc_version or 99, args.filter, san_list,
            analyzer_active=args.analyzer,
            libstdcxx_release=None)  # dry-run: skip the probe entirely
        report(eligible_dr, skipped_dr, len(examples))
        return 0

    if args.show_cmds:
        for ex in examples:
            cmd = build_cmd_for(ex, sanitize=args.sanitize, analyzer=args.analyzer)
            print(f"{ex.path.name}:")
            print(f"  {' '.join(cmd)}")
        return 0

    if shutil.which("g++") is None:
        print("error: g++ not found in PATH", file=sys.stderr)
        return 2

    gcc_version = args.gcc_version or detect_gcc_version()
    libstdcxx_release = detect_libstdcxx_release()
    san_note = f" with sanitizers={args.sanitize}" if args.sanitize else ""
    ana_note = " with -fanalyzer (compile-only)" if args.analyzer else ""
    libstdcxx_note = (f", libstdc++ release {libstdcxx_release}"
                      if libstdcxx_release is not None
                      else ", libstdc++ release unknown")
    print(f"using g++ major version {gcc_version}{libstdcxx_note}"
          f"{san_note}{ana_note}")

    san_list = [s for s in (args.sanitize or "").split(",") if s]
    eligible, skipped = filter_examples(examples, gcc_version, args.filter,
                                         san_list, analyzer_active=args.analyzer,
                                         libstdcxx_release=libstdcxx_release)
    report(eligible, skipped, len(examples))

    if BUILD_DIR.exists():
        shutil.rmtree(BUILD_DIR)

    pass_count = 0
    fail_count = 0
    soft_fail_count = 0
    fail_lines: list[str] = []
    skip_lines: list[str] = []   # buffered; emitted at the end inside ::group::
    t0 = time.time()

    in_gh = os.environ.get("GITHUB_ACTIONS") == "true"
    # In CI, always show the g++ command + diagnostics + run output for every
    # example. Fold by feature folder rather than by every individual example to
    # keep large sanitizer logs navigable. Locally, only --verbose opts in to the
    # noise; the default is the one-line PASS/FAIL summary.
    verbose = args.verbose or in_gh

    def workflow_command_value(text: str) -> str:
        return text.replace("%", "%25").replace("\r", "%0D").replace("\n", "%0A")

    def grp_start(title: str) -> None:
        if in_gh:
            print(f"::group::{workflow_command_value(title)}", flush=True)

    def grp_end() -> None:
        if in_gh:
            print("::endgroup::", flush=True)

    current_group: str | None = None

    def switch_example_group(ex: Example) -> None:
        nonlocal current_group
        if not in_gh:
            return
        title = str(ex.path.parent.relative_to(REPO_ROOT))
        if title == current_group:
            return
        if current_group is not None:
            grp_end()
        grp_start(title)
        current_group = title

    def close_example_group() -> None:
        nonlocal current_group
        if in_gh and current_group is not None:
            grp_end()
            current_group = None

    global ACTIVE_GROUP_CLOSER
    ACTIVE_GROUP_CLOSER = close_example_group

    def classify_failure(ex: Example, info: str) -> tuple[str, bool, str | None]:
        """Return (status_label, is_soft_pass, note). status_label is colourised.

        - non-experimental: hard FAIL.
        - experimental + expect_error matches stderr: EXP-FAIL (soft pass).
        - experimental + expect_error does NOT match: hard FAIL with EXP-WRONG-ERR
          flag — the file failed for an unexpected reason; the experimental
          escape hatch must not hide that.
        """
        if not ex.experimental:
            return f"{ANSI_RED}FAIL    {ANSI_RESET}", False, None
        if ex.expect_error and ex.expect_error.search(info):
            first = info.splitlines()[0] if info else ""
            return f"{ANSI_YELLOW}EXP-FAIL{ANSI_RESET}", True, first
        # Experimental file failed but stderr does NOT match expect-error.
        return (f"{ANSI_RED}EXP-WRONG-ERR{ANSI_RESET}", False,
                f"experimental file failed but stderr did not match "
                f"expect-error={ex.expect_error.pattern!r}" if ex.expect_error
                else "experimental=true but expect-error is unset")

    for ex in eligible:
        label = f"[{ex.std}] {ex.path.name}"
        switch_example_group(ex)
        ok, info = compile_one(ex, gcc_version, verbose=verbose,
                                sanitize=args.sanitize,
                                analyzer=args.analyzer)
        if not ok:
            status, soft, note = classify_failure(ex, info)
            if soft:
                soft_fail_count += 1
                suffix = f": {note}" if note else ""
                result = f"{status} {label}{suffix}"
            else:
                fail_count += 1
                result = f"{status} {label}"
                detail = f"{label}"
                if note:
                    detail += f"\n  reason: {note}"
                detail += f"\n{info}"
                fail_lines.append(detail)
            print(result)
            continue
        # Analyzer demos are compile-only -- the binary deliberately contains UB
        # we don't want to actually execute.
        if ex.requires_analyzer:
            pass_count += 1
            result = f"{ANSI_GREEN}PASS    {ANSI_RESET} {label} (analyzer compile-only)"
            if ex.experimental:
                # An experimental analyzer demo that *compiles* successfully
                # has effectively shipped — the diagnostic is what we wanted.
                # Surface the promote hint but don't fail.
                result += f"  {ANSI_YELLOW}[experimental compiled cleanly — drop experimental=true]{ANSI_RESET}"
            print(result)
            continue
        ok, info = run_one(ex, info, verbose=verbose)
        if ok:
            pass_count += 1
            if ex.experimental:
                # The whole point of experimental=true + expect-error is to
                # catch the moment the feature ships. When the file passes,
                # the metadata must be flipped -- otherwise a future regression
                # would silently pass under EXP-FAIL.
                fail_count += 1
                result = (f"{ANSI_RED}EXP-PROMOTE{ANSI_RESET} {label}: "
                          f"experimental file passed; flip experimental=false "
                          f"and remove expect-error in the metadata header.")
                fail_lines.append(result)
                print(result)
                # Don't double-count: experimental→passed is not a "pass".
                pass_count -= 1
            else:
                result = f"{ANSI_GREEN}PASS    {ANSI_RESET} {label}"
                print(result)
        else:
            status, soft, note = classify_failure(ex, info)
            if soft:
                soft_fail_count += 1
                suffix = f": {note}" if note else ""
                print(f"{status} {label}{suffix}")
            else:
                fail_count += 1
                result = f"{status} {label}"
                print(result)
                if note:
                    print(f"  reason: {note}")
                print_captured_output(info)
                detail = f"{label}"
                if note:
                    detail += f"\n  reason: {note}"
                detail += f"\n{info}"
                fail_lines.append(detail)

    close_example_group()

    for ex, reason in skipped:
        skip_lines.append(f"{ANSI_DIM}SKIP    {ANSI_RESET} [{ex.std}] "
                          f"{ex.path.name} ({reason})")

    if skip_lines:
        grp_start(f"Skipped ({len(skip_lines)})")
        if not in_gh:
            print(f"\n--- skipped ({len(skip_lines)}) ---")
        for line in skip_lines:
            print(line)
        grp_end()

    elapsed = time.time() - t0
    print()
    print(f"summary: {pass_count} pass, {fail_count} fail, "
          f"{soft_fail_count} experimental-fail, {len(skipped)} skip "
          f"({elapsed:.1f}s)")

    if fail_lines:
        print("\n--- failures ---")
        for line in fail_lines:
            print_captured_output(line)

    # Job Summary: a compact markdown table that GitHub renders on the run
    # page itself, complementing the per-example ::group:: blocks in the
    # inline step log.
    summary_path = os.environ.get("GITHUB_STEP_SUMMARY")
    if summary_path:
        try:
            mode_bits = []
            if args.sanitize:
                mode_bits.append(f"sanitize={args.sanitize}")
            if args.analyzer:
                mode_bits.append("analyzer")
            mode = ", ".join(mode_bits) if mode_bits else "default"
            lines: list[str] = []
            lines.append(f"### gcc-{gcc_version} · {mode}")
            lines.append("")
            lines.append("| pass | fail | experimental-fail | skip | elapsed |")
            lines.append("|-----:|-----:|------------------:|-----:|--------:|")
            lines.append(f"| {pass_count} | {fail_count} | {soft_fail_count} | "
                         f"{len(skipped)} | {elapsed:.1f}s |")
            if fail_lines:
                lines.append("")
                lines.append("**Failures:**")
                lines.append("")
                lines.append("```")
                for entry in fail_lines:
                    lines.append(entry)
                lines.append("```")
            with open(summary_path, "a", encoding="utf-8") as f:
                f.write("\n".join(lines) + "\n")
        except OSError:
            # Summary is best-effort -- never fail the job for a write issue.
            pass

    ACTIVE_GROUP_CLOSER = None
    return 1 if fail_count else 0


if __name__ == "__main__":
    signal.signal(signal.SIGINT, handle_termination)
    signal.signal(signal.SIGTERM, handle_termination)
    try:
        exit_code = main()
    except SystemExit:
        raise
    except BaseException:
        close_active_group()
        if os.environ.get("GITHUB_ACTIONS") == "true":
            print("internal failure:")
            print_captured_output("".join(traceback.format_exc()))
        else:
            traceback.print_exc()
        sys.exit(1)
    sys.exit(exit_code)
