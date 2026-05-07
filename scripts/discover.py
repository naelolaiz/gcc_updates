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
    discover.py --emit-docs                # regenerate docs/cpp{20,23,26}.md
    discover.py --filter=ranges            # only examples whose topic matches
    discover.py --sanitize=undefined,address  # add -fsanitize=... to every build
    discover.py --show-cmds                # dump the build command per example
"""
from __future__ import annotations

import argparse
import dataclasses
import os
import re
import shlex
import shutil
import subprocess
import sys
import time
from pathlib import Path
from typing import Iterable

REPO_ROOT = Path(__file__).resolve().parent.parent
FEATURES_DIR = REPO_ROOT / "features"
DOCS_DIR = REPO_ROOT / "docs"
BUILD_DIR = Path("/tmp/gcc_updates_build")

REQUIRED_KEYS = {"std", "min-gcc", "topic", "experimental"}
KNOWN_KEYS = REQUIRED_KEYS | {
    "extra-flags",
    "run-args",
    "expect-exit",
    "max-gcc",
    "requires-sanitizer",
}
KNOWN_STDS = {"c++11", "c++14", "c++17", "c++20", "c++23", "c++26"}

DEFAULT_FLAGS = ["-Wall", "-Wextra", "-Wpedantic", "-O2", "-pthread"]

ANSI_GREEN = "\033[32m"
ANSI_RED = "\033[31m"
ANSI_YELLOW = "\033[33m"
ANSI_DIM = "\033[2m"
ANSI_RESET = "\033[0m"


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

    return Example(
        path=path,
        std=pairs["std"],
        min_gcc=int(pairs["min-gcc"]),
        max_gcc=int(pairs["max-gcc"]) if "max-gcc" in pairs else None,
        topic=pairs["topic"],
        experimental=pairs["experimental"] == "true",
        extra_flags=extra_flags,
        run_args=run_args,
        expect_exit=int(pairs.get("expect-exit", "0")),
        description=description,
        requires_sanitizer=requires_sanitizer,
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


def build_cmd_for(ex: Example, sanitize: str | None = None) -> list[str]:
    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    out_bin = BUILD_DIR / ex.stem
    return [
        "g++",
        f"-std={ex.std}",
        *DEFAULT_FLAGS,
        *sanitizer_flags(sanitize),
        *ex.extra_flags,
        str(ex.path),
        "-o",
        str(out_bin),
    ]


def compile_one(ex: Example, gcc_version: int, verbose: bool = False,
                sanitize: str | None = None) -> tuple[bool, str]:
    cmd = build_cmd_for(ex, sanitize=sanitize)
    out_bin = cmd[-1]
    if verbose:
        print(f"  $ {' '.join(cmd)}")
    proc = subprocess.run(cmd, capture_output=True, text=True)
    # In verbose mode, surface compiler diagnostics (warnings, notes) even on
    # success -- otherwise -Wall/-Wextra output is silently discarded.
    if verbose and proc.stderr:
        for line in proc.stderr.rstrip().splitlines():
            print(f"    {line}")
    if proc.returncode != 0:
        return False, f"compile failed:\n{proc.stderr}"
    return True, out_bin


def run_one(ex: Example, binary: str, verbose: bool = False) -> tuple[bool, str]:
    if verbose:
        run_cmd = " ".join(shlex.quote(p) for p in [binary, *ex.run_args])
        print(f"  $ {run_cmd}")
    proc = subprocess.run(
        [binary, *ex.run_args],
        capture_output=True,
        text=True,
        timeout=30,
    )
    if verbose:
        for line in proc.stdout.rstrip().splitlines():
            print(f"    {line}")
        for line in proc.stderr.rstrip().splitlines():
            print(f"    {line}")
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
) -> tuple[list[Example], list[Example]]:
    eligible: list[Example] = []
    skipped: list[Example] = []
    active = set(active_sanitizers or [])
    for ex in examples:
        if topic_filter and topic_filter not in ex.topic and topic_filter not in ex.stem:
            continue
        if gcc_version < ex.min_gcc:
            skipped.append(ex)
            continue
        if ex.max_gcc is not None and gcc_version > ex.max_gcc:
            skipped.append(ex)
            continue
        # Files that *require* a sanitizer only run when one of the requested
        # sanitizers is active. They DELIBERATELY trip a check, so running them
        # in the default matrix would either UB-crash or silently "pass".
        if ex.requires_sanitizer:
            if not active or not (set(ex.requires_sanitizer) & active):
                skipped.append(ex)
                continue
        eligible.append(ex)
    return eligible, skipped


def emit_docs(examples: list[Example]) -> None:
    bucket_titles = {
        "cpp11":  "C++11 examples",
        "cpp14":  "C++14 examples",
        "cpp17":  "C++17 examples",
        "cpp20":  "C++20 examples",
        "cpp23":  "C++23 examples",
        "cpp26":  "C++26 (experimental) examples",
        "gccext": "GCC extensions: attributes, builtins, OpenMP, vectorization, target multi-versioning",
        "gcc13":  "GCC 13 release-notes examples",
        "gcc14":  "GCC 14 release-notes examples",
        "gcc15":  "GCC 15 release-notes examples",
        "gcc16":  "GCC 16 release-notes examples",
    }

    # Group examples by leaf folder; each leaf gets its own README.md.
    by_leaf: dict[Path, list[Example]] = {}
    for ex in examples:
        by_leaf.setdefault(ex.path.parent, []).append(ex)

    for leaf_dir, items in sorted(by_leaf.items()):
        # Bucket of the leaf is the bucket of its examples (all share one).
        bucket = items[0].standard_bucket
        title = bucket_titles.get(bucket, f"{bucket} examples")
        path = leaf_dir / "README.md"

        items.sort(key=lambda e: (e.topic, e.stem))
        by_topic: dict[str, list[Example]] = {}
        for ex in items:
            by_topic.setdefault(ex.topic, []).append(ex)

        rel_leaf = leaf_dir.relative_to(REPO_ROOT)
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


def report(eligible: list[Example], skipped: list[Example]) -> None:
    print(f"discovered: {len(eligible) + len(skipped)} examples")
    print(f"  eligible: {len(eligible)}")
    print(f"  skipped : {len(skipped)} (min-gcc / max-gcc filters)")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--gcc-version", type=int, default=None,
                    help="Override autodetected major version of g++")
    ap.add_argument("--dry-run", action="store_true",
                    help="Parse metadata only; do not compile or run")
    ap.add_argument("--emit-docs", action="store_true",
                    help="(Re)generate docs/cpp{20,23,26}.md indexes from metadata")
    ap.add_argument("--filter", default=None,
                    help="Only process examples whose topic or filename contains this substring")
    ap.add_argument("--show-cmds", action="store_true",
                    help="Print the exact g++ build command for each example, then exit")
    ap.add_argument("--verbose", action="store_true",
                    help="Print the full g++ command before each compile")
    ap.add_argument("--sanitize", default=None,
                    help="Comma-separated sanitizer list, e.g. 'undefined,address'. "
                         "Adds -fsanitize=… -fno-omit-frame-pointer -g to every build.")
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
        report(*filter_examples(examples, args.gcc_version or 99,
                                 args.filter, san_list))
        return 0

    if args.show_cmds:
        for ex in examples:
            cmd = build_cmd_for(ex, sanitize=args.sanitize)
            print(f"{ex.path.name}:")
            print(f"  {' '.join(cmd)}")
        return 0

    if shutil.which("g++") is None:
        print("error: g++ not found in PATH", file=sys.stderr)
        return 2

    gcc_version = args.gcc_version or detect_gcc_version()
    san_note = f" with sanitizers={args.sanitize}" if args.sanitize else ""
    print(f"using g++ major version {gcc_version}{san_note}")

    san_list = [s for s in (args.sanitize or "").split(",") if s]
    eligible, skipped = filter_examples(examples, gcc_version, args.filter, san_list)
    report(eligible, skipped)

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
    # example, foldable per-example via ::group::. Locally, only --verbose
    # opts in to the noise; the default is the one-line PASS/FAIL summary.
    verbose = args.verbose or in_gh

    def grp_start(title: str) -> None:
        if in_gh:
            print(f"::group::{title}")

    def grp_end() -> None:
        if in_gh:
            print("::endgroup::")

    for ex in eligible:
        label = f"[{ex.std}] {ex.path.name}"
        grp_start(label)
        ok, info = compile_one(ex, gcc_version, verbose=verbose,
                                sanitize=args.sanitize)
        if not ok:
            if ex.experimental:
                soft_fail_count += 1
                result = f"{ANSI_YELLOW}EXP-FAIL{ANSI_RESET} {label}: {info.splitlines()[0]}"
            else:
                fail_count += 1
                result = f"{ANSI_RED}FAIL    {ANSI_RESET} {label}"
                fail_lines.append(f"{label}\n{info}")
            print(result)             # last line inside the CI group / sole line locally
            grp_end()
            if in_gh and not ex.experimental:
                print(result)          # also outside the group so failures are visible without expanding
            continue
        ok, info = run_one(ex, info, verbose=verbose)
        if ok:
            pass_count += 1
            result = f"{ANSI_GREEN}PASS    {ANSI_RESET} {label}"
            print(result)
            grp_end()
        else:
            if ex.experimental:
                soft_fail_count += 1
                result = f"{ANSI_YELLOW}EXP-FAIL{ANSI_RESET} {label}"
                print(result)
                grp_end()
            else:
                fail_count += 1
                result = f"{ANSI_RED}FAIL    {ANSI_RESET} {label}"
                print(result)
                print(info)
                grp_end()
                print(result)          # outside the group too
                fail_lines.append(f"{label}\n{info}")

    for ex in skipped:
        skip_lines.append(f"{ANSI_DIM}SKIP    {ANSI_RESET} [{ex.std}] "
                          f"{ex.path.name} (needs gcc {ex.min_gcc}+)")

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
            print(line)

    return 1 if fail_count else 0


if __name__ == "__main__":
    sys.exit(main())
