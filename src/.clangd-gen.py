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

# Include files for Python details
python3_includes = subprocess.run(
    ["python3-config", "--includes"],
    capture_output=True,
    encoding="utf-8",
)

# Include files for Pybind11
pybind11_includes = subprocess.run(
    ["pybind11-config", "--includes"],
    capture_output=True,
    encoding="utf-8",
)

compile_flags = [
    "-std=c++17",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Wno-deprecated",
    "-D _IS_APYTYPES_UNIT_TEST",
    "-I test/",
    "-I/usr/include/python3.10",
    *str(python3_includes.stdout.strip()).split(" "),
    *str(pybind11_includes.stdout.strip()).split(" "),
]

# Produce the .clangd configuration
TAB = "    "
print("CompileFlags:\n" + TAB + "Add: [")
for flag in compile_flags:
    print(2 * TAB + flag + ",")
print(TAB + "]")
