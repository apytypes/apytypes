import csv
from collections.abc import Sequence
from pathlib import Path
from typing import Literal, overload

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
from apytypes._typing import APyArray, APyScalar


@overload
def squeeze(
    a: APyFixedArray, axis: int | tuple[int, ...] | None = None
) -> APyFixedArray: ...
@overload
def squeeze(
    a: APyFloatArray, axis: int | tuple[int, ...] | None = None
) -> APyFloatArray: ...
@overload
def squeeze(
    a: APyCFloatArray, axis: int | tuple[int, ...] | None = None
) -> APyCFloatArray: ...
@overload
def squeeze(
    a: APyCFixedArray, axis: int | tuple[int, ...] | None = None
) -> APyCFixedArray: ...


def squeeze(a: APyArray, axis: int | tuple[int, ...] | None = None) -> APyArray:
    """
    Remove axes of length one from `a`.

    Parameters
    ----------
    a : :class:`APyFloatArray`, :class:`APyFixedArray`, or :class:`APyCFixedArray`, or :class:`APyCFloatArray`
        Input array.

    axis : :class:`int` or tuple of ints, optional
        Selects a subset of the entries of length one in the shape.

    Returns
    -------
    squeezed : :class:`APyFloatArray`, :class:`APyFixedArray`, :class:`APyCFixedArray`, or :class:`APyCFloatArray`
        The input array, but with all or a subset of the dimensions of length 1 removed.

    Raises
    ------
    ValueError
        If `axis` is not None, and an axis being squeezed is not of length 1

    """
    try:
        squeeze = a.squeeze
    except AttributeError:
        raise TypeError(f"Cannot squeeze {type(a)}")
    return squeeze(axis=axis)


@overload
def convolve(
    a: APyFixedArray, v: APyFixedArray, mode: Literal["full", "same", "valid"] = "full"
) -> APyFixedArray: ...
@overload
def convolve(
    a: APyFloatArray, v: APyFloatArray, mode: Literal["full", "same", "valid"] = "full"
) -> APyFloatArray: ...
@overload
def convolve(
    a: APyCFixedArray,
    v: APyCFixedArray,
    mode: Literal["full", "same", "valid"] = "full",
) -> APyCFixedArray: ...
@overload
def convolve(
    a: APyCFloatArray,
    v: APyCFloatArray,
    mode: Literal["full", "same", "valid"] = "full",
) -> APyCFloatArray: ...


def convolve(
    a: APyArray, v: APyArray, mode: Literal["full", "same", "valid"] = "full"
) -> APyArray:
    """
    Return the discrete linear convolution of two one-dimensional arrays.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        First one-dimensional array of length :code:`M`

    v : :class:`APyFloatArray` or :class:`APyFixedArray`
        Second one-dimensional array of length :code:`N`.

    mode : {'full', 'same', 'valid'}, default: 'full'
        'full':
            Return the full convolution for each point of overlap. The resulting
            single-dimensional shape will have length :code:`N + M - 1`. Boundary
            effects occurs for points where the `a` and `v` do not overlap completely.
        'same':
            Return a convolution of length :code:`max(M, N)`. Boundary effects still
            occur around the edges of the result.
        'valid':
            Return the convolution for each point of full overlap. The resulting
            single-dimensional shape will have length :code:`max(M, N) - min(M, N) + 1`

    Returns
    -------
    convolved : :class:`APyFloatArray` or :class:`APyFixedArray`
        The convolved array.

    """
    try:
        return a.convolve(v, mode=mode)
    except (TypeError, AttributeError):
        raise TypeError(f"Cannot convolve {type(a)} with {type(v)}")


@overload
def reshape(a: APyFixedArray, new_shape: tuple[int, ...]) -> APyFixedArray: ...
@overload
def reshape(a: APyFloatArray, new_shape: tuple[int, ...]) -> APyFloatArray: ...
@overload
def reshape(a: APyCFixedArray, new_shape: tuple[int, ...]) -> APyCFixedArray: ...
@overload
def reshape(a: APyCFloatArray, new_shape: tuple[int, ...]) -> APyCFloatArray: ...


def reshape(a: APyArray, new_shape: tuple[int, ...]) -> APyArray:
    """
    Reshape an array to the specified shape without changing its data.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array to reshape.
    new_shape : :class:`tuple` of :class:`int`
        The new shape should be compatible with the original shape. If a dimension is
        -1, its value will be inferred from the length of the array and remaining
        dimensions. Only one dimension can be -1.

    Returns
    -------
    reshaped : :class:`APyFloatArray` or :class:`APyFixedArray`
        The reshaped array.

    Raises
    ------
    ValueError
        If negative dimensions less than -1 are provided, if the total size of the new
        array is not unchanged and divisible by the known dimensions, or if the total
        number of elements does not match the original array.

    Examples
    --------
    >>> import apytypes as apy
    >>> a = apy.APyFixedArray([2, 3, 4, 5], int_bits=2, frac_bits=1)
    >>> a.to_numpy()
    array([ 1. ,  1.5, -2. , -1.5])

    >>> apy.reshape(a, (2, 2)).to_numpy()
    array([[ 1. ,  1.5],
           [-2. , -1.5]])

    ------
    """
    try:
        reshape = a.reshape
    except AttributeError:
        raise TypeError(f"Cannot reshape {type(a)}")
    return reshape(new_shape)


def shape(arr: APyArray) -> tuple[int, ...]:
    """
    Return the shape of an array.

    Parameters
    ----------
    arr : :class:`APyFloatArray` or :class:`APyFixedArray`
        Input data.

    Returns
    -------
    :class:`tuple` of :class:`int`
        Shape of the input array.

    Raises
    ------
    TypeError
        If the input does not have a `shape` attribute.

    """
    try:
        shape = arr.shape
    except AttributeError:
        raise TypeError(f"{type(arr)} has no shape")
    return shape


@overload
def transpose(
    a: APyFixedArray, axes: tuple[int, ...] | None = None
) -> APyFixedArray: ...
@overload
def transpose(
    a: APyFloatArray, axes: tuple[int, ...] | None = None
) -> APyFloatArray: ...
@overload
def transpose(
    a: APyCFixedArray, axes: tuple[int, ...] | None = None
) -> APyCFixedArray: ...
@overload
def transpose(
    a: APyCFloatArray, axes: tuple[int, ...] | None = None
) -> APyCFloatArray: ...


