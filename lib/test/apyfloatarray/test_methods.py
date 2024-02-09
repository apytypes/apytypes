import pytest
from apytypes import APyFloatArray


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
    assert not a.is_identical(APyFloatArray([1], [2], [3], 4, 5, 9))


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
