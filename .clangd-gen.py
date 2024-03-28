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

import os

# Include files for Python details using `python3-config`
python3_includes = subprocess.run(
    ["python3-config", "--includes"],
    capture_output=True,
    encoding="utf-8",
)

# Include files for Nanobind
nanobind_includes = subprocess.run(
    ["python3", "-m", "nanobind", "--include_dir"],
    capture_output=True,
    encoding="utf-8",
)

compile_flags = [
    "-std=c++17",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Wno-deprecated",
    f"-I{os.path.abspath('./subprojects/fmt-10.2.0/include')}",
    *str(python3_includes.stdout.strip()).split(" "),
    *["-I" + s for s in nanobind_includes.stdout.strip().split(" ")],
]

# Produce the .clangd configuration
TAB = "    "
print("CompileFlags:\n" + TAB + "Add: [")
for flag in compile_flags:
    print(2 * TAB + flag + ",")
print(TAB + "]")