def transpose(a: APyArray, axes: tuple[int, ...] | None = None) -> APyArray:
    """
    Return a copy of the array with axes transposed.

    For a 1-D array, this return the same array
    For a 2-D array, this is the standard matrix transpose.
    For an n-D array, if axes are given, their order indicates how the
    axes are permuted (see Examples). If axes are not provided, then
    ``transpose(a).shape == a.shape[::-1]``.

    Parameters
    ----------
    arr : :class:`APyFloatArray` or :class:`APyFixedArray`
        Input data.
    axes : :class:`tuple` of :class:`int`, optional
        If specified, it must be a tuple which contains a permutation
        of [0,1,...,N-1] where N is the number of axes of `a`. The `i`'th axis
        of the returned array will correspond to the axis numbered ``axes[i]``
        of the input. If not specified, defaults to ``range(a.ndim)[::-1]``,
        which reverses the order of the axes.

    Returns
    -------
    transposed : :class:`APyFloatArray` or :class:`APyFixedArray`
        copy of `a` with its axes permuted.


    Examples
    --------
    >>> import apytypes as apy
    >>> a = apy.fp(
    ...     [[1.0, 2.0, 3.0], [-4.0, -5.0, -6.0]],
    ...     exp_bits=5,
    ...     man_bits=2,
    ... )
    >>> a.to_numpy()
    array([[ 1.,  2.,  3.],
           [-4., -5., -6.]])

    >>> a = apy.transpose(a)
    >>> a.to_numpy()
    array([[ 1., -4.],
           [ 2., -5.],
           [ 3., -6.]])

    >>> a = apy.ones(
    ...     (1, 2, 3),
    ...     exp_bits=5,
    ...     man_bits=2,
    ... )
    >>> apy.transpose(a, (1, 0, 2)).shape
    (2, 1, 3)

    >>> apy.transpose(a, (-2, -3, -1)).shape
    (2, 1, 3)

    --------
    """

    try:
        transpose = a.transpose
    except AttributeError:
        raise TypeError(f"Cannot transpose {type(a)}")
    return transpose(axes=axes)


@overload
def ravel(a: APyFixedArray) -> APyFixedArray: ...
@overload
def ravel(a: APyFloatArray) -> APyFloatArray: ...
@overload
def ravel(a: APyCFixedArray) -> APyCFixedArray: ...
@overload
def ravel(a: APyCFloatArray) -> APyCFloatArray: ...


def ravel(a: APyArray) -> APyArray:
    """
    Return a copy of the array collapsed into one dimension.

    Same as :py:func:`flatten` with current memory-copy model.

    Returns
    -------
    collapsed : :class:`APyFloatArray` or :class:`APyFixedArray`
        copy of `a` but collapsed

    Examples
    --------
    >>> import apytypes as apy
    >>> signs = [[0, 0], [1, 1]]
    >>> exps = [[127, 128], [128, 129]]
    >>> mans = [[0, 0], [4194304, 0]]
    >>> arr = apy.APyFloatArray(
    ...     signs=signs, exps=exps, mans=mans, exp_bits=8, man_bits=23
    ... )
    >>> arr.to_numpy()
    array([[ 1.,  2.],
           [-3., -4.]])

    >>> apy.ravel(arr).to_numpy()
    array([ 1.,  2., -3., -4.])

    --------
    """
    try:
        ravel = a.ravel
    except AttributeError:
        raise TypeError(f"Cannot ravel {type(a)}")
    return ravel()


@overload
def moveaxis(
    a: APyFixedArray, source: int | Sequence[int], destination: int | Sequence[int]
) -> APyFixedArray: ...
@overload
def moveaxis(
    a: APyFloatArray, source: int | Sequence[int], destination: int | Sequence[int]
) -> APyFloatArray: ...
@overload
def moveaxis(
    a: APyCFixedArray, source: int | Sequence[int], destination: int | Sequence[int]
) -> APyCFixedArray: ...
@overload
def moveaxis(
    a: APyCFloatArray, source: int | Sequence[int], destination: int | Sequence[int]
) -> APyCFloatArray: ...


def moveaxis(
    a: APyArray, source: int | Sequence[int], destination: int | Sequence[int]
) -> APyArray:
    """
    Move axes of an array to new positions.

    Other axes remain in their original order.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array whose axes should be reordered.
    source : :class:`int` or sequence of int
        Original positions of the axes to move. These must be unique.
    destination : :class:`int` or sequence of int
        Destination positions for each of the original axes. These must also be unique.

    Examples
    --------
    >>> import apytypes as apy
    >>> x = apy.zeros((3, 4, 5), int_bits=16, frac_bits=0)
    >>> apy.moveaxis(x, 0, -1).shape
    (4, 5, 3)
    >>> apy.moveaxis(x, -1, 0).shape
    (5, 3, 4)

    These all achieve the same result:

    >>> apy.transpose(x).shape
    (5, 4, 3)
    >>> apy.swapaxes(x, 0, -1).shape
    (5, 4, 3)
    >>> apy.moveaxis(x, [0, 1], [-1, -2]).shape
    (5, 4, 3)
    >>> apy.moveaxis(x, [0, 1, 2], [-1, -2, -3]).shape
    (5, 4, 3)

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Copy of array with moved axes.
    """
    source = _normalize_axis_sequence(source, a.ndim)
    destination = _normalize_axis_sequence(destination, a.ndim)

    if len(source) != len(destination):
        raise ValueError(
            "`source` and `destination` arguments must have the same number of elements"
        )
    order = [n for n in range(a.ndim) if n not in source]

    for dest, src in sorted(zip(destination, source)):
        order.insert(dest, src)
    order_tuple = tuple(order)

    return transpose(a, order_tuple)


@overload
def swapaxes(a: APyFixedArray, axis1: int, axis2: int) -> APyFixedArray: ...
@overload
def swapaxes(a: APyFloatArray, axis1: int, axis2: int) -> APyFloatArray: ...
@overload
def swapaxes(a: APyCFixedArray, axis1: int, axis2: int) -> APyCFixedArray: ...
@overload
def swapaxes(a: APyCFloatArray, axis1: int, axis2: int) -> APyCFloatArray: ...


def swapaxes(a: APyArray, axis1: int, axis2: int) -> APyArray:
    """
    Interchange two axes of an array.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array whose axes should be reordered.
    axis1 : :class:`int`
      First axis.
    axis2 : :class:`int`
      Second axis.

    Examples
    --------
    >>> import apytypes as apy
    >>> x = apy.fp([[1, 2, 3]], exp_bits=5, man_bits=2)
    >>> apy.swapaxes(x, 0, 1)
    APyFloatArray(
        [[ 0],
         [ 0],
         [ 0]],
    <BLANKLINE>
        [[15],
         [16],
         [16]],
    <BLANKLINE>
        [[ 0],
         [ 0],
         [ 2]],
        exp_bits=5,
        man_bits=2
    )

    >>> x = apy.fp([[[0, 1], [2, 3]], [[4, 5], [6, 7]]], exp_bits=5, man_bits=3)
    >>> x
    APyFloatArray(
        [[[ 0,  0],
          [ 0,  0]],
    <BLANKLINE>
         [[ 0,  0],
          [ 0,  0]]],
    <BLANKLINE>
    <BLANKLINE>
        [[[ 0, 15],
          [16, 16]],
    <BLANKLINE>
         [[17, 17],
          [17, 17]]],
    <BLANKLINE>
    <BLANKLINE>
        [[[ 0,  0],
          [ 0,  4]],
    <BLANKLINE>
         [[ 0,  2],
          [ 4,  6]]],
        exp_bits=5,
        man_bits=3
    )

    >>> apy.swapaxes(x, 0, 2)
    APyFloatArray(
        [[[ 0,  0],
          [ 0,  0]],
    <BLANKLINE>
         [[ 0,  0],
          [ 0,  0]]],
    <BLANKLINE>
    <BLANKLINE>
        [[[ 0, 17],
          [16, 17]],
    <BLANKLINE>
         [[15, 17],
          [16, 17]]],
    <BLANKLINE>
    <BLANKLINE>
        [[[ 0,  0],
          [ 0,  4]],
    <BLANKLINE>
         [[ 0,  2],
          [ 4,  6]]],
        exp_bits=5,
        man_bits=3
    )

    Returns
    -------
    a_swapped : :class:`APyFloatArray` or :class:`APyFixedArray`
        Copy of `a` with axes swapped
    """

    try:
        swapaxes = a.swapaxes
    except AttributeError:
        raise TypeError(f"Cannot transpose/swapaxis of type: {type(a)}")
    return swapaxes(axis1, axis2)


