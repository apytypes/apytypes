from apytypes import APyFixedArray, APyFixed

import math
import pytest


def test_array_raises():
    a = APyFixedArray([5, 6], bits=10, int_bits=5)
    b = APyFixedArray([1, 2, 3], bits=7, int_bits=2)
    with pytest.raises(ValueError, match="APyFixedArray.__add__: shape missmatch"):
        _ = a + b
    with pytest.raises(ValueError, match="APyFixedArray.__sub__: shape missmatch"):
        _ = a - b
    with pytest.raises(ValueError, match="APyFixedArray.__mul__: shape missmatch"):
        _ = a * b


def test_array_add():
    a = APyFixedArray([-5, -6, 7], bits=10, int_bits=5)
    b = APyFixedArray([1, -2, 3], bits=7, int_bits=2)
    assert (a + b).is_identical(APyFixedArray([-4, -8, 10], bits=11, int_bits=6))
    assert (a + b).is_identical(b + a)

    a = APyFixedArray([1, 2, 3], bits=10, frac_bits=0)
    b = APyFixedArray([1, 2, 3], bits=10, frac_bits=3)
    assert (a + b).is_identical(APyFixedArray([9, 18, 27], bits=14, frac_bits=3))
    assert (a + b).is_identical(b + a)

    a = APyFixedArray.from_float([1.2345, -5.4321], bits=256, int_bits=128)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=256, int_bits=128)
    assert (a + b).is_identical(b + a)
    assert (a + b).is_identical(
        APyFixedArray(
            [
                3780877378858547263310314894523174486016,
                83981688156087430476747255228080848896,
            ],
            bits=257,
            int_bits=129,
        )
    )


def test_array_add_scalar():
    a = APyFixedArray([-5, -6, 7], bits=10, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    assert (a + b).is_identical(APyFixedArray([2046, 2045, 10], bits=11, int_bits=6))
    assert (b + a).is_identical(APyFixedArray([2046, 2045, 10], bits=11, int_bits=6))


def test_array_sum():
    a = APyFixedArray([-5, -6, 7, -1], bits=10, int_bits=5)
    assert sum(a).is_identical(APyFixedArray([-5], bits=13, int_bits=8))
    b = APyFixedArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [4.0, 5.0, 6.0],
        ],
        bits=8,
        int_bits=6,
    )
    assert sum(b).is_identical(APyFixedArray([20, 28, 36], bits=9, int_bits=7))


def test_array_sub():
    a = APyFixedArray([-5, -6, 7], bits=10, int_bits=5)
    b = APyFixedArray([1, -2, 3], bits=7, int_bits=2)
    assert (a - b).is_identical(APyFixedArray([-6, -4, 4], bits=11, int_bits=6))
    assert (b - a).is_identical(APyFixedArray([6, 4, -4], bits=11, int_bits=6))

    a = APyFixedArray([1, 2, 3], bits=10, frac_bits=0)
    b = APyFixedArray([1, 2, 3], bits=10, frac_bits=3)
    assert (a - b).is_identical(APyFixedArray([7, 14, 21], bits=14, frac_bits=3))
    assert (b - a).is_identical(APyFixedArray([-7, -14, -21], bits=14, frac_bits=3))

    a = APyFixedArray.from_float([1.2345, -5.4321], bits=256, int_bits=128)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=256, int_bits=128)
    assert (a - b).is_identical(
        APyFixedArray(
            [
                0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF75BA5E353F7CED0000000000000000000,
                0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4E395810624DD40000000000000000000,
            ],
            bits=257,
            int_bits=129,
        )
    )


def test_array_sub_scalar():
    a = APyFixedArray([-5, -6, 7], bits=10, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    assert (a - b).is_identical(APyFixedArray([2040, 2039, 4], bits=11, int_bits=6))


def test_array_mul():
    a = APyFixedArray([-5, -6, 7], bits=10, int_bits=5)
    b = APyFixedArray([1, -2, 3], bits=7, int_bits=2)
    assert (a * b).is_identical(APyFixedArray([-5, 12, 21], bits=17, int_bits=7))
    assert (a * b).is_identical(b * a)

    a = APyFixedArray.from_float([1.2345, 5.4321], bits=128, int_bits=64)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=128, int_bits=64)
    assert (a * b).is_identical(b * a)
    assert (a * b).is_identical(
        APyFixedArray(
            [
                4148906114766443653661449407827571376128,
                10497150468965098645933053068265826287616,
            ],
            bits=256,
            int_bits=128,
        )
    )

    a = APyFixedArray.from_float([1.2345, 5.4321], bits=256, int_bits=128)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=256, int_bits=128)
    assert (a * b).is_identical(b * a)
    assert (a * b).is_identical(
        APyFixedArray(
            [
                0xC314A4095F245001AC14C660A2000000000000000000000000000000000000000,
                0x1ED92DA453060C056E0ACA8950A000000000000000000000000000000000000000,
            ],
            bits=512,
            int_bits=256,
        )
    )


