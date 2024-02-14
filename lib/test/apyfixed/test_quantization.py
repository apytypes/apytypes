from apytypes import APyFixed
from apytypes import QuantizationMode

import pytest


def test_trn():
    mode = QuantizationMode.TRN
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).resize(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, mode)) == 0.75


def test_trn_inf():
    mode = QuantizationMode.TRN_INF
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).resize(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 2, mode)) == 1.0


def test_trn_zero():
    mode = QuantizationMode.TRN_ZERO
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(2, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 2, mode)) == 0.5


def test_rnd():
    mode = QuantizationMode.RND
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(2, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, mode)) == -1.0


def test_rnd_zero():
    mode = QuantizationMode.RND_ZERO
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(2, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, mode)) == 0.75


def test_rnd_inf():
    mode = QuantizationMode.RND_INF
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).resize(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.875, 4, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.25, 4, 2).resize(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 4, 2).resize(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 4, 2).resize(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(-0.25, 4, 2).resize(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 4, 2).resize(2, 2, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 2).resize(2, 2, mode)) == -1.0


def test_rnd_min_inf():
    mode = QuantizationMode.RND_MIN_INF
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 1).resize(3, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 4, 1).resize(3, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 2, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.875, 4, 1).resize(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.25, 4, 2).resize(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 4, 2).resize(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(0.75, 4, 2).resize(2, 2, mode)) == 1.0
    assert float(APyFixed.from_float(-0.25, 4, 2).resize(2, 2, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 4, 2).resize(2, 2, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 4, 2).resize(2, 2, mode)) == -1.0


def test_rnd_conv():
    mode = QuantizationMode.RND_CONV
    assert float(APyFixed.from_float(-0.75, 3, 1).resize(5, 1, mode)) == -0.75
    assert float(APyFixed.from_float(-0.25, 5, 3).resize(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 5, 3).resize(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(-0.75, 5, 3).resize(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.25, 5, 3).resize(3, 3, mode)) == -1.0
    assert float(APyFixed.from_float(-1.50, 5, 3).resize(3, 3, mode)) == -2.0
    assert float(APyFixed.from_float(-1.75, 5, 3).resize(3, 3, mode)) == -2.0
    assert float(APyFixed.from_float(0.25, 5, 3).resize(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 5, 3).resize(3, 3, mode)) == 0.0
    assert float(APyFixed.from_float(0.75, 5, 3).resize(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.25, 5, 3).resize(3, 3, mode)) == 1.0
    assert float(APyFixed.from_float(1.50, 5, 3).resize(3, 3, mode)) == 2.0
    assert float(APyFixed.from_float(1.75, 5, 3).resize(3, 3, mode)) == 2.0


# def test_rnd_conv_odd():
#    mode = QuantizationMode.RND_CONV_ODD
#    assert float(APyFixed.from_float(-0.75, 3, 1).resize(5, 1, mode)) == -0.75
#    assert float(APyFixed.from_float(-0.75, 3, 1).resize(2, 1, mode)) == -1.0
#    assert float(APyFixed.from_float(-0.75, 4, 1).resize(3, 1, mode)) == -0.75
#    assert float(APyFixed.from_float(-0.875, 4, 1).resize(3, 1, mode)) == -1.0
#    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 2, mode)) == 1.0
#    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, mode)) == 0.5
#    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, mode)) == 0.75
#    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 2, mode)) == 1.0
#    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, mode)) == 0.75
#    assert float(APyFixed.from_float(0.875, 4, 1).resize(2, 1, mode)) == -1.0
#    assert float(APyFixed.from_float(0.875, 4, 1).resize(2, 1, mode)) == -1.0


def test_jam():
    pass


def test_jam_unbiased():
    pass


def test_stochastic_weighted():
    pass


def test_stochastic_equal():
    pass


# All of the non-implemented Python quantization methods for APyFixed
@pytest.mark.parametrize(
    "mode",
    [
        QuantizationMode.RND_CONV_ODD,
        QuantizationMode.JAM,
        QuantizationMode.JAM_UNBIASED,
        QuantizationMode.STOCHASTIC_WEIGHTED,
        QuantizationMode.STOCHASTIC_EQUAL,
    ],
)
def test_not_implemented(mode):
    with pytest.raises(RuntimeError, match=r"Not implemented"):
        APyFixed(0, 1, 0).resize(bits=1, int_bits=0, quantization=mode)
