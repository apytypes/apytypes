from itertools import permutations as perm
import math
import pytest
from apytypes import APyFloat


# Negation
@pytest.mark.parametrize("sign", ["-", ""])
@pytest.mark.parametrize(
    "float_s", ["13", "0.0", pytest.param("inf", marks=pytest.mark.float_special)]
)
def test_negation(float_s, sign):
    float_s = sign + float_s
    assert float(eval(f'-APyFloat.from_float(float("{float_s}"), 5, 5)')) == -float(
        float_s
    )


# Abs
@pytest.mark.parametrize("sign", ["-", ""])
@pytest.mark.parametrize(
    "float_s", ["13", "0.0", pytest.param("inf", marks=pytest.mark.float_special)]
)
def test_abs(float_s, sign):
    float_s = sign + float_s
    assert float(eval(f'abs(APyFloat.from_float(float("{float_s}"), 5, 5))')) == abs(
        float(float_s)
    )


# Addition
@pytest.mark.float_add
@pytest.mark.parametrize("exp", list(perm(["5", "6", "10"], 2)))
@pytest.mark.parametrize("man", list(perm(["5", "6", "10"], 2)))
@pytest.mark.parametrize("sign", list(set(perm(["-", "-", "", ""], 2))))
@pytest.mark.parametrize("lhs,rhs", list(perm(["2.75", "2.5", "16.5", "0"], 2)))
def test_add_representable(exp, man, sign, lhs, rhs):
    """Test additions where the operands and the result is exactly representable by the formats."""
    expr = None
    f = float(eval(f"{sign[0]}{lhs} + {sign[1]}{rhs}"))
    e = max(int(exp[0]), int(exp[1]))
    m = max(int(man[0]), int(man[1]))
    print(repr(APyFloat.from_float(f, e, m)))
    assert float(
        eval(
            expr
            := f"APyFloat.from_float({sign[0]}{lhs}, {exp[0]}, {man[0]}) + APyFloat.from_float({sign[1]}{rhs}, {exp[1]}, {man[1]})"
        )
    ) == (float(eval(f"{sign[0]}{lhs} + {sign[1]}{rhs}")))
    res = eval(expr)
    assert res.exp_bits == max(int(exp[0]), int(exp[1]))
    assert res.man_bits == max(int(man[0]), int(man[1]))


@pytest.mark.float_add
def test_add_overflow():
    """Test that an addition can overflow."""
    # To positive infinity
    res = APyFloat(0, 30, 3, 5, 2) + APyFloat(0, 30, 3, 5, 2)
    assert res.is_inf

    # To negative infinity
    res = APyFloat(1, 30, 3, 5, 2) + APyFloat(1, 30, 3, 5, 2)
    assert res.is_inf

    # Overflow to infinity after quantization
    res = APyFloat(0, 30, 3, 5, 2) + APyFloat(0, 27, 0, 5, 2)
    assert res.is_inf


@pytest.mark.float_add
def test_add_subnormals():
    # Add two subnormal numbers
    res = APyFloat(0, 0, 1, 5, 3) + APyFloat(0, 0, 2, 5, 3)
    assert res.is_identical(APyFloat(0, 0, 3, 5, 3))

    # Addition of subnormal numbers resulting in normal number
    res = APyFloat(0, 0, 7, 5, 3) + APyFloat(0, 0, 1, 5, 3)
    assert res.is_identical(APyFloat(0, 1, 0, 5, 3))

    # Add subnormal to normal number
    res = APyFloat(0, 0, 1, 4, 3) + APyFloat(0, 1, 0, 4, 3)
    assert res.is_identical(APyFloat(0, 1, 1, 4, 3))

    # Add subnormal to big normal number
    res = APyFloat(0, 0, 1, 4, 3) + APyFloat(0, 7, 0, 4, 3)
    assert res.is_identical(APyFloat(0, 7, 0, 4, 3))


@pytest.mark.float_add
@pytest.mark.parametrize("num", [APyFloat(0, 17, 123, 5, 7), APyFloat(0, 0, 123, 5, 7)])
def test_add_zero(num):
    assert (num + APyFloat(0, 0, 0, 5, 7)).is_identical(num)
    assert (APyFloat(0, 0, 0, 5, 7) + num).is_identical(num)


@pytest.mark.float_add
@pytest.mark.parametrize(
    "num",
    [
        APyFloat(0, 17, 123, 5, 7),
        APyFloat(0, 0, 123, 5, 7),
        APyFloat(1, 17, 123, 5, 7),
        APyFloat(1, 0, 123, 5, 7),
        APyFloat(0, 7, 23, 4, 5),
        APyFloat(1, 7, 23, 4, 5),
    ],
)
def test_add_inf(num):
    assert (num + APyFloat(0, 0x1F, 0, 5, 7)).is_identical(APyFloat(0, 0x1F, 0, 5, 7))
    assert (APyFloat(0, 0x1F, 0, 5, 7) + num).is_identical(APyFloat(0, 0x1F, 0, 5, 7))


@pytest.mark.float_add
@pytest.mark.parametrize("num", [APyFloat(0, 17, 123, 5, 7), APyFloat(0, 0, 123, 5, 7)])
def test_add_nan(num):
    assert (num + APyFloat(0, 0x1F, 1, 5, 7)).is_nan
    assert (APyFloat(0, 0x1F, 1, 5, 7) + num).is_nan


@pytest.mark.float_add
def test_add_inf_nan():
    # Infinity with NaN
    assert (APyFloat(0, 0x1F, 0, 5, 7) + APyFloat(0, 0x1F, 1, 5, 7)).is_nan
    assert (APyFloat(0, 0x1F, 1, 5, 7) + APyFloat(0, 0x1F, 0, 5, 7)).is_nan

    # Infinity with infinity
    assert (APyFloat(0, 0x1F, 0, 5, 7) + APyFloat(0, 0x1F, 0, 5, 7)).is_identical(
        APyFloat(0, 0x1F, 0, 5, 7)
    )

    # NaN with NaN
    assert (APyFloat(0, 0x1F, 1, 5, 7) + APyFloat(0, 0x1F, 1, 5, 7)).is_nan


@pytest.mark.float_sub
def test_sub_mixed_bias():
    # Test that the implementation doesn't do "x.cast() - y.cast()"
    x = APyFloat(sign=0, exp=30, man=0, exp_bits=5, man_bits=2, bias=14)
    y = APyFloat(sign=0, exp=30, man=0, exp_bits=5, man_bits=2, bias=16)
    assert (_ := x - y).is_identical(
        APyFloat(sign=0, exp=30, man=2, exp_bits=5, man_bits=2, bias=15)
    )

    # Test subtraction when two numbers are equal but with different formats
    x = APyFloat(sign=0, exp=14, man=0, exp_bits=5, man_bits=2, bias=14)
    y = APyFloat(sign=0, exp=21, man=0, exp_bits=5, man_bits=2, bias=21)
    assert (_ := x - y).is_identical(
        APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=17)
    )


