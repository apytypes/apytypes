import pytest

from apytypes import APyCFixed, APyFixed, APyUFixed


def test_unsigned_arithmetic_and_result_types():
    lhs = APyUFixed.from_float(1.5, int_bits=2, frac_bits=2)
    rhs = APyUFixed.from_float(0.75, int_bits=2, frac_bits=2)

    result = lhs + rhs
    assert isinstance(result, APyUFixed)
    assert float(result) == 2.25
    assert result.int_bits == 3
    assert result.frac_bits == 2

    difference = rhs - lhs
    assert isinstance(difference, APyFixed)
    assert float(difference) == -0.75

    product = lhs * rhs
    quotient = lhs / rhs
    assert isinstance(product, APyUFixed)
    assert isinstance(quotient, APyUFixed)
    assert float(product) == 1.125
    assert float(quotient) == 2.0
    assert isinstance(-lhs, APyFixed)
    assert float(-lhs) == -1.5
    assert isinstance(lhs**2, APyUFixed)
    assert float(lhs**2) == 2.25


def test_mixed_arithmetic_promotes_to_signed_types():
    value = APyUFixed.from_float(1.5, int_bits=2, frac_bits=2)

    assert isinstance(value + 1, APyFixed)
    assert isinstance(1 + value, APyFixed)
    assert isinstance(value + 1.0, APyFixed)
    assert isinstance(1.0 + value, APyFixed)
    assert isinstance(value + 1j, APyCFixed)
    assert isinstance(1j + value, APyCFixed)
    assert value + 1 == 2.5
    assert 1 + value == 2.5


def test_unsigned_division_by_zero_raises():
    value = APyUFixed.from_float(1.0, int_bits=2, frac_bits=2)
    zero = APyUFixed(0, int_bits=2, frac_bits=2)

    with pytest.raises(ZeroDivisionError, match="fixed-point division by zero"):
        _ = value / zero
