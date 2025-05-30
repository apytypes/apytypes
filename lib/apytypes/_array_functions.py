from collections.abc import Sequence
from typing import Literal

from apytypes._apytypes import (
    APyCFixedArray,
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
)
from apytypes._typing import APyArray, APyScalar


def squeeze(a: APyArray, axis: int | tuple[int, ...] | None = None) -> APyArray:
    """
    Remove axes of length one from `a`.

    Parameters
    ----------
    a : :class:`APyFloatArray`, :class:`APyFixedArray`, or :class:`APyCFixedArray`
        Input data.

    axis : :class:`int` or tuple of ints, optional
        Selects a subset of the entries of length one in the shape.

    Returns
    -------
    squeezed : :class:`APyFloatArray`, :class:`APyFixedArray`, or :class:`APyCFixedArray`
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


def transpose(a: APyArray, axes: tuple[int, ...] | None = None) -> APyArray:
    """
    Return an copy of the array with axes transposed.

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
    >>> x.swapaxes(0, 1).to_numpy()
    array([[1.],
           [2.],
           [3.]])

    >>> x = apy.fp([[[0, 1], [2, 3]], [[4, 5], [6, 7]]], exp_bits=5, man_bits=3)
    >>> x.to_numpy()
    array([[[0., 1.],
            [2., 3.]],
    <BLANKLINE>
           [[4., 5.],
            [6., 7.]]])

    >>> apy.swapaxes(x, 0, 2).to_numpy()
    array([[[0., 4.],
            [2., 6.]],
    <BLANKLINE>
           [[1., 5.],
            [3., 7.]]])

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


def expand_dims(a: APyArray, axis: int | tuple[int, ...]) -> APyArray:
    """
    Expand the shape of an array.

    Insert a new axis that will appear at the `axis` position in the expanded
    array shape.

    Parameters
    ----------
    a : :class:`APyFloatArray`, :class:`APyFixedArray`, or :class:`APyCFixedArray`
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
    >>> y.to_numpy()
    array([[1., 2.]])
    >>> y.shape
    (1, 2)

    >>> y = apy.expand_dims(x, axis=1)
    >>> y.to_numpy()
    array([[1.],
           [2.]])
    >>> y.shape
    (2, 1)

    ``axis`` may also be a tuple:

    >>> y = apy.expand_dims(x, axis=(0, 1))
    >>> y.to_numpy()
    array([[[1., 2.]]])

    >>> y = apy.expand_dims(x, axis=(2, 0))
    >>> y.to_numpy()
    array([[[1.],
            [2.]]])

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Copy of `a` with the number of dimensions increased.
    """

    if type(axis) not in (tuple, list):
        axis = (axis,)

    out_ndim = len(axis) + a.ndim
    axis = _normalize_axis_sequence(axis, out_ndim)

    shape_it = iter(a.shape)
    shape = tuple([1 if ax in axis else next(shape_it) for ax in range(out_ndim)])

    return a.reshape(shape)


# =============================================================================
# array creation
# =============================================================================