@pytest.mark.float_add
def test_add_mixed_bias_overflow():
    """Test that a result can overflow to infinity due to a change in bias."""
    x = APyFloat(sign=0, exp=21, man=0, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=25)

    # Add with zero
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=31, man=0, exp_bits=5, man_bits=2, bias=15)
    )

    # Add with zero but with larger bias difference
    y = APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=27)
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=31, man=0, exp_bits=5, man_bits=2, bias=16)
    )

    # Add with small normal number
    y = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=28)
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=31, man=0, exp_bits=5, man_bits=2, bias=16)
    )

    # Add with subnormal number
    y = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=28)
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=31, man=0, exp_bits=5, man_bits=2, bias=16)
    )


@pytest.mark.float_add
def test_add_double():
    """Some tests for sanity checking addition with binary64. This uses the faster path."""

    # 2.5 + 24.5 = 27.0
    x = APyFloat(sign=0, exp=1024, man=0x4000000000000, exp_bits=11, man_bits=52)
    y = APyFloat(sign=0, exp=1027, man=0x8400000000000, exp_bits=11, man_bits=52)
    res = x + y
    assert res.is_identical(
        APyFloat(sign=0, exp=1027, man=0xAC00000000000, exp_bits=11, man_bits=52)
    )

    # Add two subnormals
    x = APyFloat(sign=0, exp=0, man=23, exp_bits=11, man_bits=61)
    y = APyFloat(sign=0, exp=0, man=71, exp_bits=11, man_bits=61)
    res = x + y
    assert res.is_identical(APyFloat(sign=0, exp=0, man=94, exp_bits=11, man_bits=61))

    # Mixed formats. 1.75 + (-5) = -3.25
    x = APyFloat(sign=0, exp=7, man=6, exp_bits=4, man_bits=3)
    y = APyFloat(sign=1, exp=1025, man=0x4000000000000, exp_bits=11, man_bits=52)
    res = x + y
    assert res.is_identical(
        APyFloat(sign=1, exp=1024, man=0xA000000000000, exp_bits=11, man_bits=52)
    )

    # 1.234324 + 5.21343 = 6.447754
    # These numbers cannot be represented exactly but should be performed as the following
    x = APyFloat(0, 1023, 0x3BFCA85CAAFBC, 11, 52)
    y = APyFloat(0, 1025, 0x4DA8D64D7F0ED, 11, 52)
    res = x + y
    assert res.is_identical(APyFloat(0, 1025, 0x9CA80064A9CDC, 11, 52))


@pytest.mark.float_add
def test_long_add():
    """Some tests for sanity checking addition with longer formats."""
    # 2.5 + 24.5 = 27.0
    x = APyFloat(sign=0, exp=1024, man=0x4000000000000 << 8, exp_bits=11, man_bits=60)
    y = APyFloat(sign=0, exp=1027, man=0x8400000000000 << 8, exp_bits=11, man_bits=60)
    res = x + y
    assert res.is_identical(
        APyFloat(sign=0, exp=1027, man=0xAC00000000000 << 8, exp_bits=11, man_bits=60)
    )

    # Add two subnormals
    x = APyFloat(sign=0, exp=0, man=23, exp_bits=11, man_bits=61)
    y = APyFloat(sign=0, exp=0, man=71, exp_bits=11, man_bits=61)
    res = x + y
    assert res.is_identical(APyFloat(sign=0, exp=0, man=94, exp_bits=11, man_bits=61))

    # Add two subnormals that becomes a normal number
    x = APyFloat(sign=0, exp=0, man=(1 << 61) - 1, exp_bits=11, man_bits=61)
    y = APyFloat(sign=0, exp=0, man=1, exp_bits=11, man_bits=61)
    res = x + y
    assert res.is_identical(APyFloat(sign=0, exp=1, man=0, exp_bits=11, man_bits=61))

    # Mixed formats. 1.75 + (-5) = -3.25
    x = APyFloat(sign=0, exp=7, man=6, exp_bits=4, man_bits=3)
    y = APyFloat(sign=1, exp=1025, man=0x4000000000000 << 8, exp_bits=11, man_bits=60)
    res = x + y
    assert res.is_identical(
        APyFloat(sign=1, exp=1024, man=0xA000000000000 << 8, exp_bits=11, man_bits=60)
    )

    # 1.234324 + 5.21343 = 6.447754
    # These numbers cannot be represented exactly but should be performed as the following
    x = APyFloat(0, 1023, 0x3BFCA85CAAFBC << 8, 11, 60)
    y = APyFloat(0, 1025, 0x4DA8D64D7F0ED << 8, 11, 60)
    res = x + y
    assert res.is_identical(APyFloat(0, 1025, 0x9CA80064A9CDC << 8, 11, 60))

    # Test carry from addition of mantissas
    x = APyFloat(0, 1023, 1 << 59, 11, 60)
    res = x + x
    assert res.is_identical(APyFloat(0, 1024, 1 << 59, 11, 60))

    # Test carry from addition of mantissas can overflow to infinity
    x = APyFloat(0, 2046, 1 << 59, 11, 60)
    y = APyFloat(0, 2046, 1 << 59, 11, 60)
    res = x + y
    assert res.is_identical(APyFloat(0, 2047, 0, 11, 60))

    # Test carry from quantization
    x = APyFloat(0, 1023, (1 << 60) - 1, 11, 60)
    y = APyFloat(0, 1023 - 61, 0, 11, 60)  # Tie break
    res = x + y
    assert res.is_identical(APyFloat(0, 1024, 0, 11, 60))

    # Test carry from quantization can overflow to infinity
    x = APyFloat(0, 2046, (1 << 60) - 1, 11, 60)
    y = APyFloat(0, 2046 - 61, 0, 11, 60)  # Tie break
    res = x + y
    assert res.is_identical(APyFloat(0, 2047, 0, 11, 60))

    # Test subtracting infinities
    x = APyFloat(0, 2047, 0, 11, 60)
    y = APyFloat(1, 2047, 0, 11, 60)
    res = x + y
    assert res.is_nan

    # Test subtracting two equal numbers
    x = APyFloat(0, 1046, 1232143, 11, 60)
    y = APyFloat(1, 1046, 1232143, 11, 60)
    res = x + y
    assert res.is_identical(APyFloat(0, 0, 0, 11, 60))


