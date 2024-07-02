from apytypes import (
    squeeze,
    reshape,
    shape,
    transpose,
    ravel,
    APyFixedArray,
    APyFloatArray,
)


def test_squeeze():
    a = APyFixedArray([[1, 2]], 3, 0)
    assert a.shape == (1, 2)

    b = squeeze(a)
    assert b.shape == (2,)


def test_reshape():
    a = APyFixedArray([[1, 2]], 3, 0)
    assert a.shape == (1, 2)
    b = reshape(a, (2, 1))
    assert b.shape == (2, 1)
    c = reshape(b, (-1,))
    assert c.shape == (2,)


def test_shape():
    a = APyFixedArray([[1, 2]], 3, 0)
    assert shape(a) == (1, 2)
    b = reshape(a, (2, 1))
    assert shape(b) == (2, 1)
    c = reshape(b, (-1,))
    assert shape(c) == (2,)


def test_transpose():
    a = APyFloatArray.from_float([[1, 2], [3, 4]], 5, 5)
    transpose(a).is_identical(APyFloatArray.from_float([[1, 4], [3, 2]], 5, 5))


def test_ravel():
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # manually create 1d arr
    arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=5, man_bits=2)
    reshaped = arr.reshape((3, 2, 2))
    assert APyFloatArray.is_identical(ravel(reshaped), arr)