@overload
def expand_dims(a: APyFixedArray, axis: int | tuple[int, ...]) -> APyFixedArray: ...
@overload
def expand_dims(a: APyFloatArray, axis: int | tuple[int, ...]) -> APyFloatArray: ...
@overload
def expand_dims(a: APyCFixedArray, axis: int | tuple[int, ...]) -> APyCFixedArray: ...
@overload
def expand_dims(a: APyCFloatArray, axis: int | tuple[int, ...]) -> APyCFloatArray: ...


def expand_dims(a: APyArray, axis: int | tuple[int, ...]) -> APyArray:
    """
    Expand the shape of an array.

    Insert a new axis that will appear at the `axis` position in the expanded array
    shape.

    Parameters
    ----------
    a : :class:`APyFloatArray`, :class:`APyFixedArray`, :class:`APyCFixedArray`, or
        :class:`APyCFloatArray`
        The array whose axes should be reordered.
    axis : :class:`int` or tuple of ints
        Position in the expanded axes where the new axis (or axes) is placed.

    Examples
    --------
    >>> import apytypes as apy
    >>> x = apy.fx([1, 2], int_bits=5, frac_bits=0)
    >>> x.shape
    (2,)

    >>> y = apy.expand_dims(x, axis=0)
    >>> y
    APyFixedArray([[1, 2]], int_bits=5, frac_bits=0)
    >>> y.shape
    (1, 2)

    >>> y = apy.expand_dims(x, axis=1)
    >>> y
    APyFixedArray([[1],
                   [2]], int_bits=5, frac_bits=0)

    >>> y.shape
    (2, 1)

    ``axis`` may also be a tuple:

    >>> y = apy.expand_dims(x, axis=(0, 1))
    >>> y
    APyFixedArray([[[1, 2]]], int_bits=5, frac_bits=0)
    >>> y.shape
    (1, 1, 2)

    >>> y = apy.expand_dims(x, axis=(2, 0))
    >>> y
    APyFixedArray([[[1],
                    [2]]], int_bits=5, frac_bits=0)
    >>> y.shape
    (1, 2, 1)

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Copy of `a` with the number of dimensions increased.
    """

    if not isinstance(axis, tuple):
        axis = (axis,)

    out_ndim = len(axis) + a.ndim
    axis = _normalize_axis_sequence(axis, out_ndim)

    shape_it = iter(a.shape)
    shape = tuple([1 if ax in axis else next(shape_it) for ax in range(out_ndim)])

    return a.reshape(shape)


# =============================================================================
# array creation
# =============================================================================


