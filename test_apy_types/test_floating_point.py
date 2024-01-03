from itertools import permutations as perm
import pytest
import sys
sys.path.append('../builddir/')
from apy_types import APyFloat

# Conversion to special numbers
@pytest.mark.float_special
@pytest.mark.parametrize(
    "float_s", ['nan', 'inf', '-inf', '0.0', '-0.0']
)
def test_special_conversions(float_s):
    assert str(float(APyFloat(5, 5, float(float_s)))) ==  str(float(float_s)) == float_s

# Comparison operators
@pytest.mark.float_comp
@pytest.mark.parametrize(
    "lhs,rhs,test_exp", [("APyFloat(5, 5, 2.75)", "APyFloat(5, 5, 2.75)", True), 
                         ("APyFloat(5, 5, 2.75)", "APyFloat(5, 5, -2.75)", False),
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 6.5)", False),
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 3.75)", False)]
)
def test_equality(lhs, rhs, test_exp):
    assert (eval(lhs) == eval(rhs)) == test_exp
    assert (eval(lhs) != eval(rhs)) == (not test_exp)

@pytest.mark.float_comp
@pytest.mark.parametrize(
    "lhs,rhs,test_exp", [("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 6.75)", True), 
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 6.25)", True),
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 2.75)", False),
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 3.5)", False),
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, -6.75)", False)]
)
def test_less_greater_than(lhs, rhs, test_exp):
    assert (eval(lhs) < eval(rhs)) == test_exp
    assert (eval(lhs) > eval(rhs)) == (not test_exp)

@pytest.mark.float_comp
@pytest.mark.parametrize(
    "lhs,rhs,test_exp", [("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 6.75)", True), 
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 6.25)", True),
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 2.75)", False),
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, 3.5)", True),
                         ("APyFloat(5, 5, 3.5)", "APyFloat(5, 5, -6.75)", False)]
)
def test_leq_geq(lhs, rhs, test_exp):
    assert (eval(lhs) <= eval(rhs)) == test_exp
    assert (eval(rhs) >= eval(lhs)) == test_exp

@pytest.mark.float_comp
@pytest.mark.float_special
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 2.75)", "APyFloat(5, 5, float('nan'))"]))
                                    + list(perm(["APyFloat(5, 5, float('inf'))", "APyFloat(5, 5, float('nan'))"]))
                                    + list(perm(["APyFloat(5, 5, float('nan'))", "APyFloat(5, 5, 0)"]))
                                    + [("APyFloat(5, 5, float('nan'))", "APyFloat(5, 5, float('nan'))")])
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
    assert APyFloat(4, 1, 12) < APyFloat(5, 5, float('inf'))
    assert APyFloat(4, 1, -12) > APyFloat(5, 5, float('-inf'))
    assert APyFloat(5, 5, 0) > APyFloat(5, 5, float('-inf'))
    assert APyFloat(5, 5, float('inf')) == APyFloat(5, 5, float('inf'))
    assert APyFloat(5, 5, float('inf')) != APyFloat(5, 5, float('-inf'))


# Addition
@pytest.mark.float_neg
@pytest.mark.parametrize(
    "test_in,test_exp", [("-APyFloat(5, 5, 2.75)", -2.75), 
                        ("-APyFloat(5, 5, -13.25)", 13.25)]
)
def test_negation(test_in, test_exp):
    assert float(eval(test_in)) == test_exp

@pytest.mark.float_add
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 2.75)", "APyFloat(5, 5, 2.5)"])))
def test_add_same_exp(lhs, rhs):
    # Add two numbers that have the same exponent
    assert float(eval(f"{lhs} + {rhs}")) == 5.25

@pytest.mark.float_add
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 4)", "APyFloat(5, 5, 16)"])))
def test_add_diff_exp(lhs, rhs):
    # Add two numbers that have different exponent
    assert float(eval(f"{lhs} + {rhs}")) == 20.0

@pytest.mark.float_add
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 4, 12)", "APyFloat(5, 4, -4)"])))
def test_add_diff_sign(lhs, rhs):
    # Add two numbers that have different sign
    assert float(eval(f"{lhs} + {rhs}")) == 8.0

@pytest.mark.float_add
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 5.75)", "APyFloat(5, 5, 0.0)"])))
def test_add_zero(lhs, rhs):
    # Add two numbers that have different sign
    assert float(eval(f"{lhs} + {rhs}")) == 5.75


# Subtraction
@pytest.mark.float_sub
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 2.75)", "APyFloat(5, 5, 2.5)"])))
def test_sub_same_exp(lhs, rhs):
    # Subtract two numbers that have the same exponent
    assert float(eval(f"{lhs} - {rhs}")) == (0.25 if float(eval(lhs)) == 2.75 else -0.25)

@pytest.mark.float_sub
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 4)", "APyFloat(5, 5, 16)"])))
def test_sub_diff_exp(lhs, rhs):
    # Subtract two numbers that have different exponent
    assert float(eval(f"{lhs} - {rhs}")) == (-12 if float(eval(lhs)) == 4 else 12)

@pytest.mark.float_sub
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 4, 12)", "APyFloat(5, 4, -4)"])))
def test_sub_diff_sign(lhs, rhs):
    # Subtract two numbers that have different sign
    assert float(eval(f"{lhs} - {rhs}")) == (16.0 * (-1 if eval(lhs).is_sign_neg() else 1))

@pytest.mark.float_sub
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 5.75)", "APyFloat(5, 5, 0.0)"])))
def test_sub_zero(lhs, rhs):
    # Subtract two numbers that have different sign
    assert float(eval(f"{lhs} - {rhs}")) == (5.75 * (-1 if eval(lhs).is_zero() else 1))


# Multiplication
@pytest.mark.float_mul
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 2.75)", "APyFloat(5, 5, 2.5)"])))
def test_mul_same_exp(lhs, rhs):
    assert float(eval(f"{lhs} * {rhs}")) == 6.875

@pytest.mark.float_mul
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 2.75)", "APyFloat(5, 5, 0)"])))
def test_mul_zero(lhs, rhs):
    assert float(eval(f"{lhs} * {rhs}")) == 0.0

@pytest.mark.float_mul
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 2.75)", "APyFloat(5, 5, 1)"])))
def test_mul_identity(lhs, rhs):
    assert float(eval(f"{lhs} * {rhs}")) == 2.75

@pytest.mark.float_mul
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 2.75)", "APyFloat(5, 5, -1)"])))
def test_mul_sign(lhs, rhs):
    assert float(eval(f"{lhs} * {rhs}")) == -2.75

@pytest.mark.float_mul
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 6, -2.75)", "APyFloat(5, 6, -3.5)"])))
def test_mul_two_neg(lhs, rhs):
    assert float(eval(f"{lhs} * {rhs}")) == 9.625