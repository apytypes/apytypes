from itertools import permutations
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
    with pytest.raises(
        ValueError,
        match="Cannot select an axis to squeeze that has size other than one",
    ):
        _ = m.squeeze(axis=0)
    m1 = m.squeeze(axis=1)
    assert m1.is_identical(APyFixedArray([1, 2, 3], bits=2, int_bits=2))
    with pytest.raises(
        IndexError,
        match="Specified axis with larger than number of dimensions in the APyFixedArray",
    ):
        _ = m.squeeze(axis=2)

    n = APyFixedArray([[[[[[[[2]]]]]]]], bits=2, int_bits=2)
    o = n.squeeze((0, 1, 2, 3))
    assert o.is_identical(APyFixedArray([[[[2]]]], bits=2, int_bits=2))
    p = n.squeeze((0, 1, 3))
    assert p.is_identical(APyFixedArray([[[[[2]]]]], bits=2, int_bits=2))
    q = APyFixedArray([[[1]], [[2]], [[3]], [[4]]], bits=2, int_bits=2)
    with pytest.raises(
        ValueError,
        match="Cannot select an axis to squeeze that has size other than one",
    ):
        _ = q.squeeze((0, 1, 2))
    with pytest.raises(
        IndexError,
        match="Specified axis with larger than number of dimensions in the APyFixedArray",
    ):
        _ = m.squeeze((1, 4))


@pytest.mark.parametrize("sum_func", ["sum", "nansum"])
def test_sum(sum_func):
    a = APyFixedArray([[1, 2], [3, 4], [5, 6], [7, 8]], bits=5, int_bits=5)
    b = a.__getattribute__(sum_func)()
    assert b.is_identical(APyFixed(36, bits=8, int_bits=8))
    c = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=5)
    d = c.__getattribute__(sum_func)((0, 1))
    e = c.__getattribute__(sum_func)(0)
    f = c.__getattribute__(sum_func)(1)
    assert d.is_identical(APyFixed(15, bits=8, int_bits=8))
    assert e.is_identical(APyFixedArray([3, 5, 7], bits=6, int_bits=6))
    assert f.is_identical(APyFixedArray([3, 12], bits=7, int_bits=7))

    # test for size larger than 32 and 64 when number over multiple limbs
    g = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=33)
    h = g.__getattribute__(sum_func)(0)
    assert h.is_identical(APyFixedArray([3, 5, 7], frac_bits=0, int_bits=34))
    j = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=65)
    k = j.__getattribute__(sum_func)(0)
    assert k.is_identical(APyFixedArray([3, 5, 7], frac_bits=0, int_bits=66))

    # test some float and negative summation
    j = APyFixedArray.from_float([0.25, 1.375, 3.25], frac_bits=3, int_bits=5)
    k = j.__getattribute__(sum_func)()
    assert k.is_identical(APyFixed.from_float(4.875, bits=10, int_bits=7))
    m = APyFixedArray.from_float([0.25, 1.25, 3.25], frac_bits=3, int_bits=5)
    n = m.__getattribute__(sum_func)()
    assert n.is_identical(APyFixed.from_float(4.75, frac_bits=3, int_bits=7))

    o = APyFixedArray([[-1, -2], [-3, -4]], frac_bits=0, int_bits=5)
    p = o.__getattribute__(sum_func)(1)
    assert p.is_identical(APyFixedArray([-3, -7], frac_bits=0, int_bits=6))

    q = APyFixedArray([[-1, -2], [1, 2]], frac_bits=0, int_bits=5)
    r = q.__getattribute__(sum_func)(0)
    assert r.is_identical(APyFixedArray([0, 0], frac_bits=0, int_bits=6))

    m = APyFixedArray([1, 2, 3], bits=2, int_bits=2)
    with pytest.raises(
        IndexError,
        match="specified axis outside number of dimensions in the APyFixedArray",
    ):
        _ = m.__getattribute__(sum_func)(1)


