from fractions import Fraction

import pytest

from apytypes import APyFixed, APyFixedArray, APyFloat, QuantizationMode


def test_large_positive_integer_rounds_to_positive_binary64_bits():
    """
    The `from_float` methods uses `RND_CONV` for correct rounding. This was likely the
    original observation point for the bugs tested in this file. The original issue (GH:
    #1043) was removed by its creator from the Github issue tracker. See GH: #1045 for
    the original PR that first proposed a fix.
    """
    value = APyFloat.from_float((1 << 63) - 1, exp_bits=11, man_bits=52)
    assert value.to_bits() == 0x43E0000000000000


@pytest.mark.parametrize("bits", [10, 32, 63, 64, 65, 127, 128, 129])
@pytest.mark.parametrize(
    "mode",
    [
        "RND",  # TIES_POS
        "RND_ZERO",  # TIES_ZERO
        "RND_INF",  # TIES_AWAY
        "RND_MIN_INF",  # TIES_NEG
        "RND_CONV",  # TIES_EVEN
        "RND_CONV_ODD",  # TIES_ODD
    ],
)
def test_simple_cast_increasing_int_bits(bits: int, mode: str):
    x1 = APyFixed.from_float(0.75, bits=bits, int_bits=1)
    x2 = APyFixedArray.from_float([0.75, -0.75], bits=bits, int_bits=1)
    y1 = x1.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    y2 = x2.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    assert y1.to_fraction() == Fraction(1)
    assert y2[0].to_fraction() == Fraction(1)
    assert y2[1].to_fraction() == Fraction(-1)


@pytest.mark.parametrize("bits", [10, 32, 63, 64, 65, 127, 128, 129])
@pytest.mark.parametrize("mode", ["TRN"])  # TO_NEG
def test_simple_cast_increasing_int_bits_trn(bits: int, mode: str):
    x1 = APyFixed.from_float(0.75, bits=bits, int_bits=1)
    x2 = APyFixedArray.from_float([0.75, -0.75], bits=bits, int_bits=1)
    y1 = x1.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    y2 = x2.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    assert y1.to_fraction() == Fraction(0)
    assert y2[0].to_fraction() == Fraction(0)
    assert y2[1].to_fraction() == Fraction(-1)


@pytest.mark.parametrize("bits", [10, 32, 63, 64, 65, 127, 128, 129])
@pytest.mark.parametrize("mode", ["TRN_INF"])  # TO_POS
def test_simple_cast_increasing_int_bits_trn_inf(bits: int, mode: str):
    x1 = APyFixed.from_float(0.75, bits=bits, int_bits=1)
    x2 = APyFixedArray.from_float([0.75, -0.75], bits=bits, int_bits=1)
    y1 = x1.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    y2 = x2.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    assert y1.to_fraction() == Fraction(1)
    assert y2[0].to_fraction() == Fraction(1)
    assert y2[1].to_fraction() == Fraction(0)


@pytest.mark.parametrize("bits", [10, 32, 63, 64, 65, 127, 128, 129])
@pytest.mark.parametrize("mode", ["TRN_ZERO", "TRN_MAG"])  # TO_ZERO
def test_simple_cast_increasing_int_bits_trn_zero(bits: int, mode: str):
    x1 = APyFixed.from_float(0.75, bits=bits, int_bits=1)
    x2 = APyFixedArray.from_float([0.75, -0.75], bits=bits, int_bits=1)
    y1 = x1.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    y2 = x2.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    assert y1.to_fraction() == Fraction(0)
    assert y2[0].to_fraction() == Fraction(0)
    assert y2[1].to_fraction() == Fraction(0)


@pytest.mark.parametrize("bits", [10, 32, 63, 64, 65, 127, 128, 129])
@pytest.mark.parametrize("mode", ["TRN_AWAY"])  # TO_AWAY
def test_simple_cast_increasing_int_bits_trn_away(bits: int, mode: str):
    x1 = APyFixed.from_float(0.75, bits=bits, int_bits=1)
    x2 = APyFixedArray.from_float([0.75, -0.75], bits=bits, int_bits=1)
    y1 = x1.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    y2 = x2.cast(int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode))
    assert y1.to_fraction() == Fraction(1)
    assert y2[0].to_fraction() == Fraction(1)
    assert y2[1].to_fraction() == Fraction(-1)


@pytest.mark.parametrize("bits", [10, 32, 63, 64, 65, 127, 128, 129])
@pytest.mark.parametrize("mode", ["STOCH_EQUAL", "STOCH_WEIGHTED"])  # TO_AWAY
def test_simple_cast_increasing_int_bits_stochastic(bits: int, mode: str):
    x1 = APyFixed.from_float(0.75, bits=bits, int_bits=1)
    x2 = APyFixedArray.from_float([0.75, -0.75], bits=bits, int_bits=1)
    for _ in range(100):
        # Chance to trigger bug per trial     : 0.5
        # Chance to trigger bug in 100 trials : 1.0 - 7.8e-31
        y1 = x1.cast(
            int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode)
        )
        y2 = x2.cast(
            int_bits=2, frac_bits=0, quantization=getattr(QuantizationMode, mode)
        )
        assert y1.to_fraction() in (Fraction(1), Fraction(0))
        assert y2[0].to_fraction() in (Fraction(1), Fraction(0))
        assert y2[1].to_fraction() in (Fraction(-1), Fraction(0))