# Subtraction
# Because subtraction is implemented as 'a+(-b)', the tests for addition will also cover subtraction,
# but we still do some tests using the subtraction operator to make sure it's not broken.
@pytest.mark.float_sub
@pytest.mark.parametrize(
    "lhs,rhs",
    list(perm(["APyFloat.from_float(2.75, 6, 5)", "APyFloat.from_float(2.5, 5, 7)"])),
)
def test_sub_same_exp(lhs, rhs):
    # Subtract two numbers that have the same exponent
    expr = None
    assert float(eval(expr := f"{lhs} - {rhs}")) == (
        0.25 if float(eval(lhs)) == 2.75 else -0.25
    )
    res = eval(expr)
    assert res.exp_bits == 6
    assert res.man_bits == 7


@pytest.mark.float_sub
@pytest.mark.parametrize(
    "lhs,rhs",
    list(perm(["APyFloat.from_float(4, 9, 5)", "APyFloat.from_float(16, 5, 12)"])),
)
def test_sub_diff_exp(lhs, rhs):
    # Subtract two numbers that have different exponent
    expr = None
    assert float(eval(expr := f"{lhs} - {rhs}")) == (
        -12 if float(eval(lhs)) == 4 else 12
    )
    res = eval(expr)
    assert res.exp_bits == 9
    assert res.man_bits == 12


@pytest.mark.float_sub
def test_sub_diff_sign():
    # Subtract two numbers that have different sign
    # 12 - -4
    res = APyFloat(0, 258, 8, 9, 4) - APyFloat(1, 17, 0, 5, 14)
    assert res.is_identical(APyFloat(0, 259, 0, 9, 14))

    # -4 - 12
    res = APyFloat(1, 17, 0, 5, 14) - APyFloat(0, 258, 8, 9, 4)
    assert res.is_identical(APyFloat(1, 259, 0, 9, 14))


@pytest.mark.float_sub
def test_sub_res_zero():
    # 12 - 12, same format
    res = APyFloat(0, 258, 8, 9, 4) - APyFloat(0, 258, 8, 9, 4)
    assert res.is_identical(APyFloat(0, 0, 0, 9, 4))

    # 12 - 12, different format
    res = APyFloat(0, 258, 4, 9, 3) - APyFloat(0, 258, 8, 9, 4)
    assert res.is_identical(APyFloat(0, 0, 0, 9, 4))


@pytest.mark.float_sub
def test_sub_overflow():
    """Test that a subtraction can overflow."""
    assert (APyFloat(1, 0b11110, 1, 5, 2) - APyFloat(0, 0b11110, 3, 5, 3)).is_inf


@pytest.mark.float_add
def test_sub_subnormals():
    # Subtract two subnormal numbers
    res = APyFloat(0, 0, 1, 5, 3) - APyFloat(0, 0, 2, 5, 3)
    assert res.is_identical(APyFloat(1, 0, 1, 5, 3))

    # Subtraction of subnormal numbers resulting in normal number
    res = APyFloat(1, 0, 7, 5, 3) - APyFloat(0, 0, 1, 5, 3)
    assert res.is_identical(APyFloat(1, 1, 0, 5, 3))

    # Subtract subnormal from normal number
    res = APyFloat(0, 1, 0, 4, 3) - APyFloat(0, 0, 1, 4, 3)
    assert res.is_identical(APyFloat(0, 0, 7, 4, 3))

    # Subtract subnormal from big normal number
    res = APyFloat(0, 7, 0, 4, 3) - APyFloat(0, 0, 1, 4, 3)
    assert res.is_identical(APyFloat(0, 7, 0, 4, 3))


@pytest.mark.float_sub
@pytest.mark.parametrize("num", [APyFloat(0, 17, 123, 5, 7), APyFloat(0, 0, 123, 5, 7)])
def test_sub_zero(num):
    assert (num - APyFloat(0, 0, 0, 5, 7)).is_identical(num)
    assert (APyFloat(0, 0, 0, 5, 7) - num).is_identical(-num)


@pytest.mark.float_sub
@pytest.mark.parametrize(
    "num",
    [
        APyFloat(0, 17, 123, 5, 7),
        APyFloat(0, 0, 123, 5, 7),
        APyFloat(1, 17, 123, 5, 7),
        APyFloat(1, 0, 123, 5, 7),
        APyFloat(0, 7, 23, 4, 6),
        APyFloat(0, 0, 23, 4, 6),
    ],
)
def test_sub_inf(num):
    assert (num - APyFloat(0, 0x1F, 0, 5, 7)).is_identical(APyFloat(1, 0x1F, 0, 5, 7))
    assert (APyFloat(0, 0x1F, 0, 5, 7) - num).is_identical(APyFloat(0, 0x1F, 0, 5, 7))


@pytest.mark.float_sub
@pytest.mark.parametrize("num", [APyFloat(0, 17, 123, 5, 7), APyFloat(0, 0, 123, 5, 7)])
def test_sub_nan(num):
    assert (num - APyFloat(0, 0x1F, 1, 5, 7)).is_nan
    assert (APyFloat(0, 0x1F, 1, 5, 7) - num).is_nan


@pytest.mark.float_sub
def test_sub_inf_nan():
    # Infinity with NaN
    assert (APyFloat(0, 0x1F, 0, 5, 7) - APyFloat(0, 0x1F, 1, 5, 7)).is_nan
    assert (APyFloat(0, 0x1F, 1, 5, 7) - APyFloat(0, 0x1F, 0, 5, 7)).is_nan
    assert (
        APyFloat(0, 0x1F, 1, 5, 8) - APyFloat(0, 0x1F, 0, 5, 7)
    ).is_nan  # Different format

    # Infinity with infinity
    assert (APyFloat(0, 0x1F, 0, 5, 7) - APyFloat(0, 0x1F, 0, 5, 7)).is_nan
    assert (
        APyFloat(0, 0x1F, 0, 5, 5) - APyFloat(0, 0x1F, 0, 5, 7)
    ).is_nan  # Different format

    # NaN with NaN
    assert (APyFloat(0, 0x1F, 1, 5, 7) - APyFloat(0, 0x1F, 1, 5, 7)).is_nan
    assert (
        APyFloat(0, 0x1F, 1, 5, 8) - APyFloat(0, 0x1F, 1, 5, 7)
    ).is_nan  # Different format


# Multiplication
@pytest.mark.float_mul
@pytest.mark.parametrize("exp", list(perm(["5", "6", "7", "8"], 2)))
@pytest.mark.parametrize("man", list(perm(["5", "6", "7", "8"], 2)))
@pytest.mark.parametrize("sign", list(set(perm(["-", "-", "", ""], 2))))
@pytest.mark.parametrize("lhs,rhs", list(perm(["2.75", "2.5", "0"], 2)))
def test_mul_mixed(exp, man, sign, lhs, rhs):
    """Test multiplications where the operands and the result is exactly representable by the formats."""
    expr = None
    assert float(
        eval(
            expr
            := f"APyFloat.from_float({sign[0]}{lhs}, {exp[0]}, {man[0]}) * APyFloat.from_float({sign[1]}{rhs}, {exp[1]}, {man[1]})"
        )
    ) == (float(eval(f"{sign[0]}{lhs} * {sign[1]}{rhs}")))
    res = eval(expr)
    assert res.exp_bits == max(int(exp[0]), int(exp[1]))
    assert res.man_bits == max(int(man[0]), int(man[1]))


