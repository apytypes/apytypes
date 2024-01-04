from itertools import permutations as perm
import pytest
import sys
sys.path.append('../builddir/')
from apy_types import APyFloat

# Conversions
@pytest.mark.float_special
@pytest.mark.parametrize(
    'float_s', ['nan', 'inf', '-inf', '0.0', '-0.0'])
def test_special_conversions(float_s):
    assert str(float(APyFloat(5, 5, float(float_s)))) ==  str(float(float_s)) == float_s

@pytest.mark.parametrize('exp', list(perm([5, 6, 7, 8], 2)))
@pytest.mark.parametrize('man', list(perm([5, 6, 7, 8], 2)))
@pytest.mark.parametrize('val', [2.625, 12])
@pytest.mark.parametrize('neg', [-1.0, 1.0])
def test_normal_conversions(exp, man, val, neg):
    val *= neg
    assert float(APyFloat(exp[0], man[0], val)) == float(APyFloat(exp[1], man[1], val)) == val


# Comparison operators
@pytest.mark.float_comp
@pytest.mark.parametrize(
    'lhs,rhs,test_exp', [('APyFloat(5, 5, 2.75)', 'APyFloat(5, 5, 2.75)', True), 
                         ('APyFloat(5, 6, 2.75)', 'APyFloat(5, 5, 2.75)', False), 
                         ('APyFloat(6, 5, 2.75)', 'APyFloat(5, 5, 2.75)', False), 
                         ('APyFloat(5, 5, 2.75)', 'APyFloat(5, 5, -2.75)', False),
                         ('APyFloat(5, 5, 3.5)', 'APyFloat(5, 5, 6.5)', False),
                         ('APyFloat(5, 5, 3.5)', 'APyFloat(5, 5, 3.75)', False)]
)
def test_equality(lhs, rhs, test_exp):
    assert (eval(lhs) == eval(rhs)) == test_exp
    assert (eval(lhs) != eval(rhs)) == (not test_exp)

@pytest.mark.float_comp
@pytest.mark.parametrize('exp', list(perm(['5', '6', '7', '8'], 2)))
@pytest.mark.parametrize('man', list(perm(['5', '6', '7', '8'], 2)))
@pytest.mark.parametrize(
    'lhs,rhs,test_exp', [('3.5', '6.75', True), 
                         ('3.5', '6.25', True),
                         ('3.5', '2.75', False),
                         ('3.5', '3.5', False),
                         ('3.5', '-6.75', False)])
def test_less_greater_than(exp, man, lhs, rhs, test_exp):
    assert (eval(f'APyFloat({exp[0]}, {man[0]}, {lhs}) < APyFloat({exp[1]}, {man[1]}, {rhs})') ) == test_exp
    assert (eval(f'APyFloat({exp[0]}, {man[0]}, {lhs}) > APyFloat({exp[1]}, {man[1]}, {rhs})') ) != test_exp

@pytest.mark.float_comp
@pytest.mark.parametrize(
    'lhs,rhs,test_exp', [('APyFloat(5, 5, 3.5)', 'APyFloat(5, 5, 6.75)', True), 
                         ('APyFloat(5, 5, 3.5)', 'APyFloat(5, 5, 6.25)', True),
                         ('APyFloat(5, 5, 3.5)', 'APyFloat(5, 5, 2.75)', False),
                         ('APyFloat(5, 5, 3.5)', 'APyFloat(5, 5, 3.5)', True),
                         ('APyFloat(5, 5, 3.5)', 'APyFloat(5, 5, -6.75)', False)])
def test_leq_geq(lhs, rhs, test_exp):
    assert (eval(lhs) <= eval(rhs)) == test_exp
    assert (eval(rhs) >= eval(lhs)) == test_exp

