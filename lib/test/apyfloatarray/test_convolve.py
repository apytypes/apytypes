import pytest

from apytypes import (
    APyCFloatArray,
    APyFloatArray,
    convolve,
)


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_raises(float_array: type[APyCFloatArray]):
    # Only 1D arrays can be convolved
    with pytest.raises(ValueError, match="can only convolve 1D arrays"):
        _ = convolve(
            float_array.from_float([[1]], 1, 0), float_array.from_float([1], 1, 0)
        )
    with pytest.raises(ValueError, match="can only convolve 1D arrays"):
        _ = convolve(
            float_array.from_float([1], 1, 0), float_array.from_float([[1]], 1, 0)
        )

    # Modes other than "full", "valid" and "same" are invalid modes
    a = float_array.from_float([1, 2, 3, 4], exp_bits=10, man_bits=3)
    b = float_array.from_float([5, 2, 5, 4], exp_bits=8, man_bits=5)
    with pytest.raises(ValueError, match="mode='cool_beans' not in 'full', 'same',"):
        _ = convolve(a, b, mode="cool_beans")

    # Can't mix types during convolution
    a = float_array.from_float([1, 2, 3, 4], exp_bits=10, man_bits=3)
    b = [4, 5, 6, 7]
    with pytest.raises(TypeError, match="Cannot convolve <class"):
        _ = convolve(a, b, mode="cool_beans")
    with pytest.raises(TypeError, match="Cannot convolve <class"):
        _ = convolve(b, a, mode="cool_beans")


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_multi_limb_convolve_full(float_array: type[APyCFloatArray]):
    # Test #1, equally many array elements, same bit specification
    a = float_array.from_float([1, 2, 3, 4], exp_bits=15, man_bits=27)
    b = float_array.from_float([5, 2, 5, 4], exp_bits=15, man_bits=27)
    result_full = float_array.from_float(
        [5, 12, 24, 40, 31, 32, 16], exp_bits=15, man_bits=27
    )
    result_same = float_array.from_float([12, 24, 40, 31], exp_bits=15, man_bits=27)
    result_valid = float_array.from_float([40], exp_bits=15, man_bits=27)
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)

    # Test #2, equally many array elements, different bit specification
    a = float_array.from_float([1, 2, 3, 4], exp_bits=15, man_bits=27)
    b = float_array.from_float([5, 2, 5, 4], exp_bits=10, man_bits=30)
    result_full = float_array.from_float(
        [5, 12, 24, 40, 31, 32, 16], exp_bits=15, man_bits=30
    )
    result_same = float_array.from_float([12, 24, 40, 31], exp_bits=15, man_bits=30)
    result_valid = float_array.from_float([40], exp_bits=15, man_bits=30)
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)

    # Test #3, different number of array elements, different bit specification
    a = float_array.from_float([1, 2, 3, 4], exp_bits=4, man_bits=4)
    b = float_array.from_float([5, 2, 5, 4, 9, 2, 9, 5, 2, 3], exp_bits=6, man_bits=9)
    result_full = float_array.from_float(
        [5, 12, 24, 40, 40, 52, 56, 65, 47, 58, 32, 17, 12], exp_bits=6, man_bits=9
    )
    result_same = float_array.from_float(
        [12, 24, 40, 40, 52, 56, 65, 47, 58, 32], exp_bits=6, man_bits=9
    )
    result_valid = float_array.from_float(
        [40, 40, 52, 56, 65, 47, 58], exp_bits=6, man_bits=9
    )
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)

    # Test #4, different number of array elements, negative elements involved
    a = float_array.from_float([1, 2, 3, -4], exp_bits=6, man_bits=8)
    b = float_array.from_float(
        [5, 2, -5, 4, 9, 2, 9, 5, -2, 3], exp_bits=12, man_bits=13
    )
    result_full = float_array.from_float(
        [5, 12, 14, -20, -6, 52, 24, -7, 27, -22, -20, 17, -12],
        exp_bits=12,
        man_bits=13,
    )
    result_same = float_array.from_float(
        [12, 14, -20, -6, 52, 24, -7, 27, -22, -20],
        exp_bits=12,
        man_bits=13,
    )
    result_valid = float_array.from_float(
        [-20, -6, 52, 24, -7, 27, -22],
        exp_bits=12,
        man_bits=13,
    )
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)
