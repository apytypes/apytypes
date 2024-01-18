from apytypes import APyFixed


def test_repr():
    assert (
        repr(APyFixed.from_float(-1.25, int_bits=2, frac_bits=4))
        == "APyFixed(44, bits=6, int_bits=2)"
    )


def test_str():
    assert str(APyFixed.from_float(-1.25, int_bits=2, frac_bits=4)) == "-1.25"
