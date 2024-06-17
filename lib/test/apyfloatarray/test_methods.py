import pytest
from apytypes import APyFloatArray, APyFloat, QuantizationMode


@pytest.mark.float_array
def test_is_identical():
    a = APyFloatArray([1], [2], [3], 4, 5)
    assert a.is_identical(a)

    assert not a.is_identical(APyFloatArray([0], [2], [3], 4, 5))
    assert not a.is_identical(APyFloatArray([1], [1], [3], 4, 5))
    assert not a.is_identical(APyFloatArray([1], [2], [2], 4, 5))
    assert not a.is_identical(APyFloatArray([1], [2], [3], 5, 5))
    assert not a.is_identical(APyFloatArray([1], [2], [3], 4, 6))
    assert not a.is_identical(APyFloatArray([1], [2], [6], 4, 6))  # Same value


@pytest.mark.float_array
def test_array_from_float_raises():
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloatArray.from_float([0], 300, 5)
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloatArray.from_float([0], -300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloatArray.from_float([0], 5, 300)
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloatArray.from_float([0], 5, -300)
    with pytest.raises(
        ValueError,
        match="python_sequence_extract_shape",
    ):
        APyFloatArray.from_float(["0"], 5, 10)


def test_from_float_with_non_floats():
    a = APyFloatArray.from_float([16], exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloatArray([0], [11], [0], exp_bits=4, man_bits=2))

    # Should quantize to 16.0
    a = APyFloatArray.from_float([17], exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloatArray([0], [11], [0], exp_bits=4, man_bits=2))

    # Should quantize to -20.0
    a = APyFloatArray.from_float([-19], exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloatArray([1], [11], [1], exp_bits=4, man_bits=2))

    # Tie break, should quantize to 16.0
    a = APyFloatArray.from_float([18], exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloatArray([0], [11], [0], exp_bits=4, man_bits=2))

    # Tie break, should quantize to 24.0
    a = APyFloatArray.from_float([22], exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloatArray([0], [11], [2], exp_bits=4, man_bits=2))

    # Should quantize to 28.0
    a = APyFloatArray.from_float([29], exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloatArray([0], [11], [3], exp_bits=4, man_bits=2))

    # Should quantize to 32.0
    a = APyFloatArray.from_float([31], exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloatArray([0], [12], [0], exp_bits=4, man_bits=2))

    # 152-bit number, should become negative infinity
    a = APyFloatArray.from_float(
        [-0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF], exp_bits=4, man_bits=2
    )
    assert a.is_identical(APyFloatArray([1], [15], [0], exp_bits=4, man_bits=2))

    # Test with bool and non IEEE-like bias
    a = APyFloatArray.from_float([True], exp_bits=4, man_bits=2, bias=4)
    assert a.is_identical(APyFloatArray([0], [4], [0], exp_bits=4, man_bits=2, bias=4))

    # Test with big integer
    a = APyFloatArray.from_float([2**2047], exp_bits=28, man_bits=2)
    assert a.is_identical(
        APyFloatArray([0], [(2047 + (2**27) - 1)], [0], exp_bits=28, man_bits=2)
    )


def test_array_cast_raises():
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloatArray([0], [0], [0], 5, 5).cast(300, 5)
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloatArray([0], [0], [0], 5, 5).cast(-300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloatArray([0], [0], [0], 5, 5).cast(5, 300)
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloatArray([0], [0], [0], 5, 5).cast(5, -300)


def test_array_from_array_raises():
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloatArray.from_array(np.array([[1, 2, 3, 4]]), 5, 5).cast(300, 5)
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloatArray.from_array(np.array([[1, 2, 3, 4]]), 5, 5).cast(-300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloatArray.from_array(np.array([[1, 2, 3, 4]]), 5, 5).cast(5, 300)
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloatArray.from_array(np.array([[1, 2, 3, 4]]), 5, 5).cast(5, -300)


@pytest.mark.float_array
def test_to_numpy():
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    assert np.array_equal(APyFloatArray([], [], [], 5, 5).to_numpy(), np.array([]))
    assert np.array_equal(
        APyFloatArray([0], [15], [1], 5, 2).to_numpy(), np.array([1.25])
    )

    float_seq = [
        [[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]],
        [[7.0, 8.0, 9.0], [10.0, 11.0, 12.0]],
    ]
    fp_arr = APyFloatArray.from_float(float_seq, 10, 10)
    assert fp_arr.to_numpy().shape == (2, 2, 3)
    assert np.array_equal(fp_arr.to_numpy(), np.array(float_seq))


@pytest.mark.float_array
def test_get_item():
    # ndim == 1
    fp_array = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)
    assert fp_array[0].is_identical(APyFloat.from_float(1, exp_bits=10, man_bits=10))
    assert fp_array[1].is_identical(APyFloat.from_float(2, exp_bits=10, man_bits=10))
    assert fp_array[5].is_identical(APyFloat.from_float(6, exp_bits=10, man_bits=10))
    with pytest.raises(IndexError, match="APyFloatArray.__getitem__: index 6 is out"):
        fp_array[6]

    # ndim == 2
    fp_array = APyFloatArray.from_float([[1, 2], [3, 4]], exp_bits=10, man_bits=10)
    assert fp_array[0].is_identical(
        APyFloatArray.from_float([1, 2], exp_bits=10, man_bits=10)
    )
    assert fp_array[1].is_identical(
        APyFloatArray.from_float([3, 4], exp_bits=10, man_bits=10)
    )
    with pytest.raises(IndexError, match="APyFloatArray.__getitem__: index 2 is out"):
        fp_array[2]

    # ndim == 3
    fp_array = APyFloatArray.from_float(
        [[[1, 2], [3, 4]], [[5, 6], [7, 8]]], exp_bits=10, man_bits=10
    )
    assert fp_array[0].is_identical(
        APyFloatArray.from_float([[1, 2], [3, 4]], exp_bits=10, man_bits=10)
    )
    assert fp_array[1].is_identical(
        APyFloatArray.from_float([[5, 6], [7, 8]], exp_bits=10, man_bits=10)
    )
    with pytest.raises(IndexError, match="APyFloatArray.__getitem__: index 2 is out"):
        fp_array[2]


@pytest.mark.float_array
def test_iterator():
    fx_array = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)
    iterator = iter(fx_array)
    assert next(iterator).is_identical(APyFloat.from_float(1, exp_bits=10, man_bits=10))
    assert next(iterator).is_identical(APyFloat.from_float(2, exp_bits=10, man_bits=10))


@pytest.mark.float_array
def test_len():
    fp_array = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)
    assert len(fp_array) == 6


