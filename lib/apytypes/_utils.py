from apytypes._apytypes import APyFixed, APyFixedArray, APyFloat, APyFloatArray

from typing import Optional, Sequence, Union


def fx(
    value: Union[int, float, Sequence[int], Sequence[float]],
    int_bits: Optional[int] = None,
    frac_bits: Optional[int] = None,
    bits: Optional[int] = None,
) -> Union[APyFixed, APyFixedArray]:
    """
    Create an :class:`APyFixed` or :class:`APyFixedArray` object.

    Convenience method that applies :func:`APyFixed.from_float` or
    :func:`APyFixedArray.from_float` depending on if the input, *value*, is a scalar or not.

    Parameters
    ----------
    val : int, float | Sequence[int] | Sequence[float]
        Floating point value(s) to initialize from.
    int_bits : int, optional
        Number of integer bits in the created fixed-point object.
    frac_bits : int, optional
        Number of fractional bits in the created fixed-point object.
    bits : int, optional
        Total number of bits in the created fixed-point object.

    Returns
    -------
     :class:`APyFixed`,  :class:`APyFixedArray`

    .. versionadded:: 0.3
    """
    try:
        iter(value)
    except TypeError:
        return APyFixed.from_float(
            value, int_bits=int_bits, frac_bits=frac_bits, bits=bits
        )
    return APyFixedArray.from_float(
        value, int_bits=int_bits, frac_bits=frac_bits, bits=bits
    )


def fp(
    val: Union[int, float, Sequence[int], Sequence[float]],
    exp_bits: int,
    man_bits: int,
    bias: Optional[int] = None,
) -> Union[APyFloat, APyFloatArray]:
    """
    Create an :class:`APyFloat` or :class:`APyFloatArray` object.

    Convenience method that applies :func:`APyFloat.from_float` or
    :func:`APyFloatArray.from_float` depending on if the input, *value*, is a scalar or not.

    Parameters
    ----------
    val : int | float | Sequence[int] | Sequence[float]
        Floating point value(s) to initialize from.
    exp_bits : int
        Number of exponent bits.
    man_bits : int
        Number of mantissa bits.
    bias : int, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

    Returns
    -------
     :class:`APyFloat`,  :class:`APyFloatArray`

    .. versionadded:: 0.3
    """
    try:
        iter(val)
    except TypeError:
        return APyFloat.from_float(val, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    return APyFloatArray.from_float(
        val, exp_bits=exp_bits, man_bits=man_bits, bias=bias
    )
