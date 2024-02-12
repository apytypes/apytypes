#!/usr/bin/env bash
#
if ! command -v pybind11-stubgen; then
    echo "Failure: 'pybind11-stubgen' not in PATH"
    exit 1
fi

pybind11-stubgen -o lib apytypes                        \
    --enum-class-location "QuantizationMode:apytypes"   \
    --enum-class-location "OverflowMode:apytypes"

# Hack the OverflowMode and QuantizationMode enums correct. Seems to a bug related to
# pybind11-stubgen. Possibly related issues:
# https://github.com/sizmailov/pybind11-stubgen/pull/196
# https://github.com/sizmailov/pybind11-stubgen/issues/207
sed -i 's/apytypes.QuantizationMode/QuantizationMode/g' lib/apytypes/_apytypes.pyi
sed -i 's/apytypes.OverflowMode/OverflowMode/g' lib/apytypes/_apytypes.pyi

# Format using black
black lib/apytypes/*.pyi
