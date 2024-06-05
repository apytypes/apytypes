from apytypes import APyFixedArray
from apytypes import APyFixed

import pytest


def test_shape():
    assert APyFixedArray([], bits=1, int_bits=0).shape == (0,)
    assert APyFixedArray([1], bits=1, int_bits=0).shape == (1,)
    assert APyFixedArray([[range(3), range(3)]], bits=1, int_bits=0).shape == (1, 2, 3)
    assert APyFixedArray([range(3), range(3)], bits=1, int_bits=0).shape == (2, 3)


def test_ndim():
    assert APyFixedArray([], bits=1, int_bits=0).ndim == 1
    assert APyFixedArray([1], bits=1, int_bits=0).ndim == 1
    assert APyFixedArray([[range(3), range(3)]], bits=1, int_bits=0).ndim == 3
    assert APyFixedArray([range(3), range(3)], bits=1, int_bits=0).ndim == 2


def test_squeeze():
    a = APyFixedArray([[1], [2], [3]], bits=2, int_bits=2)
    b = a.squeeze()
    assert (b).is_identical(APyFixedArray([1, 2, 3], bits=2, int_bits=2))
    c = APyFixedArray([[[1, 2], [3, 4], [5, 6], [7, 8]]], bits=4, int_bits=4)
    d = c.squeeze()
    assert (d).is_identical(
        APyFixedArray([[1, 2], [3, 4], [5, 6], [7, 8]], bits=4, int_bits=4)
    )
    e = APyFixedArray([1, 2, 3], bits=2, int_bits=2)
    f = e.squeeze()
    assert f.is_identical(e)
    g = APyFixedArray([[[[[[[[2]]]]]]]], bits=2, int_bits=2)
    h = g.squeeze()
    assert h.is_identical(APyFixedArray([2], bits=2, int_bits=2))
    i = APyFixedArray([], bits=4, int_bits=4)
    j = APyFixedArray([[]], bits=4, int_bits=4)
    k = i.squeeze()
    z = j.squeeze()
    assert k.is_identical(i)
    assert z.is_identical(i)
    m = APyFixedArray([[1], [2], [3]], bits=2, int_bits=2)
    with pytest.raises(ValueError):
        _ = m.squeeze(axis=0)
    m1 = m.squeeze(axis=1)
    assert m1.is_identical(APyFixedArray([1, 2, 3], bits=2, int_bits=2))
    with pytest.raises(IndexError):
        _ = m.squeeze(axis=2)

    n = APyFixedArray([[[[[[[[2]]]]]]]], bits=2, int_bits=2)
    o = n.squeeze((0, 1, 2, 3))
    assert o.is_identical(APyFixedArray([[[[2]]]], bits=2, int_bits=2))
    p = n.squeeze((0, 1, 3))
    assert p.is_identical(APyFixedArray([[[[[2]]]]], bits=2, int_bits=2))
    q = APyFixedArray([[[1]], [[2]], [[3]], [[4]]], bits=2, int_bits=2)
    with pytest.raises(ValueError):
        _ = q.squeeze((0, 1, 2))
    with pytest.raises(IndexError):
        _ = m.squeeze((1, 4))


def test_to_numpy():
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    assert np.array_equal(APyFixedArray([], 1, 0).to_numpy(), np.array([]))
    assert np.array_equal(APyFixedArray([1], 2, 0).to_numpy(), np.array([1.0]))

    float_seq = [
        [[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]],
        [[7.0, 8.0, 9.0], [10.0, 11.0, 12.0]],
    ]
    fx_arr = APyFixedArray.from_float(float_seq, bits=10, int_bits=10)
    assert fx_arr.to_numpy().shape == (2, 2, 3)
    assert np.array_equal(fx_arr.to_numpy(), np.array(float_seq))


def test_get_item():
    # ndim == 1
    fx_array = APyFixedArray([1, 2, 3, 4, 5, 6], bits=10, int_bits=10)
    assert fx_array[0].is_identical(APyFixed(1, bits=10, int_bits=10))
    assert fx_array[1].is_identical(APyFixed(2, bits=10, int_bits=10))
    assert fx_array[5].is_identical(APyFixed(6, bits=10, int_bits=10))
    with pytest.raises(IndexError, match="APyFixedArray.__getitem__: index 6 is out"):
        fx_array[6]

    # ndim == 2
    fx_array = APyFixedArray([[1, 2], [3, 4]], bits=10, int_bits=10)
    assert fx_array[0].is_identical(APyFixedArray([1, 2], bits=10, int_bits=10))
    assert fx_array[1].is_identical(APyFixedArray([3, 4], bits=10, int_bits=10))
    with pytest.raises(IndexError, match="APyFixedArray.__getitem__: index 2 is out"):
        fx_array[2]

    # ndim == 3
    fx_array = APyFixedArray([[[1, 2], [3, 4]], [[5, 6], [7, 8]]], bits=10, int_bits=10)
    assert fx_array[0].is_identical(
        APyFixedArray([[1, 2], [3, 4]], bits=10, int_bits=10)
    )
    assert fx_array[1].is_identical(
        APyFixedArray([[5, 6], [7, 8]], bits=10, int_bits=10)
    )
    with pytest.raises(IndexError, match="APyFixedArray.__getitem__: index 2 is out"):
        fx_array[2]


def test_iterator():
    fx_array = APyFixedArray([1, 2, 3, 4, 5, 6], bits=10, int_bits=10)
    iterator = iter(fx_array)
    assert next(iterator).is_identical(APyFixed(1, bits=10, int_bits=10))
    assert next(iterator).is_identical(APyFixed(2, bits=10, int_bits=10))


def test_len():
    fx_array = APyFixedArray([1, 2, 3, 4, 5, 6], bits=10, int_bits=10)
    assert len(fx_array) == 6


def test_round_trip_conversion():
    # Skip this test if `NumPy` is not present on the machine
    pytest.importorskip("numpy")
    for bit_pattern in range(256):
        a = APyFixedArray([bit_pattern], int_bits=4, frac_bits=4)
        assert (APyFixedArray.from_float(a.to_numpy(), 4, 4)).is_identical(a)
