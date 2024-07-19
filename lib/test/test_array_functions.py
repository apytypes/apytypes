from apytypes import (
    APyFixed,
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
def test_eye_fixed(n, m, nums):
    int_bits, frac_bits = 5, 5
    a = eye(APyFixedArray, n=n, m=m, int_bits=int_bits, frac_bits=frac_bits)
    m = n if m is None else m
    b = APyFixedArray.from_float(nums, int_bits=int_bits, frac_bits=frac_bits).reshape(
        (n, m)
    )
    if not a.is_identical(b):
        pytest.fail(
            f"eye on FixedArray didn't work when n={n}, m={m}. Expected result was {nums} but got \n {a}"
        )

    int_bits, frac_bits = 12314, 1832
    a = eye(APyFixedArray, n=n, m=m, int_bits=int_bits, frac_bits=frac_bits)
    b = APyFixedArray.from_float(nums, int_bits=int_bits, frac_bits=frac_bits).reshape(
        (n, m)
    )
    if not a.is_identical(b):
        pytest.fail(
            f"eye on FixedArray didn't work when n={n}, m={m}. Expected result was {nums} but got \n {a}"
        )


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
def test_identity_fixed(n, nums):
    int_bits, frac_bits = 5, 5
    a = identity(APyFixedArray, n=n, int_bits=int_bits, frac_bits=frac_bits)
    b = APyFixedArray.from_float(nums, int_bits=int_bits, frac_bits=frac_bits).reshape(
        (n, n)
    )
    if not a.is_identical(b):
        pytest.fail(
            f"identity on FixedArray didn't work when n={n}. Expected result was {nums} but got \n {a}"
        )

    int_bits, frac_bits = 38231, 1237
    a = identity(APyFixedArray, n=n, int_bits=int_bits, frac_bits=frac_bits)
    b = APyFixedArray.from_float(nums, int_bits=int_bits, frac_bits=frac_bits).reshape(
        (n, n)
    )
    if not a.is_identical(b):
        pytest.fail(
            f"identity on FixedArray didn't work when n={n}. Expected result was {nums} but got \n {a}"
        )


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_zeros_fixed(shape):
    int_bits, frac_bits = 5, 5
    a = zeros(APyFixedArray, shape, int_bits=int_bits, frac_bits=frac_bits)
    b = APyFixedArray.from_float(
        [0] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
    ).reshape(shape)
    if not a.is_identical(b):
        pytest.fail(f"zeros on FixedArray didn't work when shape={shape}.")


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_zeros_like_fixed(shape):
    int_bits, frac_bits = 5, 5
    b = APyFixedArray.from_float(
        [0] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
    ).reshape(shape)

    a = zeros_like(b, int_bits=int_bits, frac_bits=frac_bits)
    if not a.is_identical(b):
        pytest.fail(f"zeros on FixedArray didn't work when shape={shape}.")


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_ones_fixed(shape):
    int_bits, frac_bits = 5, 5
    a = ones(APyFixedArray, shape, int_bits=int_bits, frac_bits=frac_bits)
    b = APyFixedArray.from_float(
        [1] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
    ).reshape(shape)
    if not a.is_identical(b):
        pytest.fail(f"ones on FixedArray didn't work when shape={shape}.")


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_ones_like_fixed(shape):
    int_bits, frac_bits = 5, 5
    b = APyFixedArray.from_float(
        [1] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
    ).reshape(shape)
    a = ones_like(b, int_bits=int_bits, frac_bits=frac_bits)
    if not a.is_identical(b):
        pytest.fail(f"ones on FixedArray didn't work when shape={shape}.")


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_full_fixed(shape):
    int_bits, frac_bits = 5, 5
    b = APyFixedArray.from_float(
        [shape[0] + shape[1]] * (shape[0] * shape[1]),
        int_bits=int_bits,
        frac_bits=frac_bits,
    ).reshape(shape)
    num = APyFixed.from_float(
        shape[0] + shape[1], int_bits=int_bits, frac_bits=frac_bits
    )
    a = full(APyFixedArray, shape, num, int_bits=int_bits, frac_bits=frac_bits)
    if not a.is_identical(b):
        pytest.fail(f"ones on FixedArray didn't work when shape={shape}.")


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_full_like_fixed(shape):
    int_bits, frac_bits = 5, 5
    b = APyFixedArray.from_float(
        [shape[0] + shape[1]] * (shape[0] * shape[1]),
        int_bits=int_bits,
        frac_bits=frac_bits,
    ).reshape(shape)
    num = APyFixed.from_float(
        shape[0] + shape[1], int_bits=int_bits, frac_bits=frac_bits
    )
    a = full_like(b, num, int_bits=int_bits, frac_bits=frac_bits)
    if not a.is_identical(b):
        pytest.fail(f"ones on FixedArray didn't work when shape={shape}.")
    a = full_like(b, shape[0] + shape[1], int_bits=int_bits, frac_bits=frac_bits)
    if not a.is_identical(b):
        pytest.fail(f"ones on FixedArray didn't work when shape={shape}.")
