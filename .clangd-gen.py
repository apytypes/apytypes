#!/usr/bin/env python3
#
# As Clangd does not support loading environment variables from the shell, and since
# different systems have libraries at different locations, this intermediate script can
# be used to create a `.clangd` configuration on the fly.
#
# Generate a new `.clangd` configuration:
# > python3 .clangd-gen.py > .clangd
#
# APyTypes authors (2024, 2025)
#

import configparser
import subprocess
import sys
from pathlib import Path

APYTYPES_DIR = Path(__file__).parent


def run_shell(cmd: str) -> subprocess.CompletedProcess[str]:
    """
    Run a shell command and retrieve its stdout in a `list` of `str`.
    """
    run = subprocess.run
    res = run(cmd.split(" "), capture_output=True, encoding="utf-8", check=False)
    if res.returncode != 0:
        print(f"[ WARNING ]: `{cmd}` failed; code {res.returncode}", file=sys.stderr)
    return res


def include_from_subproject(wrap_file: str) -> str:
    """
    Read a Meson wrap configuration file and extract the include directory from it.
    """
    parser = configparser.ConfigParser()
    _ = parser.read(f"{APYTYPES_DIR}/{wrap_file}")
    try:
        return parser["wrap-file"]["directory"]
    except KeyError:
        print(f"[ WARNING ]: subproject include fail; {wrap_file}", file=sys.stderr)
        return ""


# Include files for the APyTypes project
python3_includes = run_shell("python3-config --includes")
nanobind_includes = run_shell("python3 -m nanobind --include_dir")
fmt_include = include_from_subproject("subprojects/fmt.wrap")
highway_include = include_from_subproject("subprojects/highway.wrap")
threadpool_include = include_from_subproject("subprojects/bshoshany-thread-pool.wrap")

compile_flags = [
    "-std=c++17",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Wno-deprecated",
    "-DCOMPILER_LIMB_SIZE=NATIVE",
    "-DNANOBIND_LEAK_WARNINGS=0",
    *str(python3_includes.stdout.strip()).split(" "),
    *["-I" + s for s in nanobind_includes.stdout.strip().split(" ")],
    f"-I{APYTYPES_DIR}/subprojects/{fmt_include}/include",
    f"-I{APYTYPES_DIR}/subprojects/{highway_include}",
    f"-I{APYTYPES_DIR}/subprojects/{threadpool_include}/include",
]

# Produce the .clangd configuration
TAB = "    "
print("CompileFlags:\n" + TAB + "Add: [")
for flag in compile_flags:
    print(2 * TAB + flag + ",")
print(TAB + "]")