@overload
def zeros(
    shape: tuple[int, ...],
    *,
    int_bits: int,
    frac_bits: int,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def zeros(
    shape: tuple[int, ...],
    *,
    bits: int,
    int_bits: int,
    frac_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def zeros(
    shape: tuple[int, ...],
    *,
    bits: int,
    frac_bits: int,
    int_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def zeros(
    shape: tuple[int, ...],
    *,
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    force_complex: None = None,
) -> APyFloatArray: ...


@overload
def zeros(
    shape: tuple[int, ...],
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool | None = None,
) -> APyArray: ...


def zeros(
    shape: tuple[int, ...],
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool | None = None,
) -> APyArray:
    """
    Create an array of `shape` with all zeros.

    Word lengths need to be specified and the return type is deduced from the bit
    specifiers. Either specify exactly two of three from `int_bits`, `frac_bits`, and
    `bits`, for :class:`APyFixedArray`, or specify both `exp_bits` and `man_bits`
    for :class:`APyFloatArray`. Specify `force_complex` to retrieve a complex-valued
    array type.

    Parameters
    ----------
    shape : :class:`tuple` of :class:`int`
        Shape of the array.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.
    force_complex : :class:`bool`, optional
        Retrieve the complex-valued array type, :class:`APyCFixedArray` for fixed-point
        and :class:`APyCFloatArray` for floating-point.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray` or :class:`APyCFixedArray`
        The initialized array with zeros.
    """
    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, man_bits, bias)
    if a_type is None:
        raise ValueError("zeros: could not determine array type from bit-specifiers")

    if a_type is APyFixedArray:
        zeros = APyCFixedArray.zeros if force_complex else APyFixedArray.zeros
        return zeros(shape=shape, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray:
        assert man_bits is not None
        assert exp_bits is not None
        zeros = APyCFloatArray.zeros if force_complex else APyFloatArray.zeros
        return zeros(shape=shape, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


@overload
def ones(
    shape: tuple[int, ...],
    *,
    int_bits: int,
    frac_bits: int,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def ones(
    shape: tuple[int, ...],
    *,
    bits: int,
    int_bits: int,
    frac_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def ones(
    shape: tuple[int, ...],
    *,
    bits: int,
    frac_bits: int,
    int_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def ones(
    shape: tuple[int, ...],
    *,
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    force_complex: None = None,
) -> APyFloatArray: ...


@overload
def ones(
    shape: tuple[int, ...],
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool | None = None,
) -> APyArray: ...


def ones(
    shape: tuple[int, ...],
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool | None = None,
) -> APyArray:
    """
    Create an array of `shape` with all ones (stored value).

    Word lengths need to be specified and the return type is deduced from the bit
    specifiers. Either specify exactly two of three from `int_bits`, `frac_bits`, and
    `bits`, for :class:`APyFixedArray`, or specify both `exp_bits` and `man_bits`
    for :class:`APyFloatArray`. Specify `force_complex` to retrieve a complex-valued
    array type.

    Parameters
    ----------
    shape : :class:`tuple` of :class:`int`
        Shape of the array.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.
    force_complex : :class:`bool`, optional
        Retrieve the complex-valued array type, :class:`APyCFixedArray` for fixed-point
        and :class:`APyCFloatArray` for floating-point.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray` or :class:`APyCFixedArray`
        The array initialized filled with ones.
    """
    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, man_bits, bias)
    if a_type is None:
        raise ValueError("ones: could not determine array type from bit-specifiers")

    if a_type is APyFixedArray:
        ones = APyCFixedArray.ones if force_complex else APyFixedArray.ones
        return ones(shape=shape, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray:
        assert man_bits is not None
        assert exp_bits is not None
        ones = APyCFloatArray.ones if force_complex else APyFloatArray.ones
        return ones(shape=shape, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


@overload
def eye(
    n: int,
    m: int | None = None,
    *,
    int_bits: int,
    frac_bits: int,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def eye(
    n: int,
    m: int | None = None,
    *,
    bits: int,
    int_bits: int,
    frac_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def eye(
    n: int,
    m: int | None = None,
    *,
    bits: int,
    frac_bits: int,
    int_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def eye(
    n: int,
    m: int | None = None,
    *,
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    bits: None = None,
    frac_bits: None = None,
    int_bits: None = None,
    force_complex: None = None,
) -> APyFloatArray: ...


@overload
def eye(
    n: int,
    m: int | None = None,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool | None = None,
) -> APyArray: ...


def eye(
    n: int,
    m: int | None = None,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool | None = None,
) -> APyArray:
    """
    Return a 2-D array with ones (stored value) on the main diagonal and zeros
    elsewhere.

    Word lengths need to be specified and the return type is deduced from the bit
    specifiers. Either specify exactly two of three from `int_bits`, `frac_bits`, and
    `bits`, for :class:`APyFixedArray`, or specify both `exp_bits` and `man_bits`
    for :class:`APyFloatArray`. Specify `force_complex` to retrieve a complex-valued
    array type.

    Parameters
    ----------
    n : :class:`int`
        Number of rows in the output.
    m : :class:`int`, optional
        Number of columns in the output. If `None`, defaults to `n`.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.
    force_complex : :class:`bool`, optional
        Return the complex-valued array type, :class:`APyCFixedArray` for fixed-point
        and :class:`APyCFloatArray` for floating-point.


    Returns
    -------
    result : :class:`APyFloatArray`, :class:`APyFixedArray` or :class:`APyCFixedArray`
        An array where all elements are equal to zero, except for the k-th diagonal,
        whose values are equal to one.
    """
    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, man_bits, bias)
    if a_type is None:
        raise ValueError("eye: could not determine array type from bit-specifiers")

    if a_type is APyFixedArray:
        eye = APyCFixedArray.eye if force_complex else APyFixedArray.eye
        return eye(n=n, m=m, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray:
        assert man_bits is not None
        assert exp_bits is not None
        eye = APyCFloatArray.eye if force_complex else APyFloatArray.eye
        return eye(n=n, m=m, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


@overload
def identity(
    n: int,
    *,
    int_bits: int,
    frac_bits: int,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def identity(
    n: int,
    *,
    bits: int,
    int_bits: int,
    frac_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def identity(
    n: int,
    *,
    bits: int,
    frac_bits: int,
    int_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
    force_complex: None = None,
) -> APyFixedArray: ...


@overload
def identity(
    n: int,
    *,
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    int_bits: None = None,
    bits: None = None,
    frac_bits: None = None,
    force_complex: None = None,
) -> APyFloatArray: ...


@overload
def identity(
    n: int,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool | None = None,
) -> APyArray: ...


def identity(
    n: int,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool | None = None,
) -> APyArray:
    """
    Return the identity array.

    Word lengths need to be specified and the return type is deduced from the bit
    specifiers. Either specify exactly two of three from `int_bits`, `frac_bits`, and
    `bits`, for :class:`APyFixedArray`, or specify both `exp_bits` and `man_bits`
    for :class:`APyFloatArray`. Specify `force_complex` to retrieve a complex-valued
    array type.

    Parameters
    ----------
    n : :class:`int`
        Number of rows and columns in output.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.
    force_complex : :class:`bool`, optional
        Return the complex-valued array type, :class:`APyCFixedArray` for fixed-point
        and :class:`APyCFloatArray` for floating-point.

    Returns
    -------
    result : :class:`APyFloatArray`, :class:`APyFixedArray` or :class:`APyCFixedArray`
        `n` x `n` array with ones (stored value) on the main diagonal and zeros
        elsewhere.
    """
    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, man_bits, bias)
    if a_type is None:
        raise ValueError("identity: could not determine array type from bit-specifiers")

    if a_type is APyFixedArray:
        identity = APyCFixedArray.identity if force_complex else APyFixedArray.identity
        return identity(n=n, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray:
        identity = APyCFloatArray.identity if force_complex else APyFloatArray.identity
        assert man_bits is not None
        assert exp_bits is not None
        return identity(n=n, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyScalar | int | float,
    *,
    int_bits: int,
    frac_bits: int,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyScalar | int | float,
    *,
    bits: int,
    int_bits: int,
    frac_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyScalar | int | float,
    *,
    bits: int,
    frac_bits: int,
    int_bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyScalar | int | float,
    *,
    exp_bits: int,
    man_bits: int,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyFloatArray: ...


@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyFixed,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyFloat,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFloatArray: ...


@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyScalar | int | float,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray: ...


def full(
    shape: tuple[int, ...],
    fill_value: APyScalar | int | float,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray:
    """
    Return a new array of given shape and type, filled with `fill_value`.

    If `fill_value` is of type `int` or `float`, you must specify the word lengths.
    Either specify exactly two of three from `int_bits`, `frac_bits`, and `bits`, for
    :class:`APyFixedArray`, or specify both `exp_bits` and `man_bits` for
    :class:`APyFloatArray`. If `fill_value` is of type `APyFloat` or `APyFixed`, the
    array will use the provided word lengths only if specified, otherwise the word
    lengths are inherited from `fill_value`.

    Parameters
    ----------
    shape : :class:`tuple`
        Shape of the array.
    fill_value : :class:`APyFloat` or :class:`APyFixed` or :class:`int` or :class`float`
        Fill value.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Array of all `fill_value` with the given shape.
    """

    fill_value = _determine_fill_value(
        fill_value, int_bits, frac_bits, bits, exp_bits, man_bits, bias
    )
    if isinstance(fill_value, APyFixed):
        return APyFixedArray.full(shape, fill_value)
    elif isinstance(fill_value, APyFloat):
        return APyFloatArray.full(shape, fill_value)
    elif isinstance(fill_value, APyCFixed):
        return APyCFixedArray.full(shape, fill_value)
    elif isinstance(fill_value, APyCFloat):
        return APyCFloatArray.full(shape, fill_value)
    else:
        raise TypeError(f"Cannot make full array of type {type(fill_value)}")


@overload
def zeros_like(
    a: APyFixedArray,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def zeros_like(
    a: APyCFixedArray,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyCFixedArray: ...


@overload
def zeros_like(
    a: APyFloatArray,
    *,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyFloatArray: ...


@overload
def zeros_like(
    a: APyCFloatArray,
    *,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyCFloatArray: ...


@overload
def zeros_like(
    a: APyArray,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray: ...


def zeros_like(
    a: APyArray,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray:
    """
    Return an array of all zeros with the same shape and type as `a`.

    The type and bit-specifiers of the returned array can be overwritten through the
    bit-specifier arguments. To modify the type, either specify exactly two of three
    from `int_bits`, `frac_bits`, and `bits`, for :class:`APyFixedArray` and
    :class:`APyCFixedArray`, or specify both `exp_bits` and `man_bits` for
    :class:`APyFloatArray`.

    Parameters
    ----------
    a : :class:`APyFixedArray`, :class:`APyFloatArray`, or :class:`APyCFixedArray`
        The shape and data-type of *a* define these same attributes of the returned array.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The initialized array with all zeros.
    """
    if isinstance(a, APyFixedArray | APyCFixedArray):
        if int_bits is None and frac_bits is None and bits is None:
            return a.zeros(a.shape, a.int_bits, a.frac_bits)
        else:
            return a.zeros(a.shape, int_bits, frac_bits, bits)
    elif isinstance(a, APyFloatArray | APyCFloatArray):
        if exp_bits is None and man_bits is None and bias is None:
            return a.zeros(a.shape, a.exp_bits, a.man_bits, a.bias)
        else:
            assert exp_bits is not None
            assert man_bits is not None
            return a.zeros(a.shape, exp_bits, man_bits, bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


@overload
def ones_like(
    a: APyFixedArray,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def ones_like(
    a: APyCFixedArray,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyCFixedArray: ...


@overload
def ones_like(
    a: APyFloatArray,
    *,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyFloatArray: ...


@overload
def ones_like(
    a: APyCFloatArray,
    *,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    bias: None = None,
) -> APyCFloatArray: ...


@overload
def ones_like(
    a: APyArray,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray: ...


def ones_like(
    a: APyArray,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray:
    """
    Return an array of all ones (stored value) with the same shape and type as `a`.

    The type and bit-specifiers of the returned array can be overwritten through the
    bit-specifier arguments. To modify the type, either specify exactly two of three
    from `int_bits`, `frac_bits`, and `bits`, for :class:`APyFixedArray`, or specify
    both `exp_bits` and `man_bits` for :class:`APyFloatArray`.

    Parameters
    ----------
    a : :class:`APyFixedArray`, :class:`APyFloatArray`, or :class:`APyCFixedArray`
        The shape and data-type of *a* define these same attributes of the returned array.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array initialized filled with all ones.
    """
    if isinstance(a, APyFixedArray | APyCFixedArray):
        if int_bits is None and frac_bits is None and bits is None:
            return a.ones(a.shape, a.int_bits, a.frac_bits)
        else:
            return a.ones(a.shape, int_bits, frac_bits, bits)
    elif isinstance(a, APyFloatArray | APyCFloatArray):
        if exp_bits is None and man_bits is None and bias is None:
            return a.ones(a.shape, a.exp_bits, a.man_bits, a.bias)
        else:
            assert exp_bits is not None
            assert man_bits is not None
            return a.ones(a.shape, exp_bits, man_bits, bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


@overload
def full_like(
    a: APyFixedArray,
    fill_value: APyScalar | int | float,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyFixedArray: ...


@overload
def full_like(
    a: APyFloatArray,
    fill_value: APyScalar | int | float,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyFloatArray: ...


@overload
def full_like(
    a: APyCFixedArray,
    fill_value: APyScalar | int | float,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyCFixedArray: ...


@overload
def full_like(
    a: APyCFloatArray,
    fill_value: APyScalar | int | float,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyCFloatArray: ...


@overload
def full_like(
    a: APyArray,
    fill_value: APyScalar | int | float,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray: ...


def full_like(
    a: APyArray,
    fill_value: APyScalar | int | float,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray:
    """
    Return an array with all values initialized to `fill_value`, with the same shape and
    type as `a`.

    The type and bit-specifiers of the returned array can be overwritten through the
    bit-specifier arguments. To overwrite the type, either specify exactly two of three
    from `int_bits`, `frac_bits`, and `bits`, for :class:`APyFixedArray`, or specify
    both `exp_bits` and `man_bits` for :class:`APyFloatArray`.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The shape and array type of the returned array.
    fill_value : :class:`APyFloat` or :class:`APyFixed` or int or float
        The value to fill the array with.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Array filled with fill_value, having the same shape and type as `a`.
    """
    try:
        full = a.full
    except AttributeError:
        raise TypeError(f"Cannot make full array of type {type(a)}")

    fill_value = _determine_fill_value(
        fill_value, int_bits, frac_bits, bits, exp_bits, man_bits, bias
    )
    if isinstance(fill_value, APyFixed) and isinstance(a, APyCFixedArray):
        fill_value = APyCFixed.from_complex(fill_value, a.int_bits, a.frac_bits)
    elif isinstance(fill_value, APyFloat) and isinstance(a, APyCFloatArray):
        fill_value = APyCFloat.from_complex(fill_value, a.exp_bits, a.man_bits, a.bias)
    return full(a.shape, fill_value)


@overload
def arange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    step: APyScalar | int | float = 1,
    *,
    int_bits: int,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def arange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    step: APyScalar | int | float = 1,
    *,
    bits: int,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def arange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    step: APyScalar | int | float = 1,
    *,
    frac_bits: int,
    bits: int | None = None,
    int_bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def arange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    step: APyScalar | int | float = 1,
    *,
    exp_bits: int,
    man_bits: int | None = None,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyFloatArray: ...


@overload
def arange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    step: APyScalar | int | float = 1,
    *,
    man_bits: int,
    exp_bits: int | None = None,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyFloatArray: ...


@overload
def arange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    step: APyScalar | int | float = 1,
    *,
    bias: int,
    man_bits: int | None = None,
    exp_bits: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyFloatArray: ...


@overload
def arange(
    start: APyFixed,
    stop: APyFixed | int | float | None = None,
    step: APyFixed | int | float = 1,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def arange(
    start: APyFixed | int | float,
    stop: APyFixed,
    step: APyFixed | int | float = 1,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def arange(
    start: APyFixed | int | float,
    stop: APyFixed | int | float,
    step: APyFixed,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def arange(
    start: APyFloat,
    stop: APyFloat | int | float | None = None,
    step: APyFloat | int | float = 1,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFloatArray: ...


@overload
def arange(
    start: APyFloat | int | float,
    stop: APyFloat,
    step: APyFloat | int | float = 1,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFloatArray: ...


@overload
def arange(
    start: APyFloat | int | float,
    stop: APyFloat | int | float,
    step: APyFloat,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFloatArray: ...


@overload
def arange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    step: APyScalar | int | float = 1,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray: ...


def arange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    step: APyScalar | int | float = 1,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray:
    """
    Create an array with evenly spaced values within a given interval.

    The function can be called with varying number of positional arguments:

    * ``arange(stop)``: Values are generated within the half-open interval ``[0, stop)`` (in other words, the interval including ``start`` but excluding ``stop``).
    * ``arange(start, stop)``: Values are generated within the half-open interval ``[start, stop)``.
    * ``arange(start, stop, step)``: Values are generated within the half-open interval ``[start, stop)``, with spacing between values given by ``step``.

    If no bit-specifiers are given, the array type is deduced based on ``start``,
    ``stop``, and ``step``. In this case, all APyTypes scalars must be of the same
    format.

    Parameters
    ----------
    start : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`
        Start number.
    stop : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`, optional
        Stop number.
    step : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`, optional
        Step size in range.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Array with evenly spaced values within a given interval.
    """
    explicit_type = _determine_array_type(
        int_bits, frac_bits, bits, exp_bits, man_bits, bias
    )

    # Deduced bit specifiers
    deduced_type = explicit_type
    (
        tmp_int_bits,
        tmp_frac_bits,
        tmp_bits,
        tmp_exp_bits,
        tmp_man_bits,
        tmp_bias,
    ) = [None] * 6

    for var in (start, stop, step):
        if isinstance(var, APyFixed):
            if explicit_type == APyFloatArray:
                continue

            if deduced_type == APyFloatArray:
                raise ValueError("Could not determine array type")
            deduced_type = APyFixedArray

            tmp_int_bits = var.int_bits if tmp_int_bits is None else tmp_int_bits
            tmp_frac_bits = var.frac_bits if tmp_frac_bits is None else tmp_frac_bits

            if (int_bits is None and (tmp_int_bits != var.int_bits)) or (
                frac_bits is None and (tmp_frac_bits != var.frac_bits)
            ):
                raise ValueError("Could not determine bit specifiers")

        elif isinstance(var, APyFloat):
            if explicit_type == APyFixedArray:
                continue

            if deduced_type == APyFixedArray:
                raise ValueError("Could not determine array type")
            deduced_type = APyFloatArray

            tmp_exp_bits = var.exp_bits if tmp_exp_bits is None else tmp_exp_bits
            tmp_man_bits = var.man_bits if tmp_man_bits is None else tmp_man_bits
            tmp_bias = var.bias if tmp_bias is None else tmp_bias

            if (
                (exp_bits is None and (tmp_exp_bits != var.exp_bits))
                or (man_bits is None and (tmp_man_bits != var.man_bits))
                or (bias is None and (tmp_bias != var.bias))
            ):
                raise ValueError("Could not determine bit specifiers")

    tmp_int_bits = tmp_int_bits if int_bits is None else int_bits
    tmp_frac_bits = tmp_frac_bits if frac_bits is None else frac_bits
    tmp_bits = tmp_bits if bits is None else bits
    tmp_exp_bits = tmp_exp_bits if exp_bits is None else exp_bits
    tmp_man_bits = tmp_man_bits if man_bits is None else man_bits
    tmp_bias = tmp_bias if bias is None and exp_bits is None else bias

    if stop is None:
        start, stop = (0, start)

    # The values will be generated using APyFixed so exceptions on infinity and NaN will
    # be generated by the constructors
    if deduced_type is APyFixedArray:
        return APyFixedArray._arange(
            start, stop, step, tmp_int_bits, tmp_frac_bits, tmp_bits
        )
    elif deduced_type is APyFloatArray:
        return APyFloatArray._arange(
            start, stop, step, tmp_exp_bits, tmp_man_bits, tmp_bias
        )
    else:
        raise ValueError("Could not determine array type")


@overload
def fullrange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    *,
    int_bits: int,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def fullrange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    *,
    bits: int,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def fullrange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    *,
    frac_bits: int,
    bits: int | None = None,
    int_bits: int | None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def fullrange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    *,
    exp_bits: int,
    man_bits: int | None = None,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyFloatArray: ...


@overload
def fullrange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    *,
    man_bits: int,
    exp_bits: int | None = None,
    bias: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyFloatArray: ...


@overload
def fullrange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    *,
    bias: int,
    man_bits: int | None = None,
    exp_bits: int | None = None,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
) -> APyFloatArray: ...


@overload
def fullrange(
    start: APyFixed,
    stop: APyFixed | int | float | None = None,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def fullrange(
    start: APyFixed | int | float,
    stop: APyFixed,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFixedArray: ...


@overload
def fullrange(
    start: APyFloat,
    stop: APyFloat | int | float | None = None,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFloatArray: ...


@overload
def fullrange(
    start: APyFloat | int | float,
    stop: APyFloat,
    *,
    int_bits: None = None,
    frac_bits: None = None,
    bits: None = None,
    exp_bits: None = None,
    man_bits: None = None,
    bias: None = None,
) -> APyFloatArray: ...


@overload
def fullrange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray: ...


def fullrange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    *,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyArray:
    """
    Create an array with all values within a given interval.

    The function can be called with varying number of positional arguments:

    * ``arange(stop)``: Values are generated within the half-open interval ``[0, stop)`` (in other words, the interval including ``start`` but excluding ``stop``).
    * ``arange(start, stop)``: Values are generated within the half-open interval ``[start, stop)``.

    If no bit-specifiers are given, the array type is deduced based on ``start`` and
    ``stop``. In this case, all APyTypes scalars must be of the same
    format.

    Parameters
    ----------
    start : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`
        Start number.
    stop : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`, optional
        Stop number.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Array with all values within a given interval.
    """
    explicit_type = _determine_array_type(
        int_bits, frac_bits, bits, exp_bits, man_bits, bias
    )

    # Deduced bit specifiers
    deduced_type = explicit_type
    (
        tmp_int_bits,
        tmp_frac_bits,
        tmp_bits,
        tmp_exp_bits,
        tmp_man_bits,
        tmp_bias,
    ) = [None] * 6

    for var in (start, stop):
        if isinstance(var, APyFixed):
            if explicit_type == APyFloatArray:
                continue

            if deduced_type == APyFloatArray:
                raise ValueError("Could not determine array type")
            deduced_type = APyFixedArray

            tmp_int_bits = var.int_bits if tmp_int_bits is None else tmp_int_bits
            tmp_frac_bits = var.frac_bits if tmp_frac_bits is None else tmp_frac_bits

            if (int_bits is None and (tmp_int_bits != var.int_bits)) or (
                frac_bits is None and (tmp_frac_bits != var.frac_bits)
            ):
                raise ValueError("Could not determine bit specifiers")

        elif isinstance(var, APyFloat):
            if explicit_type == APyFixedArray:
                continue

            if deduced_type == APyFixedArray:
                raise ValueError("Could not determine array type")
            deduced_type = APyFloatArray

            tmp_exp_bits = var.exp_bits if tmp_exp_bits is None else tmp_exp_bits
            tmp_man_bits = var.man_bits if tmp_man_bits is None else tmp_man_bits
            tmp_bias = var.bias if tmp_bias is None else tmp_bias

            if (
                (exp_bits is None and (tmp_exp_bits != var.exp_bits))
                or (man_bits is None and (tmp_man_bits != var.man_bits))
                or (bias is None and (tmp_bias != var.bias))
            ):
                raise ValueError("Could not determine bit specifiers")

    tmp_int_bits = tmp_int_bits if int_bits is None else int_bits
    tmp_frac_bits = tmp_frac_bits if frac_bits is None else frac_bits
    tmp_bits = tmp_bits if bits is None else bits
    tmp_exp_bits = tmp_exp_bits if exp_bits is None else exp_bits
    tmp_man_bits = tmp_man_bits if man_bits is None else man_bits
    tmp_bias = tmp_bias if bias is None and exp_bits is None else bias

    if stop is None:
        start, stop = (0, start)

    if deduced_type is APyFixedArray:
        return APyFixedArray._fullrange(
            start, stop, tmp_int_bits, tmp_frac_bits, tmp_bits
        )
    elif deduced_type is APyFloatArray:
        return APyFloatArray._fullrange(
            start, stop, tmp_exp_bits, tmp_man_bits, tmp_bias
        )
    else:
        raise ValueError("Could not determine array type")


@overload
def export_csv(
    a: APyFixedArray,
    fname: str,
    delimiter: str = ",",
    layout: Literal["default", "vunit"] = "default",
) -> None: ...


@overload
def export_csv(
    a: APyFloatArray,
    fname: str,
    delimiter: str = ",",
    layout: Literal["default", "vunit"] = "default",
) -> None: ...


def export_csv(
    a: APyArray,
    fname: str,
    delimiter: str = ",",
    layout: Literal["default", "vunit"] = "default",
) -> None:
    """
    Store the bit values of an array to a CSV file.

    The function supports multi-dimensional arrays:
        * For 1-D arrays, all elements are written on a single line.
        * For 2-D arrays, each row of the array is written on a separate line.
        * Higher-dimensional arrays must be handled manually unless supported by the chosen layout.

    By default, a 1-D array will be exported as a single row CSV, and a 2-D array will be exported as multiple rows.

    .. attention::
        The index order of arrays may differ between APyTypes and other programs.

    .. note::

        The VUnit layout transposes the array so that the index order of VUnit's ``get`` function will behave the same as in APyTypes.
        For a 2-D array, VUnit's :std:doc:`vunit:data_types/integer_array` will have the shape ``height, width = a.shape``.
        For a 3-D array, VUnit's :std:doc:`vunit:data_types/integer_array` must be reshaped to ``height, width, depth = a.shape`` after importing it as a 2-D array.
        The array is stored in the CSV with the shape ``height, width = a.shape[1], a.shape[0]*a.shape[2]``.
        Lastly, negative fixed-point values are represented using the minus sign to match the expectations of VUnit.

    .. hint::
        To export a complex-valued array, use separate CSV files for real and imaginary parts.

    .. versionadded:: 0.4

    Parameters
    ----------
    a : :class:`APyFixedArray` or :class:`APyFloatArray`
        The array to store the bit values of.
    fname : :class:`str`
        Path to the CSV file to store bit values.
    delimiter : :class:`str`, optional
        Character used in the CSV file to separate bit values. Default is `,`.
    layout : {"default", "vunit"}, default: "default"
        Layout to use when storing the array.

    See Also
    --------
    import_csv

    Examples
    --------
    >>> import apytypes as apy
    >>> a = apy.fx(range(4), int_bits=4, frac_bits=0).reshape((2, 2))
    >>> apy.export_csv(a, "input.csv")

    A 3-D VUnit array can be exported as

    >>> b = apy.fx(range(8), int_bits=4, frac_bits=0).reshape((2, 2, 2))
    >>> apy.export_csv(b, "input_vunit.csv", layout="vunit")

    """

    if type(a) in (APyCFixedArray, APyCFloatArray):
        raise ValueError("Complex data types are not supported yet")

    if layout not in ("default", "vunit"):
        raise ValueError(f"Unknown layout: {layout}")

    if a.ndim > 3 or (a.ndim > 2 and layout != "vunit"):
        raise ValueError(f"Unsupported array dimension {a.ndim} for layout {layout}")

    to_bits_fn = (
        "_to_signed_bits"
        if type(a) is APyFixedArray and layout == "vunit"
        else "to_bits"
    )

    if layout == "vunit":
        if a.ndim == 2:
            a = a.T
        elif a.ndim == 3:
            a = a.transpose((1, 0, 2))

    with Path(fname).open(mode="w", newline="") as f:
        writer = csv.writer(f, delimiter=delimiter)
        if a.ndim == 1:
            writer.writerow(getattr(a, to_bits_fn)())
        elif a.ndim == 2:
            for row in a:
                writer.writerow(getattr(row, to_bits_fn)())
        elif a.ndim == 3:
            for depth in a:
                writer.writerow(getattr(depth.flatten(), to_bits_fn)())


@overload
def import_csv(
    fname: str,
    *,
    delimiter: str = ",",
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool = False,
    layout: Literal["default", "vunit"] = "default",
    vunit_3d_shape: tuple[int, ...] | None = None,
) -> APyFixedArray: ...


@overload
def import_csv(
    fname: str,
    *,
    delimiter: str = ",",
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool = False,
    layout: Literal["default", "vunit"] = "default",
    vunit_3d_shape: tuple[int, ...] | None = None,
) -> APyFloatArray: ...


def import_csv(
    fname: str,
    *,
    delimiter: str = ",",
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
    force_complex: bool = False,
    layout: Literal["default", "vunit"] = "default",
    vunit_3d_shape: tuple[int, ...] | None = None,
) -> APyArray:
    """
    Create an array from a CSV file containing bit values.

    1-D and 2-D arrays are directly supported, while higher dimensional arrays must be reshaped manually after importing as either a 1-D or 2-D array.

    By default, a one row CSV file is imported as a 1-D array, while a multi-row CSV file is imported as a 2-D array.

    .. attention::

       The index order may differ between APyTypes and other programs.

    .. note::

       1-D VUnit arrays can be imported using the default layout.

    .. versionadded:: 0.4

    Parameters
    ----------
    fname : :class:`str`
        Path to the CSV file containing bit values.
    delimiter : :class:`str`, optional
        Character used in the CSV file to separate bit values. Default is `,`.
    int_bits : :class:`int`, optional
        Number of fixed-point integer bits.
    frac_bits : :class:`int`, optional
        Number of fixed-point fractional bits.
    bits : :class:`int`, optional
        Number of fixed-point bits.
    exp_bits : :class:`int`, optional
        Number of floating-point exponential bits.
    man_bits : :class:`int`, optional
        Number of floating-point mantissa bits.
    bias : :class:`int`, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.
    force_complex : :class:`bool`, optional
        If True, forces the array to be treated as complex.
    layout : {"default", "vunit"}, default: "default"
        Layout of the stored array.
    vunit_3d_shape : :class:`tuple`, optional
        Shape of the resulting 3-D array when using the "vunit" layout. Do not provide this for 1-D or 2-D arrays.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Array created from the bit values in the CSV file.

    See Also
    --------
    export_csv

    Examples
    --------
    >>> import apytypes as apy
    >>> a = apy.import_csv("input.csv", int_bits=4, frac_bits=0)
    >>> a
    APyFixedArray([[0, 1],
                   [2, 3]], int_bits=4, frac_bits=0)

    A 3-D VUnit array can be imported as

    >>> b = apy.import_csv(
    ...     "input_vunit.csv",
    ...     int_bits=4,
    ...     frac_bits=0,
    ...     layout="vunit",
    ...     vunit_3d_shape=(2, 2, 2),
    ... )
    >>> b
    APyFixedArray([[[0, 1],
                    [2, 3]],
    <BLANKLINE>
                   [[4, 5],
                    [6, 7]]], int_bits=4, frac_bits=0)

    """

    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, man_bits, bias)

    if not a_type:
        raise ValueError("Could not determine array type from bit-specifiers")

    if force_complex:
        raise ValueError("Complex data types are not supported yet")

    if layout not in ("default", "vunit"):
        raise ValueError(f"Unknown layout: {layout}")

    if layout != "vunit" and vunit_3d_shape is not None:
        raise ValueError("vunit_3d_shape provided while not using 'vunit' layout")

    with Path(fname).open(mode="r", newline="") as f:
        reader = csv.reader(f, delimiter=delimiter)
        bit_values = []
        for row in reader:
            bit_values_row = [int(bit_value) for bit_value in row]
            bit_values.append(bit_values_row)

    # Check if 1D or 2D array
    if len(bit_values) == 1:
        bit_values = bit_values[0]

    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        arr = APyFixedArray(
            bit_values, bits=bits, int_bits=int_bits, frac_bits=frac_bits
        )
    else:
        arr = APyFloatArray.from_bits(
            bit_values, exp_bits=exp_bits, man_bits=man_bits, bias=bias
        )

    if layout == "vunit":
        if vunit_3d_shape is None:
            return arr.T
        else:
            arr = arr.reshape(
                (vunit_3d_shape[1], vunit_3d_shape[0], vunit_3d_shape[2])
            ).transpose((1, 0, 2))

    return arr


def meshgrid(
    *arrays: APyArray,
    indexing: Literal["xy", "ij"] = "xy",
) -> list[APyArray]:
    """
    Create a list of coordinate arrays based on several coordinate vectors (1-D arrays).
    The input vectors must all have the same bit specifiers.

    .. versionadded:: 0.4

    Parameters
    ----------
    arrays : :class:`APyFixedArray`, :class:`APyFloatArray`, :class:`APyCFixedArray`, :class:`APyCFloatArray`
        The coordinate vectors (1-D arrays).
    indexing : {"xy", "ij"}, default: "xy"
        The indexing mode to use for the meshgrid. "xy" is the standard Cartesian indexing,
        while "ij" is the matrix indexing.

    Returns
    -------
    result : list of :class:`APyFixedArray`, :class:`APyFloatArray`, :class:`APyCFixedArray`, :class:`APyCFloatArray`
        List of 2-D arrays representing the meshgrid.

    Examples
    --------
    >>> import apytypes as apy
    >>> x = apy.fx(range(3), int_bits=5, frac_bits=0)
    >>> y = apy.fx(range(4, 7), int_bits=5, frac_bits=0)
    >>> xx, yy = apy.meshgrid(x, y)
    >>> xx
    APyFixedArray([[0, 1, 2],
                   [0, 1, 2],
                   [0, 1, 2]], int_bits=5, frac_bits=0)
    >>> yy
    APyFixedArray([[4, 4, 4],
                   [5, 5, 5],
                   [6, 6, 6]], int_bits=5, frac_bits=0)

    """
    if not arrays:
        raise ValueError("meshgrid: at least one array is required")

    if not all(type(arr) is type(arrays[0]) for arr in arrays):
        raise ValueError("meshgrid: input arrays must be of same type")

    try:
        meshgrid = arrays[0]._meshgrid
    except AttributeError:
        raise ValueError("meshgrid: input arrays must have a meshgrid method")

    return meshgrid(arrays, indexing)


# =============================================================================
# Helpers
# =============================================================================
def _determine_array_type(
    int_bits: int | None,
    frac_bits: int | None,
    bits: int | None,
    exp_bits: int | None,
    man_bits: int | None,
    bias: int | None,
) -> type[APyFixedArray] | type[APyFloatArray] | None:
    is_float_array = exp_bits is not None or man_bits is not None or bias is not None
    is_fixed_array = bits is not None or int_bits is not None or frac_bits is not None

    if is_float_array and not is_fixed_array:
        return APyFloatArray
    elif not is_float_array and is_fixed_array:
        return APyFixedArray
    else:
        return None


def _determine_fill_value(
    fill_value: int | float | complex | APyScalar,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyScalar:
    if isinstance(fill_value, int | float | complex):
        if man_bits is None and exp_bits is None:
            FixedType = APyCFixed if isinstance(fill_value, complex) else APyFixed
            return FixedType.from_float(fill_value, int_bits, frac_bits, bits)
        else:
            assert exp_bits is not None
            assert man_bits is not None
            FloatType = APyCFloat if isinstance(fill_value, complex) else APyFloat
            return FloatType.from_float(fill_value, exp_bits, man_bits, bias)
    elif isinstance(fill_value, APyFixed | APyCFixed):
        if int_bits is None and frac_bits is None and bits is None:
            return fill_value
        else:
            return fill_value.cast(int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif isinstance(fill_value, APyFloat | APyCFloat):
        if exp_bits is None and man_bits is None and bias is None:
            return fill_value
        else:
            return fill_value.cast(exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError(
            "Only `int`, `float`, `complex`, and apytypes' scalars are supported "
            + "fill values"
        )


def _normalize_axis(axis: int, ndim: int) -> int:
    """Normalize a single axis."""
    if axis < 0:
        axis += ndim
    if axis < 0 or axis >= ndim:
        raise ValueError(f"Axis {axis} is out of bounds for array of dimension {ndim}")
    return axis


def _normalize_axis_sequence(
    axis_sequence: int | tuple[int, ...], ndim: int
) -> tuple[int, ...]:
    """Normalize a sequence of axes."""
    if isinstance(axis_sequence, int):
        axis_sequence = (axis_sequence,)
    return tuple(_normalize_axis(ax, ndim) for ax in axis_sequence)
