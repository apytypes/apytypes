import pytest
from apytypes import APyFloat, APyFloatArray


@pytest.mark.float_array
def test_array_raises():
    a = APyFloatArray([1], [5], [4], 10, 10)
    b = APyFloatArray([1, 2], [5, 3], [4, 4], 10, 10)
    with pytest.raises(ValueError, match="APyFloatArray.__add__: shape missmatch"):
        _ = a + b
    with pytest.raises(ValueError, match="APyFloatArray.__sub__: shape missmatch"):
        _ = a - b
    with pytest.raises(ValueError, match="APyFloatArray.__mul__: shape missmatch"):
        _ = a * b
    with pytest.raises(ValueError, match="APyFloatArray.__truediv__: shape missmatch"):
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
            [-8.0, 15.0, -0.0],
        ],
        exp_bits=8,
        man_bits=8,
    )
    assert (a + b).is_identical(ans)
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


@pytest.mark.float_array
def test_array_add_int_float():
    a = APyFloatArray.from_float([12, 23, 34], 8, 10)
    assert (a + 0).is_identical(a)
    assert (0 + a).is_identical(a)
    assert (a + 0.0).is_identical(a)
    assert (0.0 + a).is_identical(a)


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
            [-0.0, -25.0, -12.0],
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
def test_array_sub_int_float():
    a = APyFloatArray.from_float([12, 23, 34], 8, 10)
    assert (a - 0).is_identical(a)
    assert (a - 0.0).is_identical(a)


@pytest.mark.float_array
def test_array_mul():
    a = APyFloatArray.from_float([0, 0.125, 2.5, 12], 5, 7)
    b = APyFloatArray.from_float([3, -0.75, -5, 8], 6, 5)
    ans = APyFloatArray.from_float([0, -0.09375, -12.5, 96], 6, 7)
    assert (a * b).is_identical(ans)
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
    assert (a * b).is_identical(ans)
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
    assert (a * b).is_identical(ans)
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
    assert (a * b).is_identical(ans)
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
    assert (a * b).is_identical(ans)
    assert (b * a).is_identical(ans)

    a = APyFloatArray([False, True, True], [60, 127, 0], [10, 192, 10], 8, 20)
    b = APyFloatArray([True, False, True], [60, 127, 0], [192, 192, 15], 8, 20)
    ans = APyFloatArray([1, 1, 0], [0, 127, 0], [4097, 384, 0], exp_bits=8, man_bits=20)
    assert (a * b).is_identical(ans)
    assert (b * a).is_identical(ans)


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

    a = APyFloatArray([0, 1], [0, 0], [7, 14], 4, 5)
    b = APyFloat(0, 0, 11, 4, 5)
    assert (a * b).is_identical(
        APyFloatArray([0, 1], [0, 0], [0, 0], exp_bits=4, man_bits=5)
    )
    assert (b * a).is_identical(
        APyFloatArray([0, 1], [0, 0], [0, 0], exp_bits=4, man_bits=5)
    )

    a = APyFloatArray.from_float([0.5, 1.5, 5], 10, 35)
    b = APyFloat.from_float(8, 10, 35)
    assert (a * b).is_identical(APyFloatArray.from_float([4, 12, 40], 10, 35))
    assert (b * a).is_identical(APyFloatArray.from_float([4, 12, 40], 10, 35))

    a = APyFloatArray.from_float([0.5, 1.5, 5], 8, 10)
    b = APyFloat.from_float(0, 9, 8)
    assert (a * b).is_identical(APyFloatArray.from_float([0, 0, 0], 9, 10))
    assert (b * a).is_identical(APyFloatArray.from_float([0, 0, 0], 9, 10))

    a = APyFloatArray.from_float([0.5, 0, -5], 8, 10)
    b = APyFloat.from_float(float("inf"), 9, 8)
    assert (a * b).is_identical(
        APyFloatArray.from_float([float("inf"), float("nan"), -float("inf")], 9, 10)
    )
    assert (b * a).is_identical(
        APyFloatArray.from_float([float("inf"), float("nan"), -float("inf")], 9, 10)
    )

    a = APyFloatArray.from_float([0.5, 0, -5], 8, 10)
    b = APyFloat.from_float(float("nan"), 9, 8)
    assert (a * b).is_identical(
        APyFloatArray.from_float([float("nan"), float("nan"), -float("nan")], 9, 10)
    )
    assert (b * a).is_identical(
        APyFloatArray.from_float([float("nan"), float("nan"), -float("nan")], 9, 10)
    )

    a = APyFloatArray.from_float([float("inf"), float("nan"), -float("inf")], 8, 10)
    b = APyFloat.from_float(0, 9, 8)
    assert (a * b).is_identical(
        APyFloatArray.from_float([float("nan"), float("nan"), -float("nan")], 9, 10)
    )
    assert (b * a).is_identical(
        APyFloatArray.from_float([float("nan"), float("nan"), -float("nan")], 9, 10)
    )


@pytest.mark.float_array
def test_array_mul_int_float():
    a = APyFloatArray.from_float([12, 23, 34], 8, 10)
    assert (a * 1).is_identical(a)
    assert (1 * a).is_identical(a)
    assert (a * 1.0).is_identical(a)
    assert (1.0 * a).is_identical(a)


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


@pytest.mark.float_array
def test_array_div_scalar():
    a = APyFloatArray.from_float([4, 12, 40], 9, 10)
    b = APyFloat.from_float(8, 9, 8)
    assert (a / b).is_identical(APyFloatArray.from_float([0.5, 1.5, 5], 9, 10))


@pytest.mark.float_array
def test_array_rdiv_scalar():
    a = APyFloatArray.from_float([4, 5, 32], 9, 10)
    b = APyFloat.from_float(8, 5, 8)
    assert (b / a).is_identical(APyFloatArray.from_float([2, 1.6, 0.25], 9, 10))


@pytest.mark.float_array
def test_array_div_int_float():
    a = APyFloatArray.from_float([4, 12, 40], 9, 10)
    assert (a / 1).is_identical(a)
    assert (a / 1.0).is_identical(a)


@pytest.mark.float_array
def test_transpose():

    # High-dimensional transor transposition not implemented
    with pytest.raises(ValueError, match="Not implemented: high-dimensional"):
        APyFloatArray([[[1]]], [[[1]]], [[[1]]], 3, 3).T

    # 1-D transposition simply returns the input (just like NumPy-arrays)
    assert APyFloatArray([], [], [], 4, 3).T.is_identical(
        APyFloatArray([], [], [], 4, 3)
    )
    assert APyFloatArray([1, 0, 1], [15, 4, 20], [7, 2, 3], 5, 5).T.is_identical(
        APyFloatArray([1, 0, 1], [15, 4, 20], [7, 2, 3], 5, 5)
    )

    # # 2-D transposition returns the matrix transposition
    a = APyFloatArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=5,
        man_bits=2,
    )
    assert a.T.T.is_identical(a)
    assert a.T.is_identical(
        APyFloatArray.from_float(
            [
                [1.0, -4.0],
                [2.0, -5.0],
                [3.0, -6.0],
            ],
            exp_bits=5,
            man_bits=2,
        )
    )


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
