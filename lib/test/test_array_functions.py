from apytypes import squeeze, reshape, APyFixedArray


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
