from apytypes import APyFixed
from apytypes import QuantizationMode

import pytest


def test_trn():
    mode = QuantizationMode.TRN
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(2, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 1, 3).cast(1, 2, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(1, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 2, mode)) == 0.75

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_trn_inf():
    mode = QuantizationMode.TRN_INF
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 1, 2).cast(2, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(1, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 2, mode)) == -1.0
    assert float(APyFixed.from_float(0.875, 1, 3).cast(2, 1, mode)) == 1.0

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_trn_away():
    mode = QuantizationMode.TRN_AWAY
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 1, 3).cast(1, 2, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(2, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(1, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 2, mode)) == -1.0
    assert float(APyFixed.from_float(0.875, 1, 3).cast(2, 1, mode)) == 1.0

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_trn_zero():
    mode = QuantizationMode.TRN_ZERO
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 1, 2).cast(2, 0, mode)) == 0.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(1, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(2, 1, mode)) == 0.5

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_trn_mag():
    mode = QuantizationMode.TRN_MAG
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 1, 3).cast(1, 2, mode)) == -0.5
    assert float(APyFixed.from_float(-0.875, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 1, 2).cast(1, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 2, mode)) == 0.75

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_rnd():
    mode = QuantizationMode.RND
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 1, 2).cast(2, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(1, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(2, 1, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 2, mode)) == -1.0

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_rnd_zero():
    mode = QuantizationMode.RND_ZERO
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 1, mode)) == -0.5
    assert float(APyFixed.from_float(-0.75, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(0.75, 1, 2).cast(2, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(1, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(2, 1, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 2, mode)) == 0.75

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_rnd_inf():
    mode = QuantizationMode.RND_INF
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 1, 3).cast(1, 2, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(2, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(1, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(2, 1, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 2, mode)) == -1.0
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.25, 2, 2).cast(2, 0, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 2, 2).cast(2, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 2, 2).cast(2, 0, mode)) == 1.0
    assert float(APyFixed.from_float(-0.25, 2, 2).cast(2, 0, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 2, 2).cast(2, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 2, 2).cast(2, 0, mode)) == -1.0

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_rnd_min_inf():
    mode = QuantizationMode.RND_MIN_INF
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 1, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 1, 3).cast(1, 2, mode)) == -0.75
    assert float(APyFixed.from_float(-0.875, 1, 3).cast(1, 2, mode)) == -1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(2, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 1, 2).cast(1, 1, mode)) == 0.5
    assert float(APyFixed.from_float(0.75, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(2, 1, mode)) == 1.0
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 2, mode)) == 0.75
    assert float(APyFixed.from_float(0.875, 1, 3).cast(1, 1, mode)) == -1.0
    assert float(APyFixed.from_float(0.25, 2, 2).cast(2, 0, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 2, 2).cast(2, 0, mode)) == 0.0
    assert float(APyFixed.from_float(0.75, 2, 2).cast(2, 0, mode)) == 1.0
    assert float(APyFixed.from_float(-0.25, 2, 2).cast(2, 0, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 2, 2).cast(2, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 2, 2).cast(2, 0, mode)) == -1.0

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_rnd_conv():
    mode = QuantizationMode.RND_CONV
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.25, 3, 2).cast(3, 0, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 3, 2).cast(3, 0, mode)) == 0.0
    assert float(APyFixed.from_float(-0.75, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.25, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.50, 3, 2).cast(3, 0, mode)) == -2.0
    assert float(APyFixed.from_float(-1.75, 3, 2).cast(3, 0, mode)) == -2.0
    assert float(APyFixed.from_float(0.25, 3, 2).cast(3, 0, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 3, 2).cast(3, 0, mode)) == 0.0
    assert float(APyFixed.from_float(0.75, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.25, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.50, 3, 2).cast(3, 0, mode)) == 2.0
    assert float(APyFixed.from_float(1.75, 3, 2).cast(3, 0, mode)) == 2.0

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_rnd_conv_odd():
    mode = QuantizationMode.RND_CONV_ODD
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75
    assert float(APyFixed.from_float(-0.25, 3, 2).cast(3, 0, mode)) == 0.0
    assert float(APyFixed.from_float(-0.50, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.25, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.50, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.75, 3, 2).cast(3, 0, mode)) == -2.0
    assert float(APyFixed.from_float(0.25, 3, 2).cast(3, 0, mode)) == 0.0
    assert float(APyFixed.from_float(0.50, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.25, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.50, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.75, 3, 2).cast(3, 0, mode)) == 2.0

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 0.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


