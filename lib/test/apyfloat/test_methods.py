from itertools import permutations as perm
import pytest
from apytypes import APyFloat


def test_scalar_constructor_raises():
    # Too many exponent bits
    with pytest.raises(ValueError, match=".*exponent"):
        APyFloat(0, 0, 0, 100, 5)
    # Too many mantissa bits
    with pytest.raises(ValueError, match=".*mantissa"):
        APyFloat(0, 0, 0, 5, 100)


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


def test_cast_special_cases():
    # Only change the number of exponent bits
    a = APyFloat(1, 15, 4, 5, 3)
    a = a.cast(4, 3)
    assert a.is_identical(APyFloat(1, 7, 4, 4, 3))

    # Only change the number of mantissa bits
    a = APyFloat(1, 15, 4, 5, 3)
    a = a.cast(4, 2)
    assert a.is_identical(APyFloat(1, 7, 2, 4, 2))

    # Cast from big number becomes infinity
    a = APyFloat(1, 30, 4, 5, 3)
    a = a.cast(4, 3)
    assert a.is_identical(APyFloat(1, 15, 0, 4, 3))

    # Cast from small number becomes zero (for TIES_EVEN)
    a = APyFloat(0, 0, 1, 5, 3)
    a = a.cast(4, 3)
    assert a.is_identical(APyFloat(0, 0, 0, 4, 3))


def test_casting_special_numbers():
    # Cast -0 to -0
    a = APyFloat(1, 0, 0, 5, 7)
    a = a.cast(10, 34)
    assert a.is_identical(APyFloat(1, 0, 0, 10, 34))

    # Cast -inf to -inf
    a = APyFloat(1, 31, 0, 5, 7)
    a = a.cast(11, 34)
    assert a.is_identical(APyFloat(1, 2047, 0, 11, 34))

    # Cast NaN to NaN
    a = APyFloat(0, 31, 123, 5, 7)
    a = a.cast(11, 34)
    assert a.is_nan
    assert a.exp_bits == 11
    assert a.man_bits == 34

    # Cast subnormal to normal
    a = APyFloat(0, 0, 1, 4, 3)
    a = a.cast(5, 3)
    assert a.is_identical(APyFloat(0, 6, 0, 5, 3))

    # Cast normal to subnormal
    a = APyFloat(0, 6, 0, 5, 3)
    a = a.cast(4, 3)
    assert a.is_identical(APyFloat(0, 0, 1, 4, 3))


def test_properties():
    a = APyFloat(0, 3, 4, 4, 6)
    assert a.man == 4
    assert a.true_man == 68
    assert a.exp == 3
    assert a.bias == 7
    assert a.true_exp == -4
    assert a.sign == False
    assert a.true_sign == 1
    assert a.bits == 11

    a = APyFloat(1, 0, 4, 4, 6)
    assert a.man == 4
    assert a.true_man == 4
    assert a.exp == 0
    assert a.bias == 7
    assert a.true_exp == -6
    assert a.sign == True
    assert a.true_sign == -1
    assert a.bits == 11

    # Subnormal number
    a = APyFloat(0, 0, 1, 5, 2)
    assert a.is_finite
    assert not a.is_normal
    assert a.is_subnormal
    assert not a.is_nan
    assert not a.is_inf
    assert not a.is_zero

    # Normal number
    a = APyFloat(0, 1, 1, 5, 2)
    assert a.is_finite
    assert a.is_normal
    assert not a.is_subnormal
    assert not a.is_nan
    assert not a.is_inf
    assert not a.is_zero

    # Infinity
    a = APyFloat(0, 31, 0, 5, 2)
    assert not a.is_finite
    assert not a.is_normal
    assert not a.is_subnormal
    assert not a.is_nan
    assert a.is_inf
    assert not a.is_zero

    # NaN
    a = APyFloat(0, 31, 1, 5, 2)
    assert not a.is_finite
    assert not a.is_normal
    assert not a.is_subnormal
    assert a.is_nan
    assert not a.is_inf
    assert not a.is_zero

    # Zero
    a = APyFloat(0, 0, 0, 5, 2)
    assert a.is_finite
    assert not a.is_normal
    assert a.is_subnormal
    assert not a.is_nan
    assert not a.is_inf
    assert a.is_zero
