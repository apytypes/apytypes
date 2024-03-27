from apytypes import APyFixed
from apytypes import QuantizationMode

import pytest


def test_trn():
    mode = QuantizationMode.TRN
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).cast(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 1, mode)) == 0.75


def test_trn_inf():
    mode = QuantizationMode.TRN_INF
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).cast(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 2, mode)) == 1.0


def test_trn_zero():
    mode = QuantizationMode.TRN_ZERO
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(2, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 2, mode)) == 0.5


def test_trn_mag():
    mode = QuantizationMode.TRN_MAG
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(2, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 4, 1).cast(3, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.875, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 1, mode)) == 0.75


def test_rnd():
    mode = QuantizationMode.RND
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(2, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 1, mode)) == -1.0


def test_rnd_zero():
    mode = QuantizationMode.RND_ZERO
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(2, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 1, mode)) == 0.75


def test_rnd_inf():
    mode = QuantizationMode.RND_INF
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).cast(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.875, 4, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.25, 4, 2).cast(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 4, 2).cast(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 4, 2).cast(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(-0.25, 4, 2).cast(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 4, 2).cast(2, 2, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 2).cast(2, 2, mode)) == -1.0


def test_rnd_min_inf():
    mode = QuantizationMode.RND_MIN_INF
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 1).cast(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).cast(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).cast(2, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 4, 1).cast(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.875, 4, 1).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.25, 4, 2).cast(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 4, 2).cast(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(0.75, 4, 2).cast(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(-0.25, 4, 2).cast(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 4, 2).cast(2, 2, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 2).cast(2, 2, mode)) == -1.0


def test_rnd_conv():
    mode = QuantizationMode.RND_CONV
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.25, 5, 3).cast(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 5, 3).cast(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(-0.75, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.25, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.50, 5, 3).cast(3, 3, mode)) == -2.0
    assert float(APyFixed.from_float(-1.75, 5, 3).cast(3, 3, mode)) == -2.0
    assert float(APyFixed.from_float(0.25, 5, 3).cast(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 5, 3).cast(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(0.75, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.25, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.50, 5, 3).cast(3, 3, mode)) == 2.0
    assert float(APyFixed.from_float(1.75, 5, 3).cast(3, 3, mode)) == 2.0


def test_rnd_conv_odd():
    mode = QuantizationMode.RND_CONV_ODD
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.25, 5, 3).cast(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.25, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.50, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.75, 5, 3).cast(3, 3, mode)) == -2.0
    assert float(APyFixed.from_float(0.25, 5, 3).cast(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.25, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.50, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.75, 5, 3).cast(3, 3, mode)) == 2.0


def test_jam():
    mode = QuantizationMode.JAM
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.6875

    assert float(APyFixed.from_float(-2.25, 5, 3).cast(3, 3, mode)) == -3.0
    assert float(APyFixed.from_float(-2.00, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.75, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.50, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.25, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.00, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-0.50, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-0.25, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(0.00, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(0.25, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(0.50, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.00, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.25, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.50, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.75, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(2.00, 5, 3).cast(3, 3, mode)) == 3.0
    assert float(APyFixed.from_float(2.25, 5, 3).cast(3, 3, mode)) == 3.0


def test_jam_unbiased():
    mode = QuantizationMode.JAM_UNBIASED
    assert float(APyFixed.from_float(-0.75, 3, 1).cast(5, 1, mode)) == -0.75

    assert float(APyFixed.from_float(-2.25, 5, 3).cast(3, 3, mode)) == -3.0
    assert float(APyFixed.from_float(-2.00, 5, 3).cast(3, 3, mode)) == -2.0
    assert float(APyFixed.from_float(-1.75, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.50, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.25, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.00, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-0.50, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-0.25, 5, 3).cast(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(0.00, 5, 3).cast(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(0.25, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(0.50, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.00, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.25, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.50, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.75, 5, 3).cast(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(2.00, 5, 3).cast(3, 3, mode)) == 2.0
    assert float(APyFixed.from_float(2.25, 5, 3).cast(3, 3, mode)) == 3.0


def test_stochastic_weighted():
    pass


def test_stochastic_equal():
    pass


@pytest.mark.parametrize(
    "mode",
    [
        QuantizationMode.TRN,
        QuantizationMode.TRN_INF,
        QuantizationMode.TRN_ZERO,
        QuantizationMode.RND,
        QuantizationMode.RND_INF,
        QuantizationMode.RND_MIN_INF,
        QuantizationMode.RND_ZERO,
        QuantizationMode.RND_CONV,
        QuantizationMode.RND_CONV_ODD,
        QuantizationMode.JAM_UNBIASED,
    ],
)
def test_huge_narrowing_cast(mode):
    assert float(APyFixed.from_float(-0.75, 1000, 500).cast(5, 1, mode)) == -0.75


@pytest.mark.parametrize(
    "mode",
    [
        QuantizationMode.TRN,
        QuantizationMode.TRN_INF,
        QuantizationMode.TRN_ZERO,
        QuantizationMode.RND,
        QuantizationMode.RND_INF,
        QuantizationMode.RND_MIN_INF,
        QuantizationMode.RND_ZERO,
        QuantizationMode.RND_CONV,
        QuantizationMode.RND_CONV_ODD,
        QuantizationMode.JAM_UNBIASED,
    ],
)
def test_huge_extending_cast(mode):
    assert float(APyFixed.from_float(-0.75, 5, 2).cast(1000, 500, mode)) == -0.75


# All of the non-implemented Python quantization methods for APyFixed
@pytest.mark.parametrize(
    "mode",
    [
        QuantizationMode.STOCH_WEIGHTED,
        QuantizationMode.STOCH_EQUAL,
    ],
)
def test_not_implemented(mode):
    with pytest.raises(ValueError, match=r"Not implemented"):
        APyFixed(0, 1, 0).cast(bits=1, int_bits=0, quantization=mode)


def test_issue_112():
    # Smoke test to make sure that it doesn't seg-fault
    APyFixed.from_float(123, int_bits=119, frac_bits=0).cast(4, 4)


def test_issue_179():
    #
    # https://github.com/apytypes/apytypes/issues/179
    #
    # C++ function `limb_vector_add_pow2(n)` out-of-bounds access when `n` >= `n_limbs`.
    # Test all corner out-of-bound cases for `limb_vector_add_pow2()`
    #
    a = APyFixed(0x8000000000000000, bits=64, int_bits=0)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert float(b) == -1.0
    assert float(c) == 0.0

    a = APyFixed(0x7FFFFFFFFFFFFFFF, bits=64, int_bits=-128)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b.is_identical(APyFixed(0x0, bits=64, int_bits=64))
    assert c.is_identical(APyFixed(0x0, bits=64, int_bits=64))

    a = APyFixed(0xFFFFFFFFFFFFFFFF, bits=64, int_bits=-128)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b.is_identical(APyFixed.from_float(-1, bits=64, int_bits=64))
    assert c.is_identical(APyFixed.from_float(-1, bits=64, int_bits=64))

    a = APyFixed(0x7FFFFFFFFFFFFFFF, bits=64, int_bits=128)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b.is_identical(APyFixed(0, bits=64, int_bits=64))
    assert c.is_identical(APyFixed(0, bits=64, int_bits=64))

    a = APyFixed(0xFFFFFFFFFFFFFFFF, bits=64, int_bits=128)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b.is_identical(APyFixed.from_float(0, bits=64, int_bits=64))
    assert c.is_identical(APyFixed.from_float(0, bits=64, int_bits=64))
