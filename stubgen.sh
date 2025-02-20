#!/usr/bin/env sh
python3 -c "from nanobind import __version__ as v; print(f'Nanobind version: {v}')"
python3 -m nanobind.stubgen -m apytypes._apytypes -o lib/apytypes/_apytypes.pyi
