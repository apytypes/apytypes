import pytest
from apytypes import (
    APyFloat,
    APyFloatArray,
    APyFloatQuantizationContext,
    QuantizationMode,
)


@pytest.mark.float_array
def test_array_raises():
    a = APyFloatArray([1, 2, 3], [5, 2, 3], [4, 2, 3], 10, 10)
    b = APyFloatArray([1, 2], [5, 3], [4, 4], 10, 10)
    with pytest.raises(ValueError, match="APyFloatArray.__add__: shape mismatch"):
        _ = a + b
    with pytest.raises(ValueError, match="APyFloatArray.__sub__: shape mismatch"):
        _ = a - b
    with pytest.raises(ValueError, match="APyFloatArray.__mul__: shape mismatch"):
        _ = a * b
    with pytest.raises(ValueError, match="APyFloatArray.__truediv__: shape mismatch"):
        _ = a / b


@pytest.mark.float_array
def test_array_add():
    a = APyFloatArray.from_float([0, 0.125, 2.5, 12], 5, 7)
    b = APyFloatArray.from_float([3, -0.75, -5, 8], 6, 5)
    ans = APyFloatArray.from_float([3, -0.625, -2.5, 20], 6, 7)
    assert (a + b).is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    b = APyFloatArray.from_float(
        [
            [6, 2.5, 7.5],
            [-4.0, 20, 6.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    ans = APyFloatArray.from_float(
        [
            [7.0, 4.5, 10.5],
            [-8.0, 15.0, 0.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    assert (a + b).is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray.from_float(
        [
            [0.0, float("nan"), float("inf"), 25.0, 255.0],
            [-float("inf"), 2.0**127, 2.0**-130, 2.0**127, float("inf")],
        ],
        exp_bits=8,
        man_bits=8,
    )
    b = APyFloatArray.from_float(
        [
            [6, 2.5, 7.5, -24.0, -3],
            [-4.0, 2.0**127 - 2.0**118, 2.0**-132, 1, -float("inf")],
        ],
        exp_bits=8,
        man_bits=8,
    )
    ans = APyFloatArray.from_float(
        [
            [6.0, float("nan"), float("inf"), 1.0, 252],
            [
                -float("inf"),
                float("inf"),
                2.0**-130 + 2.0**-132,
                2.0**127,
                float("nan"),
            ],
        ],
        exp_bits=8,
        man_bits=8,
    )
    assert (a + b).is_identical(ans)
    # Different sign of final nan
    ans = APyFloatArray.from_float(
        [
            [6.0, float("nan"), float("inf"), 1.0, 252],
            [
                -float("inf"),
                float("inf"),
                2.0**-130 + 2.0**-132,
                2.0**127,
                -float("nan"),
            ],
        ],
        exp_bits=8,
        man_bits=8,
    )
    assert (b + a).is_identical(ans)

    a = APyFloatArray([1], [2], [1], 4, 4)
    b = APyFloatArray([0], [0], [15], 4, 4)
    ans = APyFloatArray([1], [1], [3], 4, 4)
    res = a + b
    assert res.is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray([0, 0], [0, 14], [3, 15], 4, 4)
    b = APyFloatArray([0, 0], [0, 14], [5, 2], 4, 4)
    ans = APyFloatArray([0, 0], [0, 15], [8, 0], 4, 4)
    res = a + b
    assert res.is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray([1], [2], [1], 4, 4)
    b = APyFloatArray([0], [1], [15], 4, 4)
    ans = APyFloatArray([1], [0], [3], 4, 4)
    res = a + b
    assert res.is_identical(ans)
    assert (b + a).is_identical(ans)


@pytest.mark.float_array
def test_array_add_scalar():
    a = APyFloatArray.from_float([12, 23, 34], 8, 10)
    b = APyFloat.from_float(8, 9, 8)
    assert (a + b).is_identical(APyFloatArray.from_float([20, 31, 42], 9, 10))
    assert (b + a).is_identical(APyFloatArray.from_float([20, 31, 42], 9, 10))

    z810 = APyFloat.from_float(0, 8, 10)
    z98 = APyFloat.from_float(0, 9, 8)
    assert (a + z810).is_identical(a)
    assert (a + z98).is_identical(APyFloatArray.from_float([12, 23, 34], 9, 10))
    assert (z810 + a).is_identical(a)
    assert (z98 + a).is_identical(APyFloatArray.from_float([12, 23, 34], 9, 10))

    a = APyFloatArray.from_float([12, -23, 34], 8, 8)
    b = APyFloat.from_float(8, 8, 8)
    ans = APyFloatArray.from_float([20, -15, 42], 8, 8)
    assert (a + b).is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray.from_float([12, -23, 34], 8, 60)
    b = APyFloat.from_float(0, 8, 60)
    ans = APyFloatArray.from_float([12, -23, 34], 8, 60)
    assert (a + b).is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray.from_float(
        [2.0**127, 1 + 2**-8, 2, 0, float("inf"), float("nan"), 1, 2**-7, 1 - 2**-7],
        8,
        8,
    )
    b = APyFloat.from_float(-1, 8, 8)
    ans = APyFloatArray.from_float(
        [2.0**127, 2**-8, 1, -1, float("inf"), float("nan"), 0, 2**-7 - 1, -(2**-7)],
        8,
        8,
    )
    assert (a + b).is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray.from_float([12, -23, 34], 8, 8)
    b = APyFloat.from_float(float("nan"), 8, 8)
    ans = APyFloatArray.from_float([float("nan"), -float("nan"), float("nan")], 8, 8)
    assert (a + b).is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray.from_float(
        [-float("inf"), float("nan"), float("inf"), 0, 5], 8, 8
    )
    b = APyFloat.from_float(float("inf"), 8, 8)
    ans = APyFloatArray.from_float(
        [-float("nan"), float("nan"), float("inf"), float("inf"), float("inf")], 8, 8
    )
    assert (a + b).is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloat(1, 2, 1, 4, 4)
    b = APyFloatArray([0], [0], [15], 4, 4)
    ans = APyFloatArray([1], [1], [3], 4, 4)
    res = a + b
    assert res.is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray([0], [0], [3], 4, 4)
    b = APyFloat(0, 0, 5, 4, 4)
    ans = APyFloatArray([0], [0], [8], 4, 4)
    res = a + b
    assert res.is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray([0], [14], [15], 4, 4)
    b = APyFloat(0, 14, 2, 4, 4)
    ans = APyFloatArray([0], [15], [0], 4, 4)
    res = a + b
    assert res.is_identical(ans)
    assert (b + a).is_identical(ans)

    a = APyFloatArray([1], [2], [1], 4, 4)
    b = APyFloat(0, 1, 15, 4, 4)
    ans = APyFloatArray([1], [0], [3], 4, 4)
    res = a + b
    assert res.is_identical(ans)
    assert (b + a).is_identical(ans)


@pytest.mark.float_array
def test_array_sub():
    a = APyFloatArray.from_float([0, 0.125, 2.5, 12], 5, 7)
    b = APyFloatArray.from_float([3, -0.75, -5, 8], 6, 5)
    ans = APyFloatArray.from_float([-3, 0.875, 7.5, 4], 6, 7)
    assert (a - b).is_identical(ans)

    a = APyFloatArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    b = APyFloatArray.from_float(
        [
            [6, 2.5, 7.5],
            [-4.0, 20, 6.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    ans = APyFloatArray.from_float(
        [
            [-5, -0.5, -4.5],
            [0.0, -25.0, -12.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    assert (a - b).is_identical(ans)


@pytest.mark.float_array
def test_array_sub_scalar():
    a = APyFloatArray.from_float([12, 23, 34], 8, 10)
    b = APyFloat.from_float(8, 9, 8)
    assert (a - b).is_identical(APyFloatArray.from_float([4, 15, 26], 9, 10))
    assert (b - a).is_identical(APyFloatArray.from_float([-4, -15, -26], 9, 10))

    z810 = APyFloat.from_float(0, 8, 10)
    z98 = APyFloat.from_float(0, 9, 8)
    assert (a - z810).is_identical(a)
    assert (a - z98).is_identical(APyFloatArray.from_float([12, 23, 34], 9, 10))


@pytest.mark.float_array
def test_array_mul():
    a = APyFloatArray.from_float([0, 0.125, 2.5, 12], 5, 7)
    b = APyFloatArray.from_float([3, -0.75, -5, 8], 6, 5)
    ans = APyFloatArray.from_float([0, -0.09375, -12.5, 96], 6, 7)
    res = a * b
    assert res.is_identical(ans)
    assert (a * b).is_identical(ans)

    a = APyFloatArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    b = APyFloatArray.from_float(
        [
            [6, 2.5, 7.5],
            [-4.0, 20, 6.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    ans = APyFloatArray.from_float(
        [
            [6, 5, 22.5],
            [16, -100, -36],
        ],
        exp_bits=8,
        man_bits=8,
    )
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=12,
        man_bits=40,
    )
    b = APyFloatArray.from_float(
        [
            [6, 2.5, 7.5],
            [-4.0, 20, 6.0],
        ],
        exp_bits=12,
        man_bits=40,
    )
    ans = APyFloatArray.from_float(
        [
            [6, 5, 22.5],
            [16, -100, -36],
        ],
        exp_bits=12,
        man_bits=40,
    )
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray.from_float(
        [
            [float("nan"), float("inf"), 0.0],
            [0.0, -float("inf"), float("nan")],
        ],
        exp_bits=8,
        man_bits=8,
    )
    b = APyFloatArray.from_float(
        [
            [6, 0.0, float("inf")],
            [-4.0, 2.0, float("inf")],
        ],
        exp_bits=8,
        man_bits=8,
    )
    ans = APyFloatArray.from_float(
        [
            [float("nan"), float("nan"), float("nan")],
            [-0.0, -float("inf"), float("nan")],
        ],
        exp_bits=8,
        man_bits=8,
    )
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray.from_float(
        [
            [float("nan"), float("inf"), 0.0],
            [0.0, -float("inf"), float("nan")],
        ],
        exp_bits=12,
        man_bits=35,
    )
    b = APyFloatArray.from_float(
        [
            [6, 0.0, float("inf")],
            [-4.0, 2.0, float("inf")],
        ],
        exp_bits=12,
        man_bits=35,
    )
    ans = APyFloatArray.from_float(
        [
            [float("nan"), float("nan"), float("nan")],
            [-0.0, -float("inf"), float("nan")],
        ],
        exp_bits=12,
        man_bits=35,
    )
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray([False, True, True], [60, 127, 0], [10, 192, 10], 8, 20)
    b = APyFloatArray([True, False, True], [60, 127, 0], [192, 192, 15], 8, 20)
    ans = APyFloatArray([1, 1, 0], [0, 127, 0], [4097, 384, 0], exp_bits=8, man_bits=20)
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray([0, 1], [6, 7], [7, 14], 4, 5)
    b = APyFloatArray([0, 1], [1, 0], [7, 14], 4, 5)
    ans = APyFloatArray([0, 0], [0, 0], [24, 20], exp_bits=4, man_bits=5, bias=7)
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray([0, 1], [7, 12], [30, 14], 4, 5)
    b = APyFloatArray([0, 1], [7, 12], [1, 14], 4, 5)
    ans = APyFloatArray([0, 0], [8, 15], [0, 0], exp_bits=4, man_bits=5, bias=7)
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    # Subnormals
    x = APyFloatArray([0], [0], [11], exp_bits=4, man_bits=9)
    y = APyFloatArray([0], [5], [50], exp_bits=5, man_bits=10)
    res = x * y
    assert res.is_identical(APyFloatArray([0], [0], [6], exp_bits=5, man_bits=10))

    x = APyFloatArray([0], [1], [10], exp_bits=4, man_bits=10)
    y = APyFloatArray([0], [1], [20], exp_bits=4, man_bits=10)
    res = x * y
    assert res.is_identical(APyFloatArray([0], [0], [16], exp_bits=4, man_bits=10))

    x = APyFloatArray([0], [1], [10], exp_bits=5, man_bits=15)
    y = APyFloatArray([0], [1], [20], exp_bits=5, man_bits=15)
    res = x * y
    assert res.is_identical(APyFloatArray([0], [0], [2], exp_bits=5, man_bits=15))

    x = APyFloatArray([0], [3], [10], 4, 10)
    y = APyFloatArray([0], [4], [10], 4, 10)
    res = x * y
    assert res.is_identical(APyFloatArray([0], [0], [522], exp_bits=4, man_bits=10))

    # 1.0 x subn
    res = APyFloatArray([0], [7], [0], 4, 3) * APyFloatArray([0], [0], [1], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [1], 4, 3))

    res = APyFloatArray([0], [7], [0], 4, 3) * APyFloatArray([0], [0], [2], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [2], 4, 3))

    # 2.0 x subn
    res = APyFloatArray([0], [8], [0], 4, 3) * APyFloatArray([0], [0], [1], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [2], 4, 3))

    res = APyFloatArray([0], [8], [0], 4, 3) * APyFloatArray([0], [0], [2], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [4], 4, 3))

    # 4.0 x subn
    res = APyFloatArray([0], [9], [0], 4, 3) * APyFloatArray([0], [0], [1], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [4], 4, 3))

    # subnormal becoming normal
    res = APyFloatArray([0], [10], [0], 4, 3) * APyFloatArray([0], [0], [1], 4, 3)
    assert res.is_identical(APyFloatArray([0], [1], [0], 4, 3))

    # 0.5 x subn
    res = APyFloatArray([0], [6], [0], 4, 3) * APyFloatArray([0], [0], [4], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [2], 4, 3))

    # 0.25 x subn
    res = APyFloatArray([0], [5], [0], 4, 3) * APyFloatArray([0], [0], [4], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [1], 4, 3))

    # 0.125 x subn
    res = APyFloatArray([0], [4], [0], 4, 3) * APyFloatArray([0], [0], [4], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [0], 4, 3))

    # Two normal numbers that generate carry, but with subnormal result
    a = APyFloat(sign=1, exp=2, man=957, exp_bits=5, man_bits=10)
    b = APyFloatArray([0], [12], [1015], 5, 10)
    res = a * b
    assert res.is_identical(APyFloatArray([1], [0], [986], 5, 10))

    # Subnormal becoming normal after quantization
    x = APyFloatArray(
        [0], [0], [4503599627370495], exp_bits=11, man_bits=52
    )  # (2.22507e-308)
    y = APyFloatArray(
        [0], [1023], [1], exp_bits=11, man_bits=52
    )  # Slightly larger than 1
    res = x * y
    assert res.is_identical(APyFloatArray([0], [1], [0], exp_bits=11, man_bits=52))


@pytest.mark.float_array
def test_array_mul_scalar():
    a = APyFloatArray.from_float([0.5, 1.5, 5], 8, 10)
    b = APyFloat.from_float(8, 9, 8)
    assert (a * b).is_identical(APyFloatArray.from_float([4, 12, 40], 9, 10))
    assert (b * a).is_identical(APyFloatArray.from_float([4, 12, 40], 9, 10))

    a = APyFloatArray.from_float([0, 1.75, -7.75], 8, 10)
    b = APyFloat.from_float(7.5, 9, 8)
    assert (a * b).is_identical(
        APyFloatArray([0, 0, 1], [0, 258, 260], [0, 656, 836], exp_bits=9, man_bits=10)
    )
    assert (b * a).is_identical(
        APyFloatArray([0, 0, 1], [0, 258, 260], [0, 656, 836], exp_bits=9, man_bits=10)
    )

    a = APyFloatArray([0, 1], [0, 0], [5, 14], 4, 5)
    b = APyFloat(0, 0, 11, 4, 5)
    res = a * b
    assert res.is_identical(APyFloatArray([0, 1], [0, 0], [0, 0], 4, 5))
    assert (b * a).is_identical(res)

    a = APyFloatArray.from_float([0.5, 1.5, 5], 10, 35)
    b = APyFloat.from_float(8, 10, 35)
    res = a * b
    assert res.is_identical(APyFloatArray.from_float([4, 12, 40], 10, 35))
    assert (b * a).is_identical(APyFloatArray.from_float([4, 12, 40], 10, 35))

    a = APyFloatArray.from_float([0.5, 1.5, 5], 8, 10)
    b = APyFloat.from_float(0, 9, 8)
    res = a * b
    assert res.is_identical(APyFloatArray.from_float([0, 0, 0], 9, 10))
    assert (b * a).is_identical(APyFloatArray.from_float([0, 0, 0], 9, 10))

    a = APyFloatArray.from_float([0.5, 0, -5], 8, 10)
    b = APyFloat.from_float(float("inf"), 9, 8)
    res = a * b
    assert res.is_identical(
        APyFloatArray.from_float([float("inf"), float("nan"), -float("inf")], 9, 10)
    )
    assert (b * a).is_identical(
        APyFloatArray.from_float([float("inf"), float("nan"), -float("inf")], 9, 10)
    )

    a = APyFloatArray.from_float([0.5, 0, -5], 8, 10)
    b = APyFloat.from_float(float("nan"), 9, 8)
    res = a * b
    assert res.is_identical(
        APyFloatArray.from_float([float("nan"), float("nan"), -float("nan")], 9, 10)
    )
    assert (b * a).is_identical(
        APyFloatArray.from_float([float("nan"), float("nan"), -float("nan")], 9, 10)
    )

    a = APyFloatArray.from_float([float("inf"), float("nan"), -float("inf")], 8, 10)
    b = APyFloat.from_float(0, 9, 8)
    res = a * b
    assert res.is_identical(
        APyFloatArray.from_float([float("nan"), float("nan"), -float("nan")], 9, 10)
    )
    assert (b * a).is_identical(
        APyFloatArray.from_float([float("nan"), float("nan"), -float("nan")], 9, 10)
    )

    a = APyFloatArray.from_float([float("inf"), float("nan"), -float("inf")], 8, 10)
    b = APyFloat(0, 3, 27, 9, 8)
    res = a * b
    assert res.is_identical(
        APyFloatArray.from_float([float("inf"), float("nan"), -float("inf")], 9, 10)
    )
    assert (b * a).is_identical(
        APyFloatArray.from_float([float("inf"), float("nan"), -float("inf")], 9, 10)
    )

    a = APyFloatArray([0, 1], [7, 6], [0, 31], 4, 5)
    b = APyFloat(0, 0, 5, 4, 5)
    ans = APyFloatArray([0, 1], [0, 0], [5, 5], 4, 5)
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray([0, 1], [4, 0], [0, 31], 4, 5)
    b = APyFloat(0, 0, 5, 4, 5)
    ans = APyFloatArray([0, 1], [0, 0], [1, 0], 4, 5)
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray([0, 1], [6, 7], [31, 0], 4, 5)
    b = APyFloat(0, 7, 31, 4, 5)
    ans = APyFloatArray([0, 1], [7, 7], [30, 31], 4, 5)
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray([0, 1], [7, 12], [30, 14], 4, 5)
    b = APyFloat(0, 7, 1, 4, 5)
    ans = APyFloatArray([0, 1], [8, 12], [0, 15], exp_bits=4, man_bits=5, bias=7)
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray([0, 1], [7, 12], [30, 14], 4, 5)
    b = APyFloat(1, 12, 14, 4, 5)
    ans = APyFloatArray([1, 0], [13, 15], [13, 0], exp_bits=4, man_bits=5, bias=7)
    res = a * b
    assert res.is_identical(ans)
    assert (b * a).is_identical(ans)

    # 1.0 x subn
    res = APyFloat(0, 7, 0, 4, 3) * APyFloatArray([0], [0], [1], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [1], 4, 3))

    res = APyFloat(0, 7, 0, 4, 3) * APyFloatArray([0], [0], [2], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [2], 4, 3))

    # 2.0 x subn
    res = APyFloat(0, 8, 0, 4, 3) * APyFloatArray([0], [0], [1], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [2], 4, 3))

    res = APyFloat(0, 8, 0, 4, 3) * APyFloatArray([0], [0], [2], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [4], 4, 3))

    # 4.0 x subn
    res = APyFloat(0, 9, 0, 4, 3) * APyFloatArray([0], [0], [1], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [4], 4, 3))

    # subnormal becoming normal
    res = APyFloat(0, 10, 0, 4, 3) * APyFloatArray([0], [0], [1], 4, 3)
    assert res.is_identical(APyFloatArray([0], [1], [0], 4, 3))

    # 0.5 x subn
    res = APyFloat(0, 6, 0, 4, 3) * APyFloatArray([0], [0], [4], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [2], 4, 3))

    # 0.25 x subn
    res = APyFloat(0, 5, 0, 4, 3) * APyFloatArray([0], [0], [4], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [1], 4, 3))

    # 0.125 x subn
    res = APyFloat(0, 4, 0, 4, 3) * APyFloatArray([0], [0], [4], 4, 3)
    assert res.is_identical(APyFloatArray([0], [0], [0], 4, 3))

    # Two normal numbers that generate carry, but with subnormal result
    a = APyFloatArray([1], [2], [957], 5, 10)
    b = APyFloatArray([0], [12], [1015], 5, 10)
    res = a * b
    assert res.is_identical(APyFloatArray([1], [0], [986], 5, 10))

    # Subnormal becoming normal after quantization
    x = APyFloatArray(
        [0], [0], [4503599627370495], exp_bits=11, man_bits=52
    )  # (2.22507e-308)
    y = APyFloat(0, 1023, 1, exp_bits=11, man_bits=52)  # Slightly larger than 1
    res = x * y
    assert res.is_identical(APyFloatArray([0], [1], [0], exp_bits=11, man_bits=52))


