import pytest

from apytypes import APyUFixed, OverflowMode


def test_unsigned_wrap_cast():
    wrapped = APyUFixed(15, int_bits=4, frac_bits=0).cast(
        bits=3, int_bits=3, overflow=OverflowMode.WRAP
    )

    assert wrapped.to_bits() == 7


@pytest.mark.parametrize(
    ("overflow", "expected_bits"),
    [(OverflowMode.WRAP, 0), (OverflowMode.SAT, 7)],
)
def test_unsigned_cast_overflow_modes(overflow, expected_bits):
    value = APyUFixed(8, int_bits=4, frac_bits=0)

    result = value.cast(int_bits=3, overflow=overflow)

    assert result.to_bits() == expected_bits
    assert result.int_bits == 3
    assert result.frac_bits == 0
