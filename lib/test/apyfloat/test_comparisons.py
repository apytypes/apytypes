from itertools import permutations as perm
import pytest
from apytypes import APyFloat, APyFixed


@pytest.mark.float_comp
def test_identical():
    a = APyFloat(0, 5, 3, 15, 3, 8)
    assert a.is_identical(a)
    assert not a.is_identical(-a)
    assert not a.is_identical(APyFloat(0, 6, 3, 15, 3, 8))
    assert not a.is_identical(APyFloat(0, 5, 2, 15, 3, 8))
    assert not a.is_identical(APyFloat(0, 5, 3, 16, 3, 8))
    assert not a.is_identical(APyFloat(0, 5, 3, 15, 5, 8))
    assert not a.is_identical(APyFloat(0, 5, 3, 15, 3, 9))


@pytest.mark.float_comp
def test_comparisons_with_floats():
    a = APyFloat.from_float(0.5, 3, 3)
    assert a > 0
    assert a > 0.0
    assert a >= 0
    assert not a < 0
    assert not a <= 0
    assert a != 0

    a = APyFloat.from_float(0.0, 3, 3)
    assert a == 0
    assert a == 0.0


@pytest.mark.float_comp
def test_comparisons_with_apyfixed():
    a = APyFloat.from_float(2.5, 5, 5)
    b = APyFixed.from_float(5, 5, 5)
    assert a != b
    assert b > a
    assert a < b
    assert a > -b
    assert a == (b >> 1)

    assert APyFloat.from_float(0, 4, 5) <= APyFixed.from_float(0, 16, 16)
    assert APyFloat.from_float(0.125, 4, 3) >= APyFixed.from_float(0.125, 16, 16)
    assert not (
        APyFloat.from_float(float("inf"), 4, 3) == APyFixed.from_float(1000, 16, 16)
    )
    assert not (
        APyFloat.from_float(float("nan"), 4, 3) == APyFixed.from_float(1000, 16, 16)
    )
    assert APyFloat.from_float(float("nan"), 4, 3) != APyFixed.from_float(1000, 16, 16)
    assert APyFloat.from_float(float("inf"), 4, 3) != APyFixed.from_float(1000, 16, 16)
    assert APyFloat.from_float(float("inf"), 4, 3) >= APyFixed.from_float(1000, 16, 16)
    assert not (
        APyFloat.from_float(float("nan"), 4, 3) >= APyFixed.from_float(1000, 16, 16)
    )
    assert APyFloat.from_float(float("inf"), 4, 3) > APyFixed.from_float(1000, 16, 16)
    assert not (
        APyFloat.from_float(float("nan"), 4, 3) > APyFixed.from_float(1000, 16, 16)
    )
    assert not (
        APyFloat.from_float(float("inf"), 4, 3) <= APyFixed.from_float(1000, 16, 16)
    )
    assert not (
        APyFloat.from_float(float("nan"), 4, 3) <= APyFixed.from_float(1000, 16, 16)
    )
    assert not (
        APyFloat.from_float(float("inf"), 4, 3) < APyFixed.from_float(1000, 16, 16)
    )
    assert not (
        APyFloat.from_float(float("nan"), 4, 3) < APyFixed.from_float(1000, 16, 16)
    )

    assert APyFloat.from_float(float("-inf"), 4, 3) < APyFixed.from_float(1000, 16, 16)
    assert APyFloat.from_float(float("-inf"), 4, 3) <= APyFixed.from_float(1000, 16, 16)
    assert not APyFloat.from_float(float("-inf"), 4, 3) > APyFixed.from_float(
        1000, 16, 16
    )
    assert not APyFloat.from_float(float("-inf"), 4, 3) >= APyFixed.from_float(
        1000, 16, 16
    )


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


@pytest.mark.float_comp
def test_signed_zero_comparison():
    a = APyFloat(0, 0, 0, 5, 10)
    b = APyFloat(1, 0, 0, 5, 10)

    assert a == b
    assert not (a != b)
    assert not (a > b)
    assert a >= b
    assert not (a < b)
    assert a <= b

    assert b == a
    assert not (b != a)
    assert not (b > a)
    assert b >= a
    assert not (b < a)
    assert b <= a


@pytest.mark.float_comp
def test_mixed_bias_comparisons():
    """Comparisons where all fields are different."""
    # x and y are equal
    x = APyFloat(sign=0, exp=5, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=25, man=1, exp_bits=5, man_bits=2, bias=25)
    assert x == y
    assert x >= y
    assert x <= y
    assert not x > y
    assert not x < y
    assert not x != y

    # Two normal numbers but y is smaller
    y = APyFloat(sign=0, exp=5, man=1, exp_bits=5, man_bits=2, bias=6)
    assert not x == y
    assert x >= y
    assert not x <= y
    assert x > y
    assert not x < y
    assert x != y

    # Two equal subnormal numbers
    x = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=0, man=2, exp_bits=5, man_bits=2, bias=6)
    assert x == y
    assert x >= y
    assert x <= y
    assert not x > y
    assert not x < y
    assert not x != y

    # Two subnormal numbers but y is slightly larger
    y = APyFloat(sign=0, exp=0, man=3, exp_bits=5, man_bits=2, bias=6)
    assert not x == y
    assert not x >= y
    assert x <= y
    assert not x > y
    assert x < y
    assert x != y

    # Subnormal number equal to normal number
    x = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=1, man=0, exp_bits=5, man_bits=2, bias=7)
    assert x == y
    assert x >= y
    assert x <= y
    assert not x > y
    assert not x < y
    assert not x != y


@pytest.mark.float_comp
def test_mixed_format_comparisons():
    """Comparisons with mixed formats."""
    # x and y are equal
    x = APyFloat(sign=0, exp=5, man=2, exp_bits=4, man_bits=3, bias=5)
    y = APyFloat(sign=0, exp=25, man=1, exp_bits=5, man_bits=2, bias=25)
    assert x == y
    assert x >= y
    assert x <= y
    assert not x > y
    assert not x < y
    assert not x != y

    # Two normal numbers but y is smaller
    y = APyFloat(sign=0, exp=7, man=3, exp_bits=5, man_bits=5, bias=8)
    assert not x == y
    assert x >= y
    assert not x <= y
    assert x > y
    assert not x < y
    assert x != y

    # Two equal subnormal numbers
    x = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=0, man=4, exp_bits=4, man_bits=3, bias=6)
    assert x == y
    assert x >= y
    assert x <= y
    assert not x > y
    assert not x < y
    assert not x != y

    # Two subnormal numbers but y is slightly larger
    x = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=0, man=5, exp_bits=4, man_bits=3, bias=6)
    assert not x == y
    assert not x >= y
    assert x <= y
    assert not x > y
    assert x < y
    assert x != y
