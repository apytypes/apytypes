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