@pytest.mark.float_array
def test_array_div():
    a = APyFloatArray.from_float([3, -0.09375, -12.5, 96], 6, 6)
    b = APyFloatArray.from_float([1, 0.125, 2.5, 12], 5, 7)
    ans = APyFloatArray.from_float([3, -0.75, -5, 8], 6, 7)
    assert (a / b).is_identical(ans)

    a = APyFloatArray.from_float(
        [
            [6, 5, 22.5],
            [16, -100, -36],
        ],
        exp_bits=8,
        man_bits=8,
    )
    b = APyFloatArray.from_float(
        [
            [6, 2.5, 7.5],
            [-4.0, 20, 6.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    ans = APyFloatArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    assert (a / b).is_identical(ans)

    res = APyFloatArray([0], [1], [1023], exp_bits=5, man_bits=10) / APyFloatArray(
        [0], [16], [0], exp_bits=5, man_bits=10
    )
    assert res.is_identical(APyFloatArray([0], [1], [0], exp_bits=5, man_bits=10))

    # -2 / -8.98847e+307
    res = APyFloatArray(
        [1], [1023], [4503599627370495], exp_bits=11, man_bits=52
    ) / APyFloatArray([1], [2046], [0], exp_bits=11, man_bits=52)
    assert res.is_identical(APyFloatArray([0], [1], [0], exp_bits=11, man_bits=52))


@pytest.mark.float_array
def test_array_div_scalar():
    a = APyFloatArray.from_float([4, 12, 40], 9, 10)
    b = APyFloat.from_float(8, 9, 8)
    assert (a / b).is_identical(APyFloatArray.from_float([0.5, 1.5, 5], 9, 10))

    res = APyFloatArray([0], [1], [1023], exp_bits=5, man_bits=10) / APyFloat(
        0, 16, 0, exp_bits=5, man_bits=10
    )
    assert res.is_identical(APyFloatArray([0], [1], [0], exp_bits=5, man_bits=10))

    # -2 / -8.98847e+307
    res = APyFloatArray(
        [1], [1023], [4503599627370495], exp_bits=11, man_bits=52
    ) / APyFloat(1, 2046, 0, exp_bits=11, man_bits=52)
    assert res.is_identical(APyFloatArray([0], [1], [0], exp_bits=11, man_bits=52))


@pytest.mark.float_array
def test_array_rdiv_scalar():
    a = APyFloatArray.from_float([4, 5, 32], 9, 10)
    b = APyFloat.from_float(8, 5, 8)
    assert (b / a).is_identical(APyFloatArray.from_float([2, 1.6, 0.25], 9, 10))


@pytest.mark.float_array
def test_unary_negation():
    a = APyFloatArray.from_float([-4.25, 12, -5.5], 9, 10)
    assert (-(-a)).is_identical(a)
    b = APyFloatArray.from_float([4.25, -12, 5.5], 9, 10)
    assert (-a).is_identical(b)


@pytest.mark.float_array
def test_abs():
    a = APyFloatArray.from_float([-4.25, 12, -5.5], 9, 10)
    b = APyFloatArray.from_float([4.25, 12, 5.5], 9, 10)
    assert abs(a).is_identical(b)


@pytest.mark.float_mul
def test_array_mul_with_one():
    """Test multiplication with one."""
    a = APyFloatArray([0], [7], [0], 4, 4)  # One
    for exp in reversed(range(15)):  # Skip nan/inf
        for man in reversed(range(16)):
            b = APyFloatArray([0], [exp], [man], 4, 4)
            res = a * b
            assert res.is_identical(b)
            assert (b * a).is_identical(b)


@pytest.mark.float_mul
def test_scalar_array_mul_with_one():
    """Test multiplication with one."""
    a = APyFloat(0, 7, 0, 4, 4)  # Scalar one
    for exp in reversed(range(15)):  # Skip nan/inf
        for man in reversed(range(16)):
            b = APyFloatArray([0], [exp], [man], 4, 4)
            res = a * b
            assert res.is_identical(b)
            assert (b * a).is_identical(b)


def test_array_infinity_saturation_cast():
    # Big positive number should become infinity
    res = APyFloatArray([0], [30], [2], 5, 2).cast(4, 2)
    assert res.is_identical(APyFloatArray([0], [15], [0], 4, 2))

    # Big negative number should become infinity
    res = APyFloatArray([1], [30], [2], 5, 2).cast(4, 2)
    assert res.is_identical(APyFloatArray([1], [15], [0], 4, 2))

    with APyFloatQuantizationContext(QuantizationMode.TO_ZERO):
        # Big positive number should saturate
        res = APyFloatArray([0], [30], [2], 5, 2).cast(4, 2)
        assert res.is_identical(APyFloatArray([0], [14], [3], 4, 2))

        # Big negative number should saturate
        res = APyFloatArray([1], [30], [2], 5, 2).cast(4, 2)
        assert res.is_identical(APyFloatArray([1], [14], [3], 4, 2))


@pytest.mark.float_mul
def test_array_infinity_saturation_mul():
    # Array x Scalar
    # Big positive number should become infinity
    res = APyFloatArray([0], [30], [2], 5, 2) * APyFloat(0, 30, 3, 5, 2)
    assert res.is_identical(APyFloatArray([0], [31], [0], 5, 2))

    # Big negative number should become infinity
    res = APyFloatArray([1], [30], [2], 5, 2) * APyFloat(0, 30, 3, 5, 2)
    assert res.is_identical(APyFloatArray([1], [31], [0], 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.TO_ZERO):
        # Big positive number should saturate
        res = APyFloatArray([0], [30], [2], 5, 2) * APyFloat(0, 30, 3, 5, 2)
        assert res.is_identical(APyFloatArray([0], [30], [3], 5, 2))

        # Big negative number should saturate
        res = APyFloatArray([1], [30], [2], 5, 2) * APyFloat(0, 30, 3, 5, 2)
        assert res.is_identical(APyFloatArray([1], [30], [3], 5, 2))

    # Array x Array
    # Big positive number should become infinity
    res = APyFloatArray([0], [30], [2], 5, 2) * APyFloatArray([0], [30], [2], 5, 2)
    assert res.is_identical(APyFloatArray([0], [31], [0], 5, 2))

    # Big negative number should become infinity
    res = APyFloatArray([1], [30], [2], 5, 2) * APyFloatArray([0], [30], [2], 5, 2)
    assert res.is_identical(APyFloatArray([1], [31], [0], 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.TO_ZERO):
        # Big positive number should saturate
        res = APyFloatArray([0], [30], [2], 5, 2) * APyFloatArray([0], [30], [2], 5, 2)
        assert res.is_identical(APyFloatArray([0], [30], [3], 5, 2))

        # Big negative number should saturate
        res = APyFloatArray([1], [30], [2], 5, 2) * APyFloatArray([0], [30], [2], 5, 2)
        assert res.is_identical(APyFloatArray([1], [30], [3], 5, 2))


@pytest.mark.float_mul
@pytest.mark.parametrize("with_scalar", [False, True])
def test_array_mul_mixed_bias(with_scalar):
    # Test that the implementation doesn't do "x.cast() * y.cast()"
    x = APyFloatArray([0], [30], [0], exp_bits=5, man_bits=2, bias=14)

    if with_scalar:
        y = APyFloat(sign=0, exp=15, man=0, exp_bits=5, man_bits=2, bias=16)
    else:
        y = APyFloatArray([0], [15], [0], exp_bits=5, man_bits=2, bias=16)
    assert (_ := x * y).is_identical(
        APyFloatArray([0], [30], [0], exp_bits=5, man_bits=2, bias=15)
    )

    # Multiply two one's but with different formats
    x = APyFloatArray([0], [14], [0], exp_bits=5, man_bits=2, bias=14)

    if with_scalar:
        y = APyFloat(sign=0, exp=21, man=0, exp_bits=5, man_bits=2, bias=21)
    else:
        y = APyFloatArray([0], [21], [0], exp_bits=5, man_bits=2, bias=21)
    assert (_ := x * y).is_identical(
        APyFloatArray([0], [17], [0], exp_bits=5, man_bits=2, bias=17)
    )


@pytest.mark.float_mul
@pytest.mark.parametrize("with_scalar", [False, True])
def test_array_mul_mixed_bias_overflow(with_scalar):
    """Test that a result can overflow to infinity due to a change in bias."""
    x = APyFloatArray([0], [21], [0], exp_bits=5, man_bits=2, bias=5)

    if with_scalar:
        y = APyFloat(sign=0, exp=25, man=0, exp_bits=5, man_bits=2, bias=25)
    else:
        y = APyFloatArray([0], [25], [0], exp_bits=5, man_bits=2, bias=25)

    # Multiply with one
    assert (_ := x * y).is_identical(
        APyFloatArray([0], [31], [0], exp_bits=5, man_bits=2, bias=15)
    )

    # Multiply with one but with larger bias difference
    if with_scalar:
        y = APyFloat(sign=0, exp=27, man=0, exp_bits=5, man_bits=2, bias=27)
    else:
        y = APyFloatArray([0], [27], [0], exp_bits=5, man_bits=2, bias=27)
    assert (_ := x * y).is_identical(
        APyFloatArray([0], [31], [0], exp_bits=5, man_bits=2, bias=16)
    )

    # Should become one
    x = APyFloatArray([0], [30], [0], exp_bits=5, man_bits=2, bias=3)

    if with_scalar:
        y = APyFloat(sign=0, exp=1, man=0, exp_bits=5, man_bits=2, bias=28)
    else:
        y = APyFloatArray([0], [1], [0], exp_bits=5, man_bits=2, bias=28)
    assert (_ := x * y).is_identical(
        APyFloatArray([0], [15], [0], exp_bits=5, man_bits=2)
    )


@pytest.mark.float_add
def test_add_mixed_bias_underflow():
    """Test that a result can become zero due to a change in bias."""
    x = APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=5)
    y = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=25)

    # Add with zero
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=15)
    )

    # Add with zero but with larger bias difference
    y = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=27)
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=16)
    )

    # Add with subnormal number
    y = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=28)
    assert (_ := x + y).is_identical(
        APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=16)
    )


