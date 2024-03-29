from itertools import permutations as perm
import math
import pytest
from apytypes import APyFloat


# Exhaustive tests, long running
# These should be replaced by testing against a third-party tool
# @pytest.mark.slow
# @pytest.mark.float_add
# @pytest.mark.parametrize("x_bits", range(0, 2**8))
# @pytest.mark.parametrize("y_bits", range(0, 2**8))
# def test_add_all(x_bits, y_bits):
#    y = APyFloat.from_bits(y_bits, 4, 3)
#    x = APyFloat.from_bits(x_bits, 4, 3)
#    ans = x + y
#    ref = APyFloat.from_float(float(x) + float(y), 4, 3)
#    print(f'{float(x)} + {float(y)} = {float(ref)} != {float(ans)}')
#    print(f'{x!r} + {y!r} = {ref!r} != {ans!r}')
#    assert ans == ref or (ans.is_nan and ref.is_nan) or (ans == 0 and ref == 0)


# @pytest.mark.slow
# @pytest.mark.float_sub
# @pytest.mark.parametrize("x_bits", range(0, 2**8))
# @pytest.mark.parametrize("y_bits", range(0, 2**8))
# def test_sub_all(x_bits, y_bits):
#    y = APyFloat.from_bits(y_bits, 4, 3)
#    x = APyFloat.from_bits(x_bits, 4, 3)
#    ans = x - y
#    ref = APyFloat.from_float(float(x) - float(y), 4, 3)
#    print(f'{float(x)} - {float(y)} = {float(ref)} != {float(ans)}')
#    print(f'{x!r} - {y!r} = {ref!r} != {ans!r}')
#    assert ans == ref or (ans.is_nan and ref.is_nan) or (ans == 0 and ref == 0)


# @pytest.mark.slow
# @pytest.mark.float_mul
# @pytest.mark.parametrize("x_bits", range(0, 2**8))
# @pytest.mark.parametrize("y_bits", range(0, 2**8))
# def test_mul_all(x_bits, y_bits):
#    y = APyFloat.from_bits(y_bits, 4, 3)
#    x = APyFloat.from_bits(x_bits, 4, 3)
#    ans = x * y
#    prod = float(x) * float(y)
#    ref = APyFloat.from_float(prod, 4, 3)
#    print(f'{float(x)} * {float(y)} = {float(ref)}({prod}) != {float(ans)}')
#    print(f'{x!r} * {y!r} = {ref!r} != {ans!r}')
#    assert ans.is_identical(ref) or (ans.is_nan and ref.is_nan) or (ans == 0 and ref == 0)


# @pytest.mark.slow
# @pytest.mark.float_div
# @pytest.mark.parametrize("x_bits", range(0, 2 * 8))
# @pytest.mark.parametrize("y_bits", range(0, 2**8))
# def test_div_all(x_bits, y_bits):
#     y = APyFloat.from_bits(y_bits, 4, 3)
#     if y != 0:
#         x = APyFloat.from_bits(x_bits, 4, 3)
#         ans = x / y
#         ref = APyFloat.from_float(float(x) / float(y), 4, 3)
#         print(f"{float(x)} / {float(y)} = {float(ref)} != {float(ans)}")
#         print(f"{x!r} / {y!r} = (ref){ref!r} != (ans){ans!r}")
#         assert (
#             ans.is_identical(ref)
#             or (ans.is_nan and ref.is_nan)
#             or (ans == 0 and ref == 0)
#         )


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
            expr := f"APyFloat.from_float({sign[0]}{lhs}, {exp[0]}, {man[0]}) + APyFloat.from_float({sign[1]}{rhs}, {exp[1]}, {man[1]})"
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
@pytest.mark.parametrize("num", [APyFloat(0, 17, 123, 5, 7), APyFloat(0, 0, 123, 5, 7)])
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
@pytest.mark.parametrize(
    "lhs,rhs",
    list(perm(["APyFloat.from_float(12, 9, 4)", "APyFloat.from_float(-4, 5, 14)"])),
)
def test_sub_diff_sign(lhs, rhs):
    # Subtract two numbers that have different sign
    expr = None
    assert float(eval(expr := f"{lhs} - {rhs}")) == (
        16.0 * (-1 if eval(lhs) < 0 else 1)
    )
    res = eval(expr)
    assert res.exp_bits == 9
    assert res.man_bits == 14


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
@pytest.mark.parametrize("num", [APyFloat(0, 17, 123, 5, 7), APyFloat(0, 0, 123, 5, 7)])
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

    # Infinity with infinity
    assert (APyFloat(0, 0x1F, 0, 5, 7) - APyFloat(0, 0x1F, 0, 5, 7)).is_nan

    # NaN with NaN
    assert (APyFloat(0, 0x1F, 1, 5, 7) - APyFloat(0, 0x1F, 1, 5, 7)).is_nan


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
            expr := f"APyFloat.from_float({sign[0]}{lhs}, {exp[0]}, {man[0]}) * APyFloat.from_float({sign[1]}{rhs}, {exp[1]}, {man[1]})"
        )
    ) == (float(eval(f"{sign[0]}{lhs} * {sign[1]}{rhs}")))
    res = eval(expr)
    assert res.exp_bits == max(int(exp[0]), int(exp[1]))
    assert res.man_bits == max(int(man[0]), int(man[1]))


