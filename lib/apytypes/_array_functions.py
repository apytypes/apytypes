from typing import Optional, Tuple, Union


def squeeze(a, axis=None):
    """
    Remove axes of length one from `a`.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        Input data.

    axis : int or tuple of ints, optional
        Selects a subset of the entries of length one in the shape.

    Returns
    -------
    squeezed : :class:`APyFloatArray` or :class:`APyFixedArray`
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


def convolve(a, v, mode="full"):
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


def reshape(a, new_shape):
    """
    Reshape an :class:`APyFixedArray` or :class:`APyFloatArray` to the specified shape
    without changing its data.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array to reshape.
    new_shape : tuple of ints
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
    >>> from apytypes import reshape, APyFixedArray
    >>> a = APyFixedArray([2, 3, 4, 5], int_bits=2, frac_bits=1)
    >>> a.to_numpy()
    array([ 1. ,  1.5, -2. , -1.5])

    >>> reshape(a, (2, 2)).to_numpy()
    array([[ 1. ,  1.5],
           [-2. , -1.5]])

    ------
    """
    try:
        reshape = a.reshape
    except AttributeError:
        raise TypeError(f"Cannot reshape {type(a)}")
    return reshape(new_shape)


def shape(arr):
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


def transpose(a, axes=None):
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
    axes : tuple of of ints, optional
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
    >>> from apytypes import transpose, APyFloatArray
    >>> a = APyFloatArray.from_float(
    ...     [[1.0, 2.0, 3.0], [-4.0, -5.0, -6.0]],
    ...     exp_bits=5,
    ...     man_bits=2,
    ... )
    >>> a.to_numpy()
    array([[ 1.,  2.,  3.],
           [-4., -5., -6.]])

    >>> a = transpose(a)
    >>> a.to_numpy()
    array([[ 1., -4.],
           [ 2., -5.],
           [ 3., -6.]])

    >>> a = APyFloatArray.from_float(
    ...     [1.0] * 6,
    ...     exp_bits=5,
    ...     man_bits=2,
    ... ).reshape((1, 2, 3))
    >>> transpose(a, (1, 0, 2)).shape
    (2, 1, 3)

    >>> transpose(a, (-2, -3, -1)).shape
    (2, 1, 3)

    --------
    """

    try:
        transpose = a.transpose
    except AttributeError:
        raise TypeError(f"Cannot transpose {type(a)}")
    return transpose(axes=axes)


def ravel(a):
    """
    Return a copy of the array collapsed into one dimension.

    Same as :py:func:`flatten` with current memory-copy model.

    Returns
    -------
    collapsed : :class:`APyFloatArray` or :class:`APyFixedArray`
        copy of `a` but collapsed

    Examples
    --------
    >>> from apytypes import APyFloatArray
    >>> signs = [[0, 0], [1, 1]]
    >>> exps = [[127, 128], [128, 129]]
    >>> mans = [[0, 0], [4194304, 0]]
    >>> arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=8, man_bits=23)
    >>> arr.to_numpy()
    array([[ 1.,  2.],
           [-3., -4.]])

    >>> arr.ravel().to_numpy()
    array([ 1.,  2., -3., -4.])

    --------
    """
    try:
        ravel = a.ravel
    except AttributeError:
        raise TypeError(f"Cannot ravel {type(a)}")
    return ravel()


