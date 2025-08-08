from apytypes import (
    APyCFloatArray,
    convolve,
)


def test_complex_convolve():
    # Test #1, equally many array elements, same bit specification
    a = APyCFloatArray.from_complex([1 + 2j, 2, 3 - 4j, -4], exp_bits=15, man_bits=27)
    b = APyCFloatArray.from_complex([5 - 2j, -2, -5, 4j], exp_bits=15, man_bits=27)
    result_full = APyCFloatArray.from_complex(
        [9 + 8j, 8 - 8j, -2 - 36j, -44 + 20j, -7 + 28j, 36 + 12j, 0 - 16j],
        exp_bits=15,
        man_bits=27,
    )
    result_same = APyCFloatArray.from_complex(
        [8 - 8j, -2 - 36j, -44 + 20j, -7 + 28j], exp_bits=15, man_bits=27
    )
    result_valid = APyCFloatArray.from_complex([-44 + 20j], exp_bits=15, man_bits=27)
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)

    # Test #2, equally many array elements, different bit specification
    a = APyCFloatArray.from_complex([1 + 2j, 2, 3 - 4j, -4], exp_bits=15, man_bits=27)
    b = APyCFloatArray.from_complex([5 - 2j, -2, -5, 4j], exp_bits=10, man_bits=30)
    result_full = APyCFloatArray.from_complex(
        [9 + 8j, 8 - 8j, -2 - 36j, -44 + 20j, -7 + 28j, 36 + 12j, 0 - 16j],
        exp_bits=15,
        man_bits=30,
    )
    result_same = APyCFloatArray.from_complex(
        [8 - 8j, -2 - 36j, -44 + 20j, -7 + 28j], exp_bits=15, man_bits=30
    )
    result_valid = APyCFloatArray.from_complex([-44 + 20j], exp_bits=15, man_bits=30)
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)

    # Test #3, different number of array elements, different bit specification
    a = APyCFloatArray.from_complex([1 + 2j, 2, 3 - 4j, -4], exp_bits=15, man_bits=27)
    b = APyCFloatArray.from_complex(
        [5 - 2j, -2, -5, 4j, 1 + 2j, 0.5 - 0.25j], exp_bits=10, man_bits=30
    )
    # a = APyCFloatArray.from_complex([1, 2, 3, 4], exp_bits=4, man_bits=4)
    # b = APyCFloatArray.from_complex([5, 2, 5, 4, 9, 2, 9, 5, 2, 3], exp_bits=6, man_bits=9)
    result_full = APyCFloatArray.from_complex(
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
        exp_bits=15,
        man_bits=30,
    )
    result_same = APyCFloatArray.from_complex(
        [8 - 8j, -2 - 36j, -44 + 20j, -10 + 32j, 39 + 16.75j, 12 - 14.5j],
        exp_bits=15,
        man_bits=30,
    )
    result_valid = APyCFloatArray.from_complex(
        [-44 + 20j, -10 + 32j, 39 + 16.75j], exp_bits=15, man_bits=30
    )
    assert convolve(a, b).is_identical(result_full)
    assert convolve(b, a).is_identical(result_full)
    assert convolve(a, b, mode="full").is_identical(result_full)
    assert convolve(b, a, mode="full").is_identical(result_full)
    assert convolve(a, b, mode="same").is_identical(result_same)
    assert convolve(b, a, mode="same").is_identical(result_same)
    assert convolve(a, b, mode="valid").is_identical(result_valid)
    assert convolve(b, a, mode="valid").is_identical(result_valid)
