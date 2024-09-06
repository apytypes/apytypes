from apytypes import (
    squeeze,
    reshape,
    shape,
    transpose,
    ravel,
    moveaxis,
    expand_dims,
    swapaxes,
    eye,
    identity,
    zeros,
    zeros_like,
    ones,
    ones_like,
    full,
    full_like,
    arange,
    APyFloat,
    APyFixed,
    APyFixedArray,
    APyFloatArray,
)
from math import log2, ceil
import pytest


def test_squeeze():
    a = APyFixedArray([[1, 2]], 3, 0)
    assert a.shape == (1, 2)

    b = squeeze(a)
    assert b.shape == (2,)


def test_reshape():
    a = APyFixedArray([[1, 2]], 3, 0)
    assert a.shape == (1, 2)
    b = reshape(a, (2, 1))
    assert b.shape == (2, 1)
    c = reshape(b, (-1,))
    assert c.shape == (2,)


def test_shape():
    a = APyFixedArray([[1, 2]], 3, 0)
    assert shape(a) == (1, 2)
    b = reshape(a, (2, 1))
    assert shape(b) == (2, 1)
    c = reshape(b, (-1,))
    assert shape(c) == (2,)


def test_transpose():
    a = APyFloatArray.from_float([[1, 2], [3, 4]], 5, 5)
    transpose(a).is_identical(APyFloatArray.from_float([[1, 4], [3, 2]], 5, 5))


def test_ravel():
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # manually create 1d arr
    arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=5, man_bits=2)
    reshaped = arr.reshape((3, 2, 2))
    assert APyFloatArray.is_identical(ravel(reshaped), arr)


def _fixedArrange(end, start=0, jump=1):
    int_bits = int(ceil(log2(end + 1))) + 1
    return APyFixedArray.from_float(
        [num for num in range(start, end, jump)], int_bits=int_bits, frac_bits=0
    )


def _floatArrange(end, start=0, jump=1):
    # probably buggy
    exp_bits = int(ceil(log2(end + 1))) + 1
    man_bits = 64 - 1 - exp_bits
    return APyFloatArray.from_float(
        [num for num in range(start, end, jump)], exp_bits=exp_bits, man_bits=man_bits
    )


def test_moveaxis():
    a = _fixedArrange(10).reshape((5, 2))
    b = APyFixedArray([[0, 2, 4, 6, 8], [1, 3, 5, 7, 9]], a.int_bits, frac_bits=0)
    if not moveaxis(a, 0, 1).is_identical(b):
        pytest.fail("moveaxis didn't correctly move axis")

    if not moveaxis(a, 1, 0).is_identical(b):
        pytest.fail("moveaxis didn't correctly move axis")


@pytest.mark.parametrize(
    "source, destination, expected_shape",
    [
        (0, 2, (3, 4, 2)),
        (1, 0, (3, 2, 4)),
        (2, 0, (4, 2, 3)),
    ],
)
def test_moveaxis_shape(source, destination, expected_shape):
    a = _fixedArrange(24).reshape((2, 3, 4))
    if moveaxis(a, source, destination).shape != expected_shape:
        pytest.fail(
            f"moveaxis didn't get correct shape. Got {moveaxis(a, source, destination).shape} expected {expected_shape}"
        )


def test_swapaxes():
    a = _fixedArrange(10).reshape((5, 2))

    if not swapaxes(a, 0, 1).is_identical(a.T):
        pytest.fail("swapaxes didn't correctly swap axis")

    if not swapaxes(a, 1, 0).is_identical(a.T):
        pytest.fail("swapaxes didn't correctly swap axis")

    a = _fixedArrange(24).reshape((4, 3, 2))

    if not swapaxes(a, 0, 1).shape == (3, 4, 2):
        pytest.fail("swapaxes didn't correctly swap axis")

    if not swapaxes(a, 1, 0).shape == (3, 4, 2):
        pytest.fail("swapaxes didn't correctly swap axis")

    if not swapaxes(a, 2, 0).shape == (2, 3, 4):
        pytest.fail("swapaxes didn't correctly swap axis")

    if not swapaxes(a, 0, 2).shape == (2, 3, 4):
        pytest.fail("swapaxes didn't correctly swap axis")