@pytest.mark.float_mul
def test_mul_overflow():
    """Test that a multiplication can overflow to infinity."""
    assert (APyFloat(0, 0b11110, 1, 5, 2) * APyFloat(0, 0b11110, 3, 5, 3)).is_inf


@pytest.mark.float_mul
def test_mul_underflow():
    """Test that a multiplication can underflow to zero."""
    assert (APyFloat(0, 1, 1, 5, 2) * APyFloat(0, 1, 3, 5, 3)) == 0


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
    assert (x * y).is_identical(APyFloat(sign=0, exp=0, man=6, exp_bits=5, man_bits=10))

    x = APyFloat(sign=0, exp=1, man=10, exp_bits=4, man_bits=10)
    y = APyFloat(sign=0, exp=1, man=20, exp_bits=4, man_bits=10)
    assert (x * y).is_identical(
        APyFloat(sign=0, exp=0, man=16, exp_bits=4, man_bits=10)
    )

    x = APyFloat(sign=0, exp=1, man=10, exp_bits=5, man_bits=15)
    y = APyFloat(sign=0, exp=1, man=20, exp_bits=5, man_bits=15)
    assert (x * y).is_identical(APyFloat(sign=0, exp=0, man=2, exp_bits=5, man_bits=15))

    a = APyFloat(0, 3, 10, 4, 10)
    b = APyFloat(0, 4, 10, 4, 10)
    assert (a * b).is_identical(
        APyFloat(sign=0, exp=0, man=522, exp_bits=4, man_bits=10)
    )


@pytest.mark.float_mul
def test_long_mul():
    x = APyFloat(sign=0, exp=3, man=22, exp_bits=4, man_bits=7)
    y = APyFloat(sign=1, exp=32763, man=813782734503116, exp_bits=16, man_bits=52)
    assert x * y == APyFloat.from_float(float(x) * float(y), exp_bits=16, man_bits=52)

    x = APyFloat.from_float(5200, exp_bits=17, man_bits=52)
    assert x * y == APyFloat.from_float(float(x) * float(y), exp_bits=17, man_bits=52)
    x = APyFloat(sign=0, exp=3, man=122, exp_bits=4, man_bits=7)
    assert (x * y).is_identical(
        APyFloat(sign=1, exp=32760, man=689156585396703, exp_bits=16, man_bits=52)
    )
    x = APyFloat(sign=0, exp=3, man=2**20 - 1, exp_bits=4, man_bits=20)
    y = APyFloat(sign=0, exp=32760, man=2**50 - 1, exp_bits=16, man_bits=50)
    assert (x * y).is_identical(
        APyFloat(sign=0, exp=32757, man=1125898833100799, exp_bits=16, man_bits=50)
    )
    x = APyFloat(sign=0, exp=0, man=11, exp_bits=4, man_bits=9)
    y = APyFloat(sign=0, exp=5, man=50, exp_bits=16, man_bits=50)
    assert (x * y).is_identical(
        APyFloat(sign=0, exp=0, man=6047313952768, exp_bits=16, man_bits=50)
    )
    x = APyFloat(sign=0, exp=0, man=11, exp_bits=4, man_bits=20)
    y = APyFloat(sign=0, exp=5, man=50, exp_bits=16, man_bits=50)
    assert (x * y).is_identical(
        APyFloat(sign=0, exp=0, man=2952790016, exp_bits=16, man_bits=50)
    )

    x = APyFloat(sign=0, exp=1020, man=11, exp_bits=10, man_bits=9)
    y = APyFloat(sign=0, exp=4090, man=50, exp_bits=12, man_bits=50)
    assert (x * y).is_identical(
        APyFloat(sign=0, exp=4095, man=0, exp_bits=12, man_bits=50)
    )
    x = APyFloat(sign=0, exp=1, man=10, exp_bits=4, man_bits=40)
    y = APyFloat(sign=0, exp=1, man=20, exp_bits=4, man_bits=40)
    assert (x * y).is_identical(
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
            expr := f"APyFloat.from_float({sign[0]}{lhs}, {exp[0]}, {man[0]}) / APyFloat.from_float({sign[1]}{rhs}, {exp[1]}, {man[1]})"
        )
    ) == (float(eval(f"{sign[0]}{lhs} / {sign[1]}{rhs}")))
    res = eval(expr)
    assert res.exp_bits == max(int(exp[0]), int(exp[1]))
    assert res.man_bits == max(int(man[0]), int(man[1]))


