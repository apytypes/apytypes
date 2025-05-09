from apytypes import APyCFixed, APyCFixedArray, APyFixed, APyFloat


def test_real_imag_properties():
    assert APyCFixed.from_complex(2.0 + 3.0j, 20, 30).real.is_identical(
        APyFixed.from_float(2.0, 20, 30)
    )
    assert APyCFixed.from_complex(2.0 + 3.0j, 20, 30).imag.is_identical(
        APyFixed.from_float(3.0, 20, 30)
    )


def test_negation():
    assert (-APyCFixed.from_complex(2.0 - 3.0j, 10, 12)).is_identical(
        APyCFixed.from_complex(-2.0 + 3.0j, 10, 12)
    )
    assert (-APyCFixed.from_complex(2.0 - 3.0j, 100, 120)).is_identical(
        APyCFixed.from_complex(-2.0 + 3.0j, 100, 120)
    )


def test_shift():
    a = APyCFixed.from_complex(2.0 + 3.0j, 10, 10) << 3
    b = APyCFixed.from_complex(2.0 + 3.0j, 10, 10) >> 2
    assert a.is_identical(APyCFixed.from_complex(16.0 + 24.0j, 13, 7))
    assert b.is_identical(APyCFixed.from_complex(0.5 + 0.75j, 8, 12))

    a >>= 2
    b <<= 3
    assert a.is_identical(APyCFixed.from_complex(4.0 + 6.0j, 11, 9))
    assert b.is_identical(APyCFixed.from_complex(4.0 + 6.0j, 11, 9))


def test_comparison():
    # Compare to `int`
    assert APyCFixed.from_complex(1.0 + 2j, 5, 5) != 1
    assert APyCFixed.from_complex(1.0 + 2.0j, 5, 5) != 1
    assert APyCFixed.from_complex(4.0, 5, 5) == 4
    assert APyCFixed.from_complex(4.0, 5, 5) != 6

    # Compare to `double`
    assert APyCFixed.from_complex(1.0 + 2j, 5, 5) != 1.0
    assert APyCFixed.from_complex(1.0 + 2.0j, 5, 5) != 1.0
    assert APyCFixed.from_complex(4.0, 5, 5) == 4.0
    assert APyCFixed.from_complex(4.0, 5, 5) != 6.0

    # Compare to `complex`
    assert APyCFixed.from_complex(1.0 + 2.0j, 5, 5) == 1.0 + 2.0j
    assert APyCFixed.from_complex(1.0 + 2.0j, 5, 5) != 1.0 + 2.1j
    assert APyCFixed.from_complex(4.0, 5, 5) == APyFixed.from_float(4.0, 5, 5)
    assert APyCFixed.from_complex(4.0, 5, 5) != APyFixed.from_float(5.0, 5, 5)

    # Compare to `APyFixed`
    assert APyCFixed.from_complex(1.0 + 2j, 5, 5) != APyFixed.from_float(1.0, 5, 5)
    assert APyCFixed.from_complex(1.0 + 2.0j, 5, 5) != APyFixed.from_float(1.0, 5, 5)
    assert APyCFixed.from_complex(4.0, 5, 5) == APyFixed.from_float(4.0, 5, 5)
    assert APyCFixed.from_complex(4.0, 5, 5) != APyFixed.from_float(5.0, 5, 5)


def test_to_and_from_complex():
    c_list = [
        1.0 + 3.0j,
        2.0 - 1.0j,
        -3.0 + 2.0j,
        -4.0 - 4.0j,
    ]

    for c in c_list:
        assert complex(APyCFixed.from_complex(c, int_bits=10, frac_bits=10)) == c
        assert complex(APyCFixed.from_complex(c, int_bits=1000, frac_bits=1000)) == c

    # From APyFloat
    assert APyCFixed.from_complex(
        APyFloat(1, 15, 2, 5, 2), bits=4, int_bits=3
    ).is_identical(APyCFixed((13, 0), bits=4, int_bits=3))  # (-1.5, 0)

    # From APyFixed
    assert APyCFixed.from_complex(
        APyFixed(13, int_bits=2, frac_bits=3), int_bits=2, frac_bits=2
    ).is_identical(APyCFixed((7, 0), int_bits=2, frac_bits=2))  # Rounds to (1.75, 0)

    # From APyCFixed, although bad practice
    assert APyCFixed.from_complex(
        APyCFixed((13, 11), int_bits=2, frac_bits=3), int_bits=2, frac_bits=2
    ).is_identical(APyCFixed((7, 6), int_bits=2, frac_bits=2))  # Rounds to (1.75, 1.5)


def test_copy():
    a = APyCFixed((3, 2), 4, 5)
    b = a
    assert a.is_identical(b)
    c = a.copy()
    assert a.is_identical(c)
    a <<= 2
    assert a.is_identical(b)
    assert not a.is_identical(c)


def test_python_copy():
    import copy

    a = APyCFixed((3, 2), 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.copy(a)
    assert a.is_identical(c)
    a <<= 2
    assert a.is_identical(b)
    assert not a.is_identical(c)


def test_python_deepcopy():
    import copy

    a = APyCFixed((3, 2), 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.deepcopy(a)
    assert a.is_identical(c)
    a <<= 2
    assert a.is_identical(b)
    assert not a.is_identical(c)


def test_scalar_is_not_identical_to_array():
    assert not APyCFixed.from_float(-2.5, 10, 10).is_identical(
        APyCFixedArray.from_float([-2.5], 10, 10)
    )