@pytest.mark.float_array
def test_cast():
    fp_array = APyFloatArray.from_float([[5, 4], [1, -7]], exp_bits=10, man_bits=15)
    ans = APyFloatArray.from_float([[5, 4], [1, -7]], exp_bits=5, man_bits=2)
    assert fp_array.cast(5, 2).is_identical(ans)

    fp_array = APyFloatArray.from_float(
        [[1000, -1000], [1.015625, float("nan")]], exp_bits=10, man_bits=15
    )
    ans = APyFloatArray.from_float(
        [[float("inf"), float("-inf")], [1, float("nan")]], exp_bits=4, man_bits=3
    )
    assert fp_array.cast(4, 3).is_identical(ans)

    fp_array = APyFloatArray.from_float([1.125, -1.875], exp_bits=10, man_bits=15)
    ans = APyFloatArray.from_float([1, -1.75], exp_bits=5, man_bits=2)
    assert fp_array.cast(5, 2, quantization=QuantizationMode.TO_ZERO).is_identical(ans)

    fp_array = APyFloatArray.from_float([[5, 4], [1, -7]], exp_bits=10, man_bits=15)
    ans = APyFloatArray.from_float([[5, 4], [1, -7]], exp_bits=4, man_bits=15)
    assert fp_array.cast(4).is_identical(ans)
    ans = APyFloatArray.from_float([[5, 4], [1, -7]], exp_bits=10, man_bits=5)
    assert fp_array.cast(man_bits=5).is_identical(ans)


@pytest.mark.float_array
def test_python_sum():
    fx_array = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)
    assert sum(fx_array) == 21
    assert sum(fx_array).is_identical(APyFloat.from_float(21, 10, 10))