@pytest.mark.float_mul
def test_mul_overflow():
    """Test that a multiplication can overflow to infinity."""
    res = APyFloat(0, 0b11110, 1, 5, 2) * APyFloat(0, 0b11110, 3, 5, 3)
    assert res.is_inf


@pytest.mark.float_mul
def test_mul_underflow():
    """Test that a multiplication can underflow to zero."""
    res = APyFloat(0, 1, 1, 5, 2) * APyFloat(0, 1, 3, 5, 3)
    assert res == 0

    res = APyFloat(0, 0, 31, 4, 5) * APyFloat(0, 0, 5, 4, 5)
    assert res == 0

    res = APyFloat(0, 0, 31, 4, 5) * APyFloat(0, 1, 1, 4, 5)
    assert res == 0

    # From GH#406
    res = APyFloat(0, 53, 32895, 8, 23) * APyFloat(1, 1, 1999049, 8, 23)
    assert res == 0


@pytest.mark.float_mul
def test_mul_with_one():
    """Test multiplication with one."""
    a = APyFloat(0, 7, 0, 4, 4)  # One
    for exp in reversed(range(15)):  # Skip nan/inf
        for man in reversed(range(16)):
            b = APyFloat(0, exp, man, 4, 4)
            res = a * b
            assert res.is_identical(b)
            res = b * a
            assert res.is_identical(b)


@pytest.mark.float_mul
def test_long_mul_with_one():
    """Test long multiplication with one."""
    a = APyFloat.from_float(1.0, 10, 50)  # One
    for exp in reversed(range(1023)):  # Skip nan/inf
        for man in reversed(range(4)):  # Try subset
            b = APyFloat(0, exp, man, 10, 50)
            res = a * b
            assert res.is_identical(b)
            res = b * a
            assert res.is_identical(b)


@pytest.mark.float_mul
def test_mul_mixed_bias():
    # Test that the implementation doesn't do "x.cast() * y.cast()"
    x = APyFloat(sign=0, exp=30, man=0, exp_bits=5, man_bits=2, bias=14)
    y = APyFloat(sign=0, exp=15, man=0, exp_bits=5, man_bits=2, bias=16)
    assert (_ := x * y).is_identical(
        APyFloat(sign=0, exp=30, man=0, exp_bits=5, man_bits=2, bias=15)
    )

    # Multiply two one's but with different formats
    x = APyFloat(sign=0, exp=14, man=0, exp_bits=5, man_bits=2, bias=14)
    y = APyFloat(sign=0, exp=21, man=0, exp_bits=5, man_bits=2, bias=21)
    assert (_ := x * y).is_identical(
        APyFloat(sign=0, exp=17, man=0, exp_bits=5, man_bits=2, bias=17)
    )


@pytest.mark.float_mul
def test_mul_mixed_bias_overflow():
    """Test that a result can overflow to infinity due to a change in bias."""
    x = APyFloat(sign=0, exp=21, man=0, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=25, man=0, exp_bits=5, man_bits=2, bias=25)

    # Multiply with one
    assert (_ := x * y).is_identical(
        APyFloat(sign=0, exp=31, man=0, exp_bits=5, man_bits=2, bias=15)
    )

    # Multiply with one but with larger bias difference
    y = APyFloat(sign=0, exp=27, man=0, exp_bits=5, man_bits=2, bias=27)
    assert (_ := x * y).is_identical(
        APyFloat(sign=0, exp=31, man=0, exp_bits=5, man_bits=2, bias=16)
    )

    # Should become one
    x = APyFloat(sign=0, exp=30, man=0, exp_bits=5, man_bits=2, bias=3)
    y = APyFloat(sign=0, exp=1, man=0, exp_bits=5, man_bits=2, bias=28)
    assert (_ := x * y).is_identical(
        APyFloat(sign=0, exp=15, man=0, exp_bits=5, man_bits=2)
    )


@pytest.mark.float_add
def test_add_mixed_bias_underflow():
    """Test that a result can become zero due to a change in bias."""
    x = APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=25)

    # Add with zero
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=15)
    )

    # Add with zero but with larger bias difference
    y = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=27)
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=16)
    )

    # Add with subnormal number
    y = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=28)
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=16)
    )


@pytest.mark.float_mul
@pytest.mark.parametrize("x", [0.0, 1.0, float("inf"), float("nan")])
@pytest.mark.parametrize("y", [0.0, 1.0, float("inf"), float("nan")])
@pytest.mark.parametrize("sign", [1, -1])
def test_mul_special_cases(x, y, sign):
    """Test the special cases for addition."""
    assert str(
        float(APyFloat.from_float(x, 4, 4) * APyFloat.from_float(sign * y, 4, 4))
    ) == str(x * sign * y)


