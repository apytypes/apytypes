from itertools import permutations as perm
import math
import pytest
from apytypes import APyFloat


# Exhaustive tests, long running
# These should be replaced by testing against a third-party tool
@pytest.mark.slow
@pytest.mark.float_add
@pytest.mark.parametrize("x_bits", range(0, 2**8))
@pytest.mark.parametrize("y_bits", range(0, 2**8))
def test_add_all(x_bits, y_bits):
    y = APyFloat.from_bits(y_bits, 4, 3)
    x = APyFloat.from_bits(x_bits, 4, 3)
    ans = x + y
    ref = APyFloat.from_float(float(x) + float(y), 4, 3)
    print(f"{float(x)} + {float(y)} = {float(ref)} != {float(ans)}")
    print(f"{x!r} + {y!r} = {ref!r} != {ans!r}")
    assert ans == ref or (ans.is_nan and ref.is_nan) or (ans == 0 and ref == 0)


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
    assert (APyFloat(0, 0b11110, 3, 5, 2) + APyFloat(0, 0b11110, 3, 5, 2)).is_inf
    assert (APyFloat(1, 0b11110, 3, 5, 2) + APyFloat(1, 0b11110, 3, 5, 2)).is_inf


@pytest.mark.float_add
@pytest.mark.parametrize("x", [0.0, 1.0, float("inf"), float("nan")])
@pytest.mark.parametrize("y", [0.0, 1.0, float("inf"), float("nan")])
def test_add_special_cases(x, y):
    """Test the special cases for addition."""
    assert str(
        float(APyFloat.from_float(x, 4, 4) + APyFloat.from_float(y, 4, 4))
    ) == str(x + y)


@pytest.mark.float_add
def test_long_add():
    x = APyFloat(sign=0, exp=3, man=22, exp_bits=4, man_bits=7)
    y = APyFloat(sign=1, exp=32763, man=813782734503116, exp_bits=16, man_bits=52)
    assert x + y == APyFloat.from_float(float(x) + float(y), exp_bits=16, man_bits=52)

    x = APyFloat.from_float(5200, exp_bits=17, man_bits=52)
    assert x + y == APyFloat.from_float(float(x) + float(y), exp_bits=17, man_bits=52)


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
@pytest.mark.parametrize(
    "lhs,rhs",
    list(perm(["APyFloat.from_float(5.75, 15, 5)", "APyFloat.from_float(0.0, 5, 12)"])),
)
def test_sub_zero(lhs, rhs):
    # Subtraction when one operand is zero
    expr = None
    assert float(eval(expr := f"{lhs} - {rhs}")) == (
        5.75 * (-1 if eval(lhs) == 0 else 1)
    )
    res = eval(expr)
    assert res.exp_bits == 15
    assert res.man_bits == 12


@pytest.mark.float_sub
def test_sub_overflow():
    """Test that a subtraction can overflow."""
    assert (APyFloat(1, 0b11110, 1, 5, 2) - APyFloat(0, 0b11110, 3, 5, 3)).is_inf


@pytest.mark.float_sub
@pytest.mark.parametrize("x", [0.0, 1.0, float("inf"), float("nan")])
@pytest.mark.parametrize("y", [0.0, 1.0, float("inf"), float("nan")])
def test_sub_special_cases(x, y):
    """Test the special cases for addition."""
    s = str(float(APyFloat.from_float(x, 4, 4) - APyFloat.from_float(y, 4, 4)))
    if x == 0 and y == 0 or x == 1 and y == 1:  # Python writes this as 0.0
        assert s == "-0.0"
    else:
        assert s == str(x - y)


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
def test_long_mul():
    x = APyFloat(sign=0, exp=3, man=22, exp_bits=4, man_bits=7)
    y = APyFloat(sign=1, exp=32763, man=813782734503116, exp_bits=16, man_bits=52)
    assert x * y == APyFloat.from_float(float(x) * float(y), exp_bits=16, man_bits=52)

    x = APyFloat.from_float(5200, exp_bits=17, man_bits=52)
    assert x * y == APyFloat.from_float(float(x) * float(y), exp_bits=17, man_bits=52)


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
def test_long_div():
    x = APyFloat(sign=0, exp=3, man=22, exp_bits=4, man_bits=7)
    y = APyFloat(sign=1, exp=32763, man=813782734503116, exp_bits=16, man_bits=52)
    assert x / y == APyFloat.from_float(float(x) / float(y), exp_bits=16, man_bits=52)

    x = APyFloat.from_float(5200, exp_bits=17, man_bits=52)
    assert x / y == APyFloat.from_float(float(x) / float(y), exp_bits=17, man_bits=52)


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