@pytest.mark.float_add
@pytest.mark.float_sub
@pytest.mark.float_array
@pytest.mark.parametrize("man", [10, 60])
@pytest.mark.parametrize("with_scalar", [False, True])
def test_array_add_sub_zero_sign(man, with_scalar):
    pos_zero = APyFloatArray([0], [0], [0], 5, 10)
    neg_zero = APyFloatArray([1], [0], [0], 5, man)
    non_zero = APyFloatArray.from_float([1.0], 5, man)

    # Scalar equivalents
    pos_zero_scal = APyFloat(0, 0, 0, 5, 10)
    neg_zero_scal = APyFloat(1, 0, 0, 5, man)
    non_zero_scal = APyFloat.from_float(1.0, 5, man)

    modes = (
        QuantizationMode.TIES_EVEN,
        QuantizationMode.TO_ZERO,
        QuantizationMode.TO_POS,
    )

    for mode in modes:
        with APyFloatQuantizationContext(mode):
            rhs = pos_zero_scal if with_scalar else pos_zero
            assert (_ := pos_zero + rhs)[0].sign == False
            rhs = neg_zero_scal if with_scalar else neg_zero
            assert (_ := pos_zero + rhs)[0].sign == False
            rhs = pos_zero_scal if with_scalar else pos_zero
            assert (_ := neg_zero + rhs)[0].sign == False
            rhs = neg_zero_scal if with_scalar else neg_zero
            assert (_ := neg_zero + rhs)[0].sign == True

            rhs = pos_zero_scal if with_scalar else pos_zero
            assert (_ := pos_zero - rhs)[0].sign == False
            rhs = neg_zero_scal if with_scalar else neg_zero
            assert (_ := pos_zero - rhs)[0].sign == False
            rhs = pos_zero_scal if with_scalar else pos_zero
            assert (_ := neg_zero - rhs)[0].sign == True
            rhs = neg_zero_scal if with_scalar else neg_zero
            assert (_ := neg_zero - rhs)[0].sign == False

            rhs = non_zero_scal if with_scalar else non_zero
            assert (_ := non_zero + (-rhs))[0].sign == False

    with APyFloatQuantizationContext(QuantizationMode.TO_NEG):
        rhs = pos_zero_scal if with_scalar else pos_zero
        assert (_ := pos_zero + rhs)[0].sign == False
        rhs = neg_zero_scal if with_scalar else neg_zero
        assert (_ := pos_zero + rhs)[0].sign == True
        rhs = pos_zero_scal if with_scalar else pos_zero
        assert (_ := neg_zero + rhs)[0].sign == True
        rhs = neg_zero_scal if with_scalar else neg_zero
        assert (_ := neg_zero + rhs)[0].sign == True

        rhs = pos_zero_scal if with_scalar else pos_zero
        assert (_ := pos_zero - pos_zero)[0].sign == True
        rhs = neg_zero_scal if with_scalar else neg_zero
        assert (_ := pos_zero - rhs)[0].sign == False
        rhs = pos_zero_scal if with_scalar else pos_zero
        assert (_ := neg_zero - rhs)[0].sign == True
        rhs = neg_zero_scal if with_scalar else neg_zero
        assert (_ := neg_zero - rhs)[0].sign == True

        rhs = non_zero_scal if with_scalar else non_zero
        assert (_ := non_zero - rhs)[0].sign == True