@pytest.mark.float_mul
def test_mul_short_subnormal():
    x = APyFloat(sign=0, exp=0, man=11, exp_bits=4, man_bits=9)
    y = APyFloat(sign=0, exp=5, man=50, exp_bits=5, man_bits=10)
    res = x * y
    assert res.is_identical(APyFloat(sign=0, exp=0, man=6, exp_bits=5, man_bits=10))

    x = APyFloat(sign=0, exp=1, man=10, exp_bits=4, man_bits=10)
    y = APyFloat(sign=0, exp=1, man=20, exp_bits=4, man_bits=10)
    res = x * y
    assert res.is_identical(APyFloat(sign=0, exp=0, man=16, exp_bits=4, man_bits=10))

    x = APyFloat(sign=0, exp=1, man=10, exp_bits=5, man_bits=15)
    y = APyFloat(sign=0, exp=1, man=20, exp_bits=5, man_bits=15)
    res = x * y
    assert res.is_identical(APyFloat(sign=0, exp=0, man=2, exp_bits=5, man_bits=15))

    x = APyFloat(0, 3, 10, 4, 10)
    y = APyFloat(0, 4, 10, 4, 10)
    res = x * y
    assert res.is_identical(APyFloat(sign=0, exp=0, man=522, exp_bits=4, man_bits=10))

    # 2.0 x subn
    res = APyFloat(0, 8, 0, 4, 3) * APyFloat(0, 0, 1, 4, 3)
    assert res.is_identical(APyFloat(0, 0, 2, 4, 3))

    res = APyFloat(0, 8, 0, 4, 3) * APyFloat(0, 0, 2, 4, 3)
    assert res.is_identical(APyFloat(0, 0, 4, 4, 3))

    # 4.0 x subn
    res = APyFloat(0, 9, 0, 4, 3) * APyFloat(0, 0, 1, 4, 3)
    assert res.is_identical(APyFloat(0, 0, 4, 4, 3))

    # subnormal becoming normal
    res = APyFloat(0, 10, 0, 4, 3) * APyFloat(0, 0, 1, 4, 3)
    assert res.is_identical(APyFloat(0, 1, 0, 4, 3))

    # 0.5 x subn
    res = APyFloat(0, 6, 0, 4, 3) * APyFloat(0, 0, 4, 4, 3)
    assert res.is_identical(APyFloat(0, 0, 2, 4, 3))

    # 0.25 x subn
    res = APyFloat(0, 5, 0, 4, 3) * APyFloat(0, 0, 4, 4, 3)
    assert res.is_identical(APyFloat(0, 0, 1, 4, 3))

    # 0.125 x subn
    res = APyFloat(0, 4, 0, 4, 3) * APyFloat(0, 0, 4, 4, 3)
    assert res.is_identical(APyFloat(0, 0, 0, 4, 3))

    # Two normal numbers that generate carry, but with subnormal result
    a = APyFloat(sign=1, exp=2, man=957, exp_bits=5, man_bits=10)
    b = APyFloat(sign=0, exp=12, man=1015, exp_bits=5, man_bits=10)
    res = a * b
    assert res.is_identical(APyFloat(sign=1, exp=0, man=986, exp_bits=5, man_bits=10))


@pytest.mark.float_mul
def test_long_mul():
    x = APyFloat(sign=0, exp=3, man=22, exp_bits=4, man_bits=7)
    y = APyFloat(sign=1, exp=32763, man=813782734503116, exp_bits=16, man_bits=52)
    assert x * y == APyFloat.from_float(float(x) * float(y), exp_bits=16, man_bits=52)

    x = APyFloat.from_float(5200, exp_bits=17, man_bits=52)
    assert x * y == APyFloat.from_float(float(x) * float(y), exp_bits=17, man_bits=52)
    x = APyFloat(sign=0, exp=3, man=122, exp_bits=4, man_bits=7)
    res = x * y
    assert res.is_identical(
        APyFloat(sign=1, exp=32760, man=689156585396703, exp_bits=16, man_bits=52)
    )
    x = APyFloat(sign=0, exp=3, man=2**20 - 1, exp_bits=4, man_bits=20)
    y = APyFloat(sign=0, exp=32760, man=2**50 - 1, exp_bits=16, man_bits=50)
    res = x * y
    assert res.is_identical(
        APyFloat(sign=0, exp=32757, man=1125898833100799, exp_bits=16, man_bits=50)
    )
    x = APyFloat(sign=0, exp=0, man=11, exp_bits=4, man_bits=9)
    y = APyFloat(sign=0, exp=5, man=50, exp_bits=16, man_bits=50)
    res = x * y
    assert res.is_identical(
        APyFloat(sign=0, exp=0, man=6047313952768, exp_bits=16, man_bits=50)
    )
    x = APyFloat(sign=0, exp=0, man=11, exp_bits=4, man_bits=20)
    y = APyFloat(sign=0, exp=5, man=50, exp_bits=16, man_bits=50)
    res = x * y
    assert res.is_identical(
        APyFloat(sign=0, exp=0, man=2952790016, exp_bits=16, man_bits=50)
    )

    x = APyFloat(sign=0, exp=1020, man=11, exp_bits=10, man_bits=9)
    y = APyFloat(sign=0, exp=4090, man=50, exp_bits=12, man_bits=50)
    res = x * y
    assert res.is_identical(APyFloat(sign=0, exp=4095, man=0, exp_bits=12, man_bits=50))
    x = APyFloat(sign=0, exp=1, man=10, exp_bits=4, man_bits=40)
    y = APyFloat(sign=0, exp=1, man=20, exp_bits=4, man_bits=40)
    res = x * y
    assert res.is_identical(
        APyFloat(sign=0, exp=0, man=17179869184, exp_bits=4, man_bits=40)
    )

    # Test that quantization can generate carry
    # The mantissa product will be very close to 2 and thus round up
    x = APyFloat(sign=0, exp=1023, man=4503599627370494, exp_bits=11, man_bits=52)
    y = APyFloat(sign=0, exp=1023, man=1, exp_bits=11, man_bits=52)
    res = x * y
    assert res.is_identical(APyFloat(sign=0, exp=1024, man=0, exp_bits=11, man_bits=52))

    # Check that multiplication can overflow
    x = APyFloat(sign=0, exp=2000, man=4503599627370494, exp_bits=11, man_bits=60)
    y = APyFloat(sign=0, exp=2000, man=66454545455555, exp_bits=11, man_bits=60)
    res = x * y
    assert res.is_identical(APyFloat(sign=0, exp=2047, man=0, exp_bits=11, man_bits=60))

    # Subnormal becoming normal after quantization
    x = APyFloat(
        sign=0, exp=0, man=4503599627370495, exp_bits=11, man_bits=52
    )  # (2.22507e-308)
    y = APyFloat(
        sign=0, exp=1023, man=1, exp_bits=11, man_bits=52
    )  # Slightly larger than 1
    res = x * y
    assert res.is_identical(APyFloat(sign=0, exp=1, man=0, exp_bits=11, man_bits=52))


@pytest.mark.float_div
@pytest.mark.parametrize("exp", list(perm(["5", "8"], 2)))
@pytest.mark.parametrize("man", list(perm(["5", "8"], 2)))
@pytest.mark.parametrize("sign", list(set(perm(["-", "-", "", ""], 2))))
@pytest.mark.parametrize("lhs,rhs", list(perm(["4.5", "1.125"], 2)))
def test_div_mixed_formats(exp, man, sign, lhs, rhs):
    """Test multiplications where the operands and the result is exactly representable by the formats."""
    expr = None
    assert float(
        eval(
            expr
            := f"APyFloat.from_float({sign[0]}{lhs}, {exp[0]}, {man[0]}) / APyFloat.from_float({sign[1]}{rhs}, {exp[1]}, {man[1]})"
        )
    ) == (float(eval(f"{sign[0]}{lhs} / {sign[1]}{rhs}")))
    res = eval(expr)
    assert res.exp_bits == max(int(exp[0]), int(exp[1]))
    assert res.man_bits == max(int(man[0]), int(man[1]))


