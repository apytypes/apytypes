import pickle
from fractions import Fraction

from apytypes import APyFixed, APyUFixed


def test_unsigned_comparison_and_pickle():
    value = APyUFixed(15, int_bits=4, frac_bits=0)

    assert value == 15
    assert value > APyUFixed(14, int_bits=4, frac_bits=0)
    assert value <= 15.0
    assert value == APyFixed(15, int_bits=5, frac_bits=0)
    assert value.is_identical(pickle.loads(pickle.dumps(value)))
    assert value + 1.0 == 16.0


def test_unsigned_fraction_and_real_imaginary_properties():
    value = APyUFixed(45, int_bits=3, frac_bits=3)

    assert value.to_fraction() == Fraction(45, 8)
    assert value.as_integer_ratio() == (45, 8)
    assert not value.is_zero
    assert value._is_positive
    assert not value._is_negative
    assert value.real.is_identical(value)
    assert value.imag.is_zero
    assert abs(value).is_identical(value)


def test_unsigned_zero_properties():
    value = APyUFixed(0, int_bits=4, frac_bits=2)

    assert value.is_zero
    assert not value._is_positive
