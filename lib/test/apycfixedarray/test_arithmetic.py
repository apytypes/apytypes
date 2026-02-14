import math

import pytest

from apytypes import APyCFixed, APyCFixedArray, APyFixed, APyFixedArray, fx


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
def test_array_multiplication_with_different_frac_bits_and_int_bit_offset(
    frac_bits: int,
):
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

    res = bv * av
    assert res[0].is_identical(a * dl)
    assert res[1].is_identical(b * al)
    assert res[2].is_identical(c * bl)
    assert res[3].is_identical(d * cl)


@pytest.mark.parametrize("frac_bits", range(10, 68))
def test_scalar_array_multiplication_with_different_frac_bits(frac_bits: int):
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
    frac_bits: int,
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


def test_arithmetic_with_apyfixed():
    a = APyCFixedArray.from_complex([1 + 2j, -4, 3.5 - 2j], int_bits=7, frac_bits=3)
    b = APyFixed.from_float(3.0, int_bits=5, frac_bits=0)
    assert (a + b).is_identical(
        APyCFixedArray.from_complex([4 + 2j, -1, 6.5 - 2j], int_bits=8, frac_bits=3)
    )
    assert (a - b).is_identical(
        APyCFixedArray.from_complex([-2 + 2j, -7, 0.5 - 2j], int_bits=8, frac_bits=3)
    )
    assert (a * b).is_identical(
        APyCFixedArray.from_complex([3 + 6j, -12, 10.5 - 6j], int_bits=12, frac_bits=3)
    )
    assert (a / b).is_identical(
        APyCFixedArray([(85, 170), (65195, 0), (298, 65366)], int_bits=8, frac_bits=8)
    )
    assert (b + a).is_identical(a + b)
    assert (b * a).is_identical(a * b)
    assert (b - a).is_identical((-(a - b)).cast(int_bits=8, frac_bits=3))
    assert (b / a).is_identical(
        APyCFixedArray([(76, 65383), (65440, 0), (82, 47)], int_bits=9, frac_bits=7)
    )

    np = pytest.importorskip("numpy")
    assert np.all(a != b)
    assert np.all(b != a)
    assert not (np.all(a == b))
    assert not (np.all(b == a))


def test_arithmetic_with_apyfixedarray():
    a = APyCFixedArray.from_complex([1 + 2j, -4, 3.5 - 2j], int_bits=7, frac_bits=3)
    b = APyFixedArray.from_float([3.0, -1.0, 4.0], int_bits=5, frac_bits=0)
    assert (a + b).is_identical(
        APyCFixedArray.from_complex([4 + 2j, -5, 7.5 - 2j], int_bits=8, frac_bits=3)
    )
    assert (a - b).is_identical(
        APyCFixedArray.from_complex([-2 + 2j, -3, -0.5 - 2j], int_bits=8, frac_bits=3)
    )
    assert (a * b).is_identical(
        APyCFixedArray.from_complex([3 + 6j, 4, 14 - 8j], int_bits=12, frac_bits=3)
    )
    assert (a / b).is_identical(
        APyCFixedArray([(85, 170), (1024, 0), (224, 65408)], int_bits=8, frac_bits=8)
    )
    assert (b + a).is_identical(a + b)
    assert (b * a).is_identical(a * b)
    assert (b - a).is_identical((-(a - b)).cast(int_bits=8, frac_bits=3))
    assert (b / a).is_identical(
        APyCFixedArray([(76, 65383), (32, 0), (110, 63)], int_bits=9, frac_bits=7)
    )

    np = pytest.importorskip("numpy")
    assert np.all(a != b)
    assert np.all(b != a)
    assert not (np.all(a == b))
    assert not (np.all(b == a))


