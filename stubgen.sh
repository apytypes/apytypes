#!/usr/bin/env sh
python3 -m nanobind.stubgen -P -m apytypes -o lib/apytypes/__init__.pyi
python3 -m nanobind.stubgen -m apytypes._apytypes -o lib/apytypes/_apytypes.pyi
python3 -m nanobind.stubgen -m apytypes._array_functions -o lib/apytypes/_array_functions.pyi
