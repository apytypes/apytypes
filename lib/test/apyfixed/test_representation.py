from typing import cast

from apytypes import APyFixed


def is_repr_identical(a: APyFixed):
    """
    Tests that an :class:`APyFixed` is identical to its representation.
    """
    return a.is_identical(cast(APyFixed, eval(repr(a))))


def test_repr():
    a = APyFixed.from_float(-1.25, int_bits=2, frac_bits=4)
    assert repr(a) == r"APyFixed(44, int_bits=2, frac_bits=4)"
    assert is_repr_identical(a)

    b = APyFixed(-1, int_bits=12, frac_bits=4)
    assert repr(b) == r"APyFixed(65535, int_bits=12, frac_bits=4)"
    assert is_repr_identical(b)

    c = APyFixed(1234, int_bits=12, frac_bits=0)
    assert repr(c) == r"APyFixed(1234, int_bits=12, frac_bits=0)"
    assert is_repr_identical(c)

    d = APyFixed.from_float(-2.666, int_bits=3, frac_bits=52)
    assert repr(d) == r"APyFixed(24022200412394226, int_bits=3, frac_bits=52)"
    assert is_repr_identical(d)

    #
    # Related GitHub issue: #2
    # `Excecive ones when printing bit-pattern for negative fixed-point number`
    #
    e = APyFixed.from_float(-5.75, int_bits=4, frac_bits=4)
    assert repr(e) == "APyFixed(164, int_bits=4, frac_bits=4)"
    assert is_repr_identical(e)
    assert e.to_bits() == 164


def test_str():
    assert str(APyFixed.from_float(-1.25, int_bits=2, frac_bits=4)) == "-1.25"


def test_latex():
    assert (
        APyFixed.from_float(-1.25, int_bits=2, frac_bits=4)._repr_latex_()
        == r"$-\frac{20}{2^{4}} = -1.25$"
    )

    assert (
        APyFixed.from_float(1.25, int_bits=2, frac_bits=4)._repr_latex_()
        == r"$\frac{20}{2^{4}} = 1.25$"
    )
