from apytypes import APyFixed, APyCFixed
from apytypes import QuantizationMode

import pytest


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_trn(fixed_type, im):
    mode = QuantizationMode.TRN
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(2, 1, mode) == -1.0 * im
    assert fixed_type.from_float(-0.75 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(-0.875 * im, 1, 3).cast(1, 2, mode) == -1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(1, 1, mode) == 0.5 * im
    assert fixed_type.from_float(0.75 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 2, mode) == 0.75 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.015625, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_trn_inf(fixed_type, im):
    mode = QuantizationMode.TRN_INF
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 1, mode) == -0.5 * im
    assert fixed_type.from_float(-0.75 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(-0.875 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(2, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(1, 1, mode) == -1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 2, mode) == -1.0 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(2, 1, mode) == 1.0 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_trn_away(fixed_type, im):
    mode = QuantizationMode.TRN_AWAY
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 1, mode) == -1.0 * im
    assert fixed_type.from_float(-0.75 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(-0.875 * im, 1, 3).cast(1, 2, mode) == -1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(2, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(1, 1, mode) == -1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 2, mode) == -1.0 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(2, 1, mode) == 1.0 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_trn_zero(fixed_type, im):
    mode = QuantizationMode.TRN_ZERO
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 1, mode) == -0.5 * im
    assert fixed_type.from_float(-0.75 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(-0.875 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(2, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(1, 1, mode) == 0.5 * im
    assert fixed_type.from_float(0.75 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(2, 1, mode) == 0.5 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_trn_mag(fixed_type, im):
    mode = QuantizationMode.TRN_MAG
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 1, mode) == -0.5 * im
    assert fixed_type.from_float(-0.75 * im, 1, 3).cast(1, 2, mode) == -0.5 * im
    assert fixed_type.from_float(-0.875 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(1, 1, mode) == 0.5 * im
    assert fixed_type.from_float(0.75 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 2, mode) == 0.75 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_rnd(fixed_type, im):
    mode = QuantizationMode.RND
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 1, mode) == -0.5 * im
    assert fixed_type.from_float(-0.75 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(-0.875 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(2, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(1, 1, mode) == -1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(2, 1, mode) == 1.0 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 2, mode) == -1.0 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_rnd_zero(fixed_type, im):
    mode = QuantizationMode.RND_ZERO
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 1, mode) == -0.5 * im
    assert fixed_type.from_float(-0.75 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(-0.875 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(2, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(1, 1, mode) == 0.5 * im
    assert fixed_type.from_float(0.75 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(2, 1, mode) == 1.0 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 2, mode) == 0.75 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_rnd_inf(fixed_type, im):
    mode = QuantizationMode.RND_INF
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 1, mode) == -1.0 * im
    assert fixed_type.from_float(-0.75 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(-0.875 * im, 1, 3).cast(1, 2, mode) == -1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(2, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(1, 1, mode) == -1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(2, 1, mode) == 1.0 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 2, mode) == -1.0 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 1, mode) == -1.0 * im
    assert fixed_type.from_float(0.25 * im, 2, 2).cast(2, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.50 * im, 2, 2).cast(2, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.75 * im, 2, 2).cast(2, 0, mode) == 1.0 * im
    assert fixed_type.from_float(-0.25 * im, 2, 2).cast(2, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.50 * im, 2, 2).cast(2, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.75 * im, 2, 2).cast(2, 0, mode) == -1.0 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_rnd_min_inf(fixed_type, im):
    mode = QuantizationMode.RND_MIN_INF
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 1, mode) == -1.0 * im
    assert fixed_type.from_float(-0.75 * im, 1, 3).cast(1, 2, mode) == -0.75 * im
    assert fixed_type.from_float(-0.875 * im, 1, 3).cast(1, 2, mode) == -1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(2, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.75 * im, 1, 2).cast(1, 1, mode) == 0.5 * im
    assert fixed_type.from_float(0.75 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(2, 1, mode) == 1.0 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 2, mode) == 0.75 * im
    assert fixed_type.from_float(0.875 * im, 1, 3).cast(1, 1, mode) == -1.0 * im
    assert fixed_type.from_float(0.25 * im, 2, 2).cast(2, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.50 * im, 2, 2).cast(2, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.75 * im, 2, 2).cast(2, 0, mode) == 1.0 * im
    assert fixed_type.from_float(-0.25 * im, 2, 2).cast(2, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.50 * im, 2, 2).cast(2, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.75 * im, 2, 2).cast(2, 0, mode) == -1.0 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_rnd_conv(fixed_type, im):
    mode = QuantizationMode.RND_CONV
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.25 * im, 3, 2).cast(3, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.50 * im, 3, 2).cast(3, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.75 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-1.25 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-1.50 * im, 3, 2).cast(3, 0, mode) == -2.0 * im
    assert fixed_type.from_float(-1.75 * im, 3, 2).cast(3, 0, mode) == -2.0 * im
    assert fixed_type.from_float(0.25 * im, 3, 2).cast(3, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.50 * im, 3, 2).cast(3, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.75 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(1.25 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(1.50 * im, 3, 2).cast(3, 0, mode) == 2.0 * im
    assert fixed_type.from_float(1.75 * im, 3, 2).cast(3, 0, mode) == 2.0 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_rnd_conv_odd(fixed_type, im):
    mode = QuantizationMode.RND_CONV_ODD
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-0.25 * im, 3, 2).cast(3, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.50 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.75 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-1.25 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-1.50 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-1.75 * im, 3, 2).cast(3, 0, mode) == -2.0 * im
    assert fixed_type.from_float(0.25 * im, 3, 2).cast(3, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.50 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.75 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(1.25 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(1.50 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(1.75 * im, 3, 2).cast(3, 0, mode) == 2.0 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


def test_jam():
    mode = QuantizationMode.JAM
    assert APyFixed.from_float(-0.75, 1, 2).cast(1, 4, mode) == -0.6875
    assert APyFixed.from_float(-2.25, 3, 2).cast(3, 0, mode) == -3.0
    assert APyFixed.from_float(-2.00, 3, 2).cast(3, 0, mode) == -1.0
    assert APyFixed.from_float(-1.75, 3, 2).cast(3, 0, mode) == -1.0
    assert APyFixed.from_float(-1.50, 3, 2).cast(3, 0, mode) == -1.0
    assert APyFixed.from_float(-1.25, 3, 2).cast(3, 0, mode) == -1.0
    assert APyFixed.from_float(-1.00, 3, 2).cast(3, 0, mode) == -1.0
    assert APyFixed.from_float(-0.75, 3, 2).cast(3, 0, mode) == -1.0
    assert APyFixed.from_float(-0.50, 3, 2).cast(3, 0, mode) == -1.0
    assert APyFixed.from_float(-0.25, 3, 2).cast(3, 0, mode) == -1.0
    assert APyFixed.from_float(0.00, 3, 2).cast(3, 0, mode) == 1.0
    assert APyFixed.from_float(0.25, 3, 2).cast(3, 0, mode) == 1.0
    assert APyFixed.from_float(0.50, 3, 2).cast(3, 0, mode) == 1.0
    assert APyFixed.from_float(0.75, 3, 2).cast(3, 0, mode) == 1.0
    assert APyFixed.from_float(1.00, 3, 2).cast(3, 0, mode) == 1.0
    assert APyFixed.from_float(1.25, 3, 2).cast(3, 0, mode) == 1.0
    assert APyFixed.from_float(1.50, 3, 2).cast(3, 0, mode) == 1.0
    assert APyFixed.from_float(1.75, 3, 2).cast(3, 0, mode) == 1.0
    assert APyFixed.from_float(2.00, 3, 2).cast(3, 0, mode) == 3.0
    assert APyFixed.from_float(2.25, 3, 2).cast(3, 0, mode) == 3.0

    assert APyCFixed.from_float(-0.75, 1, 2).cast(1, 4, mode) == -0.6875 + 2**-4 * 1j
    assert APyCFixed.from_float(-2.25, 3, 2).cast(3, 0, mode) == -3.0 + 1j
    assert APyCFixed.from_float(-2.00, 3, 2).cast(3, 0, mode) == -1.0 + 1j
    assert APyCFixed.from_float(-1.75, 3, 2).cast(3, 0, mode) == -1.0 + 1j
    assert APyCFixed.from_float(-1.50, 3, 2).cast(3, 0, mode) == -1.0 + 1j
    assert APyCFixed.from_float(-1.25, 3, 2).cast(3, 0, mode) == -1.0 + 1j
    assert APyCFixed.from_float(-1.00, 3, 2).cast(3, 0, mode) == -1.0 + 1j
    assert APyCFixed.from_float(-0.75, 3, 2).cast(3, 0, mode) == -1.0 + 1j
    assert APyCFixed.from_float(-0.50, 3, 2).cast(3, 0, mode) == -1.0 + 1j
    assert APyCFixed.from_float(-0.25, 3, 2).cast(3, 0, mode) == -1.0 + 1j
    assert APyCFixed.from_float(0.00, 3, 2).cast(3, 0, mode) == 1.0 + 1j
    assert APyCFixed.from_float(0.25, 3, 2).cast(3, 0, mode) == 1.0 + 1j
    assert APyCFixed.from_float(0.50, 3, 2).cast(3, 0, mode) == 1.0 + 1j
    assert APyCFixed.from_float(0.75, 3, 2).cast(3, 0, mode) == 1.0 + 1j
    assert APyCFixed.from_float(1.00, 3, 2).cast(3, 0, mode) == 1.0 + 1j
    assert APyCFixed.from_float(1.25, 3, 2).cast(3, 0, mode) == 1.0 + 1j
    assert APyCFixed.from_float(1.50, 3, 2).cast(3, 0, mode) == 1.0 + 1j
    assert APyCFixed.from_float(1.75, 3, 2).cast(3, 0, mode) == 1.0 + 1j
    assert APyCFixed.from_float(2.00, 3, 2).cast(3, 0, mode) == 3.0 + 1j
    assert APyCFixed.from_float(2.25, 3, 2).cast(3, 0, mode) == 3.0 + 1j

    assert APyFixed.from_float(-0.5, 0, 5).cast(5, 0, mode) == -1.0
    assert APyFixed.from_float(-0.46875, 0, 5).cast(5, 0, mode) == -1.0
    assert APyFixed.from_float(-0.03125, 0, 5).cast(5, 0, mode) == -1.0
    assert APyFixed.from_float(0.28125, 0, 5).cast(5, 0, mode) == 1.0
    assert APyFixed.from_float(0.25, 0, 5).cast(5, 0, mode) == 1.0
    assert APyFixed.from_float(0.03125, 0, 5).cast(5, 0, mode) == 1.0
    assert APyFixed.from_float(0.0, 0, 5).cast(5, 0, mode) == 1.0

    assert APyFixed.from_float(-0.25, -1, 6).cast(5, 0, mode) == -1.0
    assert APyFixed.from_float(-0.234375, -1, 6).cast(5, 0, mode) == -1.0
    assert APyFixed.from_float(-0.015625, -1, 6).cast(5, 0, mode) == -1.0
    assert APyFixed.from_float(0.140625, -1, 6).cast(5, 0, mode) == 1.0
    assert APyFixed.from_float(0.125, -1, 6).cast(5, 0, mode) == 1.0
    assert APyFixed.from_float(0.015625, -1, 6).cast(5, 0, mode) == 1.0
    assert APyFixed.from_float(0.0, -1, 6).cast(5, 0, mode) == 1.0

    assert APyCFixed.from_float(-0.5j, 0, 5).cast(5, 0, mode) == -1.0j + 1.0
    assert APyCFixed.from_float(-0.46875j, 0, 5).cast(5, 0, mode) == -1.0j + 1.0
    assert APyCFixed.from_float(-0.03125j, 0, 5).cast(5, 0, mode) == -1.0j + 1.0
    assert APyCFixed.from_float(0.28125j, 0, 5).cast(5, 0, mode) == 1.0j + 1.0
    assert APyCFixed.from_float(0.25j, 0, 5).cast(5, 0, mode) == 1.0j + 1.0
    assert APyCFixed.from_float(0.03125j, 0, 5).cast(5, 0, mode) == 1.0j + 1.0
    assert APyCFixed.from_float(0.0j, 0, 5).cast(5, 0, mode) == 1.0j + 1.0

    assert APyCFixed.from_float(-0.25j, -1, 6).cast(5, 0, mode) == -1.0j + 1.0
    assert APyCFixed.from_float(-0.234375j, -1, 6).cast(5, 0, mode) == -1.0j + 1.0
    assert APyCFixed.from_float(-0.015625j, -1, 6).cast(5, 0, mode) == -1.0j + 1.0
    assert APyCFixed.from_float(0.140625j, -1, 6).cast(5, 0, mode) == 1.0j + 1.0
    assert APyCFixed.from_float(0.125j, -1, 6).cast(5, 0, mode) == 1.0j + 1.0
    assert APyCFixed.from_float(0.015625j, -1, 6).cast(5, 0, mode) == 1.0j + 1.0
    assert APyCFixed.from_float(0.0j, -1, 6).cast(5, 0, mode) == 1.0j + 1.0


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_jam_unbiased(fixed_type, im):
    mode = QuantizationMode.JAM_UNBIASED
    assert fixed_type.from_float(-0.75 * im, 1, 2).cast(1, 4, mode) == -0.75 * im
    assert fixed_type.from_float(-2.25 * im, 3, 2).cast(3, 0, mode) == -3.0 * im
    assert fixed_type.from_float(-2.00 * im, 3, 2).cast(3, 0, mode) == -2.0 * im
    assert fixed_type.from_float(-1.75 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-1.50 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-1.25 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-1.00 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.75 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.50 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.25 * im, 3, 2).cast(3, 0, mode) == -1.0 * im
    assert fixed_type.from_float(0.00 * im, 3, 2).cast(3, 0, mode) == 0.0 * im
    assert fixed_type.from_float(0.25 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.50 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.75 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(1.00 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(1.25 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(1.50 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(1.75 * im, 3, 2).cast(3, 0, mode) == 1.0 * im
    assert fixed_type.from_float(2.00 * im, 3, 2).cast(3, 0, mode) == 2.0 * im
    assert fixed_type.from_float(2.25 * im, 3, 2).cast(3, 0, mode) == 3.0 * im

    assert fixed_type.from_float(-0.5 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.46875 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.03125 * im, 0, 5).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(0.28125 * im, 0, 5).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.25 * im, 0, 5).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.03125 * im, 0, 5).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.0 * im, 0, 5).cast(5, 0, mode) == 0.0 * im

    assert fixed_type.from_float(-0.25 * im, -1, 6).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.234375 * im, -1, 6).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(-0.015625 * im, -1, 6).cast(5, 0, mode) == -1.0 * im
    assert fixed_type.from_float(0.140625 * im, -1, 6).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.125 * im, -1, 6).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.015625 * im, -1, 6).cast(5, 0, mode) == 1.0 * im
    assert fixed_type.from_float(0.0 * im, -1, 6).cast(5, 0, mode) == 0.0 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_stochastic_weighted(fixed_type, im):
    pass


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_stochastic_equal(fixed_type, im):
    pass


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
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
def test_huge_narrowing_cast(fixed_type, im, mode):
    assert fixed_type.from_float(-0.75 * im, 1000, 500).cast(1, 4, mode) == -0.75 * im


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
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
def test_huge_extending_cast(fixed_type, im, mode):
    assert fixed_type.from_float(-0.75 * im, 5, 2).cast(500, 500, mode) == -0.75 * im


# All of the non-implemented Python quantization methods for APyFixed
@pytest.mark.parametrize("fixed_type", [APyFixed, APyCFixed])
@pytest.mark.parametrize(
    "mode",
    [
        QuantizationMode.STOCH_WEIGHTED,
        QuantizationMode.STOCH_EQUAL,
    ],
)
def test_not_implemented(fixed_type, mode):
    with pytest.raises(ValueError, match=r"Not implemented"):
        fixed_type(0, 1, 0).cast(bits=1, int_bits=0, quantization=mode)


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_issue_112(fixed_type, im):
    # Smoke test to make sure that it doesn't seg-fault
    fixed_type.from_float(123 * im, int_bits=119, frac_bits=0).cast(4, 0)


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_issue_179(fixed_type, im):
    #
    # https://github.com/apytypes/apytypes/issues/179
    #
    # C++ function `limb_vector_add_pow2(n)` out-of-bounds access when `n` >= `n_limbs`.
    # Test all corner out-of-bound cases for `limb_vector_add_pow2()`
    #
    a = fixed_type.from_float(-0.5 * im, bits=64, int_bits=0)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b == -1.0 * im
    assert c == 0.0

    a = fixed_type(0x7FFFFFFFFFFFFFFF, bits=64, int_bits=0)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b == 0.0
    assert c == 0.0

    a = fixed_type(0x7FFFFFFFFFFFFFFF, bits=64, int_bits=-128)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b.is_identical(fixed_type(0x0, bits=64, int_bits=64))
    assert c.is_identical(fixed_type(0x0, bits=64, int_bits=64))

    a = fixed_type(0xFFFFFFFFFFFFFFFF, bits=64, int_bits=-128)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b.is_identical(fixed_type.from_float(-1, bits=64, int_bits=64))
    assert c.is_identical(fixed_type.from_float(0, bits=64, int_bits=64))

    a = fixed_type(0x7FFFFFFFFFFFFFFF, bits=64, int_bits=128)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b.is_identical(fixed_type(0, bits=64, int_bits=64))
    assert c.is_identical(fixed_type(0, bits=64, int_bits=64))

    a = fixed_type(0xFFFFFFFFFFFFFFFF, bits=64, int_bits=128)
    b = a.cast(bits=64, int_bits=64)
    c = a.cast(bits=64, int_bits=64, quantization=QuantizationMode.RND)
    assert b.is_identical(fixed_type.from_float(0, bits=64, int_bits=64))
    assert c.is_identical(fixed_type.from_float(0, bits=64, int_bits=64))


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_exactly_full_limb_quantize(fixed_type, im):
    # Truncate towrads minus infinity
    mode = QuantizationMode.TRN
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == -1.0 * im
    assert fx2 == -1.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 0.0 * im

    # Truncate towards plus infinity
    mode = QuantizationMode.TRN_INF
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == 0.0 * im
    assert fx2 == 0.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 1.0 * im

    # Truncate towards zero
    mode = QuantizationMode.TRN_ZERO
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == 0.0 * im
    assert fx2 == 0.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 0.0 * im

    # Truncate away from zero
    mode = QuantizationMode.TRN_AWAY
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == -1.0 * im
    assert fx2 == -1.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 1.0 * im

    # Fixed-point magnitude truncation
    mode = QuantizationMode.TRN_MAG
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == 0.0 * im
    assert fx2 == 0.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 0.0 * im

    # Round to nearest, ties toward plus infinity
    mode = QuantizationMode.RND
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == 0.0 * im
    assert fx2 == 0.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 0.0 * im

    # Round to nearest, ties towrard zero
    mode = QuantizationMode.RND_ZERO
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == 0.0 * im
    assert fx2 == 0.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 0.0 * im

    # Round to nearest, ties away from zero
    mode = QuantizationMode.RND_INF
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == -1.0 * im
    assert fx2 == 0.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 0.0 * im

    # Round to nearest, ties toward minus infinity
    mode = QuantizationMode.RND_MIN_INF
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == -1.0 * im
    assert fx2 == 0.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 0.0 * im

    # Round to nearest, ties toward even quantization steps
    mode = QuantizationMode.RND_CONV
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == 0.0 * im
    assert fx2 == 0.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 0.0 * im

    # Round to nearest, ties toward odd quantization steps
    mode = QuantizationMode.RND_CONV_ODD
    fx1 = fixed_type.from_float(-0.5 * im, 0, 64).cast(64, 0, quantization=mode)
    fx2 = fixed_type.from_float(-0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    fx3 = fixed_type.from_float(0.0 * im, 0, 64).cast(64, 0, quantization=mode)
    fx4 = fixed_type.from_float(0.25 * im, 0, 64).cast(64, 0, quantization=mode)
    assert fx1 == -1.0 * im
    assert fx2 == 0.0 * im
    assert fx3 == 0.0 * im
    assert fx4 == 0.0 * im


@pytest.mark.parametrize("fixed_type", [APyFixed, APyCFixed])
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
def test_issue_335(fixed_type, mode):
    # Smoke test for GitHub issue #335.
    # These tests are designed to trigger invalid memory access using Valgrind memcheck.
    # Please don't remove it.
    a = fixed_type(0, bits=62002, int_bits=-12984)
    assert a.cast(63, 3, quantization=mode) == 0.0


@pytest.mark.parametrize(
    "fixed_type, im", [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_quantize_away_all_bits(fixed_type, im):
    # Quantize away all of the bits. These are non-realistic quantizations, but need to
    # work nonetheless.

    mode = QuantizationMode.TRN
    assert (
        fixed_type.from_float(0.0 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(0.0 * im, bits=200, int_bits=400))
    )
    assert (
        fixed_type.from_float(0.25 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(0.0 * im, bits=200, int_bits=400))
    )
    assert (
        fixed_type.from_float(-0.25 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(-(2.0**200) * im, bits=200, int_bits=400))
    )

    mode = QuantizationMode.TRN_AWAY
    assert (
        fixed_type.from_float(0.0 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(0 * im, bits=200, int_bits=400))
    )
    assert (
        fixed_type.from_float(0.25 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(2.0**200 * im, bits=200, int_bits=400))
    )
    assert (
        fixed_type.from_float(-0.25 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(-(2.0**200) * im, bits=200, int_bits=400))
    )

    mode = QuantizationMode.TRN_ZERO
    assert (
        fixed_type.from_float(0.0 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(0 * im, bits=200, int_bits=400))
    )
    assert (
        fixed_type.from_float(0.25 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(0 * im, bits=200, int_bits=400))
    )
    assert (
        fixed_type.from_float(-0.25 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(0 * im, bits=200, int_bits=400))
    )

    mode = QuantizationMode.TRN_MAG
    assert (
        fixed_type.from_float(0.0 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(0 * im, bits=200, int_bits=400))
    )
    assert (
        fixed_type.from_float(0.25 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(0 * im, bits=200, int_bits=400))
    )
    assert (
        fixed_type.from_float(-0.25 * im, bits=2, int_bits=0)
        .cast(bits=200, int_bits=400, quantization=mode)
        .is_identical(fixed_type.from_float(0 * im, bits=200, int_bits=400))
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
            fixed_type.from_float(0.0 * im, bits=2, int_bits=0)
            .cast(bits=200, int_bits=400, quantization=mode)
            .is_identical(fixed_type.from_float(0.0 * im, bits=200, int_bits=400))
        )
        assert (
            fixed_type.from_float(0.25 * im, bits=2, int_bits=0)
            .cast(bits=200, int_bits=400, quantization=mode)
            .is_identical(fixed_type.from_float(0.0 * im, bits=200, int_bits=400))
        )
        assert (
            fixed_type.from_float(-0.25 * im, bits=2, int_bits=0)
            .cast(bits=200, int_bits=400, quantization=mode)
            .is_identical(fixed_type.from_float(0.0 * im, bits=200, int_bits=400))
        )


@pytest.mark.parametrize("fixed_type", [APyFixed, APyCFixed])
def test_one_specifier(fixed_type):
    # It is possible to specify only `int_bits` or `frac_bits`. Doing so preserves the
    # other specifier.
    a = fixed_type(0b0011_1100, int_bits=4, frac_bits=4)
    assert a == 3.75
    assert a.cast(1) == -0.25
    assert a.cast(int_bits=1) == -0.25
    assert a.cast(frac_bits=1) == 3.5
