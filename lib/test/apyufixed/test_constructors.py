import pytest

from apytypes import APyUFixed


def test_unsigned_construction_and_conversion():
    value = APyUFixed(0b101101, int_bits=3, frac_bits=3)

    assert value.to_bits() == 0b101101
    assert value.bits == 6
    assert value.int_bits == 3
    assert value.frac_bits == 3
    assert float(value) == 5.625
    assert repr(value) == "APyUFixed(45, int_bits=3, frac_bits=3)"
    assert APyUFixed(-1, bits=8, int_bits=8).to_bits() == 255
    assert APyUFixed.from_float(3.75, int_bits=2, frac_bits=2).to_bits() == 15
    assert APyUFixed.from_str("3.75", int_bits=2, frac_bits=2).to_bits() == 15


def test_unsigned_constructor_requires_exactly_two_bit_specifiers():
    with pytest.raises(
        ValueError,
        match="Fixed-point bit specification needs exactly two of three bit specifiers",
    ):
        APyUFixed(0, bits=8)
    with pytest.raises(
        ValueError,
        match="Fixed-point bit specification needs exactly two of three bit specifiers",
    ):
        APyUFixed(0, int_bits=8)
    with pytest.raises(
        ValueError,
        match="Fixed-point bit specification needs exactly two of three bit specifiers",
    ):
        APyUFixed(0, bits=8, int_bits=4, frac_bits=4)


def test_unsigned_most_significant_integer_bit_is_magnitude():
    value = APyUFixed(0b1000, int_bits=4, frac_bits=0)

    assert float(value) == 8.0
    assert value.to_bits() == 0b1000
    assert not value._is_negative


def test_unsigned_construction_supports_wide_bit_patterns():
    bit_pattern = (1 << 130) | (1 << 67) | 0x55
    value = APyUFixed(bit_pattern, bits=140, int_bits=140)

    assert value.to_bits() == bit_pattern
    assert value.bits == 140
    assert value.int_bits == 140
    assert value.frac_bits == 0


def test_from_float_rounds_to_the_unsigned_fractional_format():
    assert APyUFixed.from_float(0.24, int_bits=1, frac_bits=1).to_bits() == 0
    assert APyUFixed.from_float(0.25, int_bits=1, frac_bits=1).to_bits() == 1