@pytest.mark.parametrize("int_bits_1", [10, 20, 30, 34, 50, 100, 300])
@pytest.mark.parametrize("int_bits_2", [10, 20, 30, 34, 50, 100, 300])
def test_arrays_raises_on_zero_div(int_bits_1: int, int_bits_2: int):
    a = APyCFixedArray.from_complex([0.0, 1.0, 2.0], int_bits=int_bits_1, frac_bits=0)
    b = APyCFixedArray.from_complex([0.0, 0.0, 1.0], int_bits=int_bits_2, frac_bits=0)
    with pytest.raises(ZeroDivisionError):
        _ = a / b
    with pytest.raises(ZeroDivisionError):
        _ = b / a

    a = APyCFixedArray.from_complex([0.0, 1.0, 2.0], int_bits=int_bits_1, frac_bits=0)
    b = APyFixedArray.from_float([0.0, 1.0, 0.0], int_bits=int_bits_2, frac_bits=0)
    with pytest.raises(ZeroDivisionError):
        _ = a / b
    with pytest.raises(ZeroDivisionError):
        _ = b / a

    a = APyCFixedArray.from_complex([0.0, 1.0, 2.0], int_bits=int_bits_1, frac_bits=0)
    b = APyCFixed.from_complex(0.0, int_bits=int_bits_2, frac_bits=0)
    with pytest.raises(ZeroDivisionError):
        _ = a / b
    with pytest.raises(ZeroDivisionError):
        _ = b / a

    a = APyCFixedArray.from_complex([0.0, 1.0, 2.0], int_bits=int_bits_1, frac_bits=0)
    b = APyFixed.from_float(0.0, int_bits=int_bits_2, frac_bits=0)
    with pytest.raises(ZeroDivisionError):
        _ = a / b
    with pytest.raises(ZeroDivisionError):
        _ = b / a


@pytest.mark.parametrize("bits", range(10, 200, 10))
def test_array_scalar_multiplication_with_different_bits(bits: int):
    a = APyCFixedArray([(1, 1), (2, -2), (-3, 5)], bits=8, frac_bits=2)
    b = APyCFixed((-1, 1), bits=bits, frac_bits=2)

    assert (a * b).is_identical(
        APyCFixedArray([(-2, 0), (0, 4), (-2, -8)], bits=bits + 9, frac_bits=4)
    )

    assert (b * a).is_identical(
        APyCFixedArray([(-2, 0), (0, 4), (-2, -8)], bits=bits + 9, frac_bits=4)
    )

    a = APyCFixedArray([(1, 1), (2, -2), (-3, 5)], bits=bits, frac_bits=2)
    b = APyCFixed((-1, 1), bits=8, frac_bits=2)

    assert (a * b).is_identical(
        APyCFixedArray([(-2, 0), (0, 4), (-2, -8)], bits=bits + 9, frac_bits=4)
    )

    assert (b * a).is_identical(
        APyCFixedArray([(-2, 0), (0, 4), (-2, -8)], bits=bits + 9, frac_bits=4)
    )


@pytest.mark.parametrize("frac_bits", range(10, 200, 10))
def test_array_scalar_division_with_different_bits(frac_bits: int):
    a1 = fx(math.pi, 4, frac_bits, force_complex=True)
    a2 = fx(1j * math.sqrt(2), 4, frac_bits, force_complex=True)
    a3 = fx(5 - 1j * 5, 4, frac_bits, force_complex=True)
    a = fx([math.pi, 1j * math.sqrt(2), 5 - 1j * 5], 4, frac_bits, force_complex=True)
    b = fx(3 + 1j * 4 / 7, 4, frac_bits)
    res = a / b
    assert (res[0]).is_identical(a1 / b)
    assert (res[1]).is_identical(a2 / b)
    assert (res[2]).is_identical(a3 / b)

    res = b / a
    assert (res[0]).is_identical(b / a1)
    assert (res[1]).is_identical(b / a2)
    assert (res[2]).is_identical(b / a3)