def moveaxis(a, source, destination):
    """
    Move axes of an array to new positions.

    Other axes remain in their original order.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array whose axes should be reordered.
    source : int or sequence of int
        Original positions of the axes to move. These must be unique.
    destination : int or sequence of int
        Destination positions for each of the original axes. These must also be unique.

    Examples
    --------
    import apytypes as apy

    >>> x = apy.APyFixedArray.from_float([0] * 60, int_bits=2**16, frac_bits=0).reshape(
    ...     (3, 4, 5)
    ... )
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
    order = tuple(order)

    result = transpose(a, order)
    return result


def swapaxes(a, axis1, axis2):
    """
    Interchange two axes of an array.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array whose axes should be reordered.
    axis1 : int
      First axis.
    axis2 : int
      Second axis.

    Examples
    --------
    >>> from apytypes import APyFloatArray
    >>> x = APyFloatArray.from_float([[1, 2, 3]], exp_bits=5, man_bits=2)
    >>> x.swapaxes(0, 1).to_numpy()
    array([[1.],
           [2.],
           [3.]])

    >>> x = APyFloatArray.from_float(
    ...     [[[0, 1], [2, 3]], [[4, 5], [6, 7]]], exp_bits=5, man_bits=3
    ... )
    >>> x.to_numpy()
    array([[[0., 1.],
            [2., 3.]],
    <BLANKLINE>
           [[4., 5.],
            [6., 7.]]])

    >>> x.swapaxes(0, 2).to_numpy()
    array([[[0., 4.],
            [2., 6.]],
    <BLANKLINE>
           [[1., 5.],
            [3., 7.]]])

    Returns
    -------
    a_swapped :class:`APyFloatArray` or :class:`APyFixedArray`
        Copy of `a` with axes swapped
    """

    try:
        swapaxes = a.swapaxes
    except AttributeError:
        raise TypeError(f"Cannot transpose/swapaxis of type: {type(a)}")
    return swapaxes(axis1, axis2)


def expand_dims(a, axis):
    """
    Expand the shape of an array.

    Insert a new axis that will appear at the `axis` position in the expanded
    array shape.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array whose axes should be reordered.
    axis : int or tuple of ints
        Position in the expanded axes where the new axis (or axes) is placed.

    Examples
    --------
    >>> import apytypes as apy
    >>> x = apy.APyFixedArray.from_float([1, 2], int_bits=5, frac_bits=0)
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
        copy of `a` with the number of dimensions increased.
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
    shape, int_bits=None, frac_bits=None, bits=None, exp_bits=None, mantissa_bits=None
):
    """
    Create an array of `shape` with all zeros.

    Word lengths need to be specified and the return type is deduced from the bit
    specifiers. Either specify exactly two of three from `int_bits`, `frac_bits`, and
    `bits`, for :class:`APyFixedArray`, or specify both `exp_bits` and `mantissa_bits`
    for :class:`APyFloatArray`.

    Parameters
    ----------
    shape : tuple
        Shape of the array.
    int_bits : int, optional
        Number of fixed-point integer bits.
    frac_bits : int, optional
        Number of fixed-point fractional bits.
    bits : int, optional
        Number of fixed-point bits.
    exp_bits : int, optional
        Number of floating-point exponential bits.
    mantissa_bits : int, optional
        Number of floating-point mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The initialized array with zeros.
    """

    from apytypes import APyFixedArray, APyFloatArray

    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, mantissa_bits)
    try:
        zeros = a_type.zeros
    except AttributeError:
        raise TypeError(f"Cannot make zeros array of type {type(a_type)}")

    if a_type is APyFixedArray:
        return zeros(shape=shape, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray:
        return zeros(shape=shape, exp_bits=exp_bits, man_bits=mantissa_bits)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def ones(
    shape, int_bits=None, frac_bits=None, bits=None, exp_bits=None, mantissa_bits=None
):
    """
    Create an array of `shape` with all ones (stored value).

    Word lengths need to be specified and the return type is deduced from the bit
    specifiers. Either specify exactly two of three from `int_bits`, `frac_bits`, and
    `bits`, for :class:`APyFixedArray`, or specify both `exp_bits` and `mantissa_bits`
    for :class:`APyFloatArray`.

    Parameters
    ----------
    shape : tuple
        Shape of the array.
    int_bits : int, optional
        Number of fixed-point integer bits.
    frac_bits : int, optional
        Number of fixed-point fractional bits.
    bits : int, optional
        Number of fixed-point bits.
    exp_bits : int, optional
        Number of floating-point exponential bits.
    mantissa_bits : int, optional
        Number of floating-point mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array initialized filled with ones.
    """

    from apytypes import APyFixedArray, APyFloatArray

    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, mantissa_bits)
    try:
        ones = a_type.ones
    except AttributeError:
        raise TypeError(f"Cannot make ones array of type {type(a_type)}")

    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        return ones(shape=shape, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray or isinstance(a_type, APyFloatArray):
        return ones(shape=shape, exp_bits=exp_bits, man_bits=mantissa_bits)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def eye(
    n: int,
    m=None,
    int_bits=None,
    frac_bits=None,
    bits=None,
    exp_bits=None,
    mantissa_bits=None,
):
    """
    Return a 2-D array with ones (stored value) on the main diagonal and zeros
    elsewhere.

    Word lengths need to be specified and the return type is deduced from the bit
    specifiers. Either specify exactly two of three from `int_bits`, `frac_bits`, and
    `bits`, for :class:`APyFixedArray`, or specify both `exp_bits` and `mantissa_bits`
    for :class:`APyFloatArray`.

    Parameters
    ----------
    n : int
        Number of rows in the output.
    m : int, optional
        Number of columns in the output. If `None`, defaults to `n`.
    int_bits : int, optional
        Number of fixed-point integer bits.
    frac_bits : int, optional
        Number of fixed-point fractional bits.
    bits : int, optional
        Number of fixed-point bits.
    exp_bits : int, optional
        Number of floating-point exponential bits.
    mantissa_bits : int, optional
        Number of floating-point mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        An array where all elements are equal to zero, except for the k-th diagonal,
        whose values are equal to one.
    """

    from apytypes import APyFixedArray, APyFloatArray

    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, mantissa_bits)
    try:
        eye = a_type.eye
    except AttributeError:
        raise TypeError(f"Cannot make eye array of type {type(a_type)}")

    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        return eye(n=n, m=m, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray or isinstance(a_type, APyFloatArray):
        return eye(n=n, m=m, exp_bits=exp_bits, man_bits=mantissa_bits)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def identity(
    n, int_bits=None, frac_bits=None, bits=None, exp_bits=None, mantissa_bits=None
):
    """
    Return the identity array.

    Word lengths need to be specified and the return type is deduced from the bit
    specifiers. Either specify exactly two of three from `int_bits`, `frac_bits`, and
    `bits`, for :class:`APyFixedArray`, or specify both `exp_bits` and `mantissa_bits`
    for :class:`APyFloatArray`.

    Parameters
    ----------
    n : int
        Number of rows and columns in output.
    int_bits : int, optional
        Number of fixed-point integer bits.
    frac_bits : int, optional
        Number of fixed-point fractional bits.
    bits : int, optional
        Number of fixed-point bits.
    exp_bits : int, optional
        Number of floating-point exponential bits.
    mantissa_bits : int, optional
        Number of floating-point mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        `n` x `n` array with ones (stored value) on the main diagonal and zeros
        elsewhere.
    """

    from apytypes import APyFixedArray, APyFloatArray

    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, mantissa_bits)
    try:
        identity = a_type.identity
    except AttributeError:
        raise TypeError(f"Cannot make identity array of type {type(a_type)}")

    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        return identity(n=n, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
    elif a_type is APyFloatArray or isinstance(a_type, APyFloatArray):
        return identity(n=n, exp_bits=exp_bits, man_bits=mantissa_bits)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def full(
    shape,
    fill_value,
    int_bits=None,
    frac_bits=None,
    bits=None,
    exp_bits=None,
    mantissa_bits=None,
):
    """
    Return a new array of given shape and type, filled with `fill_value`.

    If `fill_value` is of type `int` or `float`, you must specify the word lengths.
    Either specify exactly two of three from `int_bits`, `frac_bits`, and `bits`, for
    :class:`APyFixedArray`, or specify both `exp_bits` and `mantissa_bits` for
    :class:`APyFloatArray`. If `fill_value` is of type `APyFloat` or `APyFixed`, the
    array will use the provided word lengths only if specified, otherwise the word
    lengths are inherited from `fill_value`.

    Parameters
    ----------
    shape : tuple
        Shape of the array.
    fill_value : :class:`APyFloat` or :class:`APyFixed` or :class:`int` or :class`float`
        Fill value.
    int_bits : int, optional
        Number of fixed-point integer bits.
    frac_bits : int, optional
        Number of fixed-point fractional bits.
    bits : int, optional
        Number of fixed-point bits.
    exp_bits : int, optional
        Number of floating-point exponential bits.
    mantissa_bits : int, optional
        Number of floating-point mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Array of all `fill_value` with the given shape.
    """

    from apytypes import APyFixedArray, APyFixed, APyFloat, APyFloatArray

    fill_value = _determine_fill_value(
        fill_value, int_bits, frac_bits, bits, exp_bits, mantissa_bits
    )
    if isinstance(fill_value, APyFixed):
        return APyFixedArray.full(shape, fill_value)
    elif isinstance(fill_value, APyFloat):
        return APyFloatArray.full(shape, fill_value)
    else:
        raise TypeError(f"Cannot make full array of type {type(fill_value)}")


def zeros_like(
    a,
    int_bits=None,
    frac_bits=None,
    bits=None,
    exp_bits=None,
    mantissa_bits=None,
):
    """
    Return an array of all zeros with the same shape and type as `a`.

    The type and bit-specifiers of the returned array can be overwritten through the
    bit-specifier arguments. To overwrite the type, either specify exactly two of three
    from `int_bits`, `frac_bits`, and `bits`, for :class:`APyFixedArray`, or specify
    both `exp_bits` and `mantissa_bits` for :class:`APyFloatArray`.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The shape and data-type define these same attributes of the returned array.
    int_bits : int, optional
        Number of fixed-point integer bits.
    frac_bits : int, optional
        Number of fixed-point fractional bits.
    bits : int, optional
        Number of fixed-point bits.
    exp_bits : int, optional
        Number of floating-point exponential bits.
    mantissa_bits : int, optional
        Number of floating-point mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The initialized array with all zeros.
    """
    try:
        zeros = a.zeros
    except AttributeError:
        raise TypeError(f"Cannot make zeros array of type {type(a)}")

    from apytypes import APyFixedArray, APyFloatArray

    if isinstance(a, APyFixedArray):
        if int_bits is None and frac_bits is None and bits is None:
            return zeros(shape=a.shape, int_bits=a.int_bits, frac_bits=a.frac_bits)
        else:
            i_bits, f_bits = _extract_fixed_bit_specifiers(int_bits, frac_bits, bits)
            return zeros(shape=a.shape, int_bits=i_bits, frac_bits=f_bits)
    elif isinstance(a, APyFloatArray):
        exp_bits = exp_bits if exp_bits is not None else a.exp_bits
        mantissa_bits = mantissa_bits if mantissa_bits is not None else a.man_bits
        return zeros(shape=a.shape, exp_bits=exp_bits, man_bits=mantissa_bits)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def ones_like(
    a,
    int_bits=None,
    frac_bits=None,
    bits=None,
    exp_bits=None,
    mantissa_bits=None,
):
    """
    Return an array of all ones (stored value) with the same shape and type as `a`.

    The type and bit-specifiers of the returned array can be overwritten through the
    bit-specifier arguments. To overwrite the type, either specify exactly two of three
    from `int_bits`, `frac_bits`, and `bits`, for :class:`APyFixedArray`, or specify
    both `exp_bits` and `mantissa_bits` for :class:`APyFloatArray`.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The shape and data-type of a define these same attributes of the returned array.
    int_bits : int, optional
        Number of fixed-point integer bits.
    frac_bits : int, optional
        Number of fixed-point fractional bits.
    bits : int, optional
        Number of fixed-point bits.
    exp_bits : int, optional
        Number of floating-point exponential bits.
    mantissa_bits : int, optional
        Number of floating-point mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array initialized filled with all ones.
    """
    try:
        ones = a.ones
    except AttributeError:
        raise TypeError(f"Cannot make ones array of type {type(a)}")

    from apytypes import APyFixedArray, APyFloatArray

    if isinstance(a, APyFixedArray):
        if int_bits is None and frac_bits is None and bits is None:
            return ones(shape=a.shape, int_bits=a.int_bits, frac_bits=a.frac_bits)
        else:
            i_bits, f_bits = _extract_fixed_bit_specifiers(int_bits, frac_bits, bits)
            return ones(shape=a.shape, int_bits=i_bits, frac_bits=f_bits)
    elif isinstance(a, APyFloatArray):
        exp_bits = exp_bits if exp_bits is not None else a.exp_bits
        mantissa_bits = mantissa_bits if mantissa_bits is not None else a.man_bits
        return ones(shape=a.shape, exp_bits=exp_bits, man_bits=mantissa_bits)
    else:
        raise ValueError("Only fixed-point and floating-point array types supported")


def full_like(
    a,
    fill_value,
    int_bits=None,
    frac_bits=None,
    bits=None,
    exp_bits=None,
    mantissa_bits=None,
):
    """
    Return an array with all values initialized to `fill_value`, with the same shape and
    type as `a`.

    The type and bit-specifiers of the returned array can be overwritten through the
    bit-specifier arguments. To overwrite the type, either specify exactly two of three
    from `int_bits`, `frac_bits`, and `bits`, for :class:`APyFixedArray`, or specify
    both `exp_bits` and `mantissa_bits` for :class:`APyFloatArray`.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The shape and array type of the returned array.
    fill_value : :class:`APyFloat` or :class:`APyFixed` or int or float
        The value to fill the array with.
    int_bits : int, optional
        Number of fixed-point integer bits.
    frac_bits : int, optional
        Number of fixed-point fractional bits.
    bits : int, optional
        Number of fixed-point bits.
    exp_bits : int, optional
        Number of floating-point exponential bits.
    mantissa_bits : int, optional
        Number of floating-point mantissa bits.

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
        fill_value, int_bits, frac_bits, bits, exp_bits, mantissa_bits
    )
    return full(a.shape, fill_value)


def arange(
    start,
    stop=None,
    step=1,
    int_bits=None,
    frac_bits=None,
    bits=None,
    exp_bits=None,
    man_bits=None,
    bias=None,
):
    """
    Create an array with evenly spaced values within a given interval.

    The function can be called with varying number of positional arguments:

    * ``arange(stop)``: Values are generated within the half-open interval ``[0, stop)`` (in other words, the interval including ``start`` but excluding ``stop``).
    * ``arange(start, stop)``: Values are generated within the half-open interval ``[start, stop)``.
    * ``arange(start, stop, step)``: Values are generated within the half-open interval ``[start, stop)``, with spacing between values given by ``step``.

    Parameters
    ----------
    start : int, float
        Start number.
    stop : int, optional
        Stop number.
    step : int, float, optional
        Step size in range.
    int_bits : int, optional
        Number of fixed-point integer bits.
    frac_bits : int, optional
        Number of fixed-point fractional bits.
    bits : int, optional
        Number of fixed-point bits.
    exp_bits : int, optional
        Number of floating-point exponential bits.
    man_bits : int, optional
        Number of floating-point mantissa bits.
    bias : int, optional
        Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Array with evenly spaced values within a given interval.
    """
    a_type = _determine_array_type(int_bits, frac_bits, bits, exp_bits, man_bits)
    from apytypes import APyFixedArray, APyFloatArray

    if stop is None:
        start, stop = (0, start)

    # The values will be generated using APyFixed so exceptions on infinity and NaN will be generated by the constructors
    if a_type is APyFixedArray:
        return APyFixedArray._arange(start, stop, step, int_bits, frac_bits, bits)
    elif a_type is APyFloatArray:
        return APyFloatArray._arange(start, stop, step, exp_bits, man_bits, bias)
    raise ValueError("Undefined inputs")


# =============================================================================
# Helpers
# =============================================================================
def _determine_array_type(int_bits, frac_bits, bits, exp_bits, mantissa_bits):
    from apytypes import APyFixedArray, APyFloatArray

    if exp_bits is None and mantissa_bits is None:
        if int_bits is not None and frac_bits is not None:
            return APyFixedArray
        elif bits is not None and int_bits is not None:
            return APyFixedArray
        elif bits is not None and frac_bits is not None:
            return APyFixedArray
    elif bits is None and int_bits is None and frac_bits is None:
        if exp_bits is not None and mantissa_bits is not None:
            return APyFloatArray
    else:
        raise ValueError("Could not determine array type from bit-specifiers")


def _determine_fill_value(
    fill_value,
    int_bits=None,
    frac_bits=None,
    bits=None,
    exp_bits=None,
    man_bits=None,
):
    from apytypes import APyFixed, APyFloat

    if isinstance(fill_value, (int, float)):
        if man_bits is None and exp_bits is None:
            i_bits, f_bits = _extract_fixed_bit_specifiers(int_bits, frac_bits, bits)
            return APyFixed.from_float(fill_value, int_bits=i_bits, frac_bits=f_bits)
        else:
            return APyFloat.from_float(fill_value, exp_bits=exp_bits, man_bits=man_bits)
    elif isinstance(fill_value, APyFixed):
        if int_bits is None and frac_bits is None and bits is None:
            return fill_value
        else:
            i_bits, f_bits = _extract_fixed_bit_specifiers(int_bits, frac_bits, bits)
            return fill_value.cast(int_bits, frac_bits)
    elif isinstance(fill_value, APyFloat):
        if exp_bits is None and man_bits is None:
            return fill_value
        else:
            return fill_value.cast(exp_bits, man_bits, fill_value.bias)
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
    axis_sequence: Union[int, Tuple[int, ...]], ndim: int
) -> Tuple[int, ...]:
    """Normalize a sequence of axes."""
    if isinstance(axis_sequence, int):
        axis_sequence = (axis_sequence,)
    return tuple(_normalize_axis(ax, ndim) for ax in axis_sequence)


def _extract_fixed_bit_specifiers(
    int_bits: Optional[int], frac_bits: Optional[int], bits: Optional[int]
) -> Tuple[int, int]:
    """Retrieve `int_bits` and `frac_bits` reliably from a triplet of bit-specifiers"""
    if int_bits is not None and frac_bits is not None and bits is not None:
        if bits != int_bits + frac_bits:
            raise ValueError("Could not extract fixed-point bit-specifiers")
        else:
            return (int_bits, frac_bits)
    elif int_bits is not None and frac_bits is not None and bits is None:
        return (int_bits, frac_bits)
    elif int_bits is None and frac_bits is not None and bits is not None:
        return (bits - frac_bits, frac_bits)
    elif int_bits is not None and frac_bits is None and bits is not None:
        return (int_bits, bits - int_bits)
    else:
        raise ValueError("Could not extract fixed-point bit-specifiers")
