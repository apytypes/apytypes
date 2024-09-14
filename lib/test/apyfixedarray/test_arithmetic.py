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
    with pytest.raises(ValueError, match="APyFixedArray.__div__: shape mismatch"):
        _ = a / b


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
    # Equally many fractional bits
    a = APyFixedArray([-5, -6, 7, 10, -11], bits=10, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    assert (a + b).is_identical(
        APyFixedArray([-2, -3, 10, 13, -8], bits=11, int_bits=6)
    )
    assert (b + a).is_identical(
        APyFixedArray([-2, -3, 10, 13, -8], bits=11, int_bits=6)
    )

    # Different number of fractional bits
    a = APyFixedArray([-5, -6, 7, 10, -11], bits=11, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    assert (a + b).is_identical(
        APyFixedArray([1, 0, 13, 16, 4091], bits=12, int_bits=6)
    )
    assert (b + a).is_identical(
        APyFixedArray([1, 0, 13, 16, 4091], bits=12, int_bits=6)
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
    assert sum(a).is_identical(APyFixed(16379, bits=14, int_bits=9))
    b = APyFixedArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [4.0, 5.0, 6.0],
        ],
        bits=8,
        int_bits=6,
    )
    assert sum(b).is_identical(APyFixedArray([20, 28, 36], bits=10, int_bits=8))


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
    # Same number of fractional bits
    a = APyFixedArray([-5, -6, 7, 10, -11], bits=10, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    assert (a - b).is_identical(APyFixedArray([-8, -9, 4, 7, -14], bits=11, int_bits=6))
    assert (-(b - a)).is_identical((a - b).cast(7, 5))

    # Different number of fractional bits
    a = APyFixedArray([-5, -6, 7, 10, -11], bits=11, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    assert (a - b).is_identical(
        APyFixedArray([4085, 4084, 1, 4, 4079], bits=12, int_bits=6)
    )
    assert (b - a).is_identical(
        APyFixedArray([11, 12, 4095, 4092, 17], bits=12, int_bits=6)
    )

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
    a = APyFixedArray([-5, -6, 7, 8], bits=10, int_bits=5)
    b = APyFixedArray([1, -2, 3, -4], bits=7, int_bits=3)
    res = a * b
    assert res.is_identical(APyFixedArray([-5, 12, 21, -32], bits=17, int_bits=8))
    assert (a * b).is_identical(b * a)

    a = APyFixedArray.from_float([1.2345, 5.4321], bits=128, int_bits=64)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=128, int_bits=64)
    assert (a * b).is_identical(b * a)
    res = a * b
    assert res.is_identical(
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
    res = a * b
    assert res.is_identical(
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
    a = APyFixedArray([-5, -6, 7, 8, 9], bits=10, int_bits=5)
    b = APyFixed(3, bits=7, int_bits=2)
    res = a * b
    assert res.is_identical(
        APyFixedArray([131057, 131054, 21, 24, 27], bits=17, int_bits=7)
    )
    res = b * a
    assert res.is_identical(
        APyFixedArray([131057, 131054, 21, 24, 27], bits=17, int_bits=7)
    )
    b = APyFixed(-3, bits=7, int_bits=2)
    res = a * b
    assert res.is_identical(
        APyFixedArray([15, 18, 131051, 131048, 131045], bits=17, int_bits=7)
    )
    res = b * a
    assert res.is_identical(
        APyFixedArray([15, 18, 131051, 131048, 131045], bits=17, int_bits=7)
    )

    a = APyFixedArray([-5, -6, 7], bits=100, int_bits=50)
    b = APyFixed(3, bits=27, int_bits=20)
    res = a * b
    assert res.is_identical(
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

    a = APyFixedArray.from_float([-5, 6, -7], bits=27, int_bits=20)
    b = APyFixed.from_float(3, bits=200, int_bits=50)
    assert (a * b).is_identical(
        APyFixedArray.from_float([-15, 18, -21], bits=227, int_bits=70)
    )


def test_array_div():
    a = APyFixedArray.from_float([-5, -6, 7, 8, 9, 5], bits=10, int_bits=5)
    b = APyFixedArray.from_float([1, -2, 3, -4, -2, 2], bits=7, int_bits=3)
    assert (a / b).is_identical(
        APyFixedArray([260864, 768, 597, 261632, 260992, 640], bits=18, int_bits=10)
    )
    assert (b / a).is_identical(
        APyFixedArray([262042, 170, 219, 261888, 262031, 204], bits=18, int_bits=9)
    )

    a = APyFixedArray.from_float([-5, -6, 7, 8, 9, 5], bits=100, int_bits=50)
    b = APyFixedArray.from_float([1, -2, 3, -4, -2, 2], bits=250, int_bits=100)
    assert (a / b).is_identical(
        APyFixedArray(
            [
                4586997231980143023221641790604173881593129978336562247475170542535382222377564324710189858744569459376128,
                4281743078117879643174857908348485409148239872,
                3330244616313906389136000595382155318226408789,
                4586997231980143023221641790604173881593129978336562247475174824278460340257207499568098207229978607616000,
                4586997231980143023221641790604173881593129978336562247475171256159228575357504853853174583492137650749440,
                3568119231764899702645714923623737840956866560,
            ],
            bits=351,
            int_bits=201,
        )
    )
    assert (b / a).is_identical(
        APyFixedArray(
            [
                4586997231980143023221641790604173881593129978015174638623379623665453333708737095635596507463692806048973,
                535646014752996758513987364113720867507400997927597611767125,
                688687733253852975232269468146212543938086997335482643700589,
                4586997231980143023221641790604173881593129977533093225345682541002864706006388314878935609328854955458560,
                4586997231980143023221641790604173881593129977979464904306513173097854176101155704468436440935186298597832,
                642775217703596110216784836936465041008881197513117134120550,
            ],
            bits=351,
            int_bits=151,
        )
    )

    # Does not die
    a = APyFixedArray.from_float([5], bits=100, int_bits=50)
    b = APyFixedArray.from_float([0], bits=250, int_bits=100)
    assert (a / b).is_identical(APyFixedArray([0], bits=351, int_bits=201))


def test_array_div_scalar():
    a = APyFixedArray.from_float([-5, -6, 7, 8, 9, 5], bits=10, int_bits=5)
    b = APyFixed.from_float(2.25, bits=6, int_bits=4)
    assert (a / b).is_identical(
        APyFixedArray([129935, 129707, 1592, 1820, 2048, 1137], bits=17, int_bits=8)
    )
    assert (b / a).is_identical(
        APyFixedArray([131015, 131024, 41, 36, 32, 57], bits=17, int_bits=10)
    )

    a = APyFixedArray.from_float([-5, -6, 7, 8, 9, 5], bits=100, int_bits=50)
    b = APyFixed.from_float(2.25, bits=123, int_bits=10)
    assert (a / b).is_identical(
        APyFixedArray(
            [
                26959946667150639794667015087019630673637144422538010433315595033714,
                26959946667150639794667015087019630673637144422537498023757991990614,
                3586866903221301703,
                4099276460824344803,
                4611686018427387904,
                2562047788015215502,
            ],
            bits=224,
            int_bits=164,
        )
    )

    assert (b / a).is_identical(
        APyFixedArray(
            [
                26959946667150639789405609192628380168103879024761953610342253094503,
                26959946667150639790282510175026921919026089924391723422135812620288,
                3758147067422321789666618141270442050543826539081,
                3288378683994531565958290873611636794225848221696,
                2923003274661805836407369665432566039311865085952,
                5261405894391250505533265397778618870761357154713,
            ],
            bits=224,
            int_bits=61,
        )
    )


def test_array_prod():
    a = APyFixedArray([-5, -6, 7, -1], bits=10, int_bits=5)
    assert math.prod(a).is_identical(APyFixed(1125899906835904, bits=50, int_bits=25))
    b = APyFixedArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [4.0, 5.0, 6.0],
        ],
        bits=8,
        int_bits=6,
    )
    assert math.prod(b).is_identical(
        APyFixedArray([256, 640, 1152], bits=24, int_bits=18)
    )


def test_transpose():
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
    a = APyFixedArray.from_float([-0.75, 0.5], 500, 500)
    assert a.cast(5, 5, mode).is_identical(APyFixedArray.from_float([-0.75, 0.5], 5, 5))


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
    a = APyFixedArray.from_float([-0.75, 0.5], 5, 5)
    assert a.cast(500, 500, mode).is_identical(
        APyFixedArray.from_float([-0.75, 0.5], 500, 500)
    )


def test_long_matrix_addition():
    np = pytest.importorskip("numpy")
    a = APyFixedArray(range(10000), bits=20, int_bits=20)
    b = APyFixedArray(range(10000), bits=20, int_bits=20)
    assert (a + b).is_identical(
        APyFixedArray(2 * np.array(range(10000)), bits=21, int_bits=21)
    )


def test_operation_with_integers():
    a = APyFixedArray([5], 6, 2)
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
    a = APyFixedArray([5], 6, 2)
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