def test_round_trip_conversion():
    # Skip this test if `NumPy` is not present on the machine
    pytest.importorskip("numpy")
    for exp in reversed(range(15)):  # Skip nan/inf
        for man in reversed(range(16)):
            a = APyFloatArray([0], [exp], [man], 4, 4)
            assert (APyFloatArray.from_float(a.to_numpy(), 4, 4)).is_identical(a)


def test_squeeze():
    a = APyFloatArray.from_float([[1], [2], [3]], exp_bits=10, man_bits=10)
    b = a.squeeze()
    assert (b).is_identical(
        APyFloatArray.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    )
    c = APyFloatArray.from_float(
        [[[1, 2], [3, 4], [5, 6], [7, 8]]], exp_bits=10, man_bits=10
    )
    d = c.squeeze()
    assert (d).is_identical(
        APyFloatArray.from_float(
            [[1, 2], [3, 4], [5, 6], [7, 8]], exp_bits=10, man_bits=10
        )
    )
    e = APyFloatArray.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    f = e.squeeze()
    assert f.is_identical(e)
    g = APyFloatArray.from_float([[[[[[[[2]]]]]]]], exp_bits=10, man_bits=10)
    h = g.squeeze()
    assert h.is_identical(APyFloatArray.from_float([2], exp_bits=10, man_bits=10))
    i = APyFloatArray.from_float([], exp_bits=10, man_bits=10)
    j = APyFloatArray.from_float([[]], exp_bits=10, man_bits=10)
    k = i.squeeze()
    z = j.squeeze()
    assert k.is_identical(i)
    assert z.is_identical(i)
    m = APyFloatArray.from_float([[1], [2], [3]], exp_bits=10, man_bits=10)
    with pytest.raises(ValueError):
        _ = m.squeeze(axis=0)
    m1 = m.squeeze(axis=1)
    assert m1.is_identical(
        APyFloatArray.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    )
    with pytest.raises(IndexError):
        _ = m.squeeze(axis=2)

    n = APyFloatArray.from_float([[[[[[[[2]]]]]]]], exp_bits=10, man_bits=10)
    o = n.squeeze((0, 1, 2, 3))
    assert o.is_identical(APyFloatArray.from_float([[[[2]]]], exp_bits=10, man_bits=10))
    p = n.squeeze((0, 1, 3))
    assert p.is_identical(
        APyFloatArray.from_float([[[[[2]]]]], exp_bits=10, man_bits=10)
    )
    q = APyFloatArray.from_float([[[1]], [[2]], [[3]], [[4]]], exp_bits=10, man_bits=10)
    with pytest.raises(ValueError):
        _ = q.squeeze((0, 1, 2))
    with pytest.raises(IndexError):
        _ = m.squeeze((1, 4))


def test_sum():
    a = APyFloatArray.from_float(
        [[1, 2], [3, 4], [5, 6], [7, 8]], exp_bits=10, man_bits=10
    )
    b = a.sum()
    assert b.is_identical(APyFloatArray.from_float([36], exp_bits=10, man_bits=10))
    c = APyFloatArray.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    d = c.sum((0, 1))
    e = c.sum(0)
    f = c.sum(1)
    assert d.is_identical(APyFloatArray.from_float([15], exp_bits=10, man_bits=10))
    assert e.is_identical(APyFloatArray.from_float([3, 5, 7], exp_bits=10, man_bits=10))
    assert f.is_identical(APyFloatArray.from_float([3, 12], exp_bits=10, man_bits=10))

    # test for size larger than 32 and 64 when number over multiple limbs
    g = APyFloatArray.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    h = g.sum(0)
    assert h.is_identical(APyFloatArray.from_float([3, 5, 7], exp_bits=10, man_bits=10))
    j = APyFloatArray.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    k = j.sum(0)
    assert k.is_identical(APyFloatArray.from_float([3, 5, 7], exp_bits=10, man_bits=10))

    # test some float and negative summation
    j = APyFloatArray.from_float([0.2, 1.4, 3.3], exp_bits=10, man_bits=10)
    k = j.sum()
    assert k.is_identical(APyFloatArray.from_float([4.904], exp_bits=10, man_bits=10))
    m = APyFloatArray.from_float(
        [0.333333, 1.333333, 3.33333], exp_bits=10, man_bits=10
    )
    n = m.sum()
    assert n.is_identical(APyFloatArray.from_float([5], exp_bits=10, man_bits=10))

    o = APyFloatArray.from_float([[-1, -2], [-3, -4]], exp_bits=10, man_bits=10)
    p = o.sum(1)
    assert p.is_identical(APyFloatArray.from_float([-3, -7], exp_bits=10, man_bits=10))

    q = APyFloatArray.from_float([[-1, -2], [1, 2]], exp_bits=10, man_bits=10)
    r = q.sum(0)
    assert r.is_identical(APyFloatArray.from_float([0, 0], exp_bits=10, man_bits=10))

    m = APyFloatArray.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    with pytest.raises(IndexError):
        _ = m.sum(1)


