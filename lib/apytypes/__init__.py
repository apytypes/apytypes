from __future__ import annotations

from apytypes._apytypes import (
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
    AccumulatorContext,
    OverflowMode,
    RoundingContext,
    RoundingMode,
    get_rounding_mode,
    set_rounding_mode,
    get_rounding_seed,
    set_rounding_seed,
)

from apytypes._version import version as __version__

__all__ = [
    "APyFixed",
    "APyFixedArray",
    "APyFloat",
    "APyFloatArray",
    "AccumulatorContext",
    "OverflowMode",
    "RoundingContext",
    "RoundingMode",
    "__version__",
    "get_rounding_mode",
    "set_rounding_mode",
    "get_rounding_seed",
    "set_rounding_seed",
]

APyFloat.__doc__ = """
Class for configurable floating-point formats.
"""

APyFixed.__doc__ = """
Class for configurable fixed-point formats.
"""
