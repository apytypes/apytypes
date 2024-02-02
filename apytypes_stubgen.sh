#!/usr/bin/env bash
#
if ! command -v pybind11-stubgen; then
    echo "Failure: 'pybind11-stubgen' not in PATH"
    exit 1
fi

pybind11-stubgen -o lib apytypes
black lib/apytypes/*.pyi
