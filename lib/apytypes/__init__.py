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

from apytypes._array_functions import (
    squeeze,
    reshape,
    convolve,
    shape,
    ravel,
    transpose,
    moveaxis,
    swapaxes,
    expand_dims,
    zeros,
    zeros_like,
    ones,
    ones_like,
    eye,
    identity,
    full,
    full_like,
    arange,
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
    "squeeze",
    "convolve",
    "reshape",
    "shape",
    "ravel",
    "transpose",
    "moveaxis",
    "swapaxes",
    "expand_dims",
    "zeros",
    "zeros_like",
    "ones",
    "ones_like",
    "eye",
    "identity",
    "full",
    "full_like",
    "arange",
]

APyFloat.__doc__ = r"""
Floating-point scalar with configurable format.

The implementation is a generalization of the IEEE 754 standard, meaning that features like
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
the same as the input operands' by quantizing to nearest number with ties to even (:class:`QuantizationMode.TIES_EVEN`).
If the operands do not share the same format, the resulting
bit widths of the exponent and mantissa field will be the maximum of its inputs:

Examples
--------

>>> from apytypes import APyFloat
>>> a = APyFloat.from_float(1.25, exp_bits=5, man_bits=2)
>>> b = APyFloat.from_float(1.75, exp_bits=5, man_bits=2)

Operands with same format, result will have exp_bits=5, man_bits=2

>>> a + b
APyFloat(sign=0, exp=16, man=2, exp_bits=5, man_bits=2)

>>> d = APyFloat.from_float(1.75, exp_bits=4, man_bits=4)

Operands with different formats, result will have exp_bits=5, man_bits=4

>>> a + d
APyFloat(sign=0, exp=16, man=8, exp_bits=5, man_bits=4)

If the operands of an arithmetic operation have IEEE-like biases, then the result will
also have an IEEE-like bias -- based on the resulting number of exponent bits.
To support operations with biases deviating from the standard, the bias of the resulting format
is calculated as the "average" of the inputs' biases:

.. math::
    \texttt{bias}_3 = \frac{\left ( \left (\texttt{bias}_1 + 1 \right ) / 2^{\texttt{exp_bits}_1} + \left (\texttt{bias}_2 + 1 \right ) / 2^{\texttt{exp_bits}_2} \right ) \times 2^{\texttt{exp_bits}_3}}{2} - 1,

where :math:`\texttt{exp_bits}_1` and :math:`\texttt{exp_bits}_2` are the bit widths of the operands, :math:`\texttt{bias}_1` and :math:`\texttt{bias}_2` are the
input biases, and :math:`\texttt{exp_bits}_3` is the target bit width.
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

APyFloatQuantizationContext.__doc__ = r"""
Context for changing the quantization mode used for floating-point operations.

If not specified explicitly, floating-point operations will use the quantization mode that is set globally,
which is :class:`QuantizationMode.TIES_EVEN` by default. The mode however can be changed using the static method
:func:`apytypes.set_float_quantization_mode`, or, preferably, by using a so-called quantization context.
With a quantization context one can change the quantization mode used by all operations within a specific section
in the code.

Examples
--------

>>> from apytypes import APyFloat, QuantizationMode
>>> from apytypes import APyFloatQuantizationContext

Addition using the default round to nearest, ties even

>>> a = APyFloat.from_float(0.123, exp_bits=5, man_bits=10)
>>> b = APyFloat.from_float(3.21, exp_bits=5, man_bits=10)
>>> a + b
APyFloat(sign=0, exp=16, man=683, exp_bits=5, man_bits=10)

Addition using round towards zero

>>> m = QuantizationMode.TO_ZERO
>>> with APyFloatQuantizationContext(quantization=m):
...     a + b
APyFloat(sign=0, exp=16, man=682, exp_bits=5, man_bits=10)

Stochastic rounding with an optional seed

>>> m = QuantizationMode.STOCH_WEIGHTED
>>> s = 0x1234
>>> with APyFloatQuantizationContext(quantization=m, quantization_seed=s):
...     a + b
APyFloat(sign=0, exp=16, man=683, exp_bits=5, man_bits=10)

The quantization mode is reverted automatically upon exiting the context.
Nesting the contexts is also possible.
"""

APyFloatAccumulatorContext.__doc__ = r"""
Context for using custom accumulators when performing inner products and matrix multiplications.

Inner products and matrix multiplications will by default perform the summation in the resulting format
of the operands, but with :class:`APyFloatAccumulatorContext` a custom accumulator can be simulated
as seen in the example below.

Examples
--------

>>> import numpy as np
>>> from apytypes import APyFloatArray, QuantizationMode
>>> from apytypes import APyFloatAccumulatorContext

>>> An = np.random.normal(1, 2, size=(100, 100))
>>> A = APyFloatArray.from_float(An, exp_bits=5, man_bits=10)

>>> bn = np.random.uniform(0, 1, size=100),
>>> b = APyFloatArray.from_float(bn, exp_bits=5, man_bits=10)

Normal matrix multiplication

>>> c = A @ b.T

Matrix multiplication using stochastic quantization and a wider accumulator

>>> m = QuantizationMode.STOCH_WEIGHTED
>>> with APyFloatAccumulatorContext(exp_bits=6, man_bits=15, quantization=m):
...     d = A @ b.T


If no quantization mode is specified to the accumulator context it will fallback to the mode set globally,
see :class:`APyFloatQuantizationContext`.
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
        }_{\mathrm{int\_bits}}
        \; . \;
        \underbrace{
            x_{k-1} \; x_{k-2} \; \ldots \; x_{1} \; x_{0}
        }_{\mathrm{frac\_bits}}
    }_{\mathrm{bits}}

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
