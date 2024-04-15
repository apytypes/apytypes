from apytypes import APyFixedArray, APyFixed
from apytypes import QuantizationMode

import math
import pytest


def test_array_raises():
    a = APyFixedArray([5, 6], bits=10, int_bits=5)
    b = APyFixedArray([1, 2, 3], bits=7, int_bits=2)
    with pytest.raises(ValueError, match="APyFixedArray.__add__: shape mismatch"):
        _ = a + b
    with pytest.raises(ValueError, match="APyFixedArray.__sub__: shape mismatch"):
        _ = a - b
    with pytest.raises(ValueError, match="APyFixedArray.__mul__: shape mismatch"):
        _ = a * b


def test_array_add():
    a = APyFixedArray([-5, -6, 7, 8, 9], bits=10, int_bits=5)
    b = APyFixedArray([1, -2, 3, -2, -1], bits=7, int_bits=2)
    assert (a + b).is_identical(APyFixedArray([-4, -8, 10, 6, 8], bits=11, int_bits=6))
    assert (a + b).is_identical(b + a)

    a = APyFixedArray([1, 2, 3, 4, 5], bits=10, frac_bits=0)
    b = APyFixedArray([1, 2, 3, 8, 7], bits=10, frac_bits=3)
    assert (a + b).is_identical(
        APyFixedArray([9, 18, 27, 40, 47], bits=14, frac_bits=3)
    )
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

    a = APyFixedArray.from_float([1.2345, -5.4321], bits=255, int_bits=128)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=255, int_bits=128)
    assert (a + b).is_identical(b + a)
    assert (a + b).is_identical(
        APyFixedArray(
            [
                1890438689429273631655157447261587243008,
                41990844078043715238373627614040424448,
            ],
            bits=256,
            int_bits=129,
        )
    )

    a = APyFixedArray.from_float([1.2345, -5.4321], bits=256, int_bits=128)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=25, int_bits=12)
    assert (a + b).is_identical(b + a)

    a = APyFixedArray.from_float([1.2345, -5.4321], bits=252, int_bits=128)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=252, int_bits=130)
    assert (a + b).is_identical(b + a)
    assert (a + b).is_identical(
        APyFixedArray(
            [
                236304836178659203956894680907698405376,
                5248855509755464404796703451755053056,
            ],
            bits=255,
            int_bits=131,
        )
    )


def test_array_add_scalar():
    a = APyFixedArray([-5, -6, 7, 10, -11], bits=10, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    assert (a + b).is_identical(
        APyFixedArray([-2, -3, 10, 13, -8], bits=11, int_bits=6)
    )
    assert (b + a).is_identical(
        APyFixedArray([-2, -3, 10, 13, -8], bits=11, int_bits=6)
    )

    a = APyFixedArray([-5, -6, 7], bits=7, int_bits=4)
    b = APyFixed(3, bits=7, int_bits=4)
    assert (a + b).is_identical(APyFixedArray([254, 253, 10], bits=8, int_bits=5))

    a = APyFixedArray.from_float([-5, -6, 7], bits=100, int_bits=50)
    b = APyFixed.from_float(3, bits=7, int_bits=3)
    c = APyFixedArray.from_float([-5, -6, 7], bits=10, int_bits=5)
    d = APyFixed.from_float(3, bits=100, int_bits=50)
    assert (a + b).is_identical(c + d)
    assert (b + a).is_identical(d + c)

    z = APyFixed.from_float(0, bits=100, int_bits=50)
    assert (a + z).is_identical(
        APyFixedArray.from_float([-5, -6, 7], bits=101, int_bits=51)
    )
    assert (z + a).is_identical(
        APyFixedArray.from_float([-5, -6, 7], bits=101, int_bits=51)
    )


def test_array_sum():
    a = APyFixedArray([-5, -6, 7, -1], bits=10, int_bits=5)
    assert sum(a).is_identical(APyFixed(-5, bits=13, int_bits=8))
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
    a = APyFixedArray([-5, -6, 7, 8, 9], bits=10, int_bits=5)
    b = APyFixedArray([1, -2, 3, -2, -1], bits=7, int_bits=2)
    assert (a - b).is_identical(APyFixedArray([-6, -4, 4, 10, 10], bits=11, int_bits=6))
    assert (b - a).is_identical(
        APyFixedArray([6, 4, -4, -10, -10], bits=11, int_bits=6)
    )

    a = APyFixedArray([1, 2, 3, 4, 5], bits=10, frac_bits=0)
    b = APyFixedArray([1, 2, 3, 8, 7], bits=10, frac_bits=3)
    assert (a - b).is_identical(
        APyFixedArray([7, 14, 21, 24, 33], bits=14, frac_bits=3)
    )
    assert (b - a).is_identical(
        APyFixedArray([-7, -14, -21, -24, -33], bits=14, frac_bits=3)
    )

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

    a = APyFixedArray.from_float([1.2345, -5.4321], bits=255, int_bits=128)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=255, int_bits=128)
    assert (a - b).is_identical(
        APyFixedArray(
            [
                115792089237316195423570985008687907851799624558175188917373909031040791347200,
                115792089237316195423570985008687907851379545976211290445581358423608704106496,
            ],
            bits=256,
            int_bits=129,
        )
    )
    assert (b - a).is_identical(
        APyFixedArray(
            [
                1470360107465375122083674976872338292736,
                1890438689429273593876225584304425533440,
            ],
            bits=256,
            int_bits=129,
        )
    )

    a = APyFixedArray.from_float([1.2345, -5.4321], bits=252, int_bits=128)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=252, int_bits=130)
    assert (a - b).is_identical(
        APyFixedArray(
            [
                57896044618658097711785492504343953926451197319387110129468332631847522533376,
                57896044618658097711785492504343953926398687496641622820494263805918511628288,
            ],
            bits=255,
            int_bits=131,
        )
    )
    assert (b - a).is_identical(
        APyFixedArray(
            [
                183795013433171890260459372109042286592,
                236304836178659199234528198038053191680,
            ],
            bits=255,
            int_bits=131,
        )
    )


