from __future__ import annotations

from apytypes._apytypes import (
    APyFloat,
    APyFixed,
    APyFixedArray,
    RoundingContext,
    RoundingMode,
    OverflowMode,
    get_rounding_mode,
    set_rounding_mode,
)

from apytypes._version import version as __version__

__all__ = [
    "APyFloat",
    "APyFixed",
    "APyFixedArray",
    "OverflowMode",
    "RoundingContext",
    "RoundingMode",
    "get_rounding_mode",
    "set_rounding_mode",
    "__version__",
]

APyFloat.__doc__ = """
Class for configurable floating-point formats.
"""

APyFixed.__doc__ = """
Class for configurable fixed-point formats.
"""