@pytest.mark.float_array
@pytest.mark.float_add
def test_array_infinity_saturation_add():
    # Array x Scalar
    # Big positive number should become infinity
    res = APyFloatArray([0], [30], [2], 5, 2) + APyFloat(0, 30, 3, 5, 2)
    assert res.is_identical(APyFloatArray([0], [31], [0], 5, 2))

    # Big negative number should become infinity
    res = APyFloatArray([1], [30], [2], 5, 2) + APyFloat(1, 30, 3, 5, 2)
    assert res.is_identical(APyFloatArray([1], [31], [0], 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.TO_ZERO):
        # Big positive number should saturate
        res = APyFloatArray([0], [30], [2], 5, 2) + APyFloat(0, 30, 3, 5, 2)
        assert res.is_identical(APyFloatArray([0], [30], [3], 5, 2))

        # Big negative number should saturate
        res = APyFloatArray([1], [30], [2], 5, 2) + APyFloat(1, 30, 3, 5, 2)
        assert res.is_identical(APyFloatArray([1], [30], [3], 5, 2))

    # Array x Array
    # Big positive number should become infinity
    res = APyFloatArray([0], [30], [2], 5, 2) + APyFloatArray([0], [30], [2], 5, 2)
    assert res.is_identical(APyFloatArray([0], [31], [0], 5, 2))

    # Big negative number should become infinity
    res = APyFloatArray([1], [30], [2], 5, 2) + APyFloatArray([1], [30], [2], 5, 2)
    assert res.is_identical(APyFloatArray([1], [31], [0], 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.TO_ZERO):
        # Big positive number should saturate
        res = APyFloatArray([0], [30], [2], 5, 2) + APyFloatArray([0], [30], [2], 5, 2)
        assert res.is_identical(APyFloatArray([0], [30], [3], 5, 2))

        # Big negative number should saturate
        res = APyFloatArray([1], [30], [2], 5, 2) + APyFloatArray([1], [30], [2], 5, 2)
        assert res.is_identical(APyFloatArray([1], [30], [3], 5, 2))


@pytest.mark.float_array
@pytest.mark.float_div
def test_array_infinity_saturation_div():
    # Array x Scalar
    # Big positive number should become infinity
    res = APyFloatArray([0], [30], [2], 5, 2) / APyFloat(0, 1, 3, 5, 2)
    assert res.is_identical(APyFloatArray([0], [31], [0], 5, 2))

    # Big negative number should become infinity
    res = APyFloatArray([1], [30], [2], 5, 2) / APyFloat(0, 1, 3, 5, 2)
    assert res.is_identical(APyFloatArray([1], [31], [0], 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.TO_ZERO):
        # Big positive number should saturate
        res = APyFloatArray([0], [30], [2], 5, 2) / APyFloat(0, 1, 3, 5, 2)
        assert res.is_identical(APyFloatArray([0], [30], [3], 5, 2))

        # Big negative number should saturate
        res = APyFloatArray([1], [30], [2], 5, 2) / APyFloat(0, 1, 3, 5, 2)
        assert res.is_identical(APyFloatArray([1], [30], [3], 5, 2))

    # Array x Array
    # Big positive number should become infinity
    res = APyFloatArray([0], [30], [2], 5, 2) / APyFloatArray([0], [1], [2], 5, 2)
    assert res.is_identical(APyFloatArray([0], [31], [0], 5, 2))

    # Big negative number should become infinity
    res = APyFloatArray([1], [30], [2], 5, 2) / APyFloatArray([0], [1], [2], 5, 2)
    assert res.is_identical(APyFloatArray([1], [31], [0], 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.TO_ZERO):
        # Big positive number should saturate
        res = APyFloatArray([0], [30], [2], 5, 2) / APyFloatArray([0], [1], [2], 5, 2)
        assert res.is_identical(APyFloatArray([0], [30], [3], 5, 2))

        # Big negative number should saturate
        res = APyFloatArray([1], [30], [2], 5, 2) / APyFloatArray([0], [1], [2], 5, 2)
        assert res.is_identical(APyFloatArray([1], [30], [3], 5, 2))


@pytest.mark.float_array
@pytest.mark.float_add
@pytest.mark.parametrize("with_scalar", [False, True])
def test_array_add_mixed_bias_overflow(with_scalar):
    """Test that a result can overflow to infinity due to a change in bias."""
    x = APyFloatArray([0], [21], [0], exp_bits=5, man_bits=2, bias=5)

    if with_scalar:
        y = APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=25)
    else:
        y = APyFloatArray([0], [0], [0], exp_bits=5, man_bits=2, bias=25)

    # Add with zero
    assert (_ := x + y).is_identical(
        APyFloatArray([0], [31], [0], exp_bits=5, man_bits=2, bias=15)
    )

    # Add with zero but with larger bias difference
    if with_scalar:
        y = APyFloat(sign=0, exp=0, man=0, exp_bits=5, man_bits=2, bias=27)
    else:
        y = APyFloatArray([0], [0], [0], exp_bits=5, man_bits=2, bias=27)
    assert (_ := x + y).is_identical(
        APyFloatArray([0], [31], [0], exp_bits=5, man_bits=2, bias=16)
    )

    # Add with small normal number
    if with_scalar:
        y = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=28)
    else:
        y = APyFloatArray([0], [1], [1], exp_bits=5, man_bits=2, bias=28)
    assert (_ := x + y).is_identical(
        APyFloatArray([0], [31], [0], exp_bits=5, man_bits=2, bias=16)
    )

    # Add with subnormal number
    if with_scalar:
        y = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=28)
    else:
        y = APyFloatArray([0], [0], [1], exp_bits=5, man_bits=2, bias=28)
    assert (_ := x + y).is_identical(
        APyFloatArray([0], [31], [0], exp_bits=5, man_bits=2, bias=16)
    )


@pytest.mark.float_array
@pytest.mark.float_add
@pytest.mark.parametrize("with_scalar", [False, True])
def test_array_add_mixed_bias_underflow(with_scalar):
    """Test that a result can become zero due to a change in bias."""
    x = APyFloatArray([0], [0], [0], exp_bits=5, man_bits=2, bias=5)

    if with_scalar:
        y = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=25)
    else:
        y = APyFloatArray([0], [1], [1], exp_bits=5, man_bits=2, bias=25)

    # Add with zero
    assert (_ := x + y).is_identical(
        APyFloatArray([0], [0], [0], exp_bits=5, man_bits=2, bias=15)
    )

    # Add with zero but with larger bias difference
    if with_scalar:
        y = APyFloat(sign=0, exp=1, man=1, exp_bits=5, man_bits=2, bias=27)
    else:
        y = APyFloatArray([0], [1], [1], exp_bits=5, man_bits=2, bias=28)
    assert (_ := x + y).is_identical(
        APyFloatArray([0], [0], [0], exp_bits=5, man_bits=2, bias=16)
    )

    # Add with subnormal number
    if with_scalar:
        y = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=28)
    else:
        y = APyFloatArray([0], [0], [1], exp_bits=5, man_bits=2, bias=28)
    assert (_ := x + y).is_identical(
        APyFloatArray([0], [0], [0], exp_bits=5, man_bits=2, bias=16)
    )