def test_array_sub_scalar():
    a = APyFixedArray([-5, -6, 7, 10, -11], bits=10, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    assert (a - b).is_identical(APyFixedArray([-8, -9, 4, 7, -14], bits=11, int_bits=6))
    assert (-(b - a)).is_identical((a - b).cast(12, 7))

    a = APyFixedArray([-5, -6, 7], bits=7, int_bits=4)
    b = APyFixed(3, bits=7, int_bits=4)
    assert (a - b).is_identical(APyFixedArray([248, 247, 4], bits=8, int_bits=5))

    a = APyFixedArray.from_float([-5, -6, 7], bits=100, int_bits=50)
    b = APyFixed.from_float(3, bits=7, int_bits=3)
    c = APyFixedArray.from_float([-5, -6, 7], bits=10, int_bits=5)
    d = APyFixed.from_float(3, bits=100, int_bits=50)
    assert (a - b).is_identical(c - d)
    assert (b - a).is_identical(d - c)

    z = APyFixed.from_float(0, bits=100, int_bits=50)
    assert (a - z).is_identical(
        APyFixedArray.from_float([-5, -6, 7], bits=101, int_bits=51)
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

    a = APyFixedArray([-5, -6, 7], bits=100, int_bits=50)
    b = APyFixed(3, bits=27, int_bits=20)
    assert (a * b).is_identical(
        APyFixedArray(
            [
                170141183460469231731687303715884105713,
                170141183460469231731687303715884105710,
                21,
            ],
            bits=127,
            int_bits=70,
        )
    )


def test_array_prod():
    a = APyFixedArray([-5, -6, 7, -1], bits=10, int_bits=5)
    assert math.prod(a).is_identical(APyFixed(1099511627566, bits=40, int_bits=20))
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
    with pytest.raises(ValueError, match="Not implemented: high-dimensional"):
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

    a = APyFixedArray([1, 2, -3], bits=10, int_bits=5)
    b = a << 2
    assert not a.is_identical(b)
    a <<= 2
    assert a.is_identical(b)
    a = APyFixedArray([1, 2, -3], bits=10, int_bits=5)
    b = a >> 2
    assert not a.is_identical(b)
    a >>= 2
    assert a.is_identical(b)


def test_unary_negate():
    a = APyFixedArray.from_float([0, -1, 2, -3, 4], bits=100, int_bits=50)
    b = APyFixedArray.from_float([0, -1, 2, -3, 4], bits=32, int_bits=20)
    assert (-a).is_identical(
        APyFixedArray.from_float([0, 1, -2, 3, -4], bits=101, int_bits=51)
    )
    assert (-b).is_identical(
        APyFixedArray.from_float([0, 1, -2, 3, -4], bits=33, int_bits=21)
    )


def test_abs():
    a = APyFixedArray.from_float([0, -1, 2, -3, 4], bits=100, int_bits=50)
    b = APyFixedArray.from_float([0, -1, 2, -3, 4], bits=32, int_bits=20)
    assert abs(a).is_identical(
        APyFixedArray.from_float([0, 1, 2, 3, 4], bits=101, int_bits=51)
    )
    assert abs(b).is_identical(
        APyFixedArray.from_float([0, 1, 2, 3, 4], bits=33, int_bits=21)
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
def test_huge_narrowing_cast(mode):
    a = APyFixedArray.from_float([-0.75, 0.5], 1000, 500)
    assert a.cast(10, 5, mode).is_identical(
        APyFixedArray.from_float([-0.75, 0.5], 10, 5)
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
def test_huge_extending_cast(mode):
    a = APyFixedArray.from_float([-0.75, 0.5], 10, 5)
    print(a.cast(1000, 500, mode))
    print(APyFixedArray.from_float([-0.75, 0.5], 1000, 500))
    assert a.cast(1000, 500, mode).is_identical(
        APyFixedArray.from_float([-0.75, 0.5], 1000, 500)
    )


def test_long_matrix_addition():
    np = pytest.importorskip("numpy")
    a = APyFixedArray(range(10000), bits=20, int_bits=20)
    b = APyFixedArray(range(10000), bits=20, int_bits=20)
    assert (a + b).is_identical(
        APyFixedArray(2 * np.array(range(10000)), bits=21, int_bits=21)
    )


def test_operation_with_numbers():
    a = APyFixedArray([5, 7, -1], 6, 2)
    # Integer
    assert (a + 0).is_identical(a)
    assert (0 + a).is_identical(a)
    assert (a - 0).is_identical(a)
    assert (0 - a).is_identical(-a)
    assert (a * 1).is_identical(a)
    assert (1 * a).is_identical(a)

    # Float
    assert (a + 0.0).is_identical(a)
    assert (0.0 + a).is_identical(a)
    assert (a - 0.0).is_identical(a)
    assert (0.0 - a).is_identical(-a)
    assert (a * 1.0).is_identical(a)
    assert (1.0 * a).is_identical(a)