@pytest.mark.parametrize("sum_func", ["cumsum", "nancumsum"])
def test_cumsum(sum_func):
    a = APyFixedArray([[1, 2, 3], [4, 5, 6]], frac_bits=0, int_bits=5)
    b = a.__getattribute__(sum_func)()
    assert b.is_identical(APyFixedArray([1, 3, 6, 10, 15, 21], frac_bits=0, int_bits=8))
    c = a.__getattribute__(sum_func)(0)
    assert c.is_identical(
        APyFixedArray([[1, 2, 3], [5, 7, 9]], frac_bits=0, int_bits=6)
    )
    d = a.__getattribute__(sum_func)(1)
    assert d.is_identical(
        APyFixedArray([[1, 3, 6], [4, 9, 15]], frac_bits=0, int_bits=7)
    )
    e = APyFixedArray([[[1, 2], [3, 4]], [[5, 6], [7, 8]]], frac_bits=0, int_bits=8)
    f = e.__getattribute__(sum_func)()
    g = e.__getattribute__(sum_func)(0)
    h = e.__getattribute__(sum_func)(1)
    i = e.__getattribute__(sum_func)(2)
    assert f.is_identical(
        APyFixedArray([1, 3, 6, 10, 15, 21, 28, 36], frac_bits=0, int_bits=11)
    )
    assert g.is_identical(
        APyFixedArray([[[1, 2], [3, 4]], [[6, 8], [10, 12]]], frac_bits=0, int_bits=9)
    )
    assert h.is_identical(
        APyFixedArray([[[1, 2], [4, 6]], [[5, 6], [12, 14]]], frac_bits=0, int_bits=9)
    )
    assert i.is_identical(
        APyFixedArray([[[1, 3], [3, 7]], [[5, 11], [7, 15]]], frac_bits=0, int_bits=9)
    )
    with pytest.raises(
        IndexError,
        match="specified axis outside number of dimensions in the APyFixedArray",
    ):
        _ = e.__getattribute__(sum_func)(4)

    k = APyFixedArray.from_float(
        [[0.25, 0.25], [0.25, 0.25]], frac_bits=10, int_bits=10
    )
    m = k.__getattribute__(sum_func)()
    assert m.is_identical(
        APyFixedArray.from_float([0.25, 0.5, 0.75, 1], frac_bits=10, int_bits=12)
    )

    # test for size larger than 32 and 64 when number over multiple limbs
    g = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=33)
    h = g.__getattribute__(sum_func)(0)
    assert h.is_identical(
        APyFixedArray([[0, 1, 2], [3, 5, 7]], frac_bits=0, int_bits=34)
    )
    j = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=65)
    k = j.__getattribute__(sum_func)(0)
    assert k.is_identical(
        APyFixedArray([[0, 1, 2], [3, 5, 7]], frac_bits=0, int_bits=66)
    )

    n = APyFixedArray([[[1, 2], [3, 4]], [[5, 6], [7, 8]]], frac_bits=0, int_bits=5)
    o = n.sum((0, 2))
    assert o.is_identical(APyFixedArray([14, 22], frac_bits=0, int_bits=7))


