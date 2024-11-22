from apytypes import APyCFixedArray, APyCFixed


def test_prod():
    # Single limb
    a = APyCFixedArray.from_complex(
        [1, 2 + 1j, 3 + 3j, -4 + 1j, 5 - 2j, 6 - 3j], int_bits=6, frac_bits=0
    )
    assert a.prod().is_identical(
        APyCFixed.from_float(-1395 - 225j, bits=41, int_bits=41)
    )

    # Multi limb
    b = APyCFixedArray.from_complex(
        [1, 2 + 1j, 3 + 3j, -4 + 1j, 5 - 2j, 6 - 3j], int_bits=10, frac_bits=0
    )
    assert b.prod().is_identical(
        APyCFixed.from_float(-1395 - 225j, bits=65, int_bits=65)
    )


def test_arithmetic_float():
    a = APyCFixedArray.from_complex(
        [1, 2 + 1j, 3 + 3j, -4 + 1j, 5 - 2j, 6 - 3j], int_bits=6, frac_bits=0
    )
    assert (a + 1.0).is_identical(
        APyCFixedArray.from_complex(
            [2, 3 + 1j, 4 + 3j, -3 + 1j, 6 - 2j, 7 - 3j], int_bits=7, frac_bits=0
        )
    )
    assert (1.0 + a).is_identical(
        APyCFixedArray.from_complex(
            [2, 3 + 1j, 4 + 3j, -3 + 1j, 6 - 2j, 7 - 3j], int_bits=7, frac_bits=0
        )
    )
