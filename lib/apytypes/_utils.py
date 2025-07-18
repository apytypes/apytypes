import math
import warnings
from collections.abc import Callable, Sequence
from functools import partial

from apytypes._apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
)


def fx(
    value: int | float | Sequence[int] | Sequence[float],
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    force_complex: bool = False,
) -> APyCFixed | APyFixed | APyCFixedArray | APyFixedArray:
    """
    Create an :class:`APyFixed`, :class:`APyCFixed` or :class:`APyFixedArray` object.

    Convenience method that applies :func:`APyFixed.from_float`,
    :func:`APyCFixed.from_complex` or :func:`APyFixedArray.from_float`, or
    :func:`APyFixedArray.from_complex` depending on if the input, *value*, is a scalar
    or not. For scalar values, return :class:`APyCFixed` if *value* is complex or if
    *force_complex* is True.

    .. versionadded:: 0.3

    .. hint::
        Currently, this function will not detect sequences of complex values. Set
        *force_complex* to True.

    Parameters
    ----------
    val : :class:`int`, :class:`float`, list of int or float, ndarray
        Floating-point/integer value(s) to initialize from.
    int_bits : :class:`int`, optional
        Number of integer bits in the created fixed-point object.
    frac_bits : :class:`int`, optional
        Number of fractional bits in the created fixed-point object.
    bits : :class:`int`, optional
        Total number of bits in the created fixed-point object.
    force_complex : :class:`bool`, default: False
        If True, force the return value to be :class:`APyCFixed` even if *value* is
        real.

    Returns
    -------
    :class:`APyFixed`, :class:`APyFixedArray`

    """
    try:
        iter(value)
    except TypeError:
        if force_complex or isinstance(value, complex):
            return APyCFixed.from_complex(
                value, int_bits=int_bits, frac_bits=frac_bits, bits=bits
            )
        return APyFixed.from_float(
            value, int_bits=int_bits, frac_bits=frac_bits, bits=bits
        )
    if force_complex:
        return APyCFixedArray.from_complex(
            value, int_bits=int_bits, frac_bits=frac_bits, bits=bits
        )

    return APyFixedArray.from_float(
        value, int_bits=int_bits, frac_bits=frac_bits, bits=bits
    )


def fp(
    value: int | float | Sequence[int] | Sequence[float],
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
) -> APyFloat | APyFloatArray:
    """
    Create an :class:`APyFloat` or :class:`APyFloatArray` object.

    Convenience method that applies :func:`APyFloat.from_float` or
    :func:`APyFloatArray.from_float` depending on if the input, *value*, is a scalar or not.

    .. versionadded:: 0.3

    Parameters
    ----------
    value : :class:`int`, :class:`float`, list of int or float, ndarray
        Floating-point/integer value(s) to initialize from.
    exp_bits : :class:`int`
        Number of exponent bits.
    man_bits : :class:`int`
        Number of mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

    Returns
    -------
    :class:`APyFloat`, :class:`APyFloatArray`

    """
    try:
        iter(value)
    except TypeError:
        return APyFloat.from_float(
            value, exp_bits=exp_bits, man_bits=man_bits, bias=bias
        )
    return APyFloatArray.from_float(
        value, exp_bits=exp_bits, man_bits=man_bits, bias=bias
    )


def _process_args(
    args: tuple[APyFloat | APyFixed | APyFixedArray | APyFloatArray, ...],
) -> tuple[list[float], partial]:
    fargs = []
    exp_bits = -math.inf
    man_bits = -math.inf
    frac_bits = -math.inf
    int_bits = -math.inf
    fn = None
    bias = None
    for arg in args:
        if isinstance(arg, APyFixed):
            if fn == fp:
                warnings.warn(
                    "Mix of APyFloat(Array) and APyFixed arguments, will return APyFloat(Array).",
                    UserWarning,
                )
            else:
                fn = fx
            int_bits = max(arg.int_bits, int_bits)
            frac_bits = max(arg.frac_bits, frac_bits)
            fargs.append(float(arg))
        elif isinstance(arg, APyFixedArray):
            if fn == fp:
                warnings.warn(
                    "Mix of APyFloat(Array) and APyFixedArray arguments, will return APyFloat(Array).",
                    UserWarning,
                )
            else:
                fn = fx
            int_bits = max(arg.int_bits, int_bits)
            frac_bits = max(arg.frac_bits, frac_bits)
            fargs.append(arg.to_numpy())
        elif isinstance(arg, APyFloat):
            if fn == fx:
                warnings.warn(
                    "Mix of APyFixed(Array) and APyFloat arguments, will return APyFixed(Array).",
                    UserWarning,
                )
            else:
                fn = fp
            man_bits = max(arg.man_bits, man_bits)
            exp_bits = max(arg.exp_bits, exp_bits)
            bias = arg.bias
            fargs.append(float(arg))
        elif isinstance(arg, APyFloatArray):
            if fn == fx:
                warnings.warn(
                    "Mix of APyFixed(Array) and APyFloatArray arguments, will return APyFixed(Array).",
                    UserWarning,
                )
            else:
                fn = fp
            man_bits = max(arg.man_bits, man_bits)
            exp_bits = max(arg.exp_bits, exp_bits)
            bias = arg.bias
            fargs.append(arg.to_numpy())

    if fn == fx:
        return fargs, partial(fx, int_bits=int(int_bits), frac_bits=int(frac_bits))
    elif fn == fp:
        return fargs, partial(
            fp, exp_bits=int(exp_bits), man_bits=int(man_bits), bias=bias
        )
    else:
        raise TypeError("Expected APyTypes arguments")


def fn(
    fn: Callable, *args: APyFloat | APyFixed | APyFixedArray | APyFloatArray
) -> APyFloat | APyFixed | APyFixedArray | APyFloatArray:
    """
    Utility function to evaluate functions on arguments and convert back.

    This does exactly:

    1. Convert argument(s) to float
    2. Evaluate function with float argument(s)
    3. Convert result back to the same type as the argument

    Hence, there may be numerical issues, but it provides a simple way to
    perform, e.g., ``sin``, assuming that it is based on a look-up table.

    .. versionadded:: 0.3

    Parameters
    ----------
    fn : callable
       The function to evaluate.
    args: APyFixed, APyFloat, APyFixedArray, APyFloatArray
       The argument(s) to evaluate the function for.

    Examples
    --------
    >>> import apytypes as apy
    >>> import math
    >>> a = apy.APyFixed(19, 1, 5)
    >>> apy.fn(math.sin, a)
    APyFixed(18, bits=6, int_bits=1)

    Returns
    -------
    :class:`APyFloat`, :class:`APyFloatArray`, :class:`APyFixed`, :class:`APyFixedArray`

    """
    pargs, conv = _process_args(args)
    fval = fn(*pargs)
    return conv(fval)
