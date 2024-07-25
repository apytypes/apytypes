from typing import Tuple, Union


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

    >>> APyTypes.reshape(a, (2, 2)).to_numpy()
    array([[ 1. ,  1.5],
           [-2. , -1.5]])

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
    from ApyTypes import ApyFloatArray
    >>> a = APyFloatArray.from_float(
    ...     [[1.0, 2.0, 3.0], [-4.0, -5.0, -6.0]],
    ...     exp_bits=5,
    ...     man_bits=2,
    ... )
    >>> a.to_numpy()
    array([[ 1.,  2.,  3.],
           [-4., -5., -6.]])
    >>> a = ApyTypes.transpose(a)
    >>> a.to_numpy()
    array([[ 1., -4.],
           [ 2., -5.],
           [ 3., -6.]])

    >>> a = APyFloatArray.from_float(
    ...     [1.0] * 6,
    ...     exp_bits=5,
    ...     man_bits=2,
    ... ).reshape((1, 2, 3))
    >>> APytypes.transpose(a, (1, 0, 2)).shape
    (2, 1, 3)

    >>> APytypes.transpose(a, (-2, -3, -1)).shape
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
    Return a copy of the array collapsed into one dimension. Same as flatten with current memory-copy model.

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
        Destination positions for each of the original axes. These must also be
        unique.

    Examples
    --------
    import apytypes as apy


    >>> x = apy.ApyFixedArray.from_floats([0]*100, int_bits=2**16, frac_bits=0).reshape((3, 4, 5)))
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

    >>> a = APyFloatArray.from_float(
    ...     [[1.0, 2.0, 3.0], [-4.0, -5.0, -6.0]],
    ...     exp_bits=5,
    ...     man_bits=2,
    ... )
    >>> from apytypes import APyFloatArray
    >>> x = APyTypes.from_float([[1, 2, 3]], exp_bits=5, man_bits=2)
    >>> x.swapaxes(0, 1).to_numpy()
    array([[1.],
         [2.],
         [3.]])

    >>> x = np.from_float([[[0, 1], [2, 3]], [[4, 5], [6, 7]]], exp_bits=5, man_bits=0)
    >>> x.to_numpy()
    array([[[0., 1.],
          [2., 3.]],
         [[4., 5.],
          [6., 7.]]])

    >>> x.swapaxes(0, 2).to_numpy()
    array([[[0., 4.],
          [2., 6.]],
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
    >>> x = apy.ApyFixedArray.from_float([1, 2], int_bits=5, frac_bits=0)
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
    array([[[1, 2]]])

    >>> y = apy.expand_dims(x, axis=(2, 0))
    >>> y.to_numpy
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


def zeros(shape, int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None):
    """
    Initializes an array with zeros.

    Word lengths need to be specified and decide the return type of the array.

    Parameters
    ----------
    shape : tuple
        Shape of the array.
    int_bits : int, optional
        Number of integer bits.
    frac_bits : int, optional
        Number of fractional bits.
    exp_bits : int, optional
        Number of exponential bits.
    mantissa_bits : int, optional
        Number of mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The initialized array with zeros.
    """

    from apytypes import APyFixedArray, APyFloatArray

    a_type = _get_return_type(int_bits, frac_bits, exp_bits, mantissa_bits)
    try:
        zeros = a_type.zeros
    except AttributeError:
        raise TypeError(f"Cannot make zeros array of type {type(a_type)}")
    if a_type is APyFixedArray:
        return zeros(shape=shape, int_bits=int_bits, frac_bits=frac_bits)

    if a_type is APyFloatArray:
        return zeros(shape=shape, exp_bits=exp_bits, man_bits=mantissa_bits)

    raise ValueError("Only 'fixed' and 'float' array_types are defined")


def ones(shape, int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None):
    """
    Initializes an array with ones.

    Word lengths need to be specified and decide the return type of the array.

    Parameters
    ----------
    shape : tuple
        Shape of the array.
    int_bits : int, optional
        Number of integer bits.
    frac_bits : int, optional
        Number of fractional bits.
    exp_bits : int, optional
        Number of exponential bits.
    mantissa_bits : int, optional
        Number of mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array initialized filled with ones.
    """

    from apytypes import APyFixedArray, APyFloatArray

    a_type = _get_return_type(int_bits, frac_bits, exp_bits, mantissa_bits)
    try:
        ones = a_type.ones
    except AttributeError:
        raise TypeError(f"Cannot make ones array of type {type(a_type)}")
    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        return ones(shape=shape, int_bits=int_bits, frac_bits=frac_bits)

    if a_type is APyFloatArray or isinstance(a_type, APyFloatArray):
        return ones(shape=shape, exp_bits=exp_bits, man_bits=mantissa_bits)

    raise ValueError("Only 'fixed' and 'float' array_types are defined")


def eye(
    n: int, m=None, int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None
):
    """
    Return a 2-D array with ones on the diagonal and zeros elsewhere.

    Word lengths need to be specified and decide the return type of the array.

    Parameters
    ----------
    a_type : :class:`APyFloatArray` or :class:`APyFixedArray`, optional
        The type of array to initialize.
    n : int
        Number of rows in the output.
    m : int, optional
        Number of columns in the output. If None, defaults to N.
    int_bits : int, optional
        Number of integer bits.
    frac_bits : int, optional
        Number of fractional bits.
    exp_bits : int, optional
        Number of exponential bits.
    mantissa_bits : int, optional
        Number of mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        An array where all elements are equal to zero, except for the k-th diagonal, whose values are equal to one.
    """

    from apytypes import APyFixedArray, APyFloatArray

    a_type = _get_return_type(int_bits, frac_bits, exp_bits, mantissa_bits)
    try:
        eye = a_type.eye
    except AttributeError:
        raise TypeError(f"Cannot make eye array of type {type(a_type)}")
    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        return eye(n=n, m=m, int_bits=int_bits, frac_bits=frac_bits)

    if a_type is APyFloatArray or isinstance(a_type, APyFloatArray):
        return eye(n=n, m=m, exp_bits=exp_bits, man_bits=mantissa_bits)

    raise ValueError("Only 'fixed' and 'float' array_types are defined")


def identity(n, int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None):
    """
    Return the identity array.

    Word lengths need to be specified and decides return type of Array.

    Parameters
    ----------
    a_type : :class:`APyFloatArray` or :class:`APyFixedArray`, optional
        The type of array to initialize.
    n : int
        Number of rows (and columns) in n x n output.
    int_bits : int, optional
        Number of integer bits.
    frac_bits : int, optional
        Number of fractional bits.
    exp_bits : int, optional
        Number of exponential bits.
    mantissa_bits : int, optional
        Number of mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        n x n array with its main diagonal set to one, and all other elements 0.
    """

    from apytypes import APyFixedArray, APyFloatArray

    a_type = _get_return_type(int_bits, frac_bits, exp_bits, mantissa_bits)
    try:
        identity = a_type.identity
    except AttributeError:
        raise TypeError(f"Cannot make identity array of type {type(a_type)}")

    if a_type is APyFixedArray or isinstance(a_type, APyFixedArray):
        return identity(n=n, int_bits=int_bits, frac_bits=frac_bits)

    if a_type is APyFloatArray or isinstance(a_type, APyFloatArray):
        return identity(n=n, exp_bits=exp_bits, man_bits=mantissa_bits)

    raise ValueError("Only 'fixed' and 'float' array_types are defined")


def full(
    shape,
    fill_value,
    int_bits=None,
    frac_bits=None,
    exp_bits=None,
    mantissa_bits=None,
):
    """
    Return a new array of given shape and type, filled with fill_value.

    If fill_value is an int or float, you must specify the word lengths (int_bits, frac_bits or exp_bits, mantissa_bits).
    If fill_value is an APyFloat or APyFixed, the array will use the provided word lengths if specified.
    If no word lengths are specified, the resulting array will inherit the word lengths from fill_value.

    Parameters
    ----------
    shape : tuple
        Shape of the array.
    fill_value : :class:`APyFloat` or :class:`APyFixedArray` or int or float
        Fill value.
    int_bits : int, optional
        Number of integer bits.
    frac_bits : int, optional
        Number of fractional bits.
    exp_bits : int, optional
        Number of exponential bits.
    mantissa_bits : int, optional
        Number of mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Array of fill_value with the given shape.
    """

    from apytypes import APyFixedArray, APyFixed, APyFloat, APyFloatArray

    fill_value = _normalize_fill_value(
        fill_value, int_bits, frac_bits, exp_bits, mantissa_bits
    )
    try:
        if isinstance(fill_value, APyFixed):
            full = APyFixedArray.full
        elif isinstance(fill_value, APyFloat):
            full = APyFloatArray.full
        else:
            raise (AttributeError)
    except AttributeError:
        raise TypeError(f"Cannot make full array of type {type(fill_value)}")
    return full(shape, fill_value)


def zeros_like(a, int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None):
    """
    Return an array of zeros with the same shape and type as a given array.
    Defaults to `a` wordlength

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The shape and data-type define these same attributes of the returned array.
    int_bits : int, optional
        Number of integer bits.
    frac_bits : int, optional
        Number of fractional bits.
    exp_bits : int, optional
        Number of exponential bits.
    mantissa_bits : int, optional
        Number of mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The initialized array with zeros.
    """
    try:
        zeros = a.zeros
    except AttributeError:
        raise TypeError(f"Cannot make zeros array of type {type(a)}")

    from apytypes import APyFixedArray, APyFloatArray

    if isinstance(a, APyFixedArray):
        int_bits = int_bits or a.int_bits
        frac_bits = frac_bits or a.frac_bits
        return zeros(shape=a.shape, int_bits=int_bits, frac_bits=frac_bits)

    if isinstance(a, APyFloatArray):
        exp_bits = exp_bits or a.exp_bits
        mantissa_bits = mantissa_bits or a.man_bits
        return zeros(shape=a.shape, exp_bits=exp_bits, man_bits=mantissa_bits)

    raise ValueError("Only 'fixed' and 'float' array_types are defined")


def ones_like(a, int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None):
    """
    Return an array of ones with the same shape and type as a given array.
    Defaults to `a` wordlength

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The shape and data-type of a define these same attributes of the returned array.
    int_bits : int, optional
        Number of integer bits.
    frac_bits : int, optional
        Number of fractional bits.
    exp_bits : int, optional
        Number of exponential bits.
    mantissa_bits : int, optional
        Number of mantissa bits.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array initialized filled with ones.
    """
    try:
        ones = a.ones
    except AttributeError:
        raise TypeError(f"Cannot make ones array of type {type(a)}")

    from apytypes import APyFixedArray, APyFloatArray

    if isinstance(a, APyFixedArray):
        int_bits = int_bits or a.int_bits
        frac_bits = frac_bits or a.frac_bits
        return ones(shape=a.shape, int_bits=int_bits, frac_bits=frac_bits)

    if isinstance(a, APyFloatArray):
        exp_bits = exp_bits or a.exp_bits
        mantissa_bits = mantissa_bits or a.man_bits
        return ones(shape=a.shape, exp_bits=exp_bits, man_bits=mantissa_bits)

    raise ValueError("Only 'fixed' and 'float' array_types are defined")


def full_like(
    a,
    fill_value,
    int_bits=None,
    frac_bits=None,
    exp_bits=None,
    mantissa_bits=None,
):
    """
    Return a full array with the same shape and type as a given array.

    If fill_value is an int or float, you must specify the word lengths (int_bits, frac_bits or exp_bits, mantissa_bits).
    If fill_value is an APyFloat or APyFixed, the array will use the provided word lengths if specified.
    If no word lengths are specified, the resulting array will inherit the word lengths from fill_value.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The shape and array type of the returned array.
    fill_value : :class:`APyFloat` or :class:`APyFixed` or int or float
        The value to fill the array with.
    int_bits : int, optional
        Number of integer bits for APyFixed.
    frac_bits : int, optional
        Number of fractional bits for APyFixed.
    exp_bits : int, optional
        Number of exponent bits for APyFloat.
    mantissa_bits : int, optional
        Number of mantissa bits for APyFloat.

    Returns
    -------
    result : :class:`APyFloatArray` or :class:`APyFixedArray`
        Array filled with fill_value, having the same shape and type as `a`.
    """
    try:
        full = a.full
    except AttributeError:
        raise TypeError(f"Cannot make full array of type {type(a)}")

    fill_value = _normalize_fill_value(
        fill_value, int_bits, frac_bits, exp_bits, mantissa_bits
    )
    return full(a.shape, fill_value)


# =============================================================================
# Helpers
# =============================================================================
def _get_return_type(int_bits, frac_bits, exp_bits, mantissa_bits):
    from apytypes import APyFixedArray, APyFloatArray

    if int_bits and frac_bits:
        return APyFixedArray
    elif exp_bits and mantissa_bits:
        return APyFloatArray
    raise ValueError("You need to specify wordlengths in this function")


def _normalize_fill_value(
    fill_value,
    int_bits=None,
    frac_bits=None,
    exp_bits=None,
    mantissa_bits=None,
):
    from apytypes import APyFixed, APyFloat

    if isinstance(fill_value, (int, float)):
        if int_bits is not None and frac_bits is not None:
            return APyFixed.from_float(
                fill_value, int_bits=int_bits, frac_bits=frac_bits
            )
        elif exp_bits is not None and mantissa_bits is not None:
            return APyFloat.from_float(
                fill_value, exp_bits=exp_bits, man_bits=mantissa_bits
            )
        else:
            raise ValueError(
                "You need to specify wordlengths if input is a Python float or integer"
            )

    elif isinstance(fill_value, APyFixed):
        int_bits = int_bits or fill_value.int_bits
        frac_bits = frac_bits or fill_value.frac_bits
        return fill_value.cast(int_bits, frac_bits)

    elif isinstance(fill_value, APyFloat):
        exp_bits = exp_bits or fill_value.exp_bits
        mantissa_bits = mantissa_bits or fill_value.man_bits
        return fill_value.cast(exp_bits, mantissa_bits, fill_value.bias)

    else:
        raise ValueError(
            "Only int, float, APyFloat and APyFixed are supported for fill_value"
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
