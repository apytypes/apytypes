from apytypes import APyFixed


def test_repr():
    assert (
        repr(APyFixed.from_float(-1.25, int_bits=2, frac_bits=4))
        == "APyFixed(44, bits=6, int_bits=2)"
    )
    assert (
        APyFixed(-1, int_bits=12, frac_bits=4).__repr__()
        == "APyFixed(65535, bits=16, int_bits=12)"
    )
    assert (
        APyFixed(1234, int_bits=12, frac_bits=0).__repr__()
        == "APyFixed(1234, bits=12, int_bits=12)"
    )
    assert (
        APyFixed.from_float(-2.666, int_bits=3, frac_bits=52).__repr__()
        == "APyFixed(24022200412394226, bits=55, int_bits=3)"
    )

    #
    # Related GitHub issue: #2
    # `Excecive ones when printing bit-pattern for negative fixed-point number`
    #
    assert (
        APyFixed.from_float(-5.75, int_bits=4, frac_bits=4).__repr__()
        == "APyFixed(164, bits=8, int_bits=4)"
    )
    assert APyFixed.from_float(-5.75, int_bits=4, frac_bits=4).to_bits() == 164


def test_str():
    assert str(APyFixed.from_float(-1.25, int_bits=2, frac_bits=4)) == "-1.25"


def test_latex():
    assert (
        APyFixed.from_float(-1.25, int_bits=2, frac_bits=4)._repr_latex_()
        == r"$-\frac{44}{2^{4}} = -1.25$"
    )

    assert (
        APyFixed.from_float(1.25, int_bits=2, frac_bits=4)._repr_latex_()
        == r"$\frac{20}{2^{4}} = 1.25$"
    )
