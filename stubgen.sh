#!/usr/bin/env sh
SCRIPT_DIR="$(dirname "$0")"
STUB_FILE="$SCRIPT_DIR/lib/apytypes/_apytypes.pyi"

python3 -c "from nanobind import __version__ as v; print(f'Nanobind version: {v}')"
python3 -m nanobind.stubgen -m apytypes._apytypes -o "$STUB_FILE"

ruff format "$STUB_FILE"
ruff check --fix "$STUB_FILE"

# Replace the string literal for convolution mode with a Literal type that includes the possible values.
sed -i 's/str\s*=\s*"full"/Literal["full", "same", "valid"] = "full"/' "$STUB_FILE"

# And import Literal from typing
sed -i 's/from typing import Annotated, Any, overload/from typing import Annotated, Any, Literal, overload/' "$STUB_FILE"

# Format again
ruff format "$STUB_FILE"