@pytest.mark.parametrize("prod_func", ["prod", "nanprod"])
def test_prod(prod_func):
    a = APyFixedArray([[1, 2], [3, 4], [5, 6], [7, 8]], bits=5, int_bits=5)
    b = a.__getattribute__(prod_func)()
    assert b.is_identical(APyFixed(40320, bits=40, int_bits=40))
    c = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=5)
    d = c.__getattribute__(prod_func)((0, 1))
    e = c.__getattribute__(prod_func)(0)
    f = c.__getattribute__(prod_func)(1)
    assert d.is_identical(APyFixed(0, bits=30, int_bits=30))
    assert e.is_identical(APyFixedArray([0, 4, 10], bits=10, int_bits=10))
    assert f.is_identical(APyFixedArray([0, 60], bits=15, int_bits=15))
    # test for size larger than 32 and 64 when number over multiple limbs
    g = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=33)
    h = g.__getattribute__(prod_func)(0)
    assert h.is_identical(APyFixedArray([0, 4, 10], frac_bits=0, int_bits=66))
    j = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=65)
    k = j.__getattribute__(prod_func)(0)
    assert k.is_identical(APyFixedArray([0, 4, 10], frac_bits=0, int_bits=130))

    # test some float and negative multiplication
    j = APyFixedArray.from_float([0.25, 8], frac_bits=3, int_bits=5)
    k = j.__getattribute__(prod_func)()
    assert k.is_identical(APyFixed.from_float(2, frac_bits=6, int_bits=10))

    o = APyFixedArray([[-1, -2], [-3, -4]], frac_bits=0, int_bits=5)
    p = o.__getattribute__(prod_func)(1)
    assert p.is_identical(APyFixedArray([2, 12], frac_bits=0, int_bits=10))

    q = APyFixedArray([[-1, -2], [1, 2]], frac_bits=0, int_bits=5)
    r = q.__getattribute__(prod_func)(0)
    assert r.is_identical(APyFixedArray([-1, -4], frac_bits=0, int_bits=10))

    m = APyFixedArray([1, 2, 3], bits=2, int_bits=2)
    with pytest.raises(IndexError):
        _ = m.__getattribute__(prod_func)(1)

    n = APyFixedArray.from_float([[0.25, 0.5]], frac_bits=10, int_bits=10)
    o = n.__getattribute__(prod_func)(1)
    assert o.is_identical(APyFixedArray.from_float([0.125], frac_bits=20, int_bits=20))

    n = APyFixedArray([[[1, 2], [3, 4]], [[5, 6], [7, 8]]], frac_bits=0, int_bits=5)
    o = n.__getattribute__(prod_func)((0, 2))
    assert o.is_identical(APyFixedArray([60, 672], frac_bits=0, int_bits=20))


@pytest.mark.parametrize("prod_func", ["cumprod", "nancumprod"])
def test_cumprod(prod_func):
    a = APyFixedArray([[1, 2, 3], [4, 5, 6]], frac_bits=0, int_bits=5)
    b = a.__getattribute__(prod_func)()
    assert b.is_identical(
        APyFixedArray([1, 2, 6, 24, 120, 720], frac_bits=0, int_bits=30)
    )
    c = a.__getattribute__(prod_func)(0)
    assert c.is_identical(
        APyFixedArray([[1, 2, 3], [4, 10, 18]], frac_bits=0, int_bits=10)
    )
    d = a.__getattribute__(prod_func)(1)
    assert d.is_identical(
        APyFixedArray([[1, 2, 6], [4, 20, 120]], frac_bits=0, int_bits=15)
    )
    e = APyFixedArray([[[1, 2], [3, 4]], [[5, 6], [7, 8]]], frac_bits=0, int_bits=8)
    f = e.__getattribute__(prod_func)()
    g = e.__getattribute__(prod_func)(0)
    h = e.__getattribute__(prod_func)(1)
    i = e.__getattribute__(prod_func)(2)
    assert f.is_identical(
        APyFixedArray([1, 2, 6, 24, 120, 720, 5040, 40320], frac_bits=0, int_bits=64)
    )
    assert g.is_identical(
        APyFixedArray([[[1, 2], [3, 4]], [[5, 12], [21, 32]]], frac_bits=0, int_bits=16)
    )
    assert h.is_identical(
        APyFixedArray([[[1, 2], [3, 8]], [[5, 6], [35, 48]]], frac_bits=0, int_bits=16)
    )
    assert i.is_identical(
        APyFixedArray([[[1, 2], [3, 12]], [[5, 30], [7, 56]]], frac_bits=0, int_bits=16)
    )
    with pytest.raises(IndexError):
        _ = e.__getattribute__(prod_func)(4)

    k = APyFixedArray.from_float([[0.25, 0.5], [1, 2]], frac_bits=10, int_bits=10)
    m = k.__getattribute__(prod_func)()
    assert m.is_identical(
        APyFixedArray.from_float([0.25, 0.125, 0.125, 0.25], frac_bits=40, int_bits=40)
    )

    # test for size larger than 32 and 64 when number over multiple limbs
    g = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=33)
    h = g.__getattribute__(prod_func)(0)
    assert h.is_identical(
        APyFixedArray([[0, 1, 2], [0, 4, 10]], frac_bits=0, int_bits=66)
    )
    j = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=65)
    k = j.__getattribute__(prod_func)(0)
    assert k.is_identical(
        APyFixedArray([[0, 1, 2], [0, 4, 10]], frac_bits=0, int_bits=130)
    )


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