@pytest.mark.float_div
def test_div_overflow():
    """Test that a division can overflow to infinity."""
    assert (APyFloat(0, 30, 1, 5, 2) / APyFloat(0, 1, 3, 5, 2)).is_inf

    assert (APyFloat(0, 30, 1, 5, 2) / APyFloat(0, 0, 3, 5, 2)).is_inf


@pytest.mark.float_div
def test_div_underflow():
    """Test that a division can underflow to zero."""
    assert (APyFloat(0, 1, 3, 5, 2) / APyFloat(1, 30, 3, 5, 2)) == 0

    assert (APyFloat(0, 0, 3, 5, 2) / APyFloat(1, 30, 3, 5, 2)) == 0


@pytest.mark.float_div
@pytest.mark.parametrize("x", [0, 1, float("inf"), float("nan")])
@pytest.mark.parametrize("y", [0, 1, float("inf"), float("nan")])
def test_div_special_cases(x, y):
    """Test the special cases for division."""
    try:
        assert (
            s := str(float(APyFloat.from_float(x, 4, 4) / APyFloat.from_float(y, 4, 4)))
        ) == str(x / y)
    except ZeroDivisionError:
        if x == 1 or x == float("inf"):
            assert s == "inf"
        else:
            assert s == "nan"


@pytest.mark.float_div
def test_div_with_one():
    """Test division with one."""
    a = APyFloat(0, 7, 0, 4, 4)  # One
    for exp in reversed(range(15)):  # Skip nan/inf
        for man in reversed(range(16)):
            b = APyFloat(0, exp, man, 4, 4)
            res = b / a
            assert res.is_identical(b)


@pytest.mark.float_mul
def test_long_div_with_one():
    """Test long division with one."""
    a = APyFloat.from_float(1.0, 10, 50)  # One
    for exp in reversed(range(1023)):  # Skip nan/inf
        for man in reversed(range(4)):  # Try subset
            b = APyFloat(0, exp, man, 10, 50)
            res = b / a
            assert res.is_identical(b)


@pytest.mark.float_div
def test_subnormal_div():
    # Subnormal divided by 1
    # Related to https://github.com/apytypes/apytypes/issues/235
    x = APyFloat(sign=0, exp=0, man=1, exp_bits=4, man_bits=7)
    y = APyFloat(sign=0, exp=7, man=0, exp_bits=4, man_bits=7)
    res = x / y
    assert res.is_identical(x)

    x = APyFloat(sign=0, exp=0, man=1, exp_bits=11, man_bits=60)
    res = x / y
    assert res.is_identical(x)

    # More test cases to be added


@pytest.mark.float_div
def test_subnormal_quant_div():
    """Subnormal becoming normal after quantization."""
    res = APyFloat(sign=0, exp=1, man=1023, exp_bits=5, man_bits=10) / APyFloat(
        sign=0, exp=16, man=0, exp_bits=5, man_bits=10
    )
    assert res.is_identical(APyFloat(sign=0, exp=1, man=0, exp_bits=5, man_bits=10))

    # -2 / -8.98847e+307
    res = APyFloat(
        sign=1, exp=1023, man=4503599627370495, exp_bits=11, man_bits=52
    ) / APyFloat(sign=1, exp=2046, man=0, exp_bits=11, man_bits=52)
    assert res.is_identical(APyFloat(sign=0, exp=1, man=0, exp_bits=11, man_bits=52))


@pytest.mark.float_div
def test_long_div():
    # Division between two long formats
    x = APyFloat(sign=0, exp=1036, man=1123081534184704, exp_bits=11, man_bits=52)
    y = APyFloat(sign=1, exp=1031, man=178161345727614, exp_bits=11, man_bits=52)
    res = x / y
    assert res.is_identical(
        APyFloat(sign=1, exp=1028, man=908961869920955, exp_bits=11, man_bits=52)
    )

    # Test that division can underflow
    x = APyFloat(sign=0, exp=1, man=1123081534184704, exp_bits=11, man_bits=52)
    y = APyFloat(sign=1, exp=1078, man=178161345727614, exp_bits=11, man_bits=52)
    res = x / y
    assert res.is_identical(APyFloat(sign=1, exp=0, man=0, exp_bits=11, man_bits=52))

    # Test that division can overflow
    x = APyFloat(sign=0, exp=1, man=1123081534184704, exp_bits=11, man_bits=52)
    y = APyFloat(sign=1, exp=1078, man=178161345727614, exp_bits=11, man_bits=52)
    res = y / x
    assert res.is_identical(APyFloat(sign=1, exp=2047, man=0, exp_bits=11, man_bits=52))

    # Test division with mixed formats
    x = APyFloat(sign=0, exp=7, man=2, exp_bits=4, man_bits=3)
    y = APyFloat(sign=1, exp=1025, man=1970324836974592, exp_bits=11, man_bits=52)
    res = x / y
    assert res.is_identical(
        APyFloat(sign=1, exp=1020, man=3328747550665149, exp_bits=11, man_bits=52)
    )

    # Test division with 1
    x = APyFloat(sign=0, exp=0, man=1, exp_bits=14, man_bits=59)
    y = APyFloat(sign=0, exp=1023, man=0, exp_bits=11, man_bits=59)
    res = x / y
    assert res.is_identical(x)

    # Quantization should round x.man to 2 with division with 2
    x = APyFloat(sign=0, exp=0, man=3, exp_bits=14, man_bits=59)
    y = APyFloat(sign=0, exp=1024, man=0, exp_bits=11, man_bits=59)
    res = x / y
    assert res.is_identical(APyFloat(sign=0, exp=0, man=2, exp_bits=14, man_bits=59))


@pytest.mark.float_div
def test_div_mixed_bias():
    # Test that the implementation doesn't do "x.cast() / y.cast()"
    x = APyFloat(sign=0, exp=30, man=0, exp_bits=5, man_bits=2, bias=4)
    y = APyFloat(sign=0, exp=30, man=0, exp_bits=5, man_bits=2, bias=8)
    assert (_ := x / y).is_identical(
        APyFloat(sign=0, exp=10, man=0, exp_bits=5, man_bits=2, bias=6)
    )


@pytest.mark.float_mul
def test_div_mixed_bias_overflow():
    """Test that a result can overflow to infinity due to a change in bias."""
    x = APyFloat(sign=0, exp=21, man=0, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=25, man=0, exp_bits=5, man_bits=2, bias=25)

    # Divide with one
    assert (_ := x / y).is_identical(
        APyFloat(sign=0, exp=31, man=0, exp_bits=5, man_bits=2, bias=15)
    )

    # Divide with one but with larger bias difference
    y = APyFloat(sign=0, exp=27, man=0, exp_bits=5, man_bits=2, bias=27)
    assert (_ := x / y).is_identical(
        APyFloat(sign=0, exp=31, man=0, exp_bits=5, man_bits=2, bias=16)
    )


