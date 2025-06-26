from apytypes import APyCFixed


def test_repr():
    assert (
        repr(APyCFixed.from_float(-1.25, int_bits=2, frac_bits=4))
        == r"APyCFixed((44, 0), bits=6, int_bits=2)"
    )
    assert (
        repr(APyCFixed.from_float(-1.25 + 1j, int_bits=2, frac_bits=4))
        == r"APyCFixed((44, 16), bits=6, int_bits=2)"
    )
    assert (
        repr(APyCFixed.from_float(-1.25 - 1j, int_bits=2, frac_bits=4))
        == r"APyCFixed((44, 48), bits=6, int_bits=2)"
    )
    assert (
        APyCFixed(-1, int_bits=12, frac_bits=4).__repr__()
        == r"APyCFixed((65535, 0), bits=16, int_bits=12)"
    )
    assert (
        APyCFixed(1234, int_bits=12, frac_bits=0).__repr__()
        == r"APyCFixed((1234, 0), bits=12, int_bits=12)"
    )
    assert (
        APyCFixed.from_float(-2.666, int_bits=3, frac_bits=52).__repr__()
        == r"APyCFixed((24022200412394226, 0), bits=55, int_bits=3)"
    )

    #
    # Related GitHub issue: #2
    # `Excecive ones when printing bit-pattern for negative fixed-point number`
    #
    assert (
        APyCFixed.from_float(-5.75, int_bits=4, frac_bits=4).__repr__()
        == "APyCFixed((164, 0), bits=8, int_bits=4)"
    )
    assert APyCFixed.from_float(-5.75, int_bits=4, frac_bits=4).to_bits() == (164, 0)


def test_str():
    assert str(APyCFixed.from_float(0, int_bits=2, frac_bits=4)) == "(0+0j)"
    assert str(APyCFixed.from_float(1.25, int_bits=2, frac_bits=4)) == "(1.25+0j)"
    assert str(APyCFixed.from_float(-1.25, int_bits=2, frac_bits=4)) == "(-1.25+0j)"
    assert str(APyCFixed.from_complex(1.25j, int_bits=2, frac_bits=4)) == "(0+1.25j)"
    assert str(APyCFixed.from_complex(-1.25j, int_bits=2, frac_bits=4)) == "(0-1.25j)"
    assert (
        str(APyCFixed.from_complex(1.25 + 1.25j, int_bits=2, frac_bits=4))
        == "(1.25+1.25j)"
    )
    assert (
        str(APyCFixed.from_complex(-1.25 - 1.25j, int_bits=2, frac_bits=4))
        == "(-1.25-1.25j)"
    )
