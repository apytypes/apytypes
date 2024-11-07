from apytypes import APyCFixed, APyFixed


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
    assert not (APyCFixed.from_complex(1.0 + 2.0j, 5, 5) == 1)
    assert APyCFixed.from_complex(1.0 + 2.0j, 5, 5) != 1
    assert APyCFixed.from_complex(4.0, 5, 5) == 4
    assert APyCFixed.from_complex(4.0, 5, 5) != 6

    # Compare to `double`
    assert not (APyCFixed.from_complex(1.0 + 2.0j, 5, 5) == 1.0)
    assert APyCFixed.from_complex(1.0 + 2.0j, 5, 5) != 1.0
    assert APyCFixed.from_complex(4.0, 5, 5) == 4.0
    assert APyCFixed.from_complex(4.0, 5, 5) != 6.0

    # Compare to `complex`
    assert APyCFixed.from_complex(1.0 + 2.0j, 5, 5) == 1.0 + 2.0j
    assert APyCFixed.from_complex(1.0 + 2.0j, 5, 5) != 1.0 + 2.1j
    assert APyCFixed.from_complex(4.0, 5, 5) == APyFixed.from_float(4.0, 5, 5)
    assert APyCFixed.from_complex(4.0, 5, 5) != APyFixed.from_float(5.0, 5, 5)

    # Compare to `APyFixed`
    assert not (
        APyCFixed.from_complex(1.0 + 2.0j, 5, 5) == APyFixed.from_float(1.0, 5, 5)
    )
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