@pytest.mark.float_add
def test_div_mixed_bias_underflow():
    """Test that a result can become zero due to a change in bias."""
    x = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=25)
    y = APyFloat(sign=0, exp=5, man=0, exp_bits=5, man_bits=2, bias=5)

    # Divide with one
    assert (_ := x / y).is_identical(
        APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=15)
    )

    # Divide with one but with larger bias difference
    x = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=30)
    assert (_ := x / y).is_identical(
        APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=17)
    )


# Power
@pytest.mark.float_pow
def test_issue_253():
    # Smoke test for carry not being handled correctly in fast path
    # https://github.com/apytypes/apytypes/issues/253
    a = APyFloat(0, 510, 2**16 - 1, 10, 16)
    res = a**2
    ref = a * a
    assert res.is_identical(ref)

    # Test a larger normalization
    a = APyFloat(0, 15, 3, 5, 2)
    res = a**6
    assert res.is_identical(APyFloat(sign=0, exp=19, man=3, exp_bits=5, man_bits=2))


def test_not_implemented_yet_power_apyfloat():
    with pytest.raises(ValueError, match="Not implemented"):
        APyFloat(0, 5, 4, 4, 7) ** APyFloat(0, 5, 4, 4, 7)


@pytest.mark.float_pow
def test_power():
    """General tests of the power function."""
    assert APyFloat.from_float(4.5, 8, 10) ** 2 == APyFloat.from_float(4.5**2, 8, 10)
    assert APyFloat.from_float(-4.5, 8, 10) ** 3 == APyFloat.from_float(
        (-4.5) ** 3, 8, 10
    )
    assert APyFloat.from_float(-8.125, 8, 10) ** 4 == APyFloat.from_float(
        (-8.125) ** 4, 8, 10
    )
    # 1/sqrt(2)
    a = APyFloat(sign=0, exp=510, man=27146, exp_bits=10, man_bits=16)
    res = a**2
    assert res.is_identical(APyFloat(sign=0, exp=510, man=0, exp_bits=10, man_bits=16))
    res = a**4
    assert res.is_identical(APyFloat(sign=0, exp=509, man=0, exp_bits=10, man_bits=16))

    # sqrt(2)
    a = APyFloat(sign=0, exp=511, man=1865452045155277, exp_bits=10, man_bits=52)
    res = a**2
    assert res.is_identical(APyFloat(sign=0, exp=512, man=1, exp_bits=10, man_bits=52))

    # Cube root of three
    a = APyFloat(sign=0, exp=511, man=1116352409, exp_bits=10, man_bits=32)
    res = a**3
    assert res.is_identical(APyFloat(sign=0, exp=512, man=0, exp_bits=10, man_bits=32))

    # Test identity
    a = APyFloat(sign=0, exp=511, man=1116352409, exp_bits=10, man_bits=32)
    res = a**1
    assert res.is_identical(a)


@pytest.mark.float_pow
def test_power_subnormal_res():
    """Test that the result can become a subnormal number."""
    a = APyFloat(0, 7, 0, 5, 2)  # 2**-8
    res = a**2
    assert res.is_identical(APyFloat(0, 0, 1, 5, 2))

    # Same test but with many mantissa bits in relation to exponent bits
    a = APyFloat(0, 7, 0, 5, 20)  # 2**-8
    res = a**2
    assert res.is_identical(APyFloat(0, 0, 1 << 18, 5, 20))

    # Similar test but with long format
    a = APyFloat(0, 486, 0, 11, 52)
    res = a**2
    assert res.is_identical(APyFloat(0, 0, 1, 11, 52))

    a = APyFloat(0, 488, 0, 11, 52)
    res = a**2
    assert res.is_identical(APyFloat(0, 0, 16, 11, 52))


@pytest.mark.xfail()
@pytest.mark.float_pow
def test_negative_power():
    """Test the power function with negative exponent."""
    assert APyFloat.from_float(4.5, 8, 10) ** -2 == APyFloat.from_float(4.5**-2, 8, 10)
    assert APyFloat.from_float(-4.5, 8, 10) ** -3 == APyFloat.from_float(
        (-4.5) ** -3, 8, 10
    )
    assert APyFloat.from_float(-8.125, 8, 10) ** -4 == APyFloat.from_float(
        (-8.125) ** -4, 8, 10
    )


@pytest.mark.float_pow
def test_long_power():
    """Test the power function with long format."""
    res = APyFloat.from_float(4.5, 11, 52) ** 2
    assert res.is_identical(APyFloat.from_float(4.5**2, 11, 52))

    res = APyFloat.from_float(-4.5, 11, 52) ** 3
    assert res.is_identical(APyFloat.from_float((-4.5) ** 3, 11, 52))

    res = APyFloat.from_float(-8.125, 11, 52) ** 4
    assert res.is_identical(APyFloat.from_float((-8.125) ** 4, 11, 52))

    # Test carry
    res = APyFloat.from_float(1.75, 11, 52) ** 3
    assert res.is_identical(APyFloat.from_float(1.75**3, 11, 52))

    # Test carry from quantization
    res = (
        APyFloat(
            sign=0, exp=1023, man=(932726022577638 << 2) | 1, exp_bits=11, man_bits=53
        )
        ** 2
    )
    assert res.is_identical(APyFloat(0, 1024, 0, 11, 53))

    # Test overflow
    # Related to https://github.com/apytypes/apytypes/issues/288
    res = APyFloat(0, 30, 1, 5, 60) ** 2
    assert res.is_identical(APyFloat(0, 31, 0, 5, 60))
    res = APyFloat(0, 30, 1, 5, 60) ** 1000
    assert res.is_identical(APyFloat(0, 31, 0, 5, 60))

    # Test underflow
    res = APyFloat(1, 0, 1, 5, 60) ** 1000
    assert res.is_identical(APyFloat(0, 0, 0, 5, 60))

    # Test identity
    a = APyFloat(sign=0, exp=511, man=1116352409, exp_bits=20, man_bits=61)
    res = a**1
    assert res.is_identical(a)


@pytest.mark.xfail()
@pytest.mark.float_pow
def test_negative_long_power():
    """Test the power function with long format and negative exponent."""
    assert APyFloat.from_float(4.5, 11, 52) ** -2 == APyFloat.from_float(
        4.5**-2, 11, 52
    )
    assert APyFloat.from_float(-4.5, 11, 52) ** -3 == APyFloat.from_float(
        (-4.5) ** -3, 11, 52
    )
    assert APyFloat.from_float(-8.125, 11, 52) ** -4 == APyFloat.from_float(
        (-8.125) ** -4, 11, 52
    )


