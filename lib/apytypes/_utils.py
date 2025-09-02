import math
import warnings
from collections.abc import Callable, Iterable
from functools import partial
from typing import Any, Literal, overload

from apytypes._apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyCFloat,
    APyCFloatArray,
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
)


@overload
def fx(
    value: int | float,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    *,
    force_complex: Literal[False] = False,
) -> APyFixed: ...


@overload
def fx(
    value: int | float,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    *,
    force_complex: Literal[True],
) -> APyCFixed: ...


@overload
def fx(
    value: complex,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    *,
    force_complex: bool = False,
) -> APyCFixed: ...


@overload
def fx(
    value: Iterable[Any],
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    *,
    force_complex: Literal[False] = False,
) -> APyFixedArray: ...


@overload
def fx(
    value: Iterable[Any],
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    *,
    force_complex: Literal[True],
) -> APyCFixedArray: ...


def fx(
    value: int | float | complex | Iterable[Any],
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    *,
    force_complex: bool = False,
) -> APyFixed | APyCFixed | APyFixedArray | APyCFixedArray:
    """
    Create a fixed-point scalar or array.

    Convenience method that passes `value` and bit-specifiers to one of:

    * :func:`APyFixed.from_float`
    * :func:`APyFixedArray.from_float`
    * :func:`APyCFixed.from_complex`
    * :func:`APyCFixedArray.from_complex`

    depending on `value`. Returns :class:`APyFixed` when `value` is :class:`int` or
    :class:`float`. Returns :class:`APyCFixed` when `value` is :class:`complex` or
    *force_complex* is True. Returns :class:`APyFixedArray` or :class:`APyCFixedArray`
    when `value` is a sequence of numbers.

    .. versionadded:: 0.3

    .. hint::
        Currently, this function will not detect sequences of complex values. Set
        *force_complex* to True.

    Parameters
    ----------
    val : :class:`int`, :class:`float`, list of int or float, ndarray
        Floating-point/integer/complex value(s) to initialize from.
    int_bits : :class:`int`, optional
        Number of integer bits in the created fixed-point object.
    frac_bits : :class:`int`, optional
        Number of fractional bits in the created fixed-point object.
    bits : :class:`int`, optional
        Total number of bits in the created fixed-point object.
    force_complex : :class:`bool`, default: False
        If True, force the return value to be :class:`APyCFixed` or
        :class:`APyCFixedArray`, even if *value* is real.

    Returns
    -------
    :class:`APyFixed`, :class:`APyFixedArray`, :class:`APyCFixed`, or
    :class:`APyCFixedArray`

    """
    if isinstance(value, Iterable):
        if force_complex:
            return APyCFixedArray.from_complex(value, int_bits, frac_bits, bits)
        else:
            return APyFixedArray.from_float(value, int_bits, frac_bits, bits)
    else:
        if isinstance(value, complex):
            return APyCFixed.from_complex(value, int_bits, frac_bits, bits)
        else:
            if force_complex:
                return APyCFixed.from_complex(value, int_bits, frac_bits, bits)
            else:
                return APyFixed.from_float(value, int_bits, frac_bits, bits)


@overload
def fp(
    value: int | float,
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    *,
    force_complex: Literal[False] = False,
) -> APyFloat: ...


@overload
def fp(
    value: int | float,
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    *,
    force_complex: Literal[True],
) -> APyCFloat: ...


@overload
def fp(
    value: complex,
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    *,
    force_complex: bool = False,
) -> APyCFloat: ...


@overload
def fp(
    value: Iterable[Any],
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    *,
    force_complex: Literal[False] = False,
) -> APyFloatArray: ...


@overload
def fp(
    value: Iterable[Any],
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    *,
    force_complex: Literal[True],
) -> APyCFloatArray: ...


def fp(
    value: int | float | complex | Iterable[Any],
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    *,
    force_complex: bool = False,
) -> APyFloat | APyCFloat | APyFloatArray | APyCFloatArray:
    """
    Create a floating-point scalar or array.

    Convenience method that passes `value` and bit-specifiers to one of:

    * :func:`APyFloat.from_float`
    * :func:`APyFloatArray.from_float`
    * :func:`APyCFloat.from_complex`
    * :func:`APyCFloatArray.from_complex`

    depending on `value`. Returns :class:`APyFloat` when `value` is :class:`int` or
    :class:`float`. Returns :class:`APyCFloat` when `value` is :class:`complex` or when
    *is_complex* is True. Returns :class:`APyFloatArray` or :class:`APyCFloatArray` when
    `value` is a sequence of numbers.

    .. versionadded:: 0.3

    .. hint::
        Currently, this function will not detect sequences of complex values. Set
        *force_complex* to True.

    Parameters
    ----------
    value : :class:`int`, :class:`float`, list of int or float, ndarray
        Floating-point/integer/complex value(s) to initialize from.
    exp_bits : :class:`int`
        Number of exponent bits.
    man_bits : :class:`int`
        Number of mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.
    force_complex : :class:`bool`, default: False
        If True, force the return value to be :class:`APyCFloat` or
        :class:`APyCFloatArray`, even if *value* is real.

    Returns
    -------
    :class:`APyFloat`, :class:`APyFloatArray`, :class:`APyCFloat`, or
    :class:`APyCFloatArray`

    """
    if isinstance(value, Iterable):
        if force_complex:
            return APyCFloatArray.from_complex(value, exp_bits, man_bits, bias)
        else:
            return APyFloatArray.from_float(value, exp_bits, man_bits, bias)
    else:
        if isinstance(value, complex):
            return APyCFloat.from_complex(value, exp_bits, man_bits, bias)
        else:
            if force_complex:
                return APyCFloat.from_complex(value, exp_bits, man_bits, bias)
            else:
                return APyFloat.from_float(value, exp_bits, man_bits, bias)


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
    fn: Callable[[int | float], int | float],
    *args: APyFloat | APyFixed | APyFixedArray | APyFloatArray,
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
