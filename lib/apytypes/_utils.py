from functools import partial
import math
from typing import Callable, List, Optional, Sequence, Tuple, Union
import warnings

from apytypes._apytypes import (
    APyCFixed,
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
)


def fx(
    value: Union[int, float, Sequence[int], Sequence[float]],
    int_bits: Optional[int] = None,
    frac_bits: Optional[int] = None,
    bits: Optional[int] = None,
    force_complex: bool = False,
) -> Union[APyCFixed, APyFixed, APyFixedArray]:
    """
    Create an :class:`APyFixed`, :class:`APyCFixed` or :class:`APyFixedArray` object.

    Convenience method that applies :func:`APyFixed.from_float`, :func:`APyCFixed.from_complex` or
    :func:`APyFixedArray.from_float` depending on if the input, *value*, is a scalar or not.
    For scalar values, return :class:`APyCFixed` if *value* is complex or if
    *force_complex* is True.

    .. versionadded:: 0.3

    Parameters
    ----------
    val : int, float, list(int), list(float)
        Floating point value(s) to initialize from.
    int_bits : int, optional
        Number of integer bits in the created fixed-point object.
    frac_bits : int, optional
        Number of fractional bits in the created fixed-point object.
    bits : int, optional
        Total number of bits in the created fixed-point object.
    force_complex : bool, default: False
        If True, force the return value to be :class:`APyCFixed` even if *value* is real.

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
    return APyFixedArray.from_float(
        value, int_bits=int_bits, frac_bits=frac_bits, bits=bits
    )


def fp(
    value: Union[int, float, Sequence[int], Sequence[float]],
    exp_bits: int,
    man_bits: int,
    bias: Optional[int] = None,
) -> Union[APyFloat, APyFloatArray]:
    """
    Create an :class:`APyFloat` or :class:`APyFloatArray` object.

    Convenience method that applies :func:`APyFloat.from_float` or
    :func:`APyFloatArray.from_float` depending on if the input, *value*, is a scalar or not.

    .. versionadded:: 0.3

    Parameters
    ----------
    value : int, float, list(int), list(float)
        Floating point value(s) to initialize from.
    exp_bits : int
        Number of exponent bits.
    man_bits : int
        Number of mantissa bits.
    bias : int, optional
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


def _process_args(args) -> Tuple[List[float], partial]:
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
    fn: Callable, *args: Union[APyFloat, APyFixed, APyFixedArray, APyFloatArray]
) -> Union[APyFloat, APyFixed, APyFixedArray, APyFloatArray]:
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
    >>> from apytypes import APyFixed, fn
    >>> import math
    >>> a = APyFixed(19, 1, 5)
    >>> fn(math.sin, a)
    APyFixed(18, bits=6, int_bits=1)

    Returns
    -------
    :class:`APyFloat`, :class:`APyFloatArray`, :class:`APyFixed`, :class:`APyFixedArray`

    """
    pargs, conv = _process_args(args)
    fval = fn(*pargs)
    return conv(fval)
