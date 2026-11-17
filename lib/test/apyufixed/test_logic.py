import pytest

from apytypes import APyUFixed


def test_unsigned_bit_indexing_and_ranges():
    value = APyUFixed(0b101101, int_bits=3, frac_bits=3)

    assert value[0] == 1
    assert value[1] == 0
    assert value[2] == 1
    assert value[-1] == 1
    assert value[-2] == 0
    assert value[-3] == 1
    assert value[2:0] == 0b101
    assert value[2:-3] == 0b101101


def test_unsigned_bit_indexing_excludes_the_backing_sign_bit():
    value = APyUFixed(0b1111, int_bits=4, frac_bits=0)

    assert value[3:1] == 0b111
    with pytest.raises(IndexError):
        _ = value[4]
    with pytest.raises(IndexError):
        _ = value[-1]


def test_unsigned_complement_and_shifts():
    value = APyUFixed.from_float(1.5, int_bits=2, frac_bits=2)

    assert (~value).to_bits() == 9
    assert float(value << 1) == 3.0
    assert float(value >> 1) == 0.75
