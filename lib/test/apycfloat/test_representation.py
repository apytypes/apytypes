import pytest

from apytypes import APyCFloat


@pytest.mark.parametrize("exp_bits", [5, 11])
@pytest.mark.parametrize("man_bits", [11, 13])
@pytest.mark.parametrize("sign", [(0, 0), (0, 1), (1, 1)])
@pytest.mark.parametrize("exp", [(0, 0), (0, 17), (5, 9)])
@pytest.mark.parametrize("man", [(0, 0), (13, 0), (5, 9)])
@pytest.mark.parametrize("bias", [None, 27])
def test_repr(
    exp_bits: int,
    man_bits: int,
    bias: int | None,
    sign: tuple[int, int],
    exp: tuple[int, int],
    man: tuple[int, int],
):
    a = APyCFloat(
        sign=sign, exp=exp, man=man, exp_bits=exp_bits, man_bits=man_bits, bias=bias
    )

    repr_bias = "" if bias is None else f", bias={bias}"
    repr_val = f"sign={sign}, exp={exp}, man={man}"
    repr_bit_spec = f"exp_bits={exp_bits}, man_bits={man_bits}"

    assert repr(a) == f"APyCFloat({repr_val}, {repr_bit_spec}{repr_bias})"


def test_str():
    assert (
        str(APyCFloat((0, 0), exp=(0, 0), man=(0, 0), exp_bits=5, man_bits=5))
        == "(0+0j)"
    )
    assert (
        str(
            APyCFloat((1, 0), exp=(63, 64), man=(2**18, 2**18), exp_bits=7, man_bits=19)
        )
        == "(-1.5+3j)"
    )
    assert (
        str(
            APyCFloat((1, 7), exp=(60, 61), man=(2**17, 2**16), exp_bits=7, man_bits=19)
        )
        == "(-0.15625-0.28125j)"
    )
    assert str(APyCFloat.from_float(complex("nan+nanj"), 7, 7)) == "(nan+nanj)"
    assert str(APyCFloat.from_float(complex("nan-nanj"), 7, 7)) == "(nan+nanj)"
    assert str(APyCFloat.from_float(complex("-nan+nanj"), 7, 7)) == "(nan+nanj)"
    assert str(APyCFloat.from_float(complex("-nan-nanj"), 7, 7)) == "(nan+nanj)"


def test_latex():
    assert (
        APyCFloat((0, 0), exp=(0, 0), man=(0, 0), exp_bits=5, man_bits=5)._repr_latex_()
        == "$0 + 0j$"
    )
    assert (
        APyCFloat((0, 1), exp=(5, 0), man=(1, 4), exp_bits=5, man_bits=5)._repr_latex_()
        == "$\\left(1 + \\frac{1}{2^{5}}\\right)2^{5-15} - \\frac{4}{2^{5}}2^{1-15}j = 33\\times 2^{-15} - 4\\times 2^{-19}j = 0.001007080078125 - 0.00000762939453125j$"
    )
    assert (
        APyCFloat(
            (1, 0), exp=(5, 3), man=(14, 4), exp_bits=4, man_bits=5
        )._repr_latex_()
        == "$-\\left(1 + \\frac{14}{2^{5}}\\right)2^{5-7} + \\left(1 + \\frac{4}{2^{5}}\\right)2^{3-7}j = -46\\times 2^{-7} + 36\\times 2^{-9}j = -0.359375 + 0.0703125j$"
    )
