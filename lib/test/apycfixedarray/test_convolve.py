from apytypes import (
    APyCFixedArray,
    convolve,
)


def test_single_limb_convolve_full():
    # Test #1, equally many array elements
    a = APyCFixedArray.from_float([1 + 2j, 2, 3 - 4j, -4], int_bits=10, frac_bits=3)
    b = APyCFixedArray.from_float([5 - 2j, -2, -5, 4j], int_bits=8, frac_bits=5)
    result_full = APyCFixedArray.from_float(
        [9 + 8j, 8 - 8j, -2 - 36j, -44 + 20j, -7 + 28j, 36 + 12j, 0 - 16j],
        int_bits=21,
        frac_bits=8,
    )
    result_same = APyCFixedArray.from_float(
        [8 - 8j, -2 - 36j, -44 + 20j, -7 + 28j], int_bits=21, frac_bits=8
    )
    result_valid = APyCFixedArray.from_float([-44 + 20j], int_bits=21, frac_bits=8)
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)


def test_multi_limb_convolve_full():
    # Test #1, equally many array elements
    a = APyCFixedArray.from_float([1 + 2j, 2, 3 - 4j, -4], int_bits=100, frac_bits=300)
    b = APyCFixedArray.from_float([5 - 2j, -2, -5, 4j], int_bits=80, frac_bits=50)
    result_full = APyCFixedArray.from_float(
        [9 + 8j, 8 - 8j, -2 - 36j, -44 + 20j, -7 + 28j, 36 + 12j, 0 - 16j],
        int_bits=183,
        frac_bits=350,
    )
    result_same = APyCFixedArray.from_float(
        [8 - 8j, -2 - 36j, -44 + 20j, -7 + 28j], int_bits=183, frac_bits=350
    )
    result_valid = APyCFixedArray.from_float([-44 + 20j], int_bits=183, frac_bits=350)
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)

    # Test #2, different number of array elements, multi-limb.
    a = APyCFixedArray.from_float([1 + 2j, 2, 3 - 4j, -4], int_bits=100, frac_bits=300)
    b = APyCFixedArray.from_float(
        [5 - 2j, -2, -5, 4j, 1 + 2j, 0.5 - 0.25j], int_bits=80, frac_bits=50
    )
    result_full = APyCFixedArray.from_float(
        [
            9 + 8j,
            8 - 8j,
            -2 - 36j,
            -44 + 20j,
            -10 + 32j,
            39 + 16.75j,
            12 - 14.5j,
            -3.5 - 10.75j,
            -2 + 1j,
        ],
        int_bits=183,
        frac_bits=350,
    )
    result_same = APyCFixedArray.from_float(
        [8 - 8j, -2 - 36j, -44 + 20j, -10 + 32j, 39 + 16.75j, 12 - 14.5j],
        int_bits=183,
        frac_bits=350,
    )
    result_valid = APyCFixedArray.from_float(
        [-44 + 20j, -10 + 32j, 39 + 16.75j], int_bits=183, frac_bits=350
    )
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)
