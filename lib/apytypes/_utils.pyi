from collections.abc import Callable as _Callable, Sequence as _Sequence
from typing import Union

import apytypes._apytypes
from apytypes._apytypes import (
    APyCFixed as APyCFixed,
    APyFixed as APyFixed,
    APyFixedArray as APyFixedArray,
    APyFloat as APyFloat,
    APyFloatArray as APyFloatArray,
)

def fn(
    fn: _Callable,
    *args: Union[
        apytypes._apytypes.APyFloat,
        apytypes._apytypes.APyFixed,
        apytypes._apytypes.APyFixedArray,
        apytypes._apytypes.APyFloatArray,
    ],
) -> Union[
    apytypes._apytypes.APyFloat,
    apytypes._apytypes.APyFixed,
    apytypes._apytypes.APyFixedArray,
    apytypes._apytypes.APyFloatArray,
]:
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

def fp(
    value: Union[int, float, _Sequence[int], _Sequence[float]],
    exp_bits: int,
    man_bits: int,
    bias: Union[int, None] = None,
) -> Union[apytypes._apytypes.APyFloat, apytypes._apytypes.APyFloatArray]:
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

def fx(
    value: Union[int, float, _Sequence[int], _Sequence[float]],
    int_bits: Union[int, None] = None,
    frac_bits: Union[int, None] = None,
    bits: Union[int, None] = None,
    force_complex: bool = False,
) -> Union[
    apytypes._apytypes.APyCFixed,
    apytypes._apytypes.APyFixed,
    apytypes._apytypes.APyFixedArray,
]:
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
