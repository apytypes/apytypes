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


def reshape(a, new_shape):
    """
    Reshape an APyFixedArray or APyFloatArray to the specified shape without changing its data.

    Parameters
    ----------
    a : :class:`APyFloatArray` or :class:`APyFixedArray`
        The array to reshape.
    new_shape : tuple of ints
        The new shape should be compatible with the original shape. If a dimension is -1, its value will be inferred from the length of the array and remaining dimensions. Only one dimension can be -1.

    Returns
    -------
    reshaped : :class:`APyFloatArray` or :class:`APyFixedArray`
        The reshaped array.

    Raises
    ------
    ValueError
        If negative dimensions less than -1 are provided, if the total size of the new array is not unchanged and divisible by the known dimensions, or if the total number of elements does not match the original array.

    Examples
    --------
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