def test_expanddims():
    a = _fixedArrange(10).reshape((2, 5))
    b = _fixedArrange(10).reshape((1, 2, 5))
    c = expand_dims(a, 0)
    assert c.is_identical(b), "expanddims failed for fixed array"

    a = _floatArrange(10).reshape((2, 5))
    b = _floatArrange(10).reshape((1, 2, 5))
    c = expand_dims(a, 0)
    assert c.is_identical(b), "expanddims failed for float array"


@pytest.mark.parametrize(
    "n, m, nums",
    [
        (1, 1, [1]),
        (1, 2, [1, 0]),
        (2, None, [1, 0, 0, 1]),
        (2, 3, [1, 0, 0, 0, 1, 0]),
        (4, None, [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]),
    ],
)
def test_eye(n, m, nums):
    def check_eye(int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None):
        a = eye(
            n=n,
            m=m,
            int_bits=int_bits,
            frac_bits=frac_bits,
            exp_bits=exp_bits,
            mantissa_bits=mantissa_bits,
        )
        if isinstance(a, APyFixedArray):
            b = APyFixedArray.from_float(
                nums, int_bits=int_bits, frac_bits=frac_bits
            ).reshape((n, m if m is not None else n))
        elif isinstance(a, APyFloatArray):
            b = APyFloatArray.from_float(
                nums, exp_bits=exp_bits, man_bits=mantissa_bits
            ).reshape((n, m if m is not None else n))
        assert a.is_identical(
            b
        ), f"eye on {type(a).__name__} didn't work when n={n}, m={m}. Expected result was {nums} but got \n {a}"

    # Test cases for APyFixedArray
    check_eye(int_bits=5, frac_bits=5)
    check_eye(int_bits=12314, frac_bits=1832)

    # Test cases for APyFloatArray
    check_eye(exp_bits=13, mantissa_bits=28)
    check_eye(exp_bits=16, mantissa_bits=5)


@pytest.mark.parametrize(
    "n, nums",
    [
        (1, [1]),
        (2, [1, 0, 0, 1]),
        (3, [1, 0, 0, 0, 1, 0, 0, 0, 1]),
        (4, [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]),
        (
            5,
            [1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1],
        ),
    ],
)
def test_identity(n, nums):
    def check_identity(
        int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None
    ):
        a = identity(
            n=n,
            int_bits=int_bits,
            frac_bits=frac_bits,
            exp_bits=exp_bits,
            mantissa_bits=mantissa_bits,
        )
        if isinstance(a, APyFixedArray):
            b = APyFixedArray.from_float(
                nums, int_bits=int_bits, frac_bits=frac_bits
            ).reshape((n, n))
        elif isinstance(a, APyFloatArray):
            b = APyFloatArray.from_float(
                nums, exp_bits=exp_bits, man_bits=mantissa_bits
            ).reshape((n, n))
        assert a.is_identical(
            b
        ), f"identity on {type(a).__name__} didn't work when n={n}. Expected result was {nums} but got \n {a}"

    # Test cases for APyFixedArray
    check_identity(int_bits=5, frac_bits=5)
    check_identity(int_bits=38231, frac_bits=1237)

    # Test cases for APyFloatArray
    check_identity(exp_bits=13, mantissa_bits=28)
    check_identity(exp_bits=16, mantissa_bits=5)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_zeros(shape):
    def check_zeros(int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None):
        a = zeros(
            shape,
            int_bits=int_bits,
            frac_bits=frac_bits,
            exp_bits=exp_bits,
            mantissa_bits=mantissa_bits,
        )
        if isinstance(a, APyFixedArray):
            b = APyFixedArray.from_float(
                [0] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
            ).reshape(shape)
        if isinstance(a, APyFloatArray):
            b = APyFloatArray.from_float(
                [0] * (shape[0] * shape[1]), exp_bits=exp_bits, man_bits=mantissa_bits
            ).reshape(shape)
        assert a.is_identical(
            b
        ), f"zeros on {a.__name__} didn't work when shape={shape}."

    # APyFixedArray
    check_zeros(int_bits=5, frac_bits=5)
    check_zeros(int_bits=12314, frac_bits=1832)
    check_zeros(int_bits=0, frac_bits=5)

    # APyFloatArray
    check_zeros(exp_bits=13, mantissa_bits=28)
    check_zeros(exp_bits=13, mantissa_bits=0)
    check_zeros(exp_bits=16, mantissa_bits=5)


