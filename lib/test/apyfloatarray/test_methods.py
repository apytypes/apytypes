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


@pytest.mark.float_array
def test_python_sum():
    fx_array = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)
    assert sum(fx_array) == 21
    assert sum(fx_array).is_identical(APyFloat.from_float(21, 10, 10))
