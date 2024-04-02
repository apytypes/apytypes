from apytypes import APyFloat


def test_str():
    assert str(APyFloat.from_float(0, 2, 2)) == "0"
    assert str(APyFloat.from_float(20, 2, 2)) == "inf"
    assert str(APyFloat.from_float(0.5, 2, 2)) == "0.5"
    assert str(APyFloat.from_float(-0.5, 2, 2)) == "-0.5"

    assert str(APyFloat.from_float(float("NaN"), 2, 2)) == "nan"


def test_repr():
    assert (
        repr(APyFloat.from_float(0, 3, 5))
        == "APyFloat(sign=0, exp=0, man=0, exp_bits=3, man_bits=5)"
    )
    assert (
        repr(APyFloat.from_float(20, 2, 3))
        == "APyFloat(sign=0, exp=3, man=0, exp_bits=2, man_bits=3)"
    )
    assert (
        repr(APyFloat.from_float(0.5, 2, 2))
        == "APyFloat(sign=0, exp=0, man=2, exp_bits=2, man_bits=2)"
    )
    assert (
        repr(APyFloat.from_float(-0.5, 2, 2))
        == "APyFloat(sign=1, exp=0, man=2, exp_bits=2, man_bits=2)"
    )

    assert (
        repr(APyFloat.from_float(float("NaN"), 2, 2))
        == "APyFloat(sign=0, exp=3, man=1, exp_bits=2, man_bits=2)"
    )


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