def test_convenience_cast():
    a = APyFloatArray.from_float([0.893820, 3e20, -float("inf"), float("nan")], 10, 50)
    assert a.cast_to_double().is_identical(
        APyFloatArray(
            [0, 0, 1, 0],
            [1022, 1091, 2047, 2047],
            [3547215210502096, 74037091379504, 0, 1],
            exp_bits=11,
            man_bits=52,
            bias=1023,
        )
    )
    assert a.cast_to_single().is_identical(
        APyFloatArray(
            [0, 0, 1, 0],
            [126, 195, 255, 255],
            [6607203, 137905, 0, 1],
            exp_bits=8,
            man_bits=23,
            bias=127,
        )
    )
    assert a.cast_to_half().is_identical(
        APyFloatArray(
            [0, 0, 1, 0],
            [14, 31, 31, 31],
            [807, 0, 0, 1],
            exp_bits=5,
            man_bits=10,
            bias=15,
        )
    )
    assert a.cast_to_bfloat16().is_identical(
        APyFloatArray(
            [0, 0, 1, 0],
            [126, 195, 255, 255],
            [101, 2, 0, 1],
            exp_bits=8,
            man_bits=7,
            bias=127,
        )
    )


@pytest.mark.float_array
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
    signs = [[52, 15, 32], [12, 43, 5]]
    exps = [[5, 10, 6], [15, 20, 3]]
    mans = [[3, 1, 2], [4, 2, 8]]

    arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=8, man_bits=23)

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


@pytest.mark.float_array
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
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # Creating 1D array to be reshaped
    arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=5, man_bits=2)

    reshaped_arr = arr.reshape(shape)
    for i, row in enumerate(reshaped_arr):
        for j, float_ in enumerate(row):
            arr_index = i * reshaped_arr.shape[1] + j
            if not APyFloat.is_identical(arr[arr_index], float_):
                pytest.fail(f"Mismatch at index {arr_index} during reshape")

    go_back = reshaped_arr.reshape(arr.shape)
    if not APyFloatArray.is_identical(go_back, arr):
        pytest.fail(f"Mismatch after reshaping back at index {arr_index}")


@pytest.mark.float_array
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
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # Creating 1D array to be reshaped
    arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=5, man_bits=2)

    reshaped_arr = arr.reshape(shape)
    for i, matrix in enumerate(reshaped_arr):
        for j, row in enumerate(matrix):
            for k, float_ in enumerate(row):
                arr_index = (
                    i * reshaped_arr.shape[1] * reshaped_arr.shape[2]
                    + j * reshaped_arr.shape[2]
                    + k
                )
                if not APyFloat.is_identical(arr[arr_index], float_):
                    pytest.fail(f"Mismatch at index {arr_index} during reshape")

    go_back = reshaped_arr.reshape(arr.shape)
    if not APyFloatArray.is_identical(go_back, arr):
        pytest.fail(f"Mismatch after reshaping back at index {arr_index}")


@pytest.mark.float_array
@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
    ],
)
def test_flatten(shape):
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # manually create 1d arr
    arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=5, man_bits=2)
    reshaped = arr.reshape(shape)
    if not APyFloatArray.is_identical(reshaped.flatten(), arr):
        pytest.fail(f"Flatten didn't return to original 1d list after reshape {shape}")


@pytest.mark.float_array
@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
    ],
)
def test_ravel(shape):
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # manually create 1d arr
    arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=5, man_bits=2)
    reshaped = arr.reshape(shape)
    if not APyFloatArray.is_identical(reshaped.ravel(), arr):
        pytest.fail(f"Flatten didn't return to original 1d list after reshape {shape}")