@pytest.mark.float_pow
def test_power_overflow():
    """Test that the power function can overflow to infinity."""
    res = APyFloat(0, 30, 1, 5, 4) ** 2
    assert res.is_identical(APyFloat(0, 31, 0, 5, 4))
    # Related to https://github.com/apytypes/apytypes/issues/288
    res = APyFloat(1, 30, 1, 5, 4) ** 1001
    assert res.is_identical(APyFloat(1, 31, 0, 5, 4))


@pytest.mark.float_pow
def test_power_underflow():
    """Test that the power function can underflow to zero."""
    res = APyFloat(1, 1, 1, 5, 2) ** 3
    assert res.is_identical(APyFloat(1, 0, 0, 5, 2))
    res = APyFloat(0, 0, 1, 5, 2) ** 2
    assert res.is_identical(APyFloat(0, 0, 0, 5, 2))


@pytest.mark.float_pow
@pytest.mark.parametrize(
    "x,n,test_exp",
    [
        # 1 if x is not a signaling NaN
        ("'inf'", "0", 1.0),
        ("'-inf'", "0", 1.0),
        ("0.0", "0", 1.0),
        ("-0.0", "0", 1.0),
        ("'nan'", "0", float("nan")),
        # pown (+-0, n) is +-inf for odd n < 0
        ("0.0", "-1", float("inf")),
        ("-0.0", "-3", float("-inf")),
        # pown (+-0, n) is +inf for even n < 0
        ("0.0", "-2", float("inf")),
        ("-0.0", "-4", float("inf")),
        # pown (+-0, n) is +0 for even n > 0
        ("0.0", "2", 0.0),
        ("-0.0", "4", 0.0),
        # pown (+-0, n) is +-0 for odd n > 0
        ("0.0", "3", 0.0),
        ("-0.0", "1", -0.0),
        # pown (+inf, n) is +inf for n > 0
        ("'inf'", "3", float("inf")),
        # pown (-inf, n) is -inf for odd n > 0
        ("'-inf'", "3", float("-inf")),
        # pown (-inf, n) is +inf for even n > 0
        ("'-inf'", "2", float("inf")),
        # pown (+inf, n) is +0 for n < 0
        ("'inf'", "-2", 0.0),
        # pown (-inf, n) is -0 for odd n < 0
        ("'-inf'", "-3", -0.0),
        # pown (-inf, n) is +0 for even n < 0
        ("'-inf'", "-4", 0.0),
    ],
)
def test_power_special_cases(x, n, test_exp):
    """Test the special cases for the power function."""
    if n[0] == "-":
        pytest.skip(
            "Skip tests with negative powers as the function currently throws, see https://github.com/apytypes/apytypes/issues/191."
        )

    if str(test_exp) == "nan":
        assert eval(f"(APyFloat.from_float(float({x}), 9, 7)**{n}).is_nan")
    else:
        assert eval(f"float(APyFloat.from_float(float({x}), 9, 7)**{n})") == test_exp


@pytest.mark.float_add
def test_sum():
    s = [APyFloat.from_float(0.3, 2, 5), APyFloat.from_float(0.7, 4, 7)]
    assert sum(s).is_identical(s[0] + s[1])


@pytest.mark.float_mul
def test_prod():
    s = [APyFloat.from_float(0.3, 2, 5), APyFloat.from_float(0.7, 4, 7)]
    assert math.prod(s).is_identical(s[0] * s[1])


def test_binary_logic():
    a = APyFloat(1, 3, 7, 3, 4)
    b = APyFloat(1, 5, 9, 3, 4)
    c = APyFloat(0, 7, 19, 4, 5)
    d = APyFloat.from_float(float("nan"), 5, 6)

    assert (a & b).is_identical(APyFloat(1, 1, 1, 3, 4))
    assert (a | b).is_identical(APyFloat(1, 7, 15, 3, 4))
    assert (a ^ b).is_identical(APyFloat(0, 6, 14, 3, 4))
    assert (~a).is_identical(APyFloat(0, 4, 8, 3, 4))
    assert (a & c).is_identical(APyFloat(0, 7, 2, 4, 5))
    assert (a | c).is_identical(APyFloat(1, 7, 31, 4, 5))
    assert (a ^ c).is_identical(APyFloat(1, 0, 29, 4, 5))
    assert (a & d).is_identical(APyFloat(0, 15, 0, 5, 6))
    assert (a | d).is_identical(APyFloat(1, 31, 29, 5, 6))
    assert (a ^ d).is_identical(APyFloat(1, 16, 29, 5, 6))
    assert (~d).is_identical(APyFloat(1, 0, 62, 5, 6))


def test_operation_with_numbers():
    a = APyFloat(0, 15, 2, 5, 2)  # 1.75

    # Integer identities
    assert (a + 0).is_identical(a)
    assert (0 + a).is_identical(a)
    assert (a - 0).is_identical(a)
    assert (0 - a).is_identical(-a)
    assert (a * 1).is_identical(a)
    assert (1 * a).is_identical(a)
    assert (a / 1).is_identical(a)

    # Operations with floats
    q_eight = 9  # Should quantize to eight
    eight = APyFloat(0, 18, 0, 5, 2)
    assert (_ := a + q_eight).is_identical(a + eight)
    assert (_ := q_eight + a).is_identical(eight + a)
    assert (_ := a - q_eight).is_identical(a - eight)
    assert (_ := q_eight - a).is_identical(eight - a)
    assert (_ := a * q_eight).is_identical(a * eight)
    assert (_ := q_eight * a).is_identical(eight * a)
    assert (_ := a / q_eight).is_identical(a / eight)
    assert (_ := q_eight / a).is_identical(eight / a)

    # Operations with floats
    q_one = 1.125  # Should quantize to one
    q_zero = 0.125  # Should quantize to zero
    assert (_ := a + q_zero).is_identical(a)
    assert (_ := q_zero + a).is_identical(a)
    assert (_ := a - q_zero).is_identical(a)
    assert (_ := q_zero - a).is_identical(-a)
    assert (_ := a * q_one).is_identical(a)
    assert (_ := q_one * a).is_identical(a)
    assert (_ := a / q_one).is_identical(a)
    assert (_ := q_one / a).is_identical(APyFloat(0, 14, 1, 5, 2))


def test_mantissa_full_range():
    """
    Support for utilizing the full mantissa range during addition/subraction was added
    in #440
    """
    a = APyFloat(sign=0, exp=1023, man=0, exp_bits=11, man_bits=61)
    b = APyFloat(sign=0, exp=1023 - 61, man=0, exp_bits=11, man_bits=61)
    assert (a + b) == APyFloat(sign=0, exp=1023, man=1, exp_bits=11, man_bits=61)
    assert (a - b) == APyFloat(
        sign=0, exp=1022, man=(1 << 61) - 2, exp_bits=11, man_bits=61
    )