@pytest.mark.float_array
@pytest.mark.float_sub
@pytest.mark.parametrize("with_scalar", [False, True])
def test_array_sub_mixed_bias(with_scalar):
    # Test that the implementation doesn't do "x.cast() - y.cast()"
    x = APyFloatArray([0], [30], [0], exp_bits=5, man_bits=2, bias=14)

    if with_scalar:
        y = APyFloat(sign=0, exp=30, man=0, exp_bits=5, man_bits=2, bias=16)
    else:
        y = APyFloatArray([0], [30], [0], exp_bits=5, man_bits=2, bias=16)

    assert (_ := x - y).is_identical(
        APyFloatArray([0], [30], [2], exp_bits=5, man_bits=2, bias=15)
    )

    # Test subtraction when two numbers are equal but with different formats
    x = APyFloatArray([0], [14], [0], exp_bits=5, man_bits=2, bias=14)

    if with_scalar:
        y = APyFloat(sign=0, exp=21, man=0, exp_bits=5, man_bits=2, bias=21)
    else:
        y = APyFloatArray([0], [21], [0], exp_bits=5, man_bits=2, bias=21)

    assert (_ := x - y).is_identical(
        APyFloatArray([0], [0], [0], exp_bits=5, man_bits=2, bias=17)
    )


