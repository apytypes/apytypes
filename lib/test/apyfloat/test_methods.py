from itertools import permutations as perm
import pytest
from apytypes import APyFloat


def test_scalar_constructor_raises():
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloat(0, 0, 0, 300, 5)
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloat(0, 0, 0, -300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloat(0, 0, 0, 5, 300)
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloat(0, 0, 0, 5, -300)


def test_scalar_from_float_raises():
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloat.from_float(0, 300, 5)
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloat.from_float(0, -300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloat.from_float(0, 5, 300)
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloat.from_float(0, 5, -300)
    with pytest.raises(
        ValueError,
        match="Non supported type",
    ):
        APyFloat.from_float("0", 5, 10)


def test_scalar_from_bits_raises():
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloat.from_bits(0, 300, 5)
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloat.from_bits(0, -300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloat.from_bits(0, 5, 300)
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloat.from_bits(0, 5, -300)


def test_scalar_cast_raises():
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloat(0, 0, 0, 5, 5).cast(300, 5)
    with pytest.raises(
        ValueError,
        match="Exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloat(0, 0, 0, 5, 5).cast(-300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloat(0, 0, 0, 5, 5).cast(5, 300)
    with pytest.raises(
        ValueError,
        match="Mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        APyFloat(0, 0, 0, 5, 5).cast(5, -300)


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
    assert b.is_identical(a)

    # Big float should become infinity
    a = APyFloat.from_float(-(2**10), 4, 15)
    assert a.is_identical(APyFloat(1, 15, 0, 4, 15))


def test_from_float_with_non_floats():
    a = APyFloat.from_float(16, exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloat(sign=0, exp=11, man=0, exp_bits=4, man_bits=2))

    # Should quantize to 16.0
    a = APyFloat.from_float(17, exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloat(sign=0, exp=11, man=0, exp_bits=4, man_bits=2))

    # Should quantize to -20.0
    a = APyFloat.from_float(-19, exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloat(sign=1, exp=11, man=1, exp_bits=4, man_bits=2))

    # Tie break, should quantize to 16.0
    a = APyFloat.from_float(18, exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloat(sign=0, exp=11, man=0, exp_bits=4, man_bits=2))

    # Tie break, should quantize to 24.0
    a = APyFloat.from_float(22, exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloat(sign=0, exp=11, man=2, exp_bits=4, man_bits=2))

    # Should quantize to 28.0
    a = APyFloat.from_float(29, exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloat(sign=0, exp=11, man=3, exp_bits=4, man_bits=2))

    # Should quantize to 32.0
    a = APyFloat.from_float(31, exp_bits=4, man_bits=2)
    assert a.is_identical(APyFloat(sign=0, exp=12, man=0, exp_bits=4, man_bits=2))

    # 152-bit number, should become negative infinity
    a = APyFloat.from_float(
        -0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, exp_bits=4, man_bits=2
    )
    assert a.is_identical(APyFloat(sign=1, exp=15, man=0, exp_bits=4, man_bits=2))

    # Test with bool and non IEEE-like bias
    a = APyFloat.from_float(True, exp_bits=4, man_bits=2, bias=4)
    assert a.is_identical(
        APyFloat(sign=0, exp=4, man=0, exp_bits=4, man_bits=2, bias=4)
    )

    # Test with big integer
    a = APyFloat.from_float(2**2047, exp_bits=28, man_bits=2)
    assert a.is_identical(
        APyFloat(sign=0, exp=(2047 + (2**27) - 1), man=0, exp_bits=28, man_bits=2)
    )


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
    assert not a.is_subnormal
    assert not a.is_nan
    assert not a.is_inf
    assert a.is_zero


def test_round_trip_conversion():
    for exp in reversed(range(15)):  # Skip nan
        for man in reversed(range(16)):
            a = APyFloat(0, exp, man, 4, 4)
            assert (APyFloat.from_float(float(a), 4, 4)).is_identical(a)


@pytest.mark.parametrize(
    "apyfloat, test_exp",
    [
        # First some general tests
        (APyFloat(0, 0, 1, 4, 3), APyFloat(0, 0, 2, 4, 3)),
        (APyFloat(0, 5, 2, 4, 3), APyFloat(0, 5, 3, 4, 3)),
        # Mantissa overflow
        (APyFloat(0, 0, 7, 4, 3), APyFloat(0, 1, 0, 4, 3)),
        (APyFloat(0, 5, 7, 4, 3), APyFloat(0, 6, 0, 4, 3)),
        # Special numbers
        (APyFloat(0, 15, 0, 4, 3), APyFloat(0, 15, 0, 4, 3)),  # inf
        (APyFloat(0, 15, 1, 4, 3), APyFloat(0, 15, 1, 4, 3)),  # nan
        (APyFloat(0, 0, 0, 4, 3), APyFloat(0, 0, 1, 4, 3)),  # +0
        # General tests for negative numbers
        (APyFloat(1, 0, 2, 4, 3), APyFloat(1, 0, 1, 4, 3)),
        (APyFloat(1, 5, 2, 4, 3), APyFloat(1, 5, 1, 4, 3)),
        # Mantissa underflow
        (APyFloat(1, 1, 0, 4, 3), APyFloat(1, 0, 7, 4, 3)),
        (APyFloat(1, 5, 0, 4, 3), APyFloat(1, 4, 7, 4, 3)),
        # Special numbers
        (APyFloat(1, 15, 0, 4, 3), APyFloat(1, 14, 7, 4, 3)),  # -inf
        (APyFloat(1, 15, 1, 4, 3), APyFloat(1, 15, 1, 4, 3)),  # -nan
        (APyFloat(1, 0, 0, 4, 3), APyFloat(0, 0, 1, 4, 3)),  # -0
        (APyFloat(1, 0, 1, 4, 3), APyFloat(1, 0, 0, 4, 3)),  # Smallest negative number
    ],
)
def test_next_up(apyfloat, test_exp):
    assert (_ := apyfloat.next_up()).is_identical(test_exp)


@pytest.mark.parametrize("bit_pattern", list(range(2**7)))
def test_next_down(bit_pattern):
    """Test next_down by making use of the property: next_down(x) == -next_up(-x)."""
    apyfloat = APyFloat.from_bits(bit_pattern, 3, 3)
    neg_apyfloat = -apyfloat
    res = apyfloat.next_down()
    ref = -(neg_apyfloat.next_up())
    assert res.is_identical(ref)