@pytest.mark.parametrize("frac_bits", range(5, 200, 5))
def test_array_scalar_division_with_different_bits_in_numerator(frac_bits: int):
    a1 = fx(math.pi, 4, frac_bits, force_complex=True)
    a2 = fx(1j * math.sqrt(2), 4, frac_bits, force_complex=True)
    a3 = fx(5 - 1j * 5, 4, frac_bits, force_complex=True)
    a = fx([math.pi, 1j * math.sqrt(2), 5 - 1j * 5], 4, frac_bits, force_complex=True)
    b = fx(3 + 1j * 4 / 7, 4, 4)
    res = a / b
    assert (res[0]).is_identical(a1 / b)
    assert (res[1]).is_identical(a2 / b)
    assert (res[2]).is_identical(a3 / b)

    res = b / a
    assert (res[0]).is_identical(b / a1)
    assert (res[1]).is_identical(b / a2)
    assert (res[2]).is_identical(b / a3)

    b = fx(3 + 1j * 4 / 7, 4, 10)
    res = a / b
    assert (res[0]).is_identical(a1 / b)
    assert (res[1]).is_identical(a2 / b)
    assert (res[2]).is_identical(a3 / b)

    res = b / a
    assert (res[0]).is_identical(b / a1)
    assert (res[1]).is_identical(b / a2)
    assert (res[2]).is_identical(b / a3)

    b = fx(3 + 1j * 4 / 7, 4, 29)
    res = a / b
    assert (res[0]).is_identical(a1 / b)
    assert (res[1]).is_identical(a2 / b)
    assert (res[2]).is_identical(a3 / b)

    res = b / a
    assert (res[0]).is_identical(b / a1)
    assert (res[1]).is_identical(b / a2)
    assert (res[2]).is_identical(b / a3)

    b = fx(3 + 1j * 4 / 7, 4, 63)
    res = a / b
    assert (res[0]).is_identical(a1 / b)
    assert (res[1]).is_identical(a2 / b)
    assert (res[2]).is_identical(a3 / b)

    res = b / a
    assert (res[0]).is_identical(b / a1)
    assert (res[1]).is_identical(b / a2)
    assert (res[2]).is_identical(b / a3)


@pytest.mark.parametrize("frac_bits", range(10, 200, 10))
def test_array_division_with_different_bits(frac_bits: int):
    a1 = fx(math.pi, 4, frac_bits, force_complex=True)
    a2 = fx(1j * math.sqrt(2), 4, frac_bits, force_complex=True)
    a3 = fx(5 - 1j * 5, 4, frac_bits, force_complex=True)
    a = fx([math.pi, 1j * math.sqrt(2), 5 - 1j * 5], 4, frac_bits, force_complex=True)
    b1 = fx(3 + 1j * 4 / 7, 4, frac_bits, force_complex=True)
    b2 = fx(-3 - 1j * 4 / 9, 4, frac_bits, force_complex=True)
    b3 = fx(3 / 7 - 1j * 4 / 13, 4, frac_bits, force_complex=True)
    b = fx(
        [3 + 1j * 4 / 7, -3 - 1j * 4 / 9, 3 / 7 - 1j * 4 / 13],
        4,
        frac_bits,
        force_complex=True,
    )
    res = a / b
    assert (res[0]).is_identical(a1 / b1)
    assert (res[1]).is_identical(a2 / b2)
    assert (res[2]).is_identical(a3 / b3)

    res = b / a
    assert (res[0]).is_identical(b1 / a1)
    assert (res[1]).is_identical(b2 / a2)
    assert (res[2]).is_identical(b3 / a3)


@pytest.mark.parametrize("frac_bits", range(10, 200, 5))
def test_array_division_with_different_bits_in_numerator(frac_bits: int):
    a1 = fx(math.pi, 4, frac_bits, force_complex=True)
    a2 = fx(1j * math.sqrt(2), 4, frac_bits, force_complex=True)
    a3 = fx(5 - 1j * 5, 4, frac_bits, force_complex=True)
    a = fx([math.pi, 1j * math.sqrt(2), 5 - 1j * 5], 4, frac_bits, force_complex=True)
    b1 = fx(3 + 1j * 4 / 7, 4, 4, force_complex=True)
    b2 = fx(-3 - 1j * 4 / 9, 4, 4, force_complex=True)
    b3 = fx(3 / 7 - 1j * 4 / 13, 4, 4, force_complex=True)
    b = fx(
        [3 + 1j * 4 / 7, -3 - 1j * 4 / 9, 3 / 7 - 1j * 4 / 13],
        4,
        4,
        force_complex=True,
    )
    res = a / b
    assert (res[0]).is_identical(a1 / b1)
    assert (res[1]).is_identical(a2 / b2)
    assert (res[2]).is_identical(a3 / b3)

    res = b / a
    assert (res[0]).is_identical(b1 / a1)
    assert (res[1]).is_identical(b2 / a2)
    assert (res[2]).is_identical(b3 / a3)


