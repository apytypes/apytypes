from apytypes import APyFixedArray

import pytest


def test_array_raises():
    a = APyFixedArray([5, 6], bits=10, int_bits=5)
    b = APyFixedArray([1, 2, 3], bits=7, int_bits=2)
    with pytest.raises(RuntimeError, match="APyFixedArray.__add__: shape missmatch"):
        _ = a + b
    with pytest.raises(RuntimeError, match="APyFixedArray.__sub__: shape missmatch"):
        _ = a - b
    with pytest.raises(RuntimeError, match="APyFixedArray.__mul__: shape missmatch"):
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