def test_tuple_construction_raises():
    with pytest.raises(
        ValueError,
        match=r"cpp_shape_from_python_shape_like\(\): negative integers disallowed",
    ):
        zeros((4, -2), int_bits=2, frac_bits=2)

    with pytest.raises(
        ValueError, match=r"cpp_shape_from_python_shape_like\(\): integer to large"
    ):
        zeros((111111111111111111111111111111111111, -2), int_bits=2, frac_bits=2)

    with pytest.raises(
        ValueError,
        match=r"cpp_shape_from_python_shape_like\(\): only integer dimensions allowed",
    ):
        zeros((2.5, -2), int_bits=2, frac_bits=2)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_zeros_like(shape):
    def check_zeros_like(
        ArrayType, int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None
    ):
        if ArrayType is APyFixedArray:
            b = ArrayType.from_float(
                [0] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
            ).reshape(shape)
            a = zeros_like(b, int_bits=int_bits, frac_bits=frac_bits)
        elif ArrayType is APyFloatArray:
            b = ArrayType.from_float(
                [0] * (shape[0] * shape[1]), exp_bits=exp_bits, man_bits=mantissa_bits
            ).reshape(shape)
            a = zeros_like(b, exp_bits=exp_bits, mantissa_bits=mantissa_bits)
        assert a.is_identical(
            b
        ), f"zeros_like on {ArrayType.__name__} didn't work when shape={shape}."

    # Test cases for APyFixedArray
    check_zeros_like(APyFixedArray, int_bits=5, frac_bits=5)
    check_zeros_like(APyFixedArray, int_bits=12314, frac_bits=1832)

    # Test cases for APyFloatArray
    check_zeros_like(APyFloatArray, exp_bits=13, mantissa_bits=28)
    check_zeros_like(APyFloatArray, exp_bits=16, mantissa_bits=5)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_ones(shape):
    def check_ones(int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None):
        a = ones(
            shape,
            int_bits=int_bits,
            frac_bits=frac_bits,
            exp_bits=exp_bits,
            mantissa_bits=mantissa_bits,
        )
        if isinstance(a, APyFixedArray):
            b = APyFixedArray.from_float(
                [1] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
            ).reshape(shape)
        elif isinstance(a, APyFloatArray):
            b = APyFloatArray.from_float(
                [1] * (shape[0] * shape[1]), exp_bits=exp_bits, man_bits=mantissa_bits
            ).reshape(shape)
        assert a.is_identical(
            b
        ), f"ones on {type(a).__name__} didn't work when shape={shape}."

    # Test cases for APyFixedArray
    check_ones(int_bits=5, frac_bits=5)
    check_ones(int_bits=12314, frac_bits=1832)

    # Test cases for APyFloatArray
    check_ones(exp_bits=13, mantissa_bits=28)
    check_ones(exp_bits=16, mantissa_bits=5)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_ones_like(shape):
    def check_ones_like(
        ArrayType, int_bits=None, frac_bits=None, exp_bits=None, mantissa_bits=None
    ):
        if ArrayType is APyFixedArray:
            b = ArrayType.from_float(
                [1] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
            ).reshape(shape)
            a = ones_like(b, int_bits=int_bits, frac_bits=frac_bits)
        elif ArrayType is APyFloatArray:
            b = ArrayType.from_float(
                [1] * (shape[0] * shape[1]), exp_bits=exp_bits, man_bits=mantissa_bits
            ).reshape(shape)
            a = ones_like(b, exp_bits=exp_bits, mantissa_bits=mantissa_bits)
        assert a.is_identical(
            b
        ), f"ones_like on {ArrayType.__name__} didn't work when shape={shape}."

    # Test cases for APyFixedArray
    check_ones_like(APyFixedArray, int_bits=5, frac_bits=5)
    check_ones_like(APyFixedArray, int_bits=12314, frac_bits=1832)

    # Test cases for APyFloatArray
    check_ones_like(APyFloatArray, exp_bits=13, mantissa_bits=28)
    check_ones_like(APyFloatArray, exp_bits=16, mantissa_bits=5)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_full(shape):
    def check_full(
        ArrayType,
        fill_value,
        int_bits=None,
        frac_bits=None,
        exp_bits=None,
        mantissa_bits=None,
    ):
        if ArrayType is APyFixedArray:
            b = ArrayType.from_float(
                [fill_value] * (shape[0] * shape[1]),
                int_bits=int_bits,
                frac_bits=frac_bits,
            ).reshape(shape)
            num = APyFixed.from_float(
                fill_value, int_bits=int_bits, frac_bits=frac_bits
            )
            a = full(shape, num, int_bits=int_bits, frac_bits=frac_bits)
        elif ArrayType is APyFloatArray:
            b = ArrayType.from_float(
                [fill_value] * (shape[0] * shape[1]),
                exp_bits=exp_bits,
                man_bits=mantissa_bits,
            ).reshape(shape)
            num = APyFloat.from_float(
                fill_value, exp_bits=exp_bits, man_bits=mantissa_bits
            )
            a = full(shape, num, exp_bits=exp_bits, mantissa_bits=mantissa_bits)
        assert a.is_identical(
            b
        ), f"full on {ArrayType.__name__} didn't work when shape={shape}."

    # Test cases for APyFixedArray
    check_full(APyFixedArray, fill_value=shape[0] + shape[1], int_bits=5, frac_bits=5)
    check_full(
        APyFixedArray, fill_value=shape[0] + shape[1], int_bits=12314, frac_bits=1832
    )

    check_full(
        APyFixedArray, fill_value=shape[0] + shape[1], int_bits=0, frac_bits=1832
    )

    # Test cases for APyFloatArray
    check_full(
        APyFloatArray, fill_value=shape[0] + shape[1], exp_bits=13, mantissa_bits=28
    )
    check_full(
        APyFloatArray, fill_value=shape[0] + shape[1], exp_bits=16, mantissa_bits=5
    )
    check_full(
        APyFloatArray, fill_value=shape[0] + shape[1], exp_bits=0, mantissa_bits=5
    )


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_full_like(shape):
    def check_full_like(
        ArrayType,
        fill_value,
        int_bits=None,
        frac_bits=None,
        exp_bits=None,
        mantissa_bits=None,
    ):
        if ArrayType is APyFixedArray:
            b = ArrayType.from_float(
                [fill_value] * (shape[0] * shape[1]),
                int_bits=int_bits,
                frac_bits=frac_bits,
            ).reshape(shape)
            num = APyFixed.from_float(
                fill_value, int_bits=int_bits, frac_bits=frac_bits
            )
            a = full_like(b, num, int_bits=int_bits, frac_bits=frac_bits)
            assert a.is_identical(
                b
            ), f"full_like on {ArrayType.__name__} didn't work when shape={shape}."
            a = full_like(b, fill_value, int_bits=int_bits, frac_bits=frac_bits)
            assert a.is_identical(
                b
            ), f"full_like on {ArrayType.__name__} didn't work when shape={shape}."
        elif ArrayType is APyFloatArray:
            b = ArrayType.from_float(
                [fill_value] * (shape[0] * shape[1]),
                exp_bits=exp_bits,
                man_bits=mantissa_bits,
            ).reshape(shape)
            num = APyFloat.from_float(
                fill_value, exp_bits=exp_bits, man_bits=mantissa_bits
            )
            a = full_like(b, num, exp_bits=exp_bits, mantissa_bits=mantissa_bits)
            assert a.is_identical(
                b
            ), f"full_like on {ArrayType.__name__} didn't work when shape={shape}."
            a = full_like(b, fill_value, exp_bits=exp_bits, mantissa_bits=mantissa_bits)
            assert a.is_identical(
                b
            ), f"full_like on {ArrayType.__name__} didn't work when shape={shape}."

    # Test cases for APyFixedArray
    check_full_like(
        APyFixedArray, fill_value=shape[0] + shape[1], int_bits=5, frac_bits=5
    )
    check_full_like(
        APyFixedArray, fill_value=shape[0] + shape[1], int_bits=12314, frac_bits=1832
    )
    check_full_like(
        APyFixedArray, fill_value=shape[0] + shape[1], int_bits=0, frac_bits=5
    )

    check_full_like(
        APyFixedArray, fill_value=shape[0] + shape[1], int_bits=5, frac_bits=0
    )

    check_full_like(
        APyFixedArray, fill_value=shape[0] + shape[1], int_bits=5, frac_bits=0
    )

    # Test cases for APyFloatArray

    check_full_like(
        APyFloatArray, fill_value=shape[0] + shape[1], exp_bits=13, mantissa_bits=28
    )
    check_full_like(
        APyFloatArray, fill_value=shape[0] + shape[1], exp_bits=16, mantissa_bits=5
    )
    check_full_like(
        APyFloatArray, fill_value=shape[0] + shape[1], exp_bits=16, mantissa_bits=0
    )