def test_jam():
    mode = QuantizationMode.JAM
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.6875

    assert float(APyFixed.from_float(-2.25, 3, 2).cast(3, 0, mode)) == -3.0
    assert float(APyFixed.from_float(-2.00, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.75, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.50, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.25, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.00, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-0.50, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-0.25, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(0.00, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.25, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.50, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.00, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.25, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.50, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.75, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(2.00, 3, 2).cast(3, 0, mode)) == 3.0
    assert float(APyFixed.from_float(2.25, 3, 2).cast(3, 0, mode)) == 3.0

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 1.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 1.0


def test_jam_unbiased():
    mode = QuantizationMode.JAM_UNBIASED
    assert float(APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode)) == -0.75

    assert float(APyFixed.from_float(-2.25, 3, 2).cast(3, 0, mode)) == -3.0
    assert float(APyFixed.from_float(-2.00, 3, 2).cast(3, 0, mode)) == -2.0
    assert float(APyFixed.from_float(-1.75, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.50, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.25, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-1.00, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-0.75, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-0.50, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(-0.25, 3, 2).cast(3, 0, mode)) == -1.0
    assert float(APyFixed.from_float(0.00, 3, 2).cast(3, 0, mode)) == 0.0
    assert float(APyFixed.from_float(0.25, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.50, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(0.75, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.00, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.25, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.50, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(1.75, 3, 2).cast(3, 0, mode)) == 1.0
    assert float(APyFixed.from_float(2.00, 3, 2).cast(3, 0, mode)) == 2.0
    assert float(APyFixed.from_float(2.25, 3, 2).cast(3, 0, mode)) == 3.0

    assert float(APyFixed(0b10000, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b11111, 0, 5).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b01001, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b01000, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00001, 0, 5).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00000, 0, 5).cast(5, 0, mode)) == 0.0

    assert float(APyFixed(0b10000, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b10001, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b11111, -1, 6).cast(5, 0, mode)) == -1.0
    assert float(APyFixed(0b01001, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b01000, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00001, -1, 6).cast(5, 0, mode)) == 1.0
    assert float(APyFixed(0b00000, -1, 6).cast(5, 0, mode)) == 0.0


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
    assert float(APyFixed.from_float(-0.75, 1000, 500).cast(1, 4, mode)) == -0.75


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
    assert float(APyFixed.from_float(-0.75, 5, 2).cast(500, 500, mode)) == -0.75


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
    APyFixed.from_float(123, int_bits=119, frac_bits=0).cast(4, 0)


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
    assert c.is_identical(APyFixed.from_float(0, bits=64, int_bits=64))

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
def test_issue_335(mode):
    # Smoke test for GitHub issue #335.
    # These tests are designed to trigger invalid memory access using Valgrind memcheck.
    # Please don't remove it.
    a = APyFixed(0, bits=62002, int_bits=-12984)
    assert float(a.cast(63, 3, quantization=mode)) == 0.0


def test_quantize_away_all_bits():
    # Quantize away all of the bits. These are non-realistic quantizations, but need to
    # work nonetheless.

    mode = QuantizationMode.TRN
    assert (
        APyFixed(0b00, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(0, bits=200, int_bits=400))
    )
    assert (
        APyFixed(0b01, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(0, bits=200, int_bits=400))
    )
    assert (
        APyFixed(0b11, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(-1, bits=200, int_bits=400))
    )

    mode = QuantizationMode.TRN_AWAY
    assert (
        APyFixed(0b00, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(0, bits=200, int_bits=400))
    )
    assert (
        APyFixed(0b01, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(1, bits=200, int_bits=400))
    )
    assert (
        APyFixed(0b11, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(-1, bits=200, int_bits=400))
    )

    mode = QuantizationMode.TRN_ZERO
    assert (
        APyFixed(0b00, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(0, bits=200, int_bits=400))
    )
    assert (
        APyFixed(0b01, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(0, bits=200, int_bits=400))
    )
    assert (
        APyFixed(0b11, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(0, bits=200, int_bits=400))
    )

    mode = QuantizationMode.TRN_MAG
    assert (
        APyFixed(0b00, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(0, bits=200, int_bits=400))
    )
    assert (
        APyFixed(0b01, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(0, bits=200, int_bits=400))
    )
    assert (
        APyFixed(0b11, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(APyFixed(0, bits=200, int_bits=400))
    )

    round_nearest_modes = [
        QuantizationMode.RND,
        QuantizationMode.RND_ZERO,
        QuantizationMode.RND_INF,
        QuantizationMode.RND_MIN_INF,
        QuantizationMode.RND_CONV,
        QuantizationMode.RND_CONV_ODD,
    ]
    for mode in round_nearest_modes:
        assert (
            APyFixed(0b00, bits=2, int_bits=0)
            .cast(bits=200, int_bits=400, quantization=mode)
            .is_identical(APyFixed(0, bits=200, int_bits=400))
        )
        assert (
            APyFixed(0b01, bits=2, int_bits=0)
            .cast(bits=200, int_bits=400, quantization=mode)
            .is_identical(APyFixed(0, bits=200, int_bits=400))
        )
        assert (
            APyFixed(0b11, bits=2, int_bits=0)
            .cast(bits=200, int_bits=400, quantization=mode)
            .is_identical(APyFixed(0, bits=200, int_bits=400))
        )


def test_one_specifier():
    # It is possible to specify only `int_bits` or `frac_bits`. Doing so preserves the
    # other specifier.
    a = APyFixed(0b0011_1100, int_bits=4, frac_bits=4)
    assert float(a) == 3.75
    assert float(a.cast(1)) == -0.25
    assert float(a.cast(int_bits=1)) == -0.25
    assert float(a.cast(frac_bits=1)) == 3.5