@pytest.mark.parametrize(
    "shape, is_valid, is_invalid, test_neg_one",
    [
        ((6,), True, False, False),  # Valid shapes
        ((3, 2), True, False, False),
        ((1, 6), True, False, False),
        ((6, 1), True, False, False),
        ((2, 3), True, False, False),
        ((-1,), True, False, False),
        ((-1, 2), True, False, False),
        ((1, -1), True, False, False),
        ((6, -1), True, False, False),
        ((3, -1), True, False, False),
        ((1, 2, 50), False, True, False),  # Invalid shapes
        ((1, 2), False, True, False),
        ((1, 4, 3), False, True, False),
        ((2, 4), False, True, False),
        ((2, 2), False, True, False),
        ((-2,), False, False, True),  # Invalid negatives
        ((-1, -1), False, False, True),
        ((4, -1), False, False, True),
        ((-2, 3), False, False, True),
        ((-999, 12), False, False, True),
    ],
)
def test_reshape(shape, is_valid, is_invalid, test_neg_one):
    # 6 elems
    nums = [[5, 10, 6], [15, 20, 3]]
    arr = APyFixedArray(nums, bits=10, int_bits=10)

    if is_invalid:
        with pytest.raises(
            ValueError,
            match="Total number of elements does not match the original array.",
        ):
            arr.reshape(shape)
    elif test_neg_one:
        with pytest.raises(
            ValueError,
            match=r"Negative dimensions less than -1 are not allowed.|Only one dimension can be -1.|The total size of new array must be unchanged and divisible by the known dimensions.",
        ):
            arr.reshape(shape)
    elif is_valid:
        try:
            g = arr.reshape(shape)
            if g is arr:
                pytest.fail(
                    "Reshape should return a new array, now returns a modified one"
                )
        except ValueError:
            pytest.fail(f"Unexpected ValueError for shape {shape}")


@pytest.mark.parametrize(
    "shape",
    [
        (1, 12),
        (2, 6),
        (3, 4),
        (4, 3),
        (6, 2),
        (12, 1),
        (-1, 12),
        (2, -1),
        (3, -1),
        (4, -1),
        (-1, 3),
        (-1, 2),
        (12, -1),
    ],
)
def test_reshape_2d(shape):
    # Creating 1D array to be reshaped
    nums = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]

    # Creating 1D array to be reshaped
    arr = APyFixedArray(nums, bits=20, int_bits=10)

    reshaped_arr = arr.reshape(shape)
    for i, row in enumerate(reshaped_arr):
        for j, fixed in enumerate(row):
            arr_index = i * reshaped_arr.shape[1] + j
            if not APyFixed.is_identical(arr[arr_index], fixed):
                pytest.fail(f"Mismatch at index {arr_index} during reshape")

    go_back = reshaped_arr.reshape(arr.shape)
    if not APyFixedArray.is_identical(go_back, arr):
        pytest.fail(f"Mismatch after reshaping back at index {arr_index}")


