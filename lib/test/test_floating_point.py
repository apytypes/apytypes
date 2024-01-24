from itertools import permutations as perm
import pytest
from apytypes import APyFloat


# Conversions
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


# Comparison operators
@pytest.mark.float_comp
@pytest.mark.parametrize(
    "lhs,rhs,test_exp",
    [
        ("APyFloat.from_float(2.75, 5, 5)", "APyFloat.from_float(2.75, 5, 5)", True),
        ("APyFloat.from_float(2.75, 5, 6)", "APyFloat.from_float(2.75, 5, 5)", True),
        ("APyFloat.from_float(2.75, 6, 5)", "APyFloat.from_float(2.75, 5, 5)", True),
        ("APyFloat.from_float(2.75, 5, 5)", "APyFloat.from_float(-2.75, 5, 5)", False),
        ("APyFloat.from_float(3.5, 5, 5)", "APyFloat.from_float(6.5, 5, 5)", False),
        ("APyFloat.from_float(3.5, 5, 5)", "APyFloat.from_float(3.75, 5, 5)", False),
        ("APyFloat.from_float(2**-9, 4, 3)", "APyFloat.from_float(2**-9, 5, 2)", True),
    ],
)
def test_equality(lhs, rhs, test_exp):
    assert (eval(lhs) == eval(rhs)) == test_exp
    assert (eval(lhs) != eval(rhs)) == (not test_exp)


@pytest.mark.float_comp
@pytest.mark.parametrize("exp", list(perm(["5", "6", "7", "8"], 2)))
@pytest.mark.parametrize("man", list(perm(["5", "6", "7", "8"], 2)))
@pytest.mark.parametrize(
    "lhs,rhs,test_exp",
    [
        ("3.5", "6.75", True),
        ("3.5", "6.25", True),
        ("3.5", "2.75", False),
        ("3.5", "3.5", False),
        ("3.5", "-6.75", False),
    ],
)
def test_less_greater_than(exp, man, lhs, rhs, test_exp):
    assert (
        eval(
            f"APyFloat.from_float({lhs}, {exp[0]}, {man[0]}) < APyFloat.from_float({rhs}, {exp[1]}, {man[1]})"
        )
    ) == test_exp
    assert (
        eval(
            f"APyFloat.from_float({rhs}, {exp[0]}, {man[0]}) > APyFloat.from_float({lhs}, {exp[1]}, {man[1]})"
        )
    ) == test_exp


@pytest.mark.float_comp
@pytest.mark.parametrize(
    "lhs,rhs,test_exp",
    [
        ("APyFloat.from_float(3.5, 5, 5)", "APyFloat.from_float(6.75, 5, 5)", True),
        ("APyFloat.from_float(3.5, 5, 5)", "APyFloat.from_float(6.25, 5, 5)", True),
        ("APyFloat.from_float(3.5, 5, 5)", "APyFloat.from_float(2.75, 5, 5)", False),
        ("APyFloat.from_float(3.5, 5, 5)", "APyFloat.from_float(3.5, 5, 5)", True),
        ("APyFloat.from_float(3.5, 5, 5)", "APyFloat.from_float(-6.75, 5, 5)", False),
    ],
)
def test_leq_geq(lhs, rhs, test_exp):
    assert (eval(lhs) <= eval(rhs)) == test_exp
    assert (eval(rhs) >= eval(lhs)) == test_exp


@pytest.mark.float_comp
@pytest.mark.float_special
@pytest.mark.parametrize(
    "lhs,rhs",
    list(
        perm(
            [
                "APyFloat.from_float(2.75, 5, 5)",
                "APyFloat.from_float(float('nan'), 5, 5)",
            ]
        )
    )
    + list(
        perm(
            [
                "APyFloat.from_float(float('nan'), 5, 5)",
                "APyFloat.from_float(float('nan'), 5, 5)",
            ]
        )
    )
    + list(
        perm(
            ["APyFloat.from_float(float('nan'), 5, 5)", "APyFloat.from_float(0, 5, 5)"]
        )
    )
    + [
        (
            "APyFloat.from_float(float('nan'), 5, 5)",
            "APyFloat.from_float(float('nan'), 5, 5)",
        )
    ],
)
def test_nan_comparison(lhs, rhs):
    assert not (eval(lhs) == eval(rhs))
    assert not (eval(lhs) != eval(rhs))
    assert not (eval(lhs) < eval(rhs))
    assert not (eval(lhs) > eval(rhs))
    assert not (eval(lhs) <= eval(rhs))
    assert not (eval(lhs) >= eval(rhs))


@pytest.mark.float_comp
@pytest.mark.float_special
def test_inf_comparison():
    assert APyFloat.from_float(12, 4, 1) < APyFloat.from_float(float("inf"), 5, 5)
    assert APyFloat.from_float(-12, 4, 1) > APyFloat.from_float(float("-inf"), 5, 5)
    assert APyFloat.from_float(0, 5, 5) > APyFloat.from_float(float("-inf"), 5, 5)
    assert APyFloat.from_float(float("inf"), 5, 5) == APyFloat.from_float(
        float("inf"), 5, 5
    )
    assert APyFloat.from_float(float("inf"), 5, 5) == APyFloat.from_float(
        float("inf"), 3, 2
    )
    assert APyFloat.from_float(float("inf"), 5, 5) != APyFloat.from_float(
        float("-inf"), 5, 5
    )


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
        16.0 * (-1 if eval(lhs).is_sign_neg else 1)
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
        5.75 * (-1 if eval(lhs).is_zero else 1)
    )
    res = eval(expr)
    assert res.exp_bits == 15
    assert res.man_bits == 12


@pytest.mark.float_sub
def test_sub_overflow():
    """Test that a subtraction can overflow."""
    assert (APyFloat(1, 0b11110, 1, 5, 2) - APyFloat(0, 0b11110, 3, 5, 3)).is_inf


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
    assert (APyFloat(0, 1, 1, 5, 2) * APyFloat(0, 1, 3, 5, 3)).is_zero


# Power
@pytest.mark.float_pow
def test_power():
    """Test that a multiplication can underflow to zero."""
    assert APyFloat.from_float(4.5, 8, 10) ** 2 == APyFloat.from_float(4.5**2, 8, 10)
    assert APyFloat.from_float(-4.5, 8, 10) ** 3 == APyFloat.from_float(
        (-4.5) ** 3, 8, 10
    )
    assert APyFloat.from_float(-8.125, 8, 10) ** 4 == APyFloat.from_float(
        (-8.125) ** 4, 8, 10
    )


@pytest.mark.float_pow
def test_power_overflow():
    """Test that the power function can overflow to infinity."""
    assert (APyFloat(0, 0b11110, 1, 5, 2) ** 2).to_float() == float("inf")
    assert (APyFloat(1, 0b11110, 1, 5, 2) ** 3).to_float() == float("-inf")


@pytest.mark.float_pow
def test_power_underflow():
    """Test that the power function can underflow to zero."""
    assert (APyFloat(0, 1, 1, 5, 2) ** 3).to_float() == 0


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
    """Test that a multiplication can underflow to zero."""
    if str(test_exp) == "nan":
        assert eval(f"(APyFloat.from_float(float({x}), 9, 7)**{n}).is_nan")
    else:
        assert (
            eval(f"(APyFloat.from_float(float({x}), 9, 7)**{n}).to_float()") == test_exp
        )


@pytest.mark.xfail
def test_comparisons_failing():
    a == APyFloat.from_float(0.5, 3, 3)
    assert a > 0
    assert a > 0.0
    a == APyFloat.from_float(0.0, 3, 3)
    assert a == 0
    assert a == 0.0