def zeros(
    shape: tuple[int, ...],
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

    if not a_type:
        raise ValueError("Could not determine array type from bit-specifiers")

    try:
        zeros = a_type.zeros
    except AttributeError:
        raise TypeError(f"Cannot make zeros array of type {type(a_type)}")

    if a_type is APyFixedArray:
        if force_complex:
            return APyCFixedArray.zeros(
                shape=shape, int_bits=int_bits, frac_bits=frac_bits, bits=bits
            )
        else:
            return zeros(shape=shape, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray:
        if force_complex:
            raise NotImplementedError("APyCFloatArray.zeros not implemented yet")
        return zeros(shape=shape, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def ones(
    shape: tuple[int, ...],
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

    if not a_type:
        raise ValueError("Could not determine array type from bit-specifiers")

    try:
        ones = a_type.ones
    except AttributeError:
        raise TypeError(f"Cannot make ones array of type {type(a_type)}")

    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        if force_complex:
            return APyCFixedArray.ones(
                shape=shape, int_bits=int_bits, frac_bits=frac_bits, bits=bits
            )
        else:
            return ones(shape=shape, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray or isinstance(a_type, APyFloatArray):
        if force_complex:
            raise NotImplementedError("APyCFloatArray.ones not implemented yet")
        return ones(shape=shape, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def eye(
    n: int,
    m: int | None = None,
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

    if not a_type:
        raise ValueError("Could not determine array type from bit-specifiers")

    try:
        eye = a_type.eye
    except AttributeError:
        raise TypeError(f"Cannot make eye array of type {type(a_type)}")

    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        if force_complex:
            return APyCFixedArray.eye(
                n=n, m=m, int_bits=int_bits, frac_bits=frac_bits, bits=bits
            )
        else:
            return eye(n=n, m=m, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray or isinstance(a_type, APyFloatArray):
        if force_complex:
            raise NotImplementedError("APyCFloatArray.eye not implemented yet")
        return eye(n=n, m=m, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def identity(
    n: int,
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

    if not a_type:
        raise ValueError("Could not determine array type from bit-specifiers")

    try:
        identity = a_type.identity
    except AttributeError:
        raise TypeError(f"Cannot make identity array of type {type(a_type)}")

    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        if force_complex:
            return APyCFixedArray.identity(
                n=n, int_bits=int_bits, frac_bits=frac_bits, bits=bits
            )
        else:
            return identity(n=n, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray or isinstance(a_type, APyFloatArray):
        if force_complex:
            raise NotImplementedError("APyCFloatArray.identity not implemented yet")
        return identity(n=n, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def full(
    shape: tuple[int, ...],
    fill_value: APyScalar | int | float,
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
    else:
        raise TypeError(f"Cannot make full array of type {type(fill_value)}")


def zeros_like(
    a: APyArray,
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
    :class:`APyCFixedArray`, or specify both `exp_bits` and `man_bits` for :class:`APyFloatArray`.

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
    try:
        zeros = a.zeros
    except AttributeError:
        raise TypeError(f"Cannot make zeros array of type {type(a)}")

    if isinstance(a, APyFixedArray | APyCFixedArray):
        if int_bits is None and frac_bits is None and bits is None:
            return zeros(shape=a.shape, int_bits=a.int_bits, frac_bits=a.frac_bits)
        else:
            i_bits, f_bits = _extract_fixed_bit_specifiers(int_bits, frac_bits, bits)
            return zeros(shape=a.shape, int_bits=i_bits, frac_bits=f_bits)
    elif isinstance(a, APyFloatArray):
        exp_bits = exp_bits if exp_bits is not None else a.exp_bits
        man_bits = man_bits if man_bits is not None else a.man_bits
        bias = bias if bias is not None else a.bias
        return zeros(shape=a.shape, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def ones_like(
    a: APyArray,
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
    try:
        ones = a.ones
    except AttributeError:
        raise TypeError(f"Cannot make ones array of type {type(a)}")

    if isinstance(a, APyFixedArray | APyCFixedArray):
        if int_bits is None and frac_bits is None and bits is None:
            return ones(shape=a.shape, int_bits=a.int_bits, frac_bits=a.frac_bits)
        else:
            i_bits, f_bits = _extract_fixed_bit_specifiers(int_bits, frac_bits, bits)
            return ones(shape=a.shape, int_bits=i_bits, frac_bits=f_bits)
    elif isinstance(a, APyFloatArray):
        exp_bits = exp_bits if exp_bits is not None else a.exp_bits
        man_bits = man_bits if man_bits is not None else a.man_bits
        bias = bias if bias is not None else a.bias
        return ones(shape=a.shape, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def full_like(
    a: APyArray,
    fill_value: APyScalar | int | float,
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
    return full(a.shape, fill_value)


def arange(
    start: APyScalar | int | float,
    stop: APyScalar | int | float | None = None,
    step: APyScalar | int | float = 1,
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

    If no bit-specifiers are given, the array type is deduced based on ``start``, ``stop``, and ``step``.
    In this case, all APyTypes scalars must be of the same format.

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

    # The values will be generated using APyFixed so exceptions on infinity and NaN will be generated by the constructors
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
    float_arr = exp_bits is not None or man_bits is not None or bias is not None
    fixed_arr = bits is not None or int_bits is not None or frac_bits is not None

    if float_arr and not fixed_arr:
        return APyFloatArray
    elif not float_arr and fixed_arr:
        return APyFixedArray
    else:
        return None


def _determine_fill_value(
    fill_value: int | float | APyFixed | APyFloat,
    int_bits: int | None = None,
    frac_bits: int | None = None,
    bits: int | None = None,
    exp_bits: int | None = None,
    man_bits: int | None = None,
    bias: int | None = None,
) -> APyFloat | APyFixed:
    if isinstance(fill_value, int | float):
        if man_bits is None and exp_bits is None:
            i_bits, f_bits = _extract_fixed_bit_specifiers(int_bits, frac_bits, bits)
            return APyFixed.from_float(fill_value, int_bits=i_bits, frac_bits=f_bits)
        else:
            return APyFloat.from_float(
                fill_value, exp_bits=exp_bits, man_bits=man_bits, bias=bias
            )
    elif isinstance(fill_value, APyFixed):
        if int_bits is None and frac_bits is None and bits is None:
            return fill_value
        else:
            i_bits, f_bits = _extract_fixed_bit_specifiers(int_bits, frac_bits, bits)
            return fill_value.cast(int_bits, frac_bits)
    elif isinstance(fill_value, APyFloat):
        if exp_bits is None and man_bits is None and bias is None:
            return fill_value
        else:
            return fill_value.cast(exp_bits, man_bits, bias)
    else:
        raise ValueError(
            "Only `int`, `float`, `APyFloat` and `APyFixed` are supported fill values"
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


def _extract_fixed_bit_specifiers(
    int_bits: int | None, frac_bits: int | None, bits: int | None
) -> tuple[int, int]:
    """Retrieve `int_bits` and `frac_bits` reliably from a triplet of bit-specifiers"""
    if int_bits is not None and frac_bits is not None and bits is not None:
        if bits != int_bits + frac_bits:
            raise ValueError("Could not extract fixed-point bit-specifiers")
        return (int_bits, frac_bits)
    elif int_bits is not None and frac_bits is not None and bits is None:
        return (int_bits, frac_bits)
    elif int_bits is None and frac_bits is not None and bits is not None:
        return (bits - frac_bits, frac_bits)
    elif int_bits is not None and frac_bits is None and bits is not None:
        return (int_bits, bits - int_bits)
    else:
        raise ValueError("Could not extract fixed-point bit-specifiers")