@pytest.mark.parametrize("offset", range(-9, 10, 2))
@pytest.mark.parametrize("frac_bits", range(10, 200, 5))
def test_array_scalar_division_with_different_bits_and_offset(
    offset: int, frac_bits: int
):
    a1 = fx(math.pi, 4, frac_bits, force_complex=True)
    a2 = fx(1j * math.sqrt(2), 4, frac_bits, force_complex=True)
    a3 = fx(5 - 1j * 5, 4, frac_bits, force_complex=True)
    a = fx([math.pi, 1j * math.sqrt(2), 5 - 1j * 5], 4, frac_bits, force_complex=True)
    b = fx(3 + 1j * 4 / 7, 4, frac_bits + offset)
    res = a / b
    assert (res[0]).is_identical(a1 / b)
    assert (res[1]).is_identical(a2 / b)
    assert (res[2]).is_identical(a3 / b)

    res = b / a
    assert (res[0]).is_identical(b / a1)
    assert (res[1]).is_identical(b / a2)
    assert (res[2]).is_identical(b / a3)


@pytest.mark.parametrize("offset", range(-9, 10, 2))
@pytest.mark.parametrize("frac_bits", range(10, 200, 5))
def test_array_division_with_different_bits_and_offset(offset: int, frac_bits: int):
    a1 = fx(math.pi, 4, frac_bits, force_complex=True)
    a2 = fx(1j * math.sqrt(2), 4, frac_bits, force_complex=True)
    a3 = fx(5 - 1j * 5, 4, frac_bits, force_complex=True)
    a = fx([math.pi, 1j * math.sqrt(2), 5 - 1j * 5], 4, frac_bits, force_complex=True)
    b1 = fx(3 + 1j * 4 / 7, 4, frac_bits + offset, force_complex=True)
    b2 = fx(-3 - 1j * 4 / 9, 4, frac_bits + offset, force_complex=True)
    b3 = fx(3 / 7 - 1j * 4 / 13, 4, frac_bits + offset, force_complex=True)
    b = fx(
        [3 + 1j * 4 / 7, -3 - 1j * 4 / 9, 3 / 7 - 1j * 4 / 13],
        4,
        frac_bits + offset,
        force_complex=True,
    )
    res = a / b
    assert (res[0]).is_identical(a1 / b1)
    assert (res[1]).is_identical(a2 / b2)
    assert (res[2]).is_identical(a3 / b3)

    res = b / a
    assert (res[0]).is_identical(b1 / a1)
    assert (res[1]).is_identical(b2 / a2)
    assert (res[2]).is_identical(b3 / a3)


@pytest.mark.parametrize("frac_bits", range(10, 200, 5))
def test_array_real_scalar_multiplication_different_wordlengths(frac_bits: int):
    a = fx([1, 2 + 1j, -3 + 5j, -1j], 7, frac_bits, force_complex=True)
    b = fx(3, 4, frac_bits)
    ref = fx([3, 6 + 3j, -9 + 15j, -3j], 11, 2 * frac_bits, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)

    b = fx(3, 4, 0)
    ref = fx([3, 6 + 3j, -9 + 15j, -3j], 11, frac_bits, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)

    b = fx(3, 4, 20)
    ref = fx([3, 6 + 3j, -9 + 15j, -3j], 11, frac_bits + 20, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)

    b = fx(3, 4, 40)
    ref = fx([3, 6 + 3j, -9 + 15j, -3j], 11, frac_bits + 40, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)

    b = fx(3, 4, 100)
    ref = fx([3, 6 + 3j, -9 + 15j, -3j], 11, frac_bits + 100, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)


