from typing import cast

from apytypes import APyFloat


def is_repr_identical(a: APyFloat):
    """
    Tests that an :class:`APyFloat` is identical to its representation.
    """
    return a.is_identical(cast(APyFloat, eval(repr(a))))


def test_str():
    assert str(APyFloat.from_float(0, 2, 2)) == "0"
    assert str(APyFloat.from_float(20, 2, 2)) == "inf"
    assert str(APyFloat.from_float(0.5, 2, 2)) == "0.5"
    assert str(APyFloat.from_float(-0.5, 2, 2)) == "-0.5"

    assert str(APyFloat.from_float(float("NaN"), 2, 2)) == "nan"


def test_repr():
    a = APyFloat.from_float(0, 3, 5)
    assert repr(a) == "APyFloat(sign=0, exp=0, man=0, exp_bits=3, man_bits=5)"
    assert is_repr_identical(a)

    b = APyFloat.from_float(20, 2, 3)
    assert repr(b) == "APyFloat(sign=0, exp=3, man=0, exp_bits=2, man_bits=3)"
    assert is_repr_identical(b)

    c = APyFloat.from_float(0.5, 2, 2)
    assert repr(c) == "APyFloat(sign=0, exp=0, man=2, exp_bits=2, man_bits=2)"
    assert is_repr_identical(c)

    d = APyFloat.from_float(-0.5, 2, 2)
    assert repr(d) == "APyFloat(sign=1, exp=0, man=2, exp_bits=2, man_bits=2)"
    assert is_repr_identical(d)

    e = APyFloat.from_float(float("NaN"), 2, 2)
    assert repr(e) == "APyFloat(sign=0, exp=3, man=1, exp_bits=2, man_bits=2)"
    assert is_repr_identical(e)


def test_latex():
    assert APyFloat.from_float(0, 2, 2)._repr_latex_() == "$0$"
    assert APyFloat.from_float(20, 2, 2)._repr_latex_() == r"$\infty$"
    assert (
        APyFloat.from_float(0.5, 2, 2)._repr_latex_()
        == r"$\frac{2}{2^{2}}2^{1-1} = 2\times 2^{-2} = 0.5$"
    )
    assert (
        APyFloat.from_float(-0.5, 2, 2)._repr_latex_()
        == r"$-\frac{2}{2^{2}}2^{1-1} = -2\times 2^{-2} = -0.5$"
    )
    assert (
        APyFloat.from_float(1.5, 2, 2)._repr_latex_()
        == r"$\left(1 + \frac{2}{2^{2}}\right)2^{1-1} = 6\times 2^{-2} = 1.5$"
    )
    assert APyFloat.from_float(float("NaN"), 2, 2)._repr_latex_() == r"$\textrm{NaN}$"

    assert (
        APyFloat.from_float(800, 6, 8)._repr_latex_()
        == r"$\left(1 + \frac{144}{2^{8}}\right)2^{40-31} = 400\times 2^{1} = 800$"
    )