@pytest.mark.float_comp
@pytest.mark.float_special
@pytest.mark.parametrize('lhs,rhs', list(perm(['APyFloat(5, 5, 2.75)', "APyFloat(5, 5, float('nan'))"]))
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


# Negation
@pytest.mark.parametrize('sign', ['-', ''])
@pytest.mark.parametrize('float_s', ['13',
                                     '0.0',
                                     pytest.param('inf', marks=pytest.mark.float_special)])
def test_negation(float_s, sign):
    float_s = sign + float_s
    assert float(eval(f'-APyFloat(5, 5, float("{float_s}"))')) == -float(float_s)


# Addition
@pytest.mark.float_add
@pytest.mark.parametrize('exp', list(perm(['5', '6', '10'], 2)))
@pytest.mark.parametrize('man', list(perm(['5', '6', '10'], 2)))
@pytest.mark.parametrize('sign', list(set(perm(['-', '-', '', ''], 2))))
@pytest.mark.parametrize('lhs,rhs', list(perm(['2.75', '2.5', '16.5', '0'], 2)))
def test_add_representable(exp, man, sign, lhs, rhs):
    """ Test additions where the operands and the result is exactly representable by the formats. """
    expr = None
    assert float(eval(expr := f'APyFloat({exp[0]}, {man[0]}, {sign[0]}{lhs}) + APyFloat({exp[1]}, {man[1]}, {sign[1]}{rhs})')) == (float(eval(f'{sign[0]}{lhs} + {sign[1]}{rhs}')))
    res = eval(expr)
    assert res.get_exp_bits() == max(int(exp[0]), int(exp[1]))
    assert res.get_man_bits() == max(int(man[0]), int(man[1]))

# Subtraction
# Because subtraction is implemented as 'a+(-b)', the tests for addition will also cover subtraction.
# But we 
@pytest.mark.float_sub
@pytest.mark.parametrize('lhs,rhs', list(perm(['APyFloat(6, 5, 2.75)', 'APyFloat(5, 7, 2.5)'])))
def test_sub_same_exp(lhs, rhs):
    # Subtract two numbers that have the same exponent
    expr = None
    assert float(eval(expr := f'{lhs} - {rhs}')) == (0.25 if float(eval(lhs)) == 2.75 else -0.25)
    res = eval(expr)
    assert res.get_exp_bits() == 6
    assert res.get_man_bits() == 7

@pytest.mark.float_sub
@pytest.mark.parametrize('lhs,rhs', list(perm(['APyFloat(9, 5, 4)', 'APyFloat(5, 12, 16)'])))
def test_sub_diff_exp(lhs, rhs):
    # Subtract two numbers that have different exponent
    expr = None
    assert float(eval(expr := f'{lhs} - {rhs}')) == (-12 if float(eval(lhs)) == 4 else 12)
    res = eval(expr)
    assert res.get_exp_bits() == 9
    assert res.get_man_bits() == 12

@pytest.mark.float_sub
@pytest.mark.parametrize('lhs,rhs', list(perm(['APyFloat(9, 4, 12)', 'APyFloat(5, 14, -4)'])))
def test_sub_diff_sign(lhs, rhs):
    # Subtract two numbers that have different sign
    expr = None
    assert float(eval(expr := f'{lhs} - {rhs}')) == (16.0 * (-1 if eval(lhs).is_sign_neg() else 1))
    res = eval(expr)
    assert res.get_exp_bits() == 9
    assert res.get_man_bits() == 14

@pytest.mark.float_sub
@pytest.mark.parametrize('lhs,rhs', list(perm(['APyFloat(15, 5, 5.75)', 'APyFloat(5, 12, 0.0)'])))
def test_sub_zero(lhs, rhs):
    # Subtraction when one operand is zero
    expr = None
    assert float(eval(expr := f'{lhs} - {rhs}')) == (5.75 * (-1 if eval(lhs).is_zero() else 1))
    res = eval(expr)
    assert res.get_exp_bits() == 15
    assert res.get_man_bits() == 12


# Multiplication
@pytest.mark.float_mul
@pytest.mark.parametrize('exp', list(perm(['5', '6', '7', '8'], 2)))
@pytest.mark.parametrize('man', list(perm(['5', '6', '7', '8'], 2)))
@pytest.mark.parametrize('sign', list(set(perm(['-', '-', '', ''], 2))))
@pytest.mark.parametrize('lhs,rhs', list(perm(['2.75', '2.5', '0'], 2)))
def test_mul_mixed(exp, man, sign, lhs, rhs):
    """ Test multiplications where the operands and the result is exactly representable by the formats. """
    expr = None
    assert float(eval(expr := f'APyFloat({exp[0]}, {man[0]}, {sign[0]}{lhs}) * APyFloat({exp[1]}, {man[1]}, {sign[1]}{rhs})')) == (float(eval(f'{sign[0]}{lhs} * {sign[1]}{rhs}')))
    res = eval(expr)
    assert res.get_exp_bits() == max(int(exp[0]), int(exp[1]))
    assert res.get_man_bits() == max(int(man[0]), int(man[1]))