def test_arrange():
    a = arange(10, int_bits=10, frac_bits=5)
    b = APyFixedArray.from_float([i for i in range(10)], int_bits=10, frac_bits=5)
    assert a.is_identical(b), "Arrange for APyFixedArray failed"

    a = arange(10, exp_bits=10, mantissa_bits=5)
    b = APyFloatArray.from_float([i for i in range(10)], exp_bits=10, man_bits=5)
    assert a.is_identical(b), "Arrange for APyFloatArray failed"

    a = arange(5, 8, int_bits=10, frac_bits=5)
    assert a.is_identical(APyFixedArray.from_float([5, 6, 7], int_bits=10, frac_bits=5))

    a = arange(8, 5, -1, int_bits=10, frac_bits=5)
    assert a.is_identical(APyFixedArray.from_float([8, 7, 6], int_bits=10, frac_bits=5))

    a = arange(8, 5, -2, int_bits=10, frac_bits=5)
    assert a.is_identical(APyFixedArray.from_float([8, 6], int_bits=10, frac_bits=5))

    a = arange(8, 5, -3, int_bits=10, frac_bits=5)
    assert a.is_identical(APyFixedArray.from_float([8], int_bits=10, frac_bits=5))

    a = arange(0, 2, 2 ** (-2), int_bits=10, frac_bits=2)
    assert a.is_identical(
        APyFixedArray.from_float(
            [0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75], int_bits=10, frac_bits=2
        )
    )

    a = arange(0, 2, 2 ** (-2), int_bits=10, frac_bits=2)
    assert a.is_identical(
        APyFixedArray.from_float(
            [0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75], int_bits=10, frac_bits=2
        )
    )

    a = arange(0, 1, 2 ** (-4), int_bits=10, frac_bits=4)
    assert a.is_identical(
        APyFixedArray.from_float(
            [
                0,
                0.0625,
                0.125,
                0.1875,
                0.25,
                0.3125,
                0.375,
                0.4375,
                0.5,
                0.5625,
                0.625,
                0.6875,
                0.75,
                0.8125,
                0.875,
                0.9375,
            ],
            int_bits=10,
            frac_bits=4,
        )
    )

    a = arange(1, 0, -(2 ** (-4)), int_bits=10, frac_bits=4)
    assert a.is_identical(
        APyFixedArray.from_float(
            [
                0.0625,
                0.125,
                0.1875,
                0.25,
                0.3125,
                0.375,
                0.4375,
                0.5,
                0.5625,
                0.625,
                0.6875,
                0.75,
                0.8125,
                0.875,
                0.9375,
                1,
            ][::-1],
            int_bits=10,
            frac_bits=4,
        )
    )
