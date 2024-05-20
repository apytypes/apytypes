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
:class:`APyFloat` is a class for floating-point scalars with configurable formats. The
implementation is a generalization of the IEEE 754 standard, meaning that features like
subnormals, infinities, and NaN, are still supported. The format is defined
by the number of exponent and mantissa bits, and a non-negative bias. These fields are
named :attr:`exp_bits`, :attr:`man_bits`, and :attr:`bias` respectively. Similar to the hardware
representation of a floating-point number, the value is stored using three fields;
a sign bit :attr:`sign`, a biased exponent :attr:`exp`, and an integral mantissa with a hidden one :attr:`man`.
The value of a *normal* number would thus be

.. math::
    (-1)^{\texttt{sign}} \times 2^{\texttt{exp} - \texttt{bias}} \times (1 + \texttt{man} \times 2^{\texttt{-man_bits}}).

In general, if the bias is not explicitly given for a format :class:`APyFloat` will default to an IEEE-like bias
using the formula

.. math::
    \texttt{bias} = 2^{\texttt{exp_bits - 1}} - 1.

Arithmetic can be performed similarly to the operations of the built-in type
:class:`float` in Python. The resulting word length from operations will be
the same as the input operands', but if the operands do not share the same format, the resulting
bit widths of the exponent and mantissa field will be the maximum of its inputs:

.. code-block:: Python

    from apytypes import APyFloat
    a = APyFixed.from_float(1.25, exp_bits=5, man_bits=2)
    b = APyFixed.from_float(1.75, exp_bits=5, man_bits=2)
    # Operands with same format, result will have exp_bits=5, man_bits=2
    c = a + b

    d = APyFixed.from_float(1.75, exp_bits=4, man_bits=4)
    # Operands with different formats, result will have exp_bits=5, man_bits=4
    e = a + d


If the operands of an arithmetic operation have IEEE-like biases, then the result will
also have an IEEE-like bias -- based on the resulting number of exponent bits.
To support operations with biases deviating from the standard, the bias of the resulting format
is calculated as the "average" of the inputs' biases:

.. math::
    \texttt{bias}_3 = \frac{\left ( \left (\texttt{bias}_1 + 1 \right ) / 2^{\texttt{exp_bits}_1} + \left (\texttt{bias}_2 + 1 \right ) / 2^{\texttt{exp_bits}_2} \right ) \times 2^{\texttt{exp_bits}_3}}{2} - 1,

where exp_bits_1 and exp_bits_2 are the bit widths of the operands, bias_1 and bias_2 are the input biases, and exp_bits_3 is the target bit width.
Note that this formula still results in an IEEE-like bias when the inputs use IEEE-like biases.
"""

APyFloatArray.__doc__ = r"""
Class for multidimensional arrays with configurable floating-point formats.

:class:`APyFloatArray` is a class for multidimensional arrays with configurable
floating-point formats. The interface is much like the one of NumPy,
and direct plotting is supported by most functions in Matplotlib.
:class:`APyFloatArray` should always be preferred, if possible, when working with
arrays as it allows for better performance, and integration with other features of APyTypes.

For information about the workings of floating-point numbers, see its scalar
equivalent :class:`APyFloat`.
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