@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
        (1, 12, 1),
        (2, 1, 6),
        (2, 2, 3),
        (2, 3, 2),
        (2, 6, 1),
        (3, 1, 4),
        (3, 2, 2),
        (3, 4, 1),
        (4, 1, 3),
        (4, 3, 1),
        (6, 1, 2),
        (6, 2, 1),
        (12, 1, 1),
        (-1, 2, 2),
        (1, -1, 2),
        (6, 1, -1),
    ],
)
def test_reshape_3d(shape):
    # Creating 1D array to be reshaped
    nums = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    # Creating 1D array to be reshaped
    arr = APyFixedArray(nums, bits=20, int_bits=10)

    reshaped_arr = arr.reshape(shape)
    for i, matrix in enumerate(reshaped_arr):
        for j, row in enumerate(matrix):
            for k, fixed in enumerate(row):
                arr_index = (
                    i * reshaped_arr.shape[1] * reshaped_arr.shape[2]
                    + j * reshaped_arr.shape[2]
                    + k
                )
                if not APyFixed.is_identical(arr[arr_index], fixed):
                    pytest.fail(f"Mismatch at index {arr_index} during reshape")

    go_back = reshaped_arr.reshape(arr.shape)
    if not APyFixedArray.is_identical(go_back, arr):
        pytest.fail(f"Mismatch after reshaping back at index {arr_index}")


@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
        (1, 12, 1),
        (2, 1, 6),
        (2, 2, 3),
        (2, 3, 2),
    ],
)
def test_flatten(shape):
    # Creating 1D array to be reshaped
    nums = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    # Creating 1D array to be reshaped
    arr = APyFixedArray(nums, bits=20, int_bits=10)
    reshaped = arr.reshape(shape)
    if not APyFixedArray.is_identical(reshaped.flatten(), arr):
        pytest.fail(f"Flatten didn't return to original 1d list after reshape {shape}")
    pass


@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
        (1, 12, 1),
        (2, 1, 6),
        (2, 2, 3),
        (2, 3, 2),
    ],
)
def test_ravel(shape):
    # Creating 1D array to be reshaped
    nums = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    # Creating 1D array to be reshaped
    arr = APyFixedArray(nums, bits=20, int_bits=10)
    reshaped = arr.reshape(shape)
    if not APyFixedArray.is_identical(reshaped.ravel(), arr):
        pytest.fail(f"ravel didn't return to original 1d list after reshape {shape}")
    pass


def _generate_dimensions(n):
    result = set()  # Use a set to store unique combinations

    def factor_combinations(target, factors):
        if target == 1:
            result.add(tuple(sorted(factors)))  # Add sorted tuple to set
            return
        for i in range(2, target + 1):
            if target % i == 0:
                factor_combinations(target // i, factors + [i])

    factor_combinations(n, [])
    return list(result)  # Convert set back to list


@pytest.mark.float_array
def test_transpose_highdim():
    np = pytest.importorskip("numpy")
    num_elems = 48
    elements = np.arange(num_elems)

    # Generate all possible axis permutations for a 3D array
    possible_shapes = _generate_dimensions(num_elems)

    # Test each permutation
    for shape in possible_shapes:
        if len(shape) < 3:
            axes_permutations = [None]
        else:
            axes_permutations = list(permutations([_ for _ in range(len(shape))]))

        for perm in axes_permutations:
            apy_array = APyFixedArray.from_array(np.reshape(elements, shape), 7474, 5)
            numpy_transposed = np.transpose(np.reshape(elements, shape), perm)

            apy_transposed = apy_array.transpose(perm)
            numpy_array = APyFixedArray.from_array(numpy_transposed, 7474, 5)

            assert apy_transposed.is_identical(numpy_array), (
                f"Failed for shape {shape} and permutation {perm}. "
                f"Original array = \n{np.reshape(elements, shape)}\n "
                f"ApyFloat array = \n{apy_transposed.to_numpy()}\n "
                f"Numpy created array = \n{numpy_array.to_numpy()}"
            )

    big = 2**325
    a = APyFixedArray.from_float([[big + 1, big + 2], [big + 3, big + 4]], 345, 95)
    b = APyFixedArray.from_float([[big + 1, big + 4], [big + 3, big + 2]], 345, 95)
    a.transpose().is_identical(b)
