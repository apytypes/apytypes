import math

import pytest

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyFixed,
    APyFixedArray,
    QuantizationMode,
    fx,
)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_array_raises(fixed_array: type[APyCFixedArray]):
    a = fixed_array([5, 6, 7], bits=10, int_bits=5)
    b = fixed_array([1, 2, 3, 11], bits=7, int_bits=2)
    with pytest.raises(ValueError, match="APyC?FixedArray.__add__: shape mismatch"):
        _ = a + b
    with pytest.raises(ValueError, match="APyC?FixedArray.__sub__: shape mismatch"):
        _ = a - b
    with pytest.raises(ValueError, match="APyC?FixedArray.__mul__: shape mismatch"):
        _ = a * b
    with pytest.raises(ValueError, match="APyC?FixedArray.__truediv__: shape mismatch"):
        _ = a / b


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_array_add(fixed_array: type[APyCFixedArray]):
    a = fixed_array([-5, -6, 7, 8, 9], bits=10, int_bits=5)
    b = fixed_array([1, -2, 3, -2, -1], bits=7, int_bits=2)
    assert (a + b).is_identical(fixed_array([-4, -8, 10, 6, 8], bits=11, int_bits=6))
    assert (a + b).is_identical(b + a)

    a = fixed_array([1, 2, 3, 4, 5], bits=10, frac_bits=0)
    b = fixed_array([1, 2, 3, 8, 7], bits=10, frac_bits=3)
    assert (a + b).is_identical(fixed_array([9, 18, 27, 40, 47], bits=14, frac_bits=3))
    assert (a + b).is_identical(b + a)

    a = fixed_array.from_float([1.2345, -5.4321, 0], bits=256, int_bits=128)
    b = fixed_array.from_float([9.8765, 5.6789, 0], bits=256, int_bits=128)
    assert (a + b).is_identical(b + a)
    assert (a + b).is_identical(
        fixed_array(
            [
                3780877378858547263310314894523174486016,
                83981688156087430476747255228080848896,
                0,
            ],
            bits=257,
            int_bits=129,
        )
    )

    a = fixed_array.from_float([1.2345, -5.4321, 0], bits=255, int_bits=128)
    b = fixed_array.from_float([9.8765, 5.6789, 0], bits=255, int_bits=128)
    assert (a + b).is_identical(b + a)
    assert (a + b).is_identical(
        fixed_array(
            [
                1890438689429273631655157447261587243008,
                41990844078043715238373627614040424448,
                0,
            ],
            bits=256,
            int_bits=129,
        )
    )

    a = fixed_array.from_float([1.2345, -5.4321], bits=256, int_bits=128)
    b = fixed_array.from_float([9.8765, 5.6789], bits=25, int_bits=12)
    assert (a + b).is_identical(b + a)

    a = fixed_array.from_float([1.2345, -5.4321, 0], bits=252, int_bits=128)
    b = fixed_array.from_float([9.8765, 5.6789, 0], bits=252, int_bits=130)
    assert (a + b).is_identical(b + a)
    assert (a + b).is_identical(
        fixed_array(
            [
                236304836178659203956894680907698405376,
                5248855509755464404796703451755053056,
                0,
            ],
            bits=255,
            int_bits=131,
        )
    )

    c = fixed_array.from_float(
        [[9.8765, 5.6789, 0], [1.2345, -5.4321, 0]], bits=252, int_bits=130
    )

    assert (b + c).is_identical(
        fixed_array(
            [
                [
                    105024962402957773554338513254185172992,
                    60388422922103665909831225372452061184,
                    0,
                ],
                [
                    59076209044664800989223670226924601344,
                    1312213877438866101199175862938763264,
                    0,
                ],
            ],
            bits=253,
            int_bits=131,
        )
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_array_add_scalar(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]
):
    # Equally many fractional bits
    a = fixed_array([-5, -6, 7, 10, -11], bits=10, int_bits=5)
    b = fixed_scalar(3, bits=7, int_bits=2)
    assert (a + b).is_identical(fixed_array([-2, -3, 10, 13, -8], bits=11, int_bits=6))
    assert (b + a).is_identical(fixed_array([-2, -3, 10, 13, -8], bits=11, int_bits=6))

    # Different number of fractional bits
    a = fixed_array([-5, -6, 7, 10, -11], bits=11, int_bits=5)
    b = fixed_scalar(3, bits=7, int_bits=2)
    assert (a + b).is_identical(fixed_array([1, 0, 13, 16, 4091], bits=12, int_bits=6))
    assert (b + a).is_identical(fixed_array([1, 0, 13, 16, 4091], bits=12, int_bits=6))

    a = fixed_array([-5, -6, 7], bits=7, int_bits=4)
    b = fixed_scalar(3, bits=7, int_bits=4)
    assert (a + b).is_identical(fixed_array([254, 253, 10], bits=8, int_bits=5))

    a = fixed_array.from_float([-5, -6, 7], bits=100, int_bits=50)
    b = fixed_scalar.from_float(3, bits=7, int_bits=3)
    c = fixed_array.from_float([-5, -6, 7], bits=10, int_bits=5)
    d = fixed_scalar.from_float(3, bits=100, int_bits=50)
    assert (a + b).is_identical(c + d)
    assert (b + a).is_identical(d + c)

    z = fixed_scalar.from_float(0, bits=100, int_bits=50)
    assert (a + z).is_identical(
        fixed_array.from_float([-5, -6, 7], bits=101, int_bits=51)
    )
    assert (z + a).is_identical(
        fixed_array.from_float([-5, -6, 7], bits=101, int_bits=51)
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_array_sum(fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]):
    a = fixed_array([-5, -6, 7, -1], bits=10, int_bits=5)
    assert sum(a).is_identical(fixed_scalar(16379, bits=14, int_bits=9))
    b = fixed_array.from_float(
        [
            [1.0, 2.0, 3.0],
            [4.0, 5.0, 6.0],
        ],
        bits=8,
        int_bits=6,
    )
    assert sum(b).is_identical(fixed_array([20, 28, 36], bits=10, int_bits=8))


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_array_sub(fixed_array: type[APyCFixedArray]):
    a = fixed_array([-5, -6, 7, 8, 9], bits=10, int_bits=5)
    b = fixed_array([1, -2, 3, -2, -1], bits=7, int_bits=2)
    assert (a - b).is_identical(fixed_array([-6, -4, 4, 10, 10], bits=11, int_bits=6))
    assert (b - a).is_identical(fixed_array([6, 4, -4, -10, -10], bits=11, int_bits=6))

    a = fixed_array([1, 2, 3, 4, 5], bits=10, frac_bits=0)
    b = fixed_array([1, 2, 3, 8, 7], bits=10, frac_bits=3)
    assert (a - b).is_identical(fixed_array([7, 14, 21, 24, 33], bits=14, frac_bits=3))
    assert (b - a).is_identical(
        fixed_array([-7, -14, -21, -24, -33], bits=14, frac_bits=3)
    )

    a = fixed_array.from_float([1.2345, -5.4321, 0], bits=256, int_bits=128)
    b = fixed_array.from_float([9.8765, 5.6789, 0], bits=256, int_bits=128)
    assert (a - b).is_identical(
        fixed_array(
            [
                0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF75BA5E353F7CED0000000000000000000,
                0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4E395810624DD40000000000000000000,
                0,
            ],
            bits=257,
            int_bits=129,
        )
    )

    a = fixed_array.from_float([1.2345, -5.4321, 0], bits=255, int_bits=128)
    b = fixed_array.from_float([9.8765, 5.6789, 0], bits=255, int_bits=128)
    assert (a - b).is_identical(
        fixed_array(
            [
                115792089237316195423570985008687907851799624558175188917373909031040791347200,
                115792089237316195423570985008687907851379545976211290445581358423608704106496,
                0,
            ],
            bits=256,
            int_bits=129,
        )
    )
    assert (b - a).is_identical(
        fixed_array(
            [
                1470360107465375122083674976872338292736,
                1890438689429273593876225584304425533440,
                0,
            ],
            bits=256,
            int_bits=129,
        )
    )

    a = fixed_array.from_float([1.2345, -5.4321, 0], bits=252, int_bits=128)
    b = fixed_array.from_float([9.8765, 5.6789, 0], bits=252, int_bits=130)
    assert (a - b).is_identical(
        fixed_array(
            [
                57896044618658097711785492504343953926451197319387110129468332631847522533376,
                57896044618658097711785492504343953926398687496641622820494263805918511628288,
                0,
            ],
            bits=255,
            int_bits=131,
        )
    )
    assert (b - a).is_identical(
        fixed_array(
            [
                183795013433171890260459372109042286592,
                236304836178659199234528198038053191680,
                0,
            ],
            bits=255,
            int_bits=131,
        )
    )

    c = fixed_array.from_float(
        [[9.8765, 5.6789, 0], [1.2345, -5.4321, 0]], bits=252, int_bits=130
    )

    assert (b - c).is_identical(
        fixed_array(
            [
                [0, 0, 0],
                [
                    45948753358292972565114843027260571648,
                    59076209044664799808632049509513297920,
                    0,
                ],
            ],
            bits=253,
            int_bits=131,
        )
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_array_sub_scalar(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]
):
    # Same number of fractional bits
    a = fixed_array([-5, -6, 7, 10, -11], bits=10, int_bits=5)
    b = fixed_scalar(3, bits=7, int_bits=2)
    assert (a - b).is_identical(fixed_array([-8, -9, 4, 7, -14], bits=11, int_bits=6))
    assert (-(b - a)).is_identical((a - b).cast(7, 5))

    # Different number of fractional bits
    a = fixed_array([-5, -6, 7, 10, -11], bits=11, int_bits=5)
    b = fixed_scalar(3, bits=7, int_bits=2)
    assert (a - b).is_identical(
        fixed_array([4085, 4084, 1, 4, 4079], bits=12, int_bits=6)
    )
    assert (b - a).is_identical(
        fixed_array([11, 12, 4095, 4092, 17], bits=12, int_bits=6)
    )

    a = fixed_array([-5, -6, 7], bits=7, int_bits=4)
    b = fixed_scalar(3, bits=7, int_bits=4)
    assert (a - b).is_identical(fixed_array([248, 247, 4], bits=8, int_bits=5))

    a = fixed_array.from_float([-5, -6, 7], bits=100, int_bits=50)
    b = fixed_scalar.from_float(3, bits=7, int_bits=3)
    c = fixed_array.from_float([-5, -6, 7], bits=10, int_bits=5)
    d = fixed_scalar.from_float(3, bits=100, int_bits=50)
    assert (a - b).is_identical(c - d)
    assert (b - a).is_identical(d - c)

    z = fixed_scalar.from_float(0, bits=100, int_bits=50)
    assert (a - z).is_identical(
        fixed_array.from_float([-5, -6, 7], bits=101, int_bits=51)
    )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_array_mul(fixed_array: type[APyCFixedArray]):
    # Complex mult results in one additional bit compared to real mult
    cb = fixed_array == APyCFixedArray

    a = fixed_array([-5, -6, 7, 8], bits=10, int_bits=5)
    b = fixed_array([1, -2, 3, -4], bits=7, int_bits=3)
    res = a * b
    assert res.is_identical(
        fixed_array([-5, 12, 21, -32], bits=17 + cb, int_bits=8 + cb)
    )
    assert (a * b).is_identical(b * a)

    a = fixed_array.from_float([1.2345, 5.4321, 0], bits=128, int_bits=64)
    b = fixed_array.from_float([9.8765, 5.6789, 0], bits=128, int_bits=64)
    assert (a * b).is_identical(b * a)
    res = a * b
    assert res.is_identical(
        fixed_array(
            [
                4148906114766443653661449407827571376128,
                10497150468965098645933053068265826287616,
                0,
            ],
            bits=256 + cb,
            int_bits=128 + cb,
        )
    )

    a = fixed_array.from_float([1.2345, 5.4321, 0], bits=256, int_bits=128)
    b = fixed_array.from_float([9.8765, 5.6789, 0], bits=256, int_bits=128)
    assert (a * b).is_identical(b * a)
    res = a * b
    assert res.is_identical(
        fixed_array(
            [
                0xC314A4095F245001AC14C660A2000000000000000000000000000000000000000,
                0x1ED92DA453060C056E0ACA8950A000000000000000000000000000000000000000,
                0,
            ],
            bits=512 + cb,
            int_bits=256 + cb,
        )
    )

    c = fixed_array.from_float(
        [[9.8765, 5.6789, 0], [1.2345, -5.4321, 0]], bits=252, int_bits=130
    )

    assert (b * c).is_identical(
        fixed_array(
            [
                [
                    0x1862E569B17481C65668C26139000000000000000000000000000000000000000,
                    0x80FFE726C0333176828327924040000000000000000000000000000000000000,
                    0,
                ],
                [
                    0x30C5290257C914006B0531982880000000000000000000000000000000000000,
                    0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF849B496EB3E7CFEA47D4D5DABD80000000000000000000000000000000000000,
                    0,
                ],
            ],
            bits=508 + cb,
            int_bits=258 + cb,
        )
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_array_mul_scalar(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]
):
    # Complex mult results in one additional bit compared to real mult
    cb = fixed_array == APyCFixedArray

    a = fixed_array([-5, -6, 7, 8, 9], bits=10, int_bits=5)
    b = fixed_scalar(3, bits=7, int_bits=2)
    res = a * b
    assert res.is_identical(
        fixed_array([-15, -18, 21, 24, 27], bits=17 + cb, int_bits=7 + cb)
    )
    res = b * a
    assert res.is_identical(
        fixed_array([-15, -18, 21, 24, 27], bits=17 + cb, int_bits=7 + cb)
    )
    b = fixed_scalar(-3, bits=7, int_bits=2)
    res = a * b
    assert res.is_identical(
        fixed_array([15, 18, -21, -24, -27], bits=17 + cb, int_bits=7 + cb)
    )
    res = b * a
    assert res.is_identical(
        fixed_array([15, 18, -21, -24, -27], bits=17 + cb, int_bits=7 + cb)
    )

    a = fixed_array([-5, -6, 7], bits=100, int_bits=50)
    b = fixed_scalar(3, bits=27, int_bits=20)
    res = a * b
    assert res.is_identical(
        fixed_array(
            [
                -15,
                -18,
                21,
            ],
            bits=127 + cb,
            int_bits=70 + cb,
        )
    )

    a = fixed_array.from_float([-5, 6, -7], bits=27, int_bits=20)
    b = fixed_scalar.from_float(3, bits=200, int_bits=50)
    assert (a * b).is_identical(
        fixed_array.from_float([-15, 18, -21], bits=227 + cb, int_bits=70 + cb)
    )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_array_div(fixed_array: type[APyCFixedArray]):
    a = fixed_array.from_float([-5, -6, 7, 8, 9, 5], bits=10, int_bits=5)
    b = fixed_array.from_float([1, -2, 3, -4, -2, 2], bits=7, int_bits=3)
    assert (a / b).is_identical(
        fixed_array([260864, 768, 597, 261632, 260992, 640], bits=18, int_bits=10)
    )
    assert (b / a).is_identical(
        fixed_array([262042, 170, 219, 261888, 262031, 204], bits=18, int_bits=9)
    )

    a = fixed_array.from_float([-5, -6, 7, 8, 9, 5], bits=100, int_bits=50)
    b = fixed_array.from_float([1, -2, 3, -4, -2, 2], bits=250, int_bits=100)
    assert (a / b).is_identical(
        fixed_array(
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
        fixed_array(
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

    c = fixed_array.from_float(
        [
            [1, -2, 3, -4, -2, 2],
            [1, 3, 4, 5, -1, -3],
        ],
        bits=250,
        int_bits=100,
    )

    assert (b / c).is_identical(
        fixed_array(
            [
                [
                    0x400000000000000000000000000000000000000000000000000000000000000,
                    0x400000000000000000000000000000000000000000000000000000000000000,
                    0x400000000000000000000000000000000000000000000000000000000000000,
                    0x400000000000000000000000000000000000000000000000000000000000000,
                    0x400000000000000000000000000000000000000000000000000000000000000,
                    0x400000000000000000000000000000000000000000000000000000000000000,
                ],
                [
                    0x400000000000000000000000000000000000000000000000000000000000000,
                    0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD55555555555555555555555555555555555555555555555555555555555556,
                    0x300000000000000000000000000000000000000000000000000000000000000,
                    0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCD,
                    0x800000000000000000000000000000000000000000000000000000000000000,
                    0x1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD55555555555555555555555555555555555555555555555555555555555556,
                ],
            ],
            bits=501,
            int_bits=251,
        )
    )

    # Does not die
    a = fixed_array.from_float([5], bits=100, int_bits=50)
    b = fixed_array.from_float([0], bits=250, int_bits=100)
    assert (a / b).is_identical(fixed_array([0], bits=351, int_bits=201))


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_array_div_scalar(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]
):
    a = fixed_array.from_float([-5, -6, 7, 8, 9, 5], bits=10, int_bits=5)
    b = fixed_scalar.from_float(2.25, bits=6, int_bits=4)
    assert (a / b).is_identical(
        fixed_array([129935, 129707, 1592, 1820, 2048, 1137], bits=17, int_bits=8)
    )
    assert (b / a).is_identical(
        fixed_array([131015, 131024, 41, 36, 32, 57], bits=17, int_bits=10)
    )

    a = fixed_array.from_float([-5, -6, 7, 8, 9, 5], bits=100, int_bits=50)
    b = fixed_scalar.from_float(2.25, bits=123, int_bits=10)
    assert (a / b).is_identical(
        fixed_array(
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
        fixed_array(
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

    a = fixed_array.from_float([-5, -6, 7, 8, 9, 5], bits=100, int_bits=50)
    b = fixed_scalar.from_float(2.25, bits=14, int_bits=4)
    assert (a / b).is_identical(
        fixed_array(
            [
                41538374868278620988211973946023026,
                41538374868278620980205574608475478,
                56044795362832839,
                64051194700380387,
                72057594037927936,
                40031996687737742,
            ],
            bits=115,
            int_bits=61,
        )
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_rdiv_issue(fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]):
    a = fixed_array.from_float([-5, -6, 7, 8, 9, 5], bits=100, int_bits=50)
    b = fixed_scalar.from_float(2.25, bits=14, int_bits=4)
    assert (a / b).is_identical(
        fixed_array(
            [
                41538374868278620988211973946023026,
                41538374868278620980205574608475478,
                56044795362832839,
                64051194700380387,
                72057594037927936,
                40031996687737742,
            ],
            bits=115,
            int_bits=61,
        )
    )

    assert (b / a).is_identical(
        fixed_array(
            [
                41538374868278620509429293560679629,
                41538374868278620595898406406193152,
                370581912195057956,
                324259173170675712,
                288230376151711744,
                518814677073081139,
            ],
            bits=115,
            int_bits=55,
        )
    )

    a = fixed_array.from_float([-5, -6, 7, 8, 9, 5], bits=200, int_bits=100)
    b = fixed_scalar.from_float(2.25, bits=54, int_bits=4)

    assert (a / b).is_identical(
        fixed_array(
            [
                57896044618658097711785492504343953926634992287748260678280635506296006406600,
                57896044618658097711785492504343953926634992278733856409991004206763894723926,
                63100829878027419096724781778716,
                72115234146317050396256893461390,
                81129638414606681695789005144064,
                45072021341448156497660558413368,
            ],
            bits=255,
            int_bits=151,
        )
    )

    assert (b / a).is_identical(
        fixed_array(
            [
                57896044618658097711785492504343311665173274650873805791042539731145192583988,
                57896044618658097711785492504343418708750227597864885162490248443280421289984,
                458758186941201390340163347323052008123025700,
                401413413573551216547642928907670507107647488,
                356811923176489970264571492362373784095686656,
                642261461717681946476228686252272811372235980,
            ],
            bits=255,
            int_bits=105,
        )
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_array_prod(fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]):
    # Complex mult results in one additional bit compared to real mult
    cb = fixed_array == APyCFixedArray

    a = fixed_array([-5, -6, 7, -1], int_bits=5, frac_bits=5)
    assert math.prod(a).is_identical(
        fixed_scalar(-6720, int_bits=25 + 4 * cb, frac_bits=25)
    )
    b = fixed_array.from_float(
        [
            [1.0, 2.0, 3.0],
            [4.0, 5.0, 6.0],
        ],
        bits=8,
        int_bits=6,
    )
    assert math.prod(b).is_identical(
        fixed_array([256, 640, 1152], int_bits=18 + 2 * cb, frac_bits=6)
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_prod_negative_int_bits(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]
):
    # Complex mult results in one additional bit compared to real mult
    cb = fixed_array == APyCFixedArray
    a = fixed_array(
        [[1, 2, 3], [1, 4, 4], [1, 5, 6], [1, 7, 8]], int_bits=-5, frac_bits=10
    )
    assert a.prod().is_identical(
        fixed_scalar(161280, int_bits=-60 + 11 * cb, frac_bits=120)
    )
    assert a.cumprod().is_identical(
        fixed_array(
            [
                1298074214633706907132624082305024,
                2535301200456458802993406410752,
                7427640235712281649394745344,
                7253554917687775048237056,
                28334198897217871282176,
                110680464442257309696,
                108086391056891904,
                527765581332480,
                3092376453120,
                3019898880,
                20643840,
                161280,
            ],
            int_bits=-5,
            frac_bits=120,
        )
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_prod_negative_frac_bits(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]
):
    # Complex mult results in one additional bit compared to real mult
    cb = fixed_array == APyCFixedArray
    a = fixed_array(
        [[1, 2, 3], [1, 4, 4], [1, 5, 6], [1, 7, 8]], int_bits=10, frac_bits=-5
    )
    assert a.prod().is_identical(
        fixed_scalar(161280, int_bits=120 + 11 * cb, frac_bits=-60)
    )
    assert a.cumprod().is_identical(
        fixed_array(
            [
                1,
                64,
                6144,
                196608,
                25165824,
                3221225472,
                103079215104,
                16492674416640,
                3166593487994880,
                101330991615836160,
                22698142121947299840,
                5810724383218508759040,
            ],
            int_bits=120 + 11 * cb,
            frac_bits=-5,
        )
    )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_transpose(fixed_array: type[APyCFixedArray]):
    # 1-D transposition simply returns the input (just like NumPy-arrays)
    assert fixed_array([], 1, 0).T.is_identical(fixed_array([], 1, 0))
    assert fixed_array([1, 2, 3], 5, 5).T.is_identical(fixed_array([1, 2, 3], 5, 5))

    # 2-D transposition returns the matrix transposition
    a = fixed_array.from_float(
        [
            [1.0, 2.0, 3.0],
            [4.0, 5.0, 6.0],
        ],
        bits=256,
        int_bits=128,
    )
    assert a.T.T.is_identical(a)
    assert a.T.is_identical(
        fixed_array.from_float(
            [
                [1.0, 4.0],
                [2.0, 5.0],
                [3.0, 6.0],
            ],
            bits=256,
            int_bits=128,
        )
    )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_shift(fixed_array: type[APyCFixedArray]):
    a = fixed_array([1, 2, -3], bits=10, int_bits=5)
    assert (a >> 2).is_identical(fixed_array([1, 2, -3], bits=10, int_bits=3))
    assert (a << 7).is_identical(fixed_array([1, 2, -3], bits=10, int_bits=12))
    assert (a >> -2).is_identical(fixed_array([1, 2, -3], bits=10, int_bits=7))
    assert (a << -7).is_identical(fixed_array([1, 2, -3], bits=10, int_bits=-2))

    a = fixed_array([1, 2, -3], bits=10, int_bits=5)
    b = a << 2
    assert not a.is_identical(b)
    a <<= 2
    assert a.is_identical(b)
    a = fixed_array([1, 2, -3], bits=10, int_bits=5)
    b = a >> 2
    assert not a.is_identical(b)
    a >>= 2
    assert a.is_identical(b)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_unary_negate(fixed_array: type[APyCFixedArray]):
    a = fixed_array.from_float([0, -1, 2, -3, 4], bits=100, int_bits=50)
    b = fixed_array.from_float([0, -1, 2, -3, 4], bits=32, int_bits=20)
    assert (-a).is_identical(
        fixed_array.from_float([0, 1, -2, 3, -4], bits=101, int_bits=51)
    )
    assert (-b).is_identical(
        fixed_array.from_float([0, 1, -2, 3, -4], bits=33, int_bits=21)
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


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
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
def test_huge_narrowing_cast(fixed_array: type[APyCFixedArray], mode: QuantizationMode):
    a = fixed_array.from_float([-0.75, 0.5], 500, 500)
    assert a.cast(5, 5, mode).is_identical(fixed_array.from_float([-0.75, 0.5], 5, 5))


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
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
def test_huge_extending_cast(fixed_array: type[APyCFixedArray], mode: QuantizationMode):
    a = fixed_array.from_float([-0.75, 0.5], 5, 5)
    assert a.cast(500, 500, mode).is_identical(
        fixed_array.from_float([-0.75, 0.5], 500, 500)
    )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_long_matrix_addition(fixed_array: type[APyCFixedArray]):
    np = pytest.importorskip("numpy")
    a = fixed_array(range(10000), bits=20, int_bits=20)
    b = fixed_array(range(10000), bits=20, int_bits=20)
    assert (a + b).is_identical(
        fixed_array(2 * np.array(range(10000)), bits=21, int_bits=21)
    )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_operation_with_integers(fixed_array: type[APyCFixedArray]):
    a = fixed_array([5], 6, 2)
    one = fixed_array([4], 6, 2)
    zero = fixed_array([0], 6, 2)

    # Integer identities
    assert (_ := a + 0).is_identical(_ := a + zero)
    assert (_ := 0 + a).is_identical(_ := zero + a)
    assert (_ := a - 0).is_identical(_ := a - zero)
    assert (_ := 0 - a).is_identical(_ := zero - a)
    assert (_ := a * 1).is_identical(_ := a * one)
    assert (_ := 1 * a).is_identical(_ := one * a)
    assert (_ := a / 1).is_identical(_ := a / one)

    # Other integer operations
    neg_two = fixed_array([248], bits=8, int_bits=6)
    assert (_ := a + (-2)).is_identical(_ := a + neg_two)
    assert (_ := (-2) + a).is_identical(_ := neg_two + a)
    assert (_ := a - (-2)).is_identical(_ := a - neg_two)
    assert (_ := (-2) - a).is_identical(_ := neg_two - a)
    assert (_ := a * (-2)).is_identical(_ := a * neg_two)
    assert (_ := (-2) * a).is_identical(_ := neg_two * a)
    assert (_ := a / (-2)).is_identical(_ := a / neg_two)
    assert (_ := (-2) / a).is_identical(_ := neg_two / a)


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)],
)
def test_operation_with_floats(
    fixed_array: type[APyCFixedArray],
    fixed_scalar: type[APyCFixed],
):
    a = fixed_array([5], 6, 2)
    one = fixed_array([4], 6, 2)
    zero = fixed_array([0], 6, 2)

    # Identities
    assert (_ := a + 0.0).is_identical(_ := a + zero)
    assert (_ := 0.0 + a).is_identical(_ := zero + a)
    assert (_ := a - 0.0).is_identical(_ := a - zero)
    assert (_ := 0.0 - a).is_identical(_ := zero - a)
    assert (_ := a * 1.0).is_identical(_ := a * one)
    assert (_ := 1.0 * a).is_identical(_ := one * a)
    assert (_ := a / 1.0).is_identical(_ := a / one)

    # Other operations. 2.125 should quantize to -2.25
    b = fixed_scalar(247, bits=8, int_bits=6)
    assert (_ := a + (-2.125)).is_identical(_ := a + b)
    assert (_ := (-2.125) + a).is_identical(_ := b + a)
    assert (_ := a - (-2.125)).is_identical(_ := a - b)
    assert (_ := (-2.125) - a).is_identical(_ := b - a)
    assert (_ := a * (-2.125)).is_identical(_ := a * b)
    assert (_ := (-2.125) * a).is_identical(_ := b * a)
    assert (_ := a / (-2.125)).is_identical(_ := a / b)
    assert (_ := (-2.125) / a).is_identical(_ := b / a)


def test_operation_with_numpy():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([1, 3, -7], 0, 4)
    n = np.array([0.3, 0.5, 0.7])
    nfx = APyFixedArray.from_array(n, 0, 4)

    assert (a + n).is_identical(a + nfx)
    assert (a - n).is_identical(a - nfx)
    assert (a * n).is_identical(a * nfx)
    assert (a / n).is_identical(a / nfx)


@pytest.mark.parametrize("frac_bits", range(10, 68))
def test_array_multiplication_with_different_frac_bits(frac_bits: int):
    a = fx(1 / 3, int_bits=3, frac_bits=frac_bits)
    b = fx(math.pi, int_bits=3, frac_bits=frac_bits)
    c = fx(-math.pi, int_bits=3, frac_bits=frac_bits)
    d = fx(-23 / 11, int_bits=3, frac_bits=frac_bits)

    av = fx([1 / 3, math.pi, -math.pi, -23 / 11], int_bits=3, frac_bits=frac_bits)
    bv = fx([math.pi, -23 / 11, 1 / 3, -math.pi], int_bits=3, frac_bits=frac_bits)
    res = av * bv
    assert res[0].is_identical(a * b)
    assert res[1].is_identical(b * d)
    assert res[2].is_identical(c * a)
    assert res[3].is_identical(d * c)


@pytest.mark.parametrize("frac_bits", range(10, 68))
def test_scalar_array_multiplication_with_different_frac_bits(frac_bits: int):
    a = fx(1 / 3, int_bits=3, frac_bits=frac_bits)
    b = fx(math.pi, int_bits=3, frac_bits=frac_bits)
    c = fx(-math.pi, int_bits=3, frac_bits=frac_bits)
    d = fx(-23 / 11, int_bits=3, frac_bits=frac_bits)

    av = fx([1 / 3, math.pi, -math.pi, -23 / 11], int_bits=3, frac_bits=frac_bits)
    res = b * av

    assert res[0].is_identical(b * a)
    assert res[1].is_identical(b * b)
    assert res[2].is_identical(b * c)
    assert res[3].is_identical(b * d)


def test_unary_arith():
    a = fx([2, -4], int_bits=5, frac_bits=0)
    nega = fx([-2, 4], int_bits=6, frac_bits=0)

    assert (-a).is_identical(nega)
    assert (+a).is_identical(a)


@pytest.mark.parametrize("limbs", range(1, 8))
@pytest.mark.parametrize("limb_size", [16, 32, 64, 128])
def test_min_max_msb(limbs: int, limb_size: int):
    #
    # Related issue: #721
    # https://github.com/apytypes/apytypes/issues/721
    #
    int_bits = limbs * limb_size
    for limb_idx in range(limbs):
        a_bits = 1 << ((limb_size * (limb_idx + 1)) - 1)
        b_bits = 1 << ((limb_size * (limb_idx + 1)) - 2)
        arr = APyFixedArray([a_bits, b_bits], int_bits=int_bits, frac_bits=0)
        if limb_idx + 1 == limbs:
            max = APyFixed(b_bits, int_bits=int_bits, frac_bits=0)
            min = APyFixed(a_bits, int_bits=int_bits, frac_bits=0)
            assert arr.max().is_identical(max)
            assert arr.min().is_identical(min)
        else:
            max = APyFixed(a_bits, int_bits=int_bits, frac_bits=0)
            min = APyFixed(b_bits, int_bits=int_bits, frac_bits=0)
            assert arr.max().is_identical(max)
            assert arr.min().is_identical(min)
