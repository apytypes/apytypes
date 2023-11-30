from itertools import permutations as perm
import pytest
import sys
sys.path.append('../builddir/')
from apy_types import APyFloat

@pytest.mark.parametrize(
    "test_in,test_exp", [("-APyFloat(5, 5, 2.75)", APyFloat(5, 5, -2.75)), 
                        ("-APyFloat(5, 5, -13.125)", APyFloat(5, 5, 13.125))]
)
def test_negation(test_in, test_exp):
    assert eval(test_in) == test_exp

@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 2.75)", "APyFloat(5, 5, 2.5)"])))
def test_addition_same_exp(lhs, rhs):
    # Add two numbers that have the same exponent
    assert eval(f"{rhs} + {lhs}") == APyFloat(5, 5, 5.25)

@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 5, 4)", "APyFloat(5, 5, 16)"])))
def test_addition_diff_exp(lhs, rhs):
    # Add two numbers that have different exponent
    assert eval(f"{rhs} + {lhs}") == APyFloat(5, 5, 20)

@pytest.mark.skip
@pytest.mark.parametrize("lhs,rhs", list(perm(["APyFloat(5, 4, 12)", "APyFloat(5, 4, -4)"])))
def test_addition_diff_sign(lhs, rhs):
    # Add two numbers that have different sign
    assert eval(f"{rhs} + {lhs}") == APyFloat(5, 4, 8)
