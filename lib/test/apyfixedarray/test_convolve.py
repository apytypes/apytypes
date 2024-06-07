from apytypes import APyFixedArray, convolve

import pytest


def test_raises():
    with pytest.raises(ValueError, match="can only convolve 1D arrays"):
        APyFixedArray([[1]], 1, 0).convolve(APyFixedArray([1], 1, 0))
    with pytest.raises(ValueError, match="can only convolve 1D arrays"):
        APyFixedArray([1], 1, 0).convolve(APyFixedArray([[1]], 1, 0))


def test_single_limb_convolve_full():
    # Test #1, equally many array elements
    a = APyFixedArray.from_float([1, 2, 3, 4], int_bits=10, frac_bits=3)
    b = APyFixedArray.from_float([5, 2, 5, 4], int_bits=8, frac_bits=5)
    result = APyFixedArray.from_float(
        [5, 12, 24, 40, 31, 32, 16], int_bits=20, frac_bits=8
    )
    assert convolve(a, b).is_identical(result)
    assert convolve(b, a).is_identical(result)

    # Test #2, different number of array elements
    a = APyFixedArray.from_float([1, 2, 3, 4], int_bits=10, frac_bits=3)
    b = APyFixedArray.from_float(
        [5, 2, 5, 4, 9, 2, 9, 5, 2, 3], int_bits=8, frac_bits=5
    )
    result = APyFixedArray.from_float(
        [5, 12, 24, 40, 40, 52, 56, 65, 47, 58, 32, 17, 12], int_bits=22, frac_bits=8
    )
    assert convolve(a, b).is_identical(result)
    assert convolve(b, a).is_identical(result)


def test_multi_limb_convolve_full():
    # Test #1, equally many array elements
    a = APyFixedArray.from_float([1, 2, 3, 4], int_bits=100, frac_bits=300)
    b = APyFixedArray.from_float([5, 2, 5, 4], int_bits=80, frac_bits=50)
    result = APyFixedArray.from_float(
        [5, 12, 24, 40, 31, 32, 16], int_bits=182, frac_bits=350
    )
    assert convolve(a, b).is_identical(result)
    assert convolve(b, a).is_identical(result)

    # Test #2, different number of array elements
    a = APyFixedArray.from_float([1, 2, 3, 4], int_bits=100, frac_bits=300)
    b = APyFixedArray.from_float(
        [5, 2, 5, 4, 9, 2, 9, 5, 2, 3], int_bits=80, frac_bits=50
    )
    result = APyFixedArray.from_float(
        [5, 12, 24, 40, 40, 52, 56, 65, 47, 58, 32, 17, 12], int_bits=184, frac_bits=350
    )
    assert convolve(a, b).is_identical(result)
    assert convolve(b, a).is_identical(result)

    # Test #3, different number of array elements, negative elements involved
    a = APyFixedArray.from_float([1, 2, 3, -4], int_bits=100, frac_bits=300)
    b = APyFixedArray.from_float(
        [5, 2, -5, 4, 9, 2, 9, 5, -2, 3], int_bits=80, frac_bits=50
    )
    result = APyFixedArray.from_float(
        [5, 12, 14, -20, -6, 52, 24, -7, 27, -22, -20, 17, -12],
        int_bits=184,
        frac_bits=350,
    )
    assert convolve(a, b).is_identical(result)
    assert convolve(b, a).is_identical(result)


def test_non_existing_mode():
    a = APyFixedArray.from_float([1, 2, 3, 4], int_bits=10, frac_bits=3)
    b = APyFixedArray.from_float([5, 2, 5, 4], int_bits=8, frac_bits=5)
    with pytest.raises(ValueError, match="mode='cool_beans' non-valid"):
        convolve(a, b, mode="cool_beans")


# TODO: Test long word-lengths and in accumulator contexts
@pytest.mark.parametrize("mode", ["same", "valid"])
def test_not_implemented(mode):
    a = APyFixedArray.from_float([1, 2, 3, 4], int_bits=10, frac_bits=3)
    b = APyFixedArray.from_float([5, 2, 5, 4], int_bits=8, frac_bits=5)
    with pytest.raises(ValueError, match="Not implemented yet"):
        convolve(a, b, mode)