@pytest.mark.float_div
def test_div_overflow():
    """Test that a division can overflow to infinity."""
    assert (APyFloat(0, 30, 1, 5, 2) / APyFloat(0, 1, 3, 5, 2)).is_inf


@pytest.mark.float_div
def test_div_underflow():
    """Test that a multiplication can underflow to zero."""
    assert (APyFloat(0, 1, 3, 5, 2) / APyFloat(1, 30, 3, 5, 2)) == 0


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


# Power
@pytest.mark.float_pow
def test_power():
    """Test the power function."""
    assert APyFloat.from_float(4.5, 8, 10) ** 2 == APyFloat.from_float(4.5**2, 8, 10)
    assert APyFloat.from_float(-4.5, 8, 10) ** 3 == APyFloat.from_float(
        (-4.5) ** 3, 8, 10
    )
    assert APyFloat.from_float(-8.125, 8, 10) ** 4 == APyFloat.from_float(
        (-8.125) ** 4, 8, 10
    )
    # Sqrt(2) (triggers carry)
    a = APyFloat(sign=0, exp=510, man=27146, exp_bits=10, man_bits=16)
    # Wrong
    # (a**2).is_identical(APyFloat(sign=0, exp=510, man=0, exp_bits=10, man_bits=16))
    (a**4).is_identical(APyFloat(sign=0, exp=509, man=0, exp_bits=10, man_bits=16))

    a = APyFloat(sign=0, exp=511, man=1865452045155277, exp_bits=10, man_bits=52)
    (a**2).is_identical(APyFloat(sign=0, exp=512, man=1, exp_bits=10, man_bits=52))

    # Cube root of three
    a = APyFloat(sign=0, exp=511, man=1116352409, exp_bits=10, man_bits=32)
    (a**3).is_identical(APyFloat(sign=0, exp=512, man=0, exp_bits=10, man_bits=32))


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
    assert APyFloat.from_float(4.5, 11, 52) ** 2 == APyFloat.from_float(4.5**2, 11, 52)
    assert APyFloat.from_float(-4.5, 11, 52) ** 3 == APyFloat.from_float(
        (-4.5) ** 3, 11, 52
    )
    assert APyFloat.from_float(-8.125, 11, 52) ** 4 == APyFloat.from_float(
        (-8.125) ** 4, 11, 52
    )


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
    assert float(APyFloat(0, 0b11110, 1, 5, 2) ** 2) == float("inf")
    assert float(APyFloat(1, 0b11110, 1, 5, 2) ** 3) == float("-inf")


@pytest.mark.float_pow
def test_power_underflow():
    """Test that the power function can underflow to zero."""
    assert float(APyFloat(0, 1, 1, 5, 2) ** 3) == 0


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
