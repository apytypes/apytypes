from __future__ import annotations

from apytypes._apytypes import (
    APyFixed,
    APyFixedArray,
    APyFixedAccumulatorContext,
    APyFixedCastContext,
    APyFloat,
    APyFloatArray,
    APyFloatAccumulatorContext,
    OverflowMode,
    APyFloatQuantizationContext,
    QuantizationMode,
    get_float_quantization_mode,
    set_float_quantization_mode,
    get_float_quantization_seed,
    set_float_quantization_seed,
    _get_simd_version_str,
)

from apytypes._version import version as __version__

__all__ = [
    "APyFixed",
    "APyFixedArray",
    "APyFixedAccumulatorContext",
    "APyFixedCastContext",
    "APyFloat",
    "APyFloatArray",
    "APyFloatAccumulatorContext",
    "OverflowMode",
    "APyFloatQuantizationContext",
    "QuantizationMode",
    "__version__",
    "get_float_quantization_mode",
    "set_float_quantization_mode",
    "get_float_quantization_seed",
    "set_float_quantization_seed",
    "_get_simd_version_str",
]

APyFloat.__doc__ = r"""
Class for configurable floating-point formats.
"""

APyFixed.__doc__ = r"""
Class for configurable scalar fixed-point formats.

:class:`APyFixed` is an arbitrary precision two's complement fixed-point scalar type. In
many ways it behaves like the built-in Python types :class:`int` and :class:`float`, in
that it can be used within ordinary arithmetic expressions. Every fixed-point instance
has an associated word length, determined by its :attr:`bits`, :attr:`int_bits`, and
:attr:`frac_bits` bit specifiers. These specifiers determine the location of the binary
fix-point and the total word length. Only two of three bit specifers need to be set to
uniquely determine the complete fixed-point format.

In general, the fixed-point representation is described by:

.. math::
    \underbrace{
        \underbrace{
            x_{n-1} \; x_{n-2} \; \ldots \; x_{k+1} \; x_{k}
        }_{\text{int_bits}}
        \; . \;
        \underbrace{
            x_{k-1} \; x_{k-2} \; \ldots \; x_{1} \; x_{0}
        }_{\text{frac_bits}}
    }_{\text{bits}}

The following is an example of a fixed-point number with :code:`bits=8`,
:code:`int_bits=5`, and :code:`frac_bits=3`, that has a stored value of -6.625:

.. math::
    \begin{align*}
        1 \, 1 \, 0 \, 0 \, 1 \,.\, 0 \, 1 \, 1_{2} = \frac{-53_{10}}{2^3} = -6.625_{10}
    \end{align*}

APyFixed uses static word-length inference to determine word lengths of results to
arithmetic operations. This ensures that overflow or quantization **never** occurs
unless explicitly instructed to by a user through the :func:`cast` method. The following
table shows word lengths of elementary arithmetic operations.

.. list-table:: Word-length of fixed-point arithmetic operations
   :widths: 12 44 44
   :header-rows: 1

   * - Operation
     - Result :attr:`int_bits`
     - Result :attr:`frac_bits`
   * - :code:`a + b`
     - :code:`max(a.int_bits, b.int_bits) + 1`
     - :code:`max(a.frac_bits, b.frac_bits)`
   * - :code:`a - b`
     - :code:`max(a.int_bits, b.int_bits) + 1`
     - :code:`max(a.frac_bits, b.frac_bits)`
   * - :code:`a * b`
     - :code:`a.int_bits + b.int_bits`
     - :code:`a.frac_bits + b.frac_bits`
   * - :code:`a / b`
     - :code:`a.int_bits + b.frac_bits`
     - :code:`a.frac_bits + b.int_bits`
   * - :code:`-a`
     - :code:`a.int_bits + 1`
     - :code:`a.frac_bits`
"""
