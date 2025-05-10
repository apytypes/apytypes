from itertools import product

import pytest

from apytypes import APyCFloat, APyFixed, APyFloat


@pytest.mark.parametrize("apyfloat", [APyFloat, APyCFloat])
@pytest.mark.float_comp
def test_identical(apyfloat: type[APyCFloat]):
    a = apyfloat(0, 5, 3, 15, 3, 8)
    assert a.is_identical(a)
    assert not a.is_identical(-a)
    assert not a.is_identical(apyfloat(0, 6, 3, 15, 3, 8))
    assert not a.is_identical(apyfloat(0, 5, 2, 15, 3, 8))
    assert not a.is_identical(apyfloat(0, 5, 3, 16, 3, 8))
    assert not a.is_identical(apyfloat(0, 5, 3, 15, 5, 8))
    assert not a.is_identical(apyfloat(0, 5, 3, 15, 3, 9))


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
    assert APyFloat.from_float(float("inf"), 4, 3) != APyFixed.from_float(1000, 16, 16)
    assert APyFloat.from_float(float("nan"), 4, 3) != APyFixed.from_float(1000, 16, 16)
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
@pytest.mark.parametrize("apyfloat", [APyFloat, APyCFloat])
@pytest.mark.parametrize("val", product([0.0, -2.5, -2.75, -(2**-9), 2**5], repeat=2))
@pytest.mark.parametrize("exp_bits", product([4, 6], repeat=2))
@pytest.mark.parametrize("man_bits", product([7, 8], repeat=2))
def test_equality(
    apyfloat: type[APyCFloat],
    val: tuple[float, float],
    exp_bits: tuple[int, int],
    man_bits: tuple[int, int],
):
    is_eq = val[0] == val[1]
    lhs = apyfloat.from_float(val[0], exp_bits=exp_bits[0], man_bits=man_bits[0])
    rhs = apyfloat.from_float(val[1], exp_bits=exp_bits[1], man_bits=man_bits[1])
    assert (lhs == rhs) == is_eq
    assert (lhs != rhs) == (not is_eq)


@pytest.mark.float_comp
@pytest.mark.parametrize("val", product([0.0, -2.5, -2.75, -(2**-9), 2**5], repeat=2))
@pytest.mark.parametrize("exp_bits", product([5, 7], repeat=2))
@pytest.mark.parametrize("man_bits", product([6, 8], repeat=2))
def test_lt_gt(
    val: tuple[float, float],
    exp_bits: tuple[int, int],
    man_bits: tuple[int, int],
):
    is_gt = val[0] > val[1]
    is_lt = val[0] < val[1]
    lhs = APyFloat.from_float(val[0], exp_bits=exp_bits[0], man_bits=man_bits[0])
    rhs = APyFloat.from_float(val[1], exp_bits=exp_bits[1], man_bits=man_bits[1])
    assert (lhs > rhs) == is_gt
    assert (lhs < rhs) == is_lt


@pytest.mark.float_comp
@pytest.mark.parametrize("val", product([0.0, -2.5, -2.75, -(2**-9), 2**5], repeat=2))
@pytest.mark.parametrize("exp_bits", product([5, 7], repeat=2))
@pytest.mark.parametrize("man_bits", product([6, 8], repeat=2))
def test_leq_geq(
    val: tuple[float, float],
    exp_bits: tuple[int, int],
    man_bits: tuple[int, int],
):
    is_geq = val[0] >= val[1]
    is_leq = val[0] <= val[1]
    lhs = APyFloat.from_float(val[0], exp_bits=exp_bits[0], man_bits=man_bits[0])
    rhs = APyFloat.from_float(val[1], exp_bits=exp_bits[1], man_bits=man_bits[1])
    assert (lhs >= rhs) == is_geq
    assert (lhs <= rhs) == is_leq


@pytest.mark.float_comp
@pytest.mark.parametrize("val", [0.0, -2.5, -2.75, -(2**-9), 2**5])
@pytest.mark.parametrize("exp_bits", product([5, 7], repeat=2))
@pytest.mark.parametrize("man_bits", product([6, 8], repeat=2))
def test_nan_comparison(
    val: float,
    exp_bits: tuple[int, int],
    man_bits: tuple[int, int],
):
    lhs = APyFloat.from_float(val, exp_bits=exp_bits[0], man_bits=man_bits[0])
    rhs = APyFloat.from_float(float("nan"), exp_bits=exp_bits[1], man_bits=man_bits[1])
    assert not lhs == rhs  # noqa: SIM201
    assert not lhs != rhs  # noqa: SIM202
    assert not lhs < rhs
    assert not lhs > rhs
    assert not lhs <= rhs
    assert not lhs >= rhs

    lhs = APyFloat.from_float(float("nan"), exp_bits=exp_bits[0], man_bits=man_bits[0])
    rhs = APyFloat.from_float(val, exp_bits=exp_bits[1], man_bits=man_bits[1])
    assert not lhs == rhs  # noqa: SIM201
    assert not lhs != rhs  # noqa: SIM202
    assert not lhs < rhs
    assert not lhs > rhs
    assert not lhs <= rhs
    assert not lhs >= rhs


@pytest.mark.float_comp
@pytest.mark.float_special
@pytest.mark.parametrize("val0", [float("inf"), float("-inf"), 0.0, -1.0])
@pytest.mark.parametrize("val1", [float("inf"), float("-inf"), 0.0, -1.0])
@pytest.mark.parametrize("exp_bits", product([5, 7], repeat=2))
@pytest.mark.parametrize("man_bits", product([6, 8], repeat=2))
def test_inf_comparison(
    val0: float,
    val1: float,
    exp_bits: tuple[int, int],
    man_bits: tuple[int, int],
):
    lhs = APyFloat.from_float(val0, exp_bits=exp_bits[0], man_bits=man_bits[0])
    rhs = APyFloat.from_float(val1, exp_bits=exp_bits[1], man_bits=man_bits[1])
    assert (lhs == rhs) == (val0 == val1)
    assert (lhs != rhs) == (val0 != val1)
    assert (lhs < rhs) == (val0 < val1)
    assert (lhs > rhs) == (val0 > val1)
    assert (lhs <= rhs) == (val0 <= val1)
    assert (lhs >= rhs) == (val0 >= val1)


@pytest.mark.float_comp
def test_signed_zero_comparison():
    a = APyFloat(0, 0, 0, 5, 10)
    b = APyFloat(1, 0, 0, 5, 10)

    assert a == b
    assert a == b
    assert not (a > b)
    assert a >= b
    assert not (a < b)
    assert a <= b

    assert b == a
    assert b == a
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
    assert x == y

    # Two normal numbers but y is smaller
    y = APyFloat(sign=0, exp=5, man=1, exp_bits=5, man_bits=2, bias=6)
    assert x != y
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
    assert x == y

    # Two subnormal numbers but y is slightly larger
    y = APyFloat(sign=0, exp=0, man=3, exp_bits=5, man_bits=2, bias=6)
    assert x != y
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
    assert x == y


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
    assert x == y

    # Two normal numbers but y is smaller
    y = APyFloat(sign=0, exp=7, man=3, exp_bits=5, man_bits=5, bias=8)
    assert x != y
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
    assert x == y

    # Two subnormal numbers but y is slightly larger
    x = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=0, man=5, exp_bits=4, man_bits=3, bias=6)
    assert x != y
    assert not x >= y
    assert x <= y
    assert not x > y
    assert x < y
    assert x != y
