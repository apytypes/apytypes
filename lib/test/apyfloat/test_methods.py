from itertools import permutations as perm
import pytest
from apytypes import APyFloat, APyFixed


@pytest.mark.float_special
@pytest.mark.parametrize("float_s", ["nan", "inf", "-inf", "0.0", "-0.0"])
def test_special_conversions(float_s):
    assert (
        str(float(APyFloat.from_float(float(float_s), 5, 5)))
        == str(float(float_s))
        == float_s
    )


@pytest.mark.parametrize("exp", list(perm([5, 6, 7, 8], 2)))
@pytest.mark.parametrize("man", list(perm([5, 6, 7, 8], 2)))
@pytest.mark.parametrize("val", [2.625, 12])
@pytest.mark.parametrize("neg", [-1.0, 1.0])
def test_normal_conversions(exp, man, val, neg):
    val *= neg
    assert (
        float(APyFloat.from_float(val, exp[0], man[0]))
        == float(APyFloat.from_float(val, exp[1], man[1]))
        == val
    )


@pytest.mark.parametrize("sign", ["1", "0"])
@pytest.mark.parametrize(
    "absx,ans",
    [
        ("00000_00", "0.0"),  # Zero
        ("0000_001", "1*2**-9"),  # Min subnorm
        ("0000_010", "2*2**-9"),
        ("0000_011", "3*2**-9"),
        ("0000_100", "4*2**-9"),
        ("0000_101", "5*2**-9"),
        ("0000_110", "6*2**-9"),
        ("0000_111", "7*2**-9"),  # Max subnorm
        ("0001_000", "2**-6"),  # Min normal
        ("1110_111", "240.0"),  # Max normal
        ("1111_000", 'float("inf")'),  # Infinity
    ],
)
def test_bit_conversions_e4m3(absx, sign, ans):
    assert float(APyFloat.from_bits(int(f"{sign}_{absx}", 2), 4, 3)) == eval(
        f'{"-" if sign == "1" else ""}{ans}'
    )
    assert APyFloat.from_float(
        eval(f'{"-" if sign == "1" else ""}{ans}'), 4, 3
    ).to_bits() == int(f"{sign}_{absx}", 2)


@pytest.mark.parametrize("sign", ["1", "0"])
@pytest.mark.parametrize(
    "absx,ans",
    [
        ("11111_01", 'float("nan")'),  # NaN
        ("11111_10", 'float("nan")'),  # NaN
        ("11111_11", 'float("nan")'),  # NaN
    ],
)
def test_bit_conversion_nan_e5m2(absx, sign, ans):
    assert str(float(APyFloat.from_bits(int(f"{sign}_{absx}", 2), 5, 2))) == str(
        eval(f'{"-" if sign == "1" else ""}{ans}')
    )
    bits = APyFloat.from_float(
        eval(f'{"-" if sign == "1" else ""}{ans}'), 5, 2
    ).to_bits()
    assert (bits & 0x3) != 0


def test_long_python_integer():
    val = 0xF000000FFFFFFFFFF
    a = APyFloat.from_bits(val, 27, 40)
    assert a.is_identical(APyFloat(1, 117440512, 1099511627775, 27, 40))
    assert a.to_bits() == val


def test_convert_to_double():
    a = APyFloat(0, 1, 1, 13, 60)
    assert float(a) == 0.0

    a = APyFloat(0, 8100, 1, 13, 60)
    assert a.is_finite
    assert float(a) == float("inf")
    assert float(-a) == float("-inf")

    a = APyFloat(0, 0, 7, 11, 54)
    assert float(a) == 1e-323

    a = APyFloat(0, 0, 1, 11, 52)
    assert float(a) == 5e-324


def test_from_float():
    a = APyFloat(0, 0, 1, 11, 52)
    b = APyFloat.from_float(5e-324, 11, 52)
    b.is_identical(a)
