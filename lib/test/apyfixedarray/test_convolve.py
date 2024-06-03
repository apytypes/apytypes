from apytypes import APyFixedArray


def test_single_limb_convolve():
    # Test #1, equally many array elements
    a = APyFixedArray.from_float([1, 2, 3, 4], int_bits=10, frac_bits=3)
    b = APyFixedArray.from_float([5, 2, 5, 4], int_bits=8, frac_bits=5)
    result = APyFixedArray.from_float(
        [5, 12, 24, 40, 31, 32, 16], int_bits=20, frac_bits=8
    )
    assert a.convolve(b).is_identical(result)
    assert b.convolve(a).is_identical(result)

    # Test #2, different number of array elements
    a = APyFixedArray.from_float([1, 2, 3, 4], int_bits=10, frac_bits=3)
    b = APyFixedArray.from_float(
        [5, 2, 5, 4, 9, 2, 9, 5, 2, 3], int_bits=8, frac_bits=5
    )
    result = APyFixedArray.from_float(
        [5, 12, 24, 40, 40, 52, 56, 65, 47, 58, 32, 17, 12], int_bits=22, frac_bits=8
    )
    assert a.convolve(b).is_identical(result)
    assert b.convolve(a).is_identical(result)


# TODO: Test long word-lengths and in accumulator contexts
