import math

import pytest

from apytypes import APyCFixed, APyCFixedArray, fx


def test_prod():
    # Single limb
    a = APyCFixedArray.from_complex(
        [1, 2 + 1j, 3 + 3j, -4 + 1j, 5 - 2j, 6 - 3j], int_bits=6, frac_bits=0
    )
    assert a.prod().is_identical(
        APyCFixed.from_float(-1395 - 225j, bits=41, int_bits=41)
    )

    # Multi limb
    b = APyCFixedArray.from_complex(
        [1, 2 + 1j, 3 + 3j, -4 + 1j, 5 - 2j, 6 - 3j], int_bits=10, frac_bits=0
    )
    assert b.prod().is_identical(
        APyCFixed.from_float(-1395 - 225j, bits=65, int_bits=65)
    )


def test_arithmetic_float():
    a = APyCFixedArray.from_complex(
        [1, 2 + 1j, 3 + 3j, -4 + 1j, 5 - 2j, 6 - 3j], int_bits=6, frac_bits=0
    )
    assert (a + 1.0).is_identical(
        APyCFixedArray.from_complex(
            [2, 3 + 1j, 4 + 3j, -3 + 1j, 6 - 2j, 7 - 3j], int_bits=7, frac_bits=0
        )
    )
    assert (1.0 + a).is_identical(
        APyCFixedArray.from_complex(
            [2, 3 + 1j, 4 + 3j, -3 + 1j, 6 - 2j, 7 - 3j], int_bits=7, frac_bits=0
        )
    )


@pytest.mark.parametrize("frac_bits", range(10, 68))
def test_array_multiplication_with_different_frac_bits(frac_bits: int):
    v1 = 1 / 3 + 1j * math.pi
    v2 = math.pi + 1j / 7
    v3 = -math.pi - 1j * math.sqrt(2)
    v4 = -23 / 11 + 1j * 5 / 13
    a = fx(v1, int_bits=3, frac_bits=frac_bits)
    b = fx(v2, int_bits=3, frac_bits=frac_bits)
    c = fx(v3, int_bits=3, frac_bits=frac_bits)
    d = fx(v4, int_bits=3, frac_bits=frac_bits)

    av = APyCFixedArray.from_complex([v1, v2, v3, v4], int_bits=3, frac_bits=frac_bits)
    bv = APyCFixedArray.from_complex([v4, v1, v2, v3], int_bits=3, frac_bits=frac_bits)
    res = av * bv
    assert res[0].is_identical(a * d)
    assert res[1].is_identical(b * a)
    assert res[2].is_identical(c * b)
    assert res[3].is_identical(d * c)


@pytest.mark.parametrize("frac_bits", range(10, 68))
def test_array_multiplication_with_different_frac_bits_and_int_bit_offset(frac_bits):
    v1 = 1 / 3 + 1j * math.pi
    v2 = math.pi + 1j / 7
    v3 = -math.pi - 1j * math.sqrt(2)
    v4 = -23 / 11 + 1j * 5 / 13
    a = fx(v1, int_bits=3, frac_bits=frac_bits)
    b = fx(v2, int_bits=3, frac_bits=frac_bits)
    c = fx(v3, int_bits=3, frac_bits=frac_bits)
    d = fx(v4, int_bits=3, frac_bits=frac_bits)

    al = fx(v1, int_bits=3, frac_bits=frac_bits + 20)
    bl = fx(v2, int_bits=3, frac_bits=frac_bits + 20)
    cl = fx(v3, int_bits=3, frac_bits=frac_bits + 20)
    dl = fx(v4, int_bits=3, frac_bits=frac_bits + 20)

    av = APyCFixedArray.from_complex([v1, v2, v3, v4], int_bits=3, frac_bits=frac_bits)
    bv = APyCFixedArray.from_complex(
        [v4, v1, v2, v3], int_bits=3, frac_bits=frac_bits + 20
    )
    res = av * bv
    assert res[0].is_identical(a * dl)
    assert res[1].is_identical(b * al)
    assert res[2].is_identical(c * bl)
    assert res[3].is_identical(d * cl)


@pytest.mark.parametrize("frac_bits", range(10, 68))
def test_scalar_array_multiplication_with_different_frac_bits(frac_bits):
    v1 = 1 / 3 + 1j * math.pi
    v2 = math.pi + 1j / 7
    v3 = -math.pi - 1j * math.sqrt(2)
    v4 = -23 / 11 + 1j * 5 / 13
    a = fx(v1, int_bits=3, frac_bits=frac_bits)
    b = fx(v2, int_bits=3, frac_bits=frac_bits)
    c = fx(v3, int_bits=3, frac_bits=frac_bits)
    d = fx(v4, int_bits=3, frac_bits=frac_bits)

    av = APyCFixedArray.from_complex([v1, v2, v3, v4], int_bits=3, frac_bits=frac_bits)
    res = b * av

    assert res[0].is_identical(b * a)
    assert res[1].is_identical(b * b)
    assert res[2].is_identical(b * c)
    assert res[3].is_identical(b * d)


@pytest.mark.parametrize("frac_bits", range(10, 68))
def test_scalar_array_multiplication_with_different_frac_bits_and_int_bit_offset(
    frac_bits,
):
    v1 = 1 / 3 + 1j * math.pi
    v2 = math.pi + 1j / 7
    v3 = -math.pi - 1j * math.sqrt(2)
    v4 = -23 / 11 + 1j * 5 / 13
    a = fx(v1, int_bits=3, frac_bits=frac_bits)
    b = fx(v2, int_bits=3, frac_bits=frac_bits)
    c = fx(v3, int_bits=3, frac_bits=frac_bits)
    d = fx(v4, int_bits=3, frac_bits=frac_bits)

    bl = fx(v2, int_bits=3, frac_bits=frac_bits + 20)

    av = APyCFixedArray.from_complex([v1, v2, v3, v4], int_bits=3, frac_bits=frac_bits)
    res = bl * av

    assert res[0].is_identical(bl * a)
    assert res[1].is_identical(bl * b)
    assert res[2].is_identical(bl * c)
    assert res[3].is_identical(bl * d)


def test_unary_arith():
    a = APyCFixedArray.from_complex(
        [1, 2 + 1j, 3 + 3j, -4 + 1j, 5 - 2j, 6 - 3j], int_bits=6, frac_bits=0
    )
    nega = APyCFixedArray.from_complex(
        [-1, -2 - 1j, -3 - 3j, 4 - 1j, -5 + 2j, -6 + 3j], int_bits=7, frac_bits=0
    )
    assert (-a).is_identical(nega)
    assert (+a).is_identical(a)
