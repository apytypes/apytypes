#!/usr/bin/env bash
#
if ! command -v stubgen; then
    echo "Failure: 'pybind11-stubgen' not in PATH"
    exit 1
fi

###
### pybind11-stubgen
###
pybind11-stubgen -o lib apytypes                        \
    --enum-class-location "QuantizationMode:apytypes"   \
    --enum-class-location "OverflowMode:apytypes"
sed -i 's/apytypes.QuantizationMode/QuantizationMode/g' lib/apytypes/_apytypes.pyi
sed -i 's/apytypes.OverflowMode/OverflowMode/g' lib/apytypes/_apytypes.pyi

###
### MyPy stubgen
###
#stubgen -o lib -m apytypes
#stubgen -o lib -m apytypes._apytypes

###
### Nanobind stubgen
###
#nanobind-stubgen --out lib apytypes

# Format using black
black lib/apytypes/*.pyi
