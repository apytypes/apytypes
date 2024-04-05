from __future__ import annotations

from apytypes._apytypes import (
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
    AccumulatorContext,
    OverflowMode,
    QuantizationContext,
    QuantizationMode,
    get_quantization_mode,
    set_quantization_mode,
    get_quantization_seed,
    set_quantization_seed,
    _get_simd_version_str,
)

from apytypes._version import version as __version__

__all__ = [
    "APyFixed",
    "APyFixedArray",
    "APyFloat",
    "APyFloatArray",
    "AccumulatorContext",
    "OverflowMode",
    "QuantizationContext",
    "QuantizationMode",
    "__version__",
    "get_quantization_mode",
    "set_quantization_mode",
    "get_quantization_seed",
    "set_quantization_seed",
    "_get_simd_version_str",
]

APyFloat.__doc__ = """
Class for configurable floating-point formats.
"""

APyFixed.__doc__ = """
Class for configurable fixed-point formats.
"""
