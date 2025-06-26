from itertools import product

import pytest

from apytypes import APyCFloat, APyFloat


def test_not_implemented():
    _ = APyCFloat.from_complex(1.0, exp_bits=10, man_bits=20).__str__(base=10)

    with pytest.raises(
        ValueError, match=r"APyCFloat.__str__: base=16 is not supported"
    ):
        _ = APyCFloat.from_complex(1.0, exp_bits=10, man_bits=20).__str__(base=16)
    with pytest.raises(
        ValueError, match=r"APyCFloat.__str__: base=17 is not supported"
    ):
        _ = APyCFloat.from_complex(1.0, exp_bits=10, man_bits=20).__str__(base=17)
    with pytest.raises(ValueError, match=r"APyCFloat.__str__: base=8 is not supported"):
        _ = APyCFloat.from_complex(1.0, exp_bits=10, man_bits=20).__str__(base=8)


@pytest.mark.parametrize("exp_bits", [5, 10, 11, 17])
@pytest.mark.parametrize("man_bits", [11, 17, 20])
@pytest.mark.parametrize("bias", [None, 17, 20])
def test_bit_specifiers(exp_bits: int, man_bits: int, bias: int):
    a = APyCFloat(0, 0, 0, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    assert a.exp_bits == exp_bits
    assert a.man_bits == man_bits
    assert a.bias == (2 ** (exp_bits - 1) - 1) if bias is None else bias
    assert a.bits == exp_bits + man_bits + 1


@pytest.mark.parametrize("exp_bits", [5, 7, 11])
@pytest.mark.parametrize("man_bits", [11, 13])
@pytest.mark.parametrize("value", [0.0, -1.0j, 3.25j, -4.125, -4.125 + 3.25j])
@pytest.mark.parametrize("ignore_zero_sign", [False, True])
def test_is_identical(
    exp_bits: int, man_bits: int, value: complex, ignore_zero_sign: bool
):
    my_float = APyCFloat.from_complex(value, exp_bits, man_bits)
    a = APyCFloat.from_complex(0.0, exp_bits=7, man_bits=13)
    b = APyCFloat.from_complex(0.0 + 3.25j, exp_bits=11, man_bits=11)
    c = APyCFloat.from_complex(-4.125 + 0j, exp_bits=7, man_bits=11)
    d = APyCFloat.from_complex(-4.125 + 3.25j, exp_bits=5, man_bits=13)

    is_a: bool = value == 0.0 and exp_bits == 7 and man_bits == 13
    is_b: bool = value == 3.25j and exp_bits == 11 and man_bits == 11
    is_c: bool = value == -4.125 + 0j and exp_bits == 7 and man_bits == 11
    is_d: bool = value == -4.125 + 3.25j and exp_bits == 5 and man_bits == 13
    assert not (is_a ^ my_float.is_identical(a, ignore_zero_sign=ignore_zero_sign))
    assert not (is_b ^ my_float.is_identical(b, ignore_zero_sign=ignore_zero_sign))
    assert not (is_c ^ my_float.is_identical(c, ignore_zero_sign=ignore_zero_sign))
    assert not (is_d ^ my_float.is_identical(d, ignore_zero_sign=ignore_zero_sign))


@pytest.mark.parametrize("exp_bits", [5, 7, 11])
@pytest.mark.parametrize("man_bits", [11, 13])
@pytest.mark.parametrize("value", [0.0, -1.0j, 3.25j, -4.125, -4.125 + 3.25j])
def test_comparison(exp_bits: int, man_bits: int, value: complex):
    my_float = APyCFloat.from_complex(value, exp_bits, man_bits)
    a = APyCFloat.from_complex(0.0, exp_bits=7, man_bits=13)
    b = APyCFloat.from_complex(0.0 + 3.25j, exp_bits=11, man_bits=11)
    c = APyCFloat.from_complex(-4.125 + 0j, exp_bits=7, man_bits=11)
    d = APyCFloat.from_complex(-4.125 + 3.25j, exp_bits=5, man_bits=13)

    equal_a: bool = value == 0.0
    equal_b: bool = value == 3.25j
    equal_c: bool = value == -4.125 + 0j
    equal_d: bool = value == -4.125 + 3.25j
    assert not (equal_a ^ (my_float == a))
    assert equal_a ^ (my_float != a)
    assert not (equal_b ^ (my_float == b))
    assert equal_b ^ (my_float != b)
    assert not (equal_c ^ (my_float == c))
    assert equal_c ^ (my_float != c)
    assert not (equal_d ^ (my_float == d))
    assert equal_d ^ (my_float != d)


def test_to_complex():
    assert (
        complex(APyCFloat((0, 0), exp=(0, 0), man=(0, 0), exp_bits=5, man_bits=5))
        == 0 + 0j
    )
    assert (
        complex(APyCFloat((1, 0), exp=(15, 0), man=(0, 0), exp_bits=5, man_bits=5))
        == -1 + 0j
    )
    assert (
        complex(APyCFloat((0, 0), exp=(15, 0), man=(8, 0), exp_bits=5, man_bits=5))
        == 1.25 + 0j
    )
    assert (
        complex(APyCFloat((1, 0), exp=(15, 0), man=(16, 0), exp_bits=5, man_bits=5))
        == -1.5 + 0j
    )
    assert (
        complex(APyCFloat((0, 1), exp=(0, 17), man=(0, 0), exp_bits=5, man_bits=5))
        == 0 - 4j
    )
    assert (
        complex(APyCFloat((0, 0), exp=(0, 17), man=(0, 2), exp_bits=5, man_bits=5))
        == 0 + 4.25j
    )
    assert (
        complex(APyCFloat((0, 1), exp=(14, 18), man=(8, 0), exp_bits=5, man_bits=5))
        == 0.625 - 8j
    )
    assert (
        complex(APyCFloat((0, 0), exp=(15, 18), man=(16, 0), exp_bits=5, man_bits=5))
        == 1.5 + 8j
    )


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


@pytest.mark.parametrize("exp_bits", product([5, 11], repeat=2))
@pytest.mark.parametrize("man_bits", product([11, 30, 51], repeat=2))
@pytest.mark.parametrize("bias", product([None, 14], repeat=2))
@pytest.mark.parametrize(
    "value", [0.0, -0.0, 4.5, -62.0625, float("-inf"), -(2**11), 2**-11]
)
def test_from_apyfloat(
    exp_bits: tuple[int, int],
    man_bits: tuple[int, int],
    bias: tuple[int | None, int | None],
    value: float,
):
    a = APyFloat.from_float(
        value, exp_bits=exp_bits[0], man_bits=man_bits[0], bias=bias[0]
    )
    b = APyCFloat.from_float(
        a, exp_bits=exp_bits[1], man_bits=man_bits[1], bias=bias[1]
    )
    assert b.is_identical(
        APyCFloat.from_float(
            value, exp_bits=exp_bits[1], man_bits=man_bits[1], bias=bias[1]
        )
    )