def test_array_mul_scalar():
    a = APyFixedArray([-5, -6, 7], bits=10, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    assert (a * b).is_identical(
        APyFixedArray([131057, 131054, 21], bits=17, int_bits=7)
    )
    assert (b * a).is_identical(
        APyFixedArray([131057, 131054, 21], bits=17, int_bits=7)
    )
    b = APyFixed(-3, bits=7, int_bits=2)
    assert (a * b).is_identical(APyFixedArray([15, 18, 131051], bits=17, int_bits=7))
    assert (b * a).is_identical(APyFixedArray([15, 18, 131051], bits=17, int_bits=7))


def test_array_prod():
    a = APyFixedArray([-5, -6, 7, -1], bits=10, int_bits=5)
    assert math.prod(a).is_identical(
        APyFixedArray([1099511627566], bits=40, int_bits=20)
    )
    b = APyFixedArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [4.0, 5.0, 6.0],
        ],
        bits=8,
        int_bits=6,
    )
    assert math.prod(b).is_identical(
        APyFixedArray([64, 160, 288], bits=16, int_bits=12)
    )


def test_transpose():

    # High-dimensional transor transposition not implemented
    with pytest.raises(RuntimeError, match="Not implemented: high-dimensional"):
        APyFixedArray([[[1]]], 1, 0).T

    # 1-D transposition simply returns the input (just like NumPy-arrays)
    assert APyFixedArray([], 1, 0).T.is_identical(APyFixedArray([], 1, 0))
    assert APyFixedArray([1, 2, 3], 5, 5).T.is_identical(APyFixedArray([1, 2, 3], 5, 5))

    # 2-D transposition returns the matrix transposition
    a = APyFixedArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [4.0, 5.0, 6.0],
        ],
        bits=256,
        int_bits=128,
    )
    assert a.T.T.is_identical(a)
    assert a.T.is_identical(
        APyFixedArray.from_float(
            [
                [1.0, 4.0],
                [2.0, 5.0],
                [3.0, 6.0],
            ],
            bits=256,
            int_bits=128,
        )
    )


def test_shift():
    a = APyFixedArray([1, 2, -3], bits=10, int_bits=5)
    assert (a >> 2).is_identical(APyFixedArray([1, 2, -3], bits=10, int_bits=3))
    assert (a << 7).is_identical(APyFixedArray([1, 2, -3], bits=10, int_bits=12))
    assert (a >> -2).is_identical(APyFixedArray([1, 2, -3], bits=10, int_bits=7))
    assert (a << -7).is_identical(APyFixedArray([1, 2, -3], bits=10, int_bits=-2))


def test_unary_negate():
    a = APyFixedArray.from_float([0, -1, 2, -3, 4], bits=100, int_bits=50)
    b = APyFixedArray.from_float([0, -1, 2, -3, 4], bits=64, int_bits=32)
    assert (-a).is_identical(
        APyFixedArray.from_float([0, 1, -2, 3, -4], bits=101, int_bits=51)
    )
    assert (-b).is_identical(
        APyFixedArray.from_float([0, 1, -2, 3, -4], bits=65, int_bits=33)
    )


def test_abs():
    a = APyFixedArray.from_float([0, -1, 2, -3, 4], bits=100, int_bits=50)
    b = APyFixedArray.from_float([0, -1, 2, -3, 4], bits=64, int_bits=32)
    assert abs(a).is_identical(
        APyFixedArray.from_float([0, 1, 2, 3, 4], bits=101, int_bits=51)
    )
    assert abs(b).is_identical(
        APyFixedArray.from_float([0, 1, 2, 3, 4], bits=65, int_bits=33)
    )