@pytest.mark.float_array
def test_array_operation_with_numbers():
    a = APyFloatArray([0], [15], [2], 5, 2)  # 1.75

    # Integer identities
    assert (a + 0).is_identical(a)
    assert (0 + a).is_identical(a)
    assert (a - 0).is_identical(a)
    assert (0 - a).is_identical(-a)
    assert (a * 1).is_identical(a)
    assert (1 * a).is_identical(a)
    assert (a / 1).is_identical(a)

    # Operations with integers
    q_eight = 9  # Should quantize to eight
    eight = APyFloatArray([0], [18], [0], 5, 2)
    assert (_ := a + q_eight).is_identical(a + eight)
    assert (_ := q_eight + a).is_identical(eight + a)
    assert (_ := a - q_eight).is_identical(a - eight)
    assert (_ := q_eight - a).is_identical(eight - a)
    assert (_ := a * q_eight).is_identical(a * eight)
    assert (_ := q_eight * a).is_identical(eight * a)
    assert (_ := a / q_eight).is_identical(a / eight)
    assert (_ := q_eight / a).is_identical(eight / a)

    # Operations with floats
    q_one = 1.125  # Should quantize to one
    q_zero = 0.125  # Should quantize to zero
    assert (_ := a + q_zero).is_identical(a)
    assert (_ := q_zero + a).is_identical(a)
    assert (_ := a - q_zero).is_identical(a)
    assert (_ := q_zero - a).is_identical(-a)
    assert (_ := a * q_one).is_identical(a)
    assert (_ := q_one * a).is_identical(a)
    assert (_ := a / q_one).is_identical(a)
    assert (_ := q_one / a).is_identical(APyFloatArray([0], [14], [1], 5, 2))
