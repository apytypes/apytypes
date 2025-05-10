#!/usr/bin/env python3
#
# As Clangd does not support loading environment variables from the shell, and since
# different systems have libraries at different locations, this intermediate script can
# be used to create a `.clangd` configuration on the fly.
#
# Generate a new `.clangd` configuration:
# > python3 .clangd-gen.py > .clangd
#
# Author: Mikael Henriksson (2024)
#

import subprocess
import sys
from pathlib import Path

APYTYPES_DIR = Path(__file__).parent

# Include files for Python details using `python3-config`
python3_includes = subprocess.run(
    ["python3-config", "--includes"],
    capture_output=True,
    encoding="utf-8",
    check=False,
)
if python3_includes.returncode != 0:
    print(
        f"[ WARNING ]: Running `python3-config --includes` failed with return code "
        f"{python3_includes.returncode}",
        file=sys.stderr,
    )


# Include files for Nanobind
nanobind_includes = subprocess.run(
    ["python3", "-m", "nanobind", "--include_dir"],
    capture_output=True,
    encoding="utf-8",
    check=False,
)
if nanobind_includes.returncode != 0:
    print(
        f"[ WARNING ]: Running `python3 -m nanobind --include_dir` failed with return "
        f"code {nanobind_includes.returncode}",
        file=sys.stderr,
    )


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
    f"-I{APYTYPES_DIR}/subprojects/fmt-11.2.0/include",
    f"-I{APYTYPES_DIR}/subprojects/highway",
]

# Produce the .clangd configuration
TAB = "    "
print("CompileFlags:\n" + TAB + "Add: [")
for flag in compile_flags:
    print(2 * TAB + flag + ",")
print(TAB + "]")
