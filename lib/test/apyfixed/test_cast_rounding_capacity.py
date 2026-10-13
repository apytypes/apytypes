from fractions import Fraction

import pytest

from apytypes import APyFixed, APyFixedArray, APyFloat, QuantizationMode


@pytest.mark.parametrize("bits", [64, 128])
def test_rounding_at_limb_boundary_keeps_positive_sign(bits):
    scalar = APyFixed.from_float(0.75, bits=bits, int_bits=1)
    array = APyFixedArray.from_float([0.75, -0.75], bits=bits, int_bits=1)

    rounded = scalar.cast(
        int_bits=2, frac_bits=0, quantization=QuantizationMode.RND_CONV
    )
    rounded_array = array.cast(
        int_bits=2, frac_bits=0, quantization=QuantizationMode.RND_CONV
    )
    assert rounded.to_fraction() == Fraction(1)
    assert rounded_array[0].to_fraction() == Fraction(1)
    assert rounded_array[1].to_fraction() == Fraction(-1)


def test_large_positive_integer_rounds_to_positive_binary64_bits():
    value = APyFloat.from_float((1 << 63) - 1, exp_bits=11, man_bits=52)
    assert value.to_bits() == 0x43E0000000000000