@pytest.mark.parametrize("frac_bits", range(10, 200, 5))
def test_array_real_array_multiplication_different_wordlengths(frac_bits: int):
    a = fx([[1, 2 + 1j], [-3 + 5j, -1j]], 7, frac_bits, force_complex=True)
    b = fx([3, -2], 4, frac_bits)
    ref = fx([[3, -4 - 2j], [-9 + 15j, 2j]], 11, 2 * frac_bits, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)

    b = fx([3, -2], 4, 0)
    ref = fx([[3, -4 - 2j], [-9 + 15j, 2j]], 11, frac_bits, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)

    b = fx([3, -2], 4, 20)
    ref = fx([[3, -4 - 2j], [-9 + 15j, 2j]], 11, frac_bits + 20, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)

    b = fx([3, -2], 4, 40)
    ref = fx([[3, -4 - 2j], [-9 + 15j, 2j]], 11, frac_bits + 40, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)

    b = fx([3, -2], 4, 100)
    ref = fx([[3, -4 - 2j], [-9 + 15j, 2j]], 11, frac_bits + 100, force_complex=True)
    assert (a * b).is_identical(ref)
    assert (b * a).is_identical(ref)


@pytest.mark.parametrize("real_int_bits", [3, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("real_frac_bits", [0, 5, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_int_bits", [4, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_frac_bits", [0, 5, 20, 40, 100, 10000])
def test_complex_real_scalar_division_different_wordlengths_different_fracbits(
    real_int_bits: int,
    real_frac_bits: int,
    complex_int_bits: int,
    complex_frac_bits: int,
):
    a = APyCFixedArray.from_complex(
        [1 + 1j, 0, -1 - 1j, -4j, 6],
        int_bits=complex_int_bits,
        frac_bits=complex_frac_bits,
    )
    b = APyFixed.from_float(2, int_bits=real_int_bits, frac_bits=real_frac_bits)
    ref = APyCFixedArray.from_complex(
        [0.5 + 0.5j, 0, -0.5 - 0.5j, -2j, 3],
        int_bits=complex_int_bits + real_frac_bits + 1,
        frac_bits=complex_frac_bits + real_int_bits,
    )
    assert (a / b).is_identical(ref)


@pytest.mark.parametrize("real_int_bits", [6, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("real_frac_bits", [0, 5, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_int_bits", [4, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_frac_bits", [0, 5, 20, 40, 100, 10000])
def test_complex_real_array_division_different_wordlengths_different_fracbits(
    real_int_bits: int,
    real_frac_bits: int,
    complex_int_bits: int,
    complex_frac_bits: int,
):
    a = APyCFixedArray.from_complex(
        [1 + 1j, 0, -1 - 1j, -4j, 6],
        int_bits=complex_int_bits,
        frac_bits=complex_frac_bits,
    )
    b = APyFixedArray.from_float(
        [2, 3, 1, -2, 3], int_bits=real_int_bits, frac_bits=real_frac_bits
    )
    ref = APyCFixedArray.from_complex(
        [0.5 + 0.5j, 0, -1 - 1j, 2j, 2],
        int_bits=complex_int_bits + real_frac_bits + 1,
        frac_bits=complex_frac_bits + real_int_bits,
    )
    assert (a / b).is_identical(ref)


@pytest.mark.parametrize("real_int_bits", [6, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("real_frac_bits", [0, 5, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_int_bits", [4, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_frac_bits", [0, 5, 20, 40, 100, 10000])
def test_real_complex_array_division_different_wordlengths_different_fracbits(
    real_int_bits: int,
    real_frac_bits: int,
    complex_int_bits: int,
    complex_frac_bits: int,
):
    a = APyFixedArray.from_float(
        [2, 0, 25, 2, 6], int_bits=real_int_bits, frac_bits=real_frac_bits
    )
    b = APyCFixedArray.from_complex(
        [1j, 3, -3 - 4j, -4j, 3], int_bits=complex_int_bits, frac_bits=complex_frac_bits
    )
    ref = APyCFixedArray.from_complex(
        [-2j, 0, -3 + 4j, 0.5j, 2],
        int_bits=real_int_bits + complex_frac_bits + 1,
        frac_bits=real_frac_bits + complex_int_bits,
    )
    assert (a / b).is_identical(ref)


@pytest.mark.parametrize("real_int_bits", [6, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("real_frac_bits", [0, 5, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_int_bits", [4, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_frac_bits", [0, 5, 20, 40, 100, 10000])
def test_real_scalar_complex_array_division_different_wordlengths_different_fracbits(
    real_int_bits: int,
    real_frac_bits: int,
    complex_int_bits: int,
    complex_frac_bits: int,
):
    a = APyFixed.from_float(25, int_bits=real_int_bits, frac_bits=real_frac_bits)
    b = APyCFixedArray.from_complex(
        [1j, 2, -3 - 4j, -4j], int_bits=complex_int_bits, frac_bits=complex_frac_bits
    )
    ref = APyCFixedArray.from_complex(
        [-25j, 12.5, -3 + 4j, 6.25j],
        int_bits=real_int_bits + complex_frac_bits + 1,
        frac_bits=real_frac_bits + complex_int_bits,
    )
    assert (a / b).is_identical(ref)


def test_operation_with_integers():
    a = APyCFixedArray([5], 6, 2)
    one = APyFixedArray([4], 6, 2)
    zero = APyFixedArray([0], 6, 2)

    # Integer identities
    assert (_ := a + 0).is_identical(_ := a + zero)
    assert (_ := 0 + a).is_identical(_ := zero + a)
    assert (_ := a - 0).is_identical(_ := a - zero)
    assert (_ := 0 - a).is_identical(_ := zero - a)
    assert (_ := a * 1).is_identical(_ := a * one)
    assert (_ := 1 * a).is_identical(_ := one * a)
    assert (_ := a / 1).is_identical(_ := a / one)

    # Other integer operations
    neg_two = APyFixedArray([248], bits=8, int_bits=6)
    assert (_ := a + (-2)).is_identical(_ := a + neg_two)
    assert (_ := (-2) + a).is_identical(_ := neg_two + a)
    assert (_ := a - (-2)).is_identical(_ := a - neg_two)
    assert (_ := (-2) - a).is_identical(_ := neg_two - a)
    assert (_ := a * (-2)).is_identical(_ := a * neg_two)
    assert (_ := (-2) * a).is_identical(_ := neg_two * a)
    assert (_ := a / (-2)).is_identical(_ := a / neg_two)
    assert (_ := (-2) / a).is_identical(_ := neg_two / a)


def test_operation_with_floats():
    a = APyCFixedArray([5], 6, 2)
    one = APyFixedArray([4], 6, 2)
    zero = APyFixedArray([0], 6, 2)

    # Identities
    assert (_ := a + 0.0).is_identical(_ := a + zero)
    assert (_ := 0.0 + a).is_identical(_ := zero + a)
    assert (_ := a - 0.0).is_identical(_ := a - zero)
    assert (_ := 0.0 - a).is_identical(_ := zero - a)
    assert (_ := a * 1.0).is_identical(_ := a * one)
    assert (_ := 1.0 * a).is_identical(_ := one * a)
    assert (_ := a / 1.0).is_identical(_ := a / one)

    # Other operations. 2.125 should quantize to -2.25
    b = APyFixed(247, bits=8, int_bits=6)
    assert (_ := a + (-2.125)).is_identical(_ := a + b)
    assert (_ := (-2.125) + a).is_identical(_ := b + a)
    assert (_ := a - (-2.125)).is_identical(_ := a - b)
    assert (_ := (-2.125) - a).is_identical(_ := b - a)
    assert (_ := a * (-2.125)).is_identical(_ := a * b)
    assert (_ := (-2.125) * a).is_identical(_ := b * a)
    assert (_ := a / (-2.125)).is_identical(_ := a / b)
    assert (_ := (-2.125) / a).is_identical(_ := b / a)
