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
    def check_zeros(
        int_bits=None, frac_bits=None, bits=None, exp_bits=None, mantissa_bits=None
    ):
        a = zeros(
            shape,
            int_bits=int_bits,
            frac_bits=frac_bits,
            bits=bits,
            exp_bits=exp_bits,
            mantissa_bits=mantissa_bits,
        )
        if isinstance(a, APyFixedArray):
            b = APyFixedArray.from_float(
                [0] * (shape[0] * shape[1]),
                int_bits=int_bits,
                frac_bits=frac_bits,
                bits=bits,
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
    check_zeros(int_bits=12314, bits=14146)
    check_zeros(bits=5, frac_bits=5)

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
        ArrayType,
        int_bits=None,
        frac_bits=None,
        bits=None,
        exp_bits=None,
        mantissa_bits=None,
    ):
        if ArrayType is APyFixedArray:
            b = ArrayType.from_float(
                [0] * (shape[0] * shape[1]),
                int_bits=int_bits,
                frac_bits=frac_bits,
                bits=bits,
            ).reshape(shape)
            a = zeros_like(b, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
        elif ArrayType is APyFloatArray:
            b = ArrayType.from_float(
                [0] * (shape[0] * shape[1]), exp_bits=exp_bits, man_bits=mantissa_bits
            ).reshape(shape)
            a = zeros_like(b, exp_bits=exp_bits, mantissa_bits=mantissa_bits)
        assert a.is_identical(
            b
        ), f"zeros_like on {ArrayType.__name__} didn't work when shape={shape}."

    # Test cases for APyFixedArray
    check_zeros_like(APyFixedArray, bits=10, frac_bits=5)
    check_zeros_like(APyFixedArray, bits=10, int_bits=5)
    check_zeros_like(APyFixedArray, int_bits=12314, frac_bits=1832)

    # Test cases for APyFloatArray
    check_zeros_like(APyFloatArray, exp_bits=13, mantissa_bits=28)
    check_zeros_like(APyFloatArray, exp_bits=16, mantissa_bits=5)


def test_zeros_like_raises():
    a = zeros((5, 5), int_bits=10, frac_bits=10)
    with pytest.raises(ValueError, match=r"Could not extract fixed-point"):
        _ = zeros_like(a, int_bits=2)
    with pytest.raises(ValueError, match=r"Could not extract fixed-point"):
        _ = zeros_like(a, bits=1000, int_bits=2, frac_bits=2)

    # Does *NOT* raise
    _ = zeros_like(a, bits=4, int_bits=2, frac_bits=2)


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


def test_arange():
    # Test raises
    with pytest.raises(
        ValueError,
        match="Step size cannot be zero",
    ):
        arange(0, 10, 0, int_bits=4, frac_bits=0)

    with pytest.raises(
        ValueError,
        match="Undefined input",
    ):
        arange(10, 2, 1, int_bits=4, frac_bits=0)

    with pytest.raises(
        ValueError,
        match="Undefined input",
    ):
        arange(2, 10, -1, int_bits=4, frac_bits=0)

    with pytest.raises(
        ValueError,
        match="Cannot convert",
    ):
        arange(0, float("inf"), 2**5, exp_bits=5, man_bits=10)

    with pytest.raises(
        ValueError,
        match="Cannot convert",
    ):
        arange(float("nan"), 10, 2**5, exp_bits=5, man_bits=10)

    with pytest.raises(
        ValueError,
        match="Cannot convert",
    ):
        arange(0, 10, float("inf"), exp_bits=5, man_bits=10)

    with pytest.raises(
        ValueError,
        match="Non supported type",
    ):
        arange("0", 10, 1, exp_bits=5, man_bits=10)

    # Test basic functionality
    a = arange(10, int_bits=10, frac_bits=5)
    b = APyFixedArray.from_float(range(10), int_bits=10, frac_bits=5)
    assert a.is_identical(b), "Arrange for APyFixedArray failed"

    a = arange(10, exp_bits=10, man_bits=5)
    b = APyFloatArray.from_float(range(10), exp_bits=10, man_bits=5)
    assert a.is_identical(b), "Arrange for APyFloatArray failed"

    a = arange(5, 8, int_bits=10, frac_bits=5)
    assert a.is_identical(APyFixedArray.from_float([5, 6, 7], int_bits=10, frac_bits=5))

    a = arange(5, 8, exp_bits=5, man_bits=12, bias=10)
    assert a.is_identical(
        APyFloatArray.from_float([5, 6, 7], exp_bits=5, man_bits=12, bias=10)
    )

    a = arange(8, 5, -1, int_bits=10, frac_bits=5)
    assert a.is_identical(APyFixedArray.from_float([8, 7, 6], int_bits=10, frac_bits=5))

    a = arange(8, 5, -1, exp_bits=5, man_bits=12, bias=10)
    assert a.is_identical(
        APyFloatArray.from_float([8, 7, 6], exp_bits=5, man_bits=12, bias=10)
    )

    a = arange(8, 5, -2, int_bits=10, frac_bits=5)
    assert a.is_identical(APyFixedArray.from_float([8, 6], int_bits=10, frac_bits=5))

    a = arange(8, 5, -2, exp_bits=5, man_bits=12, bias=10)
    assert a.is_identical(
        APyFloatArray.from_float([8, 6], exp_bits=5, man_bits=12, bias=10)
    )

    a = arange(8, 5, -3, int_bits=10, frac_bits=5)
    assert a.is_identical(APyFixedArray.from_float([8], int_bits=10, frac_bits=5))

    a = arange(8, 5, -3, exp_bits=5, man_bits=12, bias=10)
    assert a.is_identical(
        APyFloatArray.from_float([8], exp_bits=5, man_bits=12, bias=10)
    )

    a = arange(8, 5, -8, int_bits=10, frac_bits=5)
    assert a.is_identical(APyFixedArray.from_float([8], int_bits=10, frac_bits=5))

    a = arange(8, 5, -8, exp_bits=5, man_bits=12, bias=10)
    assert a.is_identical(
        APyFloatArray.from_float([8], exp_bits=5, man_bits=12, bias=10)
    )

    a = arange(0, 2, 2 ** (-2), int_bits=10, frac_bits=2)
    assert a.is_identical(
        APyFixedArray.from_float(
            [0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75], int_bits=10, frac_bits=2
        )
    )

    a = arange(0, 2, 2 ** (-2), exp_bits=5, man_bits=5)
    assert a.is_identical(
        APyFloatArray.from_float(
            [0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75], exp_bits=5, man_bits=5
        )
    )

    a = arange(2, 0, -(2 ** (-2)), int_bits=10, frac_bits=2)
    assert a.is_identical(
        APyFixedArray.from_float(
            [0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0][::-1], int_bits=10, frac_bits=2
        )
    )

    a = arange(2, 0, -(2 ** (-2)), exp_bits=5, man_bits=5)
    assert a.is_identical(
        APyFloatArray.from_float(
            [0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0][::-1], exp_bits=5, man_bits=5
        )
    )

    # Test with APy-scalars
    a = arange(
        APyFixed(2, 5, 0),
        APyFloat(0, 9, 0, 4, 3),
        APyFixed(1, 5, 2),
        exp_bits=5,
        man_bits=5,
    )
    assert a.is_identical(
        APyFloatArray.from_float(
            [2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75], exp_bits=5, man_bits=5
        )
    )

    # Test quantization effects
    a = arange(0, 100, int_bits=10, frac_bits=2)
    assert a.is_identical(
        APyFixedArray.from_float(range(100), int_bits=10, frac_bits=2)
    )

    a = arange(0, 100, exp_bits=5, man_bits=2)
    assert a.is_identical(APyFloatArray.from_float(range(100), exp_bits=5, man_bits=2))

    a = arange(0, 2, 0.125, int_bits=8, frac_bits=1)
    assert a.is_identical(
        APyFixedArray.from_float(
            [
                0.0,
                0.125,
                0.25,
                0.375,
                0.5,
                0.625,
                0.75,
                0.875,
                1.0,
                1.125,
                1.25,
                1.375,
                1.5,
                1.625,
                1.75,
                1.875,
            ],
            int_bits=8,
            frac_bits=1,
        )
    )

    a = arange(0, 2, 0.125, exp_bits=5, man_bits=2)
    assert a.is_identical(
        APyFloatArray.from_float(
            [
                0.0,
                0.125,
                0.25,
                0.375,
                0.5,
                0.625,
                0.75,
                0.875,
                1.0,
                1.125,
                1.25,
                1.375,
                1.5,
                1.625,
                1.75,
                1.875,
            ],
            exp_bits=5,
            man_bits=2,
        )
    )

    # Test subnormals
    a = arange(APyFixed(1, int_bits=1, frac_bits=1074), 1, 2, exp_bits=11, man_bits=52)
    assert a.is_identical(APyFloatArray([0], [0], [1], 11, 52))

    a = arange(0, 4 * 2**-1074, 2**-1074, exp_bits=11, man_bits=52)
    assert a.is_identical(
        APyFloatArray([0, 0, 0, 0], [0, 0, 0, 0], [0, 1, 2, 3], 11, 52)
    )

    a = arange(
        APyFixed(0, int_bits=1, frac_bits=1074),
        APyFixed(1, int_bits=1, frac_bits=1072),
        APyFixed(1, int_bits=1, frac_bits=1074),
        exp_bits=11,
        man_bits=52,
    )
    assert a.is_identical(
        APyFloatArray([0, 0, 0, 0], [0, 0, 0, 0], [0, 1, 2, 3], 11, 52)
    )

    # Negative result for APyFloatArray
    a = arange(-10, 1, exp_bits=5, man_bits=4, bias=6)
    assert a.is_identical(
        APyFloatArray(
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
            [9, 9, 9, 8, 8, 8, 8, 7, 7, 6, 0],
            [4, 2, 0, 12, 8, 4, 0, 8, 0, 0, 0],
            exp_bits=5,
            man_bits=4,
            bias=6,
        )
    )

    # Becomes infinity
    a = arange(200, 280, 20, exp_bits=4, man_bits=3)
    assert a.is_identical(
        APyFloatArray(
            [0, 0, 0, 0], [14, 14, 14, 15], [4, 6, 7, 0], exp_bits=4, man_bits=3
        )
    )

    # Test precision beyond Python floats
    a = arange(
        APyFixed(1, int_bits=1, frac_bits=1080),
        APyFixed(4, int_bits=1, frac_bits=1080),
        APyFixed(1, int_bits=1, frac_bits=1080),
        int_bits=1,
        frac_bits=1080,
    )
    assert a.is_identical(APyFixedArray([1, 2, 3], int_bits=1, frac_bits=1080))

    a = arange(
        APyFloat(0, 1, 0, exp_bits=20, man_bits=10),
        APyFloat(0, 3, 0, exp_bits=20, man_bits=10),
        APyFloat(0, 1, 0, exp_bits=20, man_bits=10),
        exp_bits=20,
        man_bits=10,
    )
    assert a.is_identical(
        APyFloatArray([0, 0, 0], [1, 2, 2], [0, 0, 1 << 9], exp_bits=20, man_bits=10)
    )
