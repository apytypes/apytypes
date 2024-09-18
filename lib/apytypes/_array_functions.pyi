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

def ones_like(
    a, int_bits=None, frac_bits=None, bits=None, exp_bits=None, mantissa_bits=None
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

def zeros_like(
    a, int_bits=None, frac_bits=None, bits=None, exp_bits=None, mantissa_bits=None
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
