from math import ceil, log2
from typing import overload

import pytest

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyCFloat,
    APyCFloatArray,
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
    arange,
    expand_dims,
    eye,
    full,
    full_like,
    fullrange,
    identity,
    meshgrid,
    moveaxis,
    ones,
    ones_like,
    ravel,
    reshape,
    shape,
    squeeze,
    swapaxes,
    transpose,
    zeros,
    zeros_like,
)
from apytypes._typing import APyArray


@overload
def _Arrange(
    array_type: type[APyFloatArray], end: int, start: int = 0, jump: int = 1
) -> APyFloatArray: ...
@overload
def _Arrange(
    array_type: type[APyCFloatArray], end: int, start: int = 0, jump: int = 1
) -> APyCFloatArray: ...
@overload
def _Arrange(
    array_type: type[APyFixedArray], end: int, start: int = 0, jump: int = 1
) -> APyFixedArray: ...
@overload
def _Arrange(
    array_type: type[APyCFixedArray], end: int, start: int = 0, jump: int = 1
) -> APyCFixedArray: ...


def _Arrange(
    array_type: type[APyFloatArray | APyCFloatArray | APyFixedArray | APyCFixedArray],
    end: int,
    start: int = 0,
    jump: int = 1,
):
    if array_type is APyFloatArray or array_type is APyCFloatArray:
        # _floatArrange
        exp_bits = ceil(log2(end + 1)) + 1
        man_bits = 64 - 1 - exp_bits
        return array_type.from_float(
            list(range(start, end, jump)), exp_bits=exp_bits, man_bits=man_bits
        )
    elif array_type is APyFixedArray or array_type is APyCFixedArray:
        # _fixedArrange
        int_bits = ceil(log2(end + 1)) + 1
        return array_type.from_float(
            list(range(start, end, jump)), int_bits=int_bits, frac_bits=0
        )
    else:
        raise TypeError()


@pytest.mark.parametrize(
    "array_type", [APyFixedArray, APyCFixedArray, APyFloatArray, APyCFloatArray]
)
def test_squeeze(array_type: type[APyArray]):
    a = array_type.from_float([[1, 2]], 5, 5)
    assert a.shape == (1, 2)

    b = squeeze(a)
    assert b.shape == (2,)


@pytest.mark.parametrize(
    "array_type", [APyFixedArray, APyCFixedArray, APyFloatArray, APyCFloatArray]
)
def test_reshape(array_type: type[APyArray]):
    a = array_type.from_float([[1, 2]], 5, 5)
    assert a.shape == (1, 2)
    b = reshape(a, (2, 1))
    assert b.shape == (2, 1)
    c = reshape(b, (-1,))
    assert c.shape == (2,)


@pytest.mark.parametrize(
    "array_type", [APyFixedArray, APyCFixedArray, APyFloatArray, APyCFloatArray]
)
def test_shape(array_type: type[APyArray]):
    a = array_type.from_float([[1, 2]], 3, 0)
    assert shape(a) == (1, 2)
    b = reshape(a, (2, 1))
    assert shape(b) == (2, 1)
    c = reshape(b, (-1,))
    assert shape(c) == (2,)


@pytest.mark.parametrize(
    "array_type", [APyFixedArray, APyCFixedArray, APyFloatArray, APyCFloatArray]
)
def test_transpose(array_type: type[APyArray]):
    a = array_type.from_float([[1, 2], [3, 4]], 5, 5)
    assert transpose(a).is_identical(array_type.from_float([[1, 3], [2, 4]], 5, 5))

    b = array_type.from_float(range(2 * 2 * 2 * 2), 10, 10).reshape((2, 2, 2, 2))
    assert transpose(b).is_identical(
        array_type.from_float(
            [
                [[[0, 8], [4, 12]], [[2, 10], [6, 14]]],
                [[[1, 9], [5, 13]], [[3, 11], [7, 15]]],
            ],
            10,
            10,
        )
    )


def test_ravel():
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # manually create 1d arr
    arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=5, man_bits=2)
    reshaped = arr.reshape((3, 2, 2))
    assert APyFloatArray.is_identical(ravel(reshaped), arr)


@pytest.mark.parametrize("array_type", [APyFixedArray, APyCFixedArray])
def test_moveaxis(array_type: type[APyCFixedArray]):
    a = _Arrange(array_type, 10).reshape((5, 2))
    b = array_type([[0, 2, 4, 6, 8], [1, 3, 5, 7, 9]], a.int_bits, frac_bits=0)
    if not moveaxis(a, 0, 1).is_identical(b):
        pytest.fail("moveaxis didn't correctly move axis")

    if not moveaxis(a, 1, 0).is_identical(b):
        pytest.fail("moveaxis didn't correctly move axis")


@pytest.mark.parametrize(
    "array_type", [APyFixedArray, APyCFixedArray, APyFloatArray, APyCFloatArray]
)
@pytest.mark.parametrize(
    ("source", "destination", "expected_shape"),
    [
        (0, 2, (3, 4, 2)),
        (1, 0, (3, 2, 4)),
        (2, 0, (4, 2, 3)),
    ],
)
def test_fixed_moveaxis_shape(
    array_type: type[APyArray],
    source: int,
    destination: int,
    expected_shape: tuple[int, int, int],
):
    a = _Arrange(array_type, 24).reshape((2, 3, 4))
    if moveaxis(a, source, destination).shape != expected_shape:
        pytest.fail(
            "moveaxis didn't get correct shape. Got "
            + f"{moveaxis(a, source, destination).shape}, expected {expected_shape}"
        )


@pytest.mark.parametrize(
    "array_type", [APyFixedArray, APyCFixedArray, APyFloatArray, APyCFloatArray]
)
def test_swapaxes(array_type: type[APyArray]):
    a = _Arrange(array_type, 10).reshape((5, 2))

    if not swapaxes(a, 0, 1).is_identical(a.T):
        pytest.fail("swapaxes didn't correctly swap axis")

    if not swapaxes(a, 1, 0).is_identical(a.T):
        pytest.fail("swapaxes didn't correctly swap axis")

    a = _Arrange(array_type, 24).reshape((4, 3, 2))

    if swapaxes(a, 0, 1).shape != (3, 4, 2):
        pytest.fail("swapaxes didn't correctly swap axis")

    if swapaxes(a, 1, 0).shape != (3, 4, 2):
        pytest.fail("swapaxes didn't correctly swap axis")

    if swapaxes(a, 2, 0).shape != (2, 3, 4):
        pytest.fail("swapaxes didn't correctly swap axis")

    if swapaxes(a, 0, 2).shape != (2, 3, 4):
        pytest.fail("swapaxes didn't correctly swap axis")


@pytest.mark.parametrize(
    "array_type", [APyFixedArray, APyCFixedArray, APyFloatArray, APyCFloatArray]
)
def test_expanddims(array_type: type[APyArray]):
    a = _Arrange(array_type, 10).reshape((2, 5))
    b = _Arrange(array_type, 10).reshape((1, 2, 5))
    c = expand_dims(a, 0)
    assert c.is_identical(b)


@pytest.mark.parametrize(
    ("n", "m", "nums"),
    [
        (1, 1, [1]),
        (1, 2, [1, 0]),
        (2, None, [1, 0, 0, 1]),
        (2, 3, [1, 0, 0, 0, 1, 0]),
        (4, None, [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]),
    ],
)
def test_eye(n: int, m: int | None, nums: list[int]):
    def check_eye(
        int_bits: int | None = None,
        frac_bits: int | None = None,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        force_complex: bool | None = None,
    ):
        a = eye(
            n=n,
            m=m,
            int_bits=int_bits,
            frac_bits=frac_bits,
            exp_bits=exp_bits,
            man_bits=man_bits,
            bias=bias,
            force_complex=force_complex,
        )
        if isinstance(a, APyFixedArray):
            assert int_bits is not None
            assert frac_bits is not None
            b = APyFixedArray.from_float(
                nums, int_bits=int_bits, frac_bits=frac_bits
            ).reshape((n, m if m is not None else n))
        elif isinstance(a, APyCFixedArray):
            assert int_bits is not None
            assert frac_bits is not None
            b = APyCFixedArray.from_float(
                nums, int_bits=int_bits, frac_bits=frac_bits
            ).reshape((n, m if m is not None else n))
        elif isinstance(a, APyFloatArray):
            assert exp_bits is not None
            assert man_bits is not None
            b = APyFloatArray.from_float(
                nums, exp_bits=exp_bits, man_bits=man_bits, bias=bias
            ).reshape((n, m if m is not None else n))
        else:
            assert exp_bits is not None
            assert man_bits is not None
            b = APyCFloatArray.from_float(
                nums, exp_bits=exp_bits, man_bits=man_bits, bias=bias
            ).reshape((n, m if m is not None else n))
        assert a.is_identical(b), str(
            f"eye on {type(a).__name__} didn't work when n={n}, "
            + f"m={m}. Expected result was {nums} but got \n {a}"
        )

    # Test cases for `APyFixedArray`
    check_eye(int_bits=5, frac_bits=5)
    check_eye(int_bits=12314, frac_bits=1832)

    # Test cases for `APyCFixedArray`
    check_eye(int_bits=5, frac_bits=5, force_complex=True)
    check_eye(int_bits=12314, frac_bits=1832, force_complex=True)

    # Test cases for `APyFloatArray`
    check_eye(exp_bits=13, man_bits=28)
    check_eye(exp_bits=16, man_bits=5, bias=8)

    # Test cases for `APyCFloatArray`
    check_eye(exp_bits=13, man_bits=28, force_complex=True)
    check_eye(exp_bits=16, man_bits=5, bias=8, force_complex=True)

    # Test raise
    with pytest.raises(ValueError, match="eye: could not determine array type"):
        _ = eye(2, 2, int_bits=4, frac_bits=0, exp_bits=15)


@pytest.mark.parametrize(
    ("n", "nums"),
    [
        (1, [1]),
        (2, [1, 0, 0, 1]),
        (3, [1, 0, 0, 0, 1, 0, 0, 0, 1]),
        (4, [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]),
    ],
)
def test_identity(n: int, nums: list[int]):
    def check_identity(
        int_bits: int | None = None,
        frac_bits: int | None = None,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        force_complex: bool | None = None,
    ):
        a = identity(
            n=n,
            int_bits=int_bits,
            frac_bits=frac_bits,
            exp_bits=exp_bits,
            man_bits=man_bits,
            bias=bias,
            force_complex=force_complex,
        )
        if isinstance(a, APyFixedArray):
            assert int_bits is not None
            assert frac_bits is not None
            b = APyFixedArray.from_float(
                nums, int_bits=int_bits, frac_bits=frac_bits
            ).reshape((n, n))
        elif isinstance(a, APyCFixedArray):
            assert int_bits is not None
            assert frac_bits is not None
            b = APyCFixedArray.from_float(
                nums, int_bits=int_bits, frac_bits=frac_bits
            ).reshape((n, n))
        elif isinstance(a, APyFloatArray):
            assert exp_bits is not None
            assert man_bits is not None
            b = APyFloatArray.from_float(
                nums, exp_bits=exp_bits, man_bits=man_bits, bias=bias
            ).reshape((n, n))
        else:
            assert exp_bits is not None
            assert man_bits is not None
            b = APyCFloatArray.from_float(
                nums, exp_bits=exp_bits, man_bits=man_bits, bias=bias
            ).reshape((n, n))
        assert a.is_identical(b), (
            f"identity on {type(a).__name__} didn't work when n={n}. "
            + f"Expected result was {nums} but got \n {a}"
        )

    # Test cases for APyFixedArray
    check_identity(int_bits=5, frac_bits=5)
    check_identity(int_bits=38231, frac_bits=1237)

    # Test cases for APyCFixedArray
    check_identity(int_bits=5, frac_bits=5, force_complex=True)
    check_identity(int_bits=38231, frac_bits=1237, force_complex=True)

    # Test cases for APyFloatArray
    check_identity(exp_bits=13, man_bits=28)
    check_identity(exp_bits=16, man_bits=5, bias=8)

    # Test cases for APyCFloatArray
    check_identity(exp_bits=13, man_bits=28, force_complex=True)
    check_identity(exp_bits=16, man_bits=5, bias=8, force_complex=True)

    # Test raise
    with pytest.raises(
        ValueError,
        match="identity: could not determine array type",
    ):
        _ = identity(2, int_bits=4, frac_bits=0, exp_bits=15)


@pytest.mark.parametrize(
    "array_type", [APyFixedArray, APyCFixedArray, APyFloatArray, APyCFloatArray]
)
def test_meshgrid(array_type):
    # Test raises
    with pytest.raises(ValueError, match="meshgrid: at least one array is required"):
        _ = meshgrid()

    with pytest.raises(
        ValueError, match="meshgrid: input arrays must have a meshgrid method"
    ):
        _ = meshgrid([1, 2, 3])

    x = range(4)
    if array_type in (APyFixedArray, APyCFixedArray):
        apy_x = array_type.from_float(x, int_bits=5, frac_bits=5)
    else:
        apy_x = array_type.from_float(x, exp_bits=5, man_bits=5)

    with pytest.raises(ValueError, match="meshgrid: input arrays must be of same type"):
        _ = meshgrid(apy_x, [0, 1, 2, 3])

    with pytest.raises(ValueError, match="meshgrid: unknown indexing yx"):
        _ = meshgrid(apy_x, indexing="yx")

    with pytest.raises(
        ValueError, match="meshgrid: all arrays must have the same bit specifiers"
    ):
        _ = meshgrid(apy_x, apy_x.cast(8, 8))

    with pytest.raises(
        ValueError, match="meshgrid: all arrays must be one dimensional"
    ):
        _ = meshgrid(apy_x.reshape((2, 2)))

    # 1-D case
    (xx,) = meshgrid(apy_x)
    assert xx.is_identical(apy_x)

    # Check if arrays are copied
    xx[0] = xx[1]
    assert not xx.is_identical(apy_x)

    # 2-D case
    x = range(4)
    y = range(3, 7)

    if array_type in (APyFixedArray, APyCFixedArray):
        apy_x = array_type.from_float(x, int_bits=5, frac_bits=5)
        apy_y = array_type.from_float(y, int_bits=5, frac_bits=5)
        xx_ref = array_type.from_float([x] * len(y), int_bits=5, frac_bits=5)
        yy_ref = array_type.from_float(
            [[yi] * len(x) for yi in y], int_bits=5, frac_bits=5
        )
    else:
        apy_x = array_type.from_float(x, exp_bits=5, man_bits=5)
        apy_y = array_type.from_float(y, exp_bits=5, man_bits=5)
        xx_ref = array_type.from_float([x] * len(y), exp_bits=5, man_bits=5)
        yy_ref = array_type.from_float(
            [[yi] * len(x) for yi in y], exp_bits=5, man_bits=5
        )

    xx, yy = meshgrid(apy_x, apy_y)
    assert xx.is_identical(xx_ref)
    assert yy.is_identical(yy_ref)

    # Test N-D
    if array_type in (APyFixedArray, APyCFixedArray):
        apy_x = array_type.from_float(range(2), int_bits=5, frac_bits=5)
        apy_y = array_type.from_float(range(2, 3), int_bits=5, frac_bits=5)
        apy_z = array_type.from_float(range(3, 6), int_bits=5, frac_bits=5)
        apy_w = array_type.from_float(range(6, 8), int_bits=5, frac_bits=5)
    else:
        apy_x = array_type.from_float(range(2), exp_bits=5, man_bits=5)
        apy_y = array_type.from_float(range(2, 3), exp_bits=5, man_bits=5)
        apy_z = array_type.from_float(range(3, 6), exp_bits=5, man_bits=5)
        apy_w = array_type.from_float(range(6, 8), exp_bits=5, man_bits=5)

    xx, yy, zz, ww = meshgrid(apy_x, apy_y, apy_z, apy_w)
    # First make sure the shapes are correct
    assert xx.shape == (1, 2, 3, 2)
    assert yy.shape == (1, 2, 3, 2)
    assert zz.shape == (1, 2, 3, 2)
    assert ww.shape == (1, 2, 3, 2)

    xx_ref = [[[[0, 0], [0, 0], [0, 0]], [[1, 1], [1, 1], [1, 1]]]]
    yy_ref = [[[[2, 2], [2, 2], [2, 2]], [[2, 2], [2, 2], [2, 2]]]]
    zz_ref = [[[[3, 3], [4, 4], [5, 5]], [[3, 3], [4, 4], [5, 5]]]]
    ww_ref = [[[[6, 7], [6, 7], [6, 7]], [[6, 7], [6, 7], [6, 7]]]]
    if array_type in (APyFixedArray, APyCFixedArray):
        xx_ref = array_type.from_float(xx_ref, int_bits=5, frac_bits=5)
        yy_ref = array_type.from_float(yy_ref, int_bits=5, frac_bits=5)
        zz_ref = array_type.from_float(zz_ref, int_bits=5, frac_bits=5)
        ww_ref = array_type.from_float(ww_ref, int_bits=5, frac_bits=5)
    else:
        xx_ref = array_type.from_float(xx_ref, exp_bits=5, man_bits=5)
        yy_ref = array_type.from_float(yy_ref, exp_bits=5, man_bits=5)
        zz_ref = array_type.from_float(zz_ref, exp_bits=5, man_bits=5)
        ww_ref = array_type.from_float(ww_ref, exp_bits=5, man_bits=5)
    assert xx.is_identical(xx_ref)
    assert yy.is_identical(yy_ref)
    assert zz.is_identical(zz_ref)
    assert ww.is_identical(ww_ref)

    # 1-D case, ij indexing
    x = range(3)
    if array_type in (APyFixedArray, APyCFixedArray):
        apy_x = array_type.from_float(x, int_bits=5, frac_bits=5)
    else:
        apy_x = array_type.from_float(x, exp_bits=5, man_bits=5)

    (xx,) = meshgrid(apy_x, indexing="ij")
    assert xx.is_identical(apy_x)

    # 2-D case, ij indexing
    x = range(4)
    y = range(3, 7)

    if array_type in (APyFixedArray, APyCFixedArray):
        apy_x = array_type.from_float(x, int_bits=5, frac_bits=5)
        apy_y = array_type.from_float(y, int_bits=5, frac_bits=5)
        xx_ref = array_type.from_float(
            [[xi] * len(y) for xi in x], int_bits=5, frac_bits=5
        )
        yy_ref = array_type.from_float([y] * len(x), int_bits=5, frac_bits=5)
    else:
        apy_x = array_type.from_float(x, exp_bits=5, man_bits=5)
        apy_y = array_type.from_float(y, exp_bits=5, man_bits=5)
        xx_ref = array_type.from_float(
            [[xi] * len(y) for xi in x], exp_bits=5, man_bits=5
        )
        yy_ref = array_type.from_float([y] * len(x), exp_bits=5, man_bits=5)

    xx, yy = meshgrid(apy_x, apy_y, indexing="ij")
    assert xx.is_identical(xx_ref)
    assert yy.is_identical(yy_ref)

    # 3-D case, ij indexing
    if array_type in (APyFixedArray, APyCFixedArray):
        apy_x = array_type.from_float(range(2), int_bits=5, frac_bits=5)
        apy_y = array_type.from_float(range(2, 3), int_bits=5, frac_bits=5)
        apy_z = array_type.from_float(range(3, 6), int_bits=5, frac_bits=5)
        apy_w = array_type.from_float(range(6, 8), int_bits=5, frac_bits=5)
    else:
        apy_x = array_type.from_float(range(2), exp_bits=5, man_bits=5)
        apy_y = array_type.from_float(range(2, 3), exp_bits=5, man_bits=5)
        apy_z = array_type.from_float(range(3, 6), exp_bits=5, man_bits=5)
        apy_w = array_type.from_float(range(6, 8), exp_bits=5, man_bits=5)

    xx, yy, zz, ww = meshgrid(apy_x, apy_y, apy_z, apy_w, indexing="ij")
    # First make sure the shapes are correct
    assert xx.shape == (2, 1, 3, 2)
    assert yy.shape == (2, 1, 3, 2)
    assert zz.shape == (2, 1, 3, 2)
    assert ww.shape == (2, 1, 3, 2)

    xx_ref = [[[[0, 0], [0, 0], [0, 0]]], [[[1, 1], [1, 1], [1, 1]]]]
    yy_ref = [[[[2, 2], [2, 2], [2, 2]]], [[[2, 2], [2, 2], [2, 2]]]]
    zz_ref = [[[[3, 3], [4, 4], [5, 5]]], [[[3, 3], [4, 4], [5, 5]]]]
    ww_ref = [[[[6, 7], [6, 7], [6, 7]]], [[[6, 7], [6, 7], [6, 7]]]]
    if array_type in (APyFixedArray, APyCFixedArray):
        xx_ref = array_type.from_float(xx_ref, int_bits=5, frac_bits=5)
        yy_ref = array_type.from_float(yy_ref, int_bits=5, frac_bits=5)
        zz_ref = array_type.from_float(zz_ref, int_bits=5, frac_bits=5)
        ww_ref = array_type.from_float(ww_ref, int_bits=5, frac_bits=5)
    else:
        xx_ref = array_type.from_float(xx_ref, exp_bits=5, man_bits=5)
        yy_ref = array_type.from_float(yy_ref, exp_bits=5, man_bits=5)
        zz_ref = array_type.from_float(zz_ref, exp_bits=5, man_bits=5)
        ww_ref = array_type.from_float(ww_ref, exp_bits=5, man_bits=5)
    assert xx.is_identical(xx_ref)
    assert yy.is_identical(yy_ref)
    assert zz.is_identical(zz_ref)
    assert ww.is_identical(ww_ref)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_zeros(shape: tuple[int, int]):
    def check_zeros(
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        force_complex: bool | None = None,
    ):
        a = zeros(
            shape,
            int_bits=int_bits,
            frac_bits=frac_bits,
            bits=bits,
            exp_bits=exp_bits,
            man_bits=man_bits,
            bias=bias,
            force_complex=force_complex,
        )
        if isinstance(a, APyFixedArray):
            assert int_bits is not None
            assert frac_bits is not None
            b = APyFixedArray.from_float(
                [0] * (shape[0] * shape[1]),
                int_bits=int_bits,
                frac_bits=frac_bits,
                bits=bits,
            ).reshape(shape)
        elif isinstance(a, APyCFixedArray):
            assert int_bits is not None
            assert frac_bits is not None
            b = APyCFixedArray.from_float(
                [0] * (shape[0] * shape[1]),
                int_bits=int_bits,
                frac_bits=frac_bits,
                bits=bits,
            ).reshape(shape)
        elif isinstance(a, APyFloatArray):
            assert exp_bits is not None
            assert man_bits is not None
            b = APyFloatArray.from_float(
                [0] * (shape[0] * shape[1]),
                exp_bits=exp_bits,
                man_bits=man_bits,
                bias=bias,
            ).reshape(shape)
        else:
            assert exp_bits is not None
            assert man_bits is not None
            b = APyCFloatArray.from_float(
                [0] * (shape[0] * shape[1]),
                exp_bits=exp_bits,
                man_bits=man_bits,
                bias=bias,
            ).reshape(shape)

        assert a.is_identical(b), (
            f"zeros on {type(a).__name__} didn't work when shape={shape}."
        )

    # APyFixedArray
    check_zeros(int_bits=5, frac_bits=5)
    check_zeros(int_bits=12314, frac_bits=14146)
    check_zeros(int_bits=0, frac_bits=5)

    # APyCFixedArray
    check_zeros(int_bits=5, frac_bits=5, force_complex=True)
    check_zeros(int_bits=12314, frac_bits=14146, force_complex=True)
    check_zeros(int_bits=0, frac_bits=5, force_complex=True)

    # APyFloatArray
    check_zeros(exp_bits=13, man_bits=28)
    check_zeros(exp_bits=13, man_bits=0)
    check_zeros(exp_bits=16, man_bits=5, bias=8)

    # APyCFloatArray
    check_zeros(exp_bits=13, man_bits=28, force_complex=True)
    check_zeros(exp_bits=13, man_bits=0, force_complex=True)
    check_zeros(exp_bits=16, man_bits=5, bias=8, force_complex=True)

    # Test raise
    with pytest.raises(ValueError, match="zeros: could not determine array type"):
        _ = zeros((2, 2), int_bits=4, frac_bits=0, exp_bits=15)


def test_tuple_construction_raises():
    with pytest.raises(
        ValueError,
        match=r"cpp_shape_from_python_shape_like\(\): negative integers disallowed",
    ):
        _ = zeros((4, -2), int_bits=2, frac_bits=2)

    with pytest.raises(
        ValueError, match=r"cpp_shape_from_python_shape_like\(\): integer too large"
    ):
        _ = zeros((111111111111111111111111111111111111, -2), int_bits=2, frac_bits=2)

    with pytest.raises(
        ValueError,
        match=r"cpp_shape_from_python_shape_like\(\): only integer dimensions allowed",
    ):
        _ = zeros((2.5, -2), int_bits=2, frac_bits=2)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_zeros_like(shape: tuple[int, int]):
    def check_zeros_like(
        ArrayType: type[APyArray],
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
    ):
        if ArrayType in (APyFixedArray, APyCFixedArray):
            b = ArrayType.from_float(
                [0] * (shape[0] * shape[1]),
                int_bits=int_bits,
                frac_bits=frac_bits,
                bits=bits,
            ).reshape(shape)
            a = zeros_like(b, int_bits=int_bits, frac_bits=frac_bits, bits=bits)
        elif ArrayType in (APyFloatArray, APyCFloatArray):
            assert exp_bits is not None
            assert man_bits is not None
            b = ArrayType.from_float(
                [0] * (shape[0] * shape[1]),
                exp_bits=exp_bits,
                man_bits=man_bits,
                bias=bias,
            ).reshape(shape)
            a = zeros_like(b, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
        else:
            raise ValueError("This shouldn't happen...")

        assert a.is_identical(b), (
            f"zeros_like on {ArrayType.__name__} didn't work when shape={shape}."
        )

        c = zeros_like(b)
        assert c.is_identical(b), (
            f"zeros_like on {ArrayType.__name__} didn't work when shape={shape}."
        )

    # Test cases for APyFixedArray
    check_zeros_like(APyFixedArray, bits=10, frac_bits=5)
    check_zeros_like(APyFixedArray, bits=10, int_bits=5)
    check_zeros_like(APyFixedArray, int_bits=12314, frac_bits=1832)

    # Test cases for APyFloatArray
    check_zeros_like(APyFloatArray, exp_bits=13, man_bits=28)
    check_zeros_like(APyFloatArray, exp_bits=16, man_bits=5, bias=8)

    # Test cases for APyCFixedArray
    check_zeros_like(APyCFixedArray, bits=10, frac_bits=5)
    check_zeros_like(APyCFixedArray, bits=10, int_bits=5)
    check_zeros_like(APyCFixedArray, int_bits=12314, frac_bits=1832)

    # Test cases for APyCFloatArray
    check_zeros_like(APyCFloatArray, exp_bits=13, man_bits=28)
    check_zeros_like(APyCFloatArray, exp_bits=16, man_bits=5, bias=8)


def test_zeros_like_raises():
    a = zeros((5, 5), int_bits=10, frac_bits=10)
    with pytest.raises(ValueError, match=r"Fixed-point bit specification needs exact"):
        _ = zeros_like(a, int_bits=2)
    with pytest.raises(ValueError, match=r"Fixed-point bit specification needs exact"):
        _ = zeros_like(a, bits=1000, int_bits=2, frac_bits=2)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_ones(shape: tuple[int, int]):
    def check_ones(
        int_bits: int | None = None,
        frac_bits: int | None = None,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        force_complex: bool | None = None,
    ):
        a = ones(
            shape,
            int_bits=int_bits,
            frac_bits=frac_bits,
            exp_bits=exp_bits,
            man_bits=man_bits,
            bias=bias,
            force_complex=force_complex,
        )
        if isinstance(a, APyFixedArray | APyCFixedArray):
            b = (
                type(a)
                .from_float(
                    [1] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
                )
                .reshape(shape)
            )
        elif isinstance(a, APyFloatArray | APyCFloatArray):
            assert exp_bits is not None
            assert man_bits is not None
            b = (
                type(a)
                .from_float(
                    [1] * (shape[0] * shape[1]),
                    exp_bits=exp_bits,
                    man_bits=man_bits,
                    bias=bias,
                )
                .reshape(shape)
            )
        else:
            raise ValueError("This should not happen...")
        assert a.is_identical(b), (
            f"ones on {type(a).__name__} didn't work when shape={shape}."
        )

    # Test cases for APyFixedArray
    check_ones(int_bits=5, frac_bits=5)
    check_ones(int_bits=12314, frac_bits=1832)

    # Test cases for APyCFixedArray
    check_ones(int_bits=5, frac_bits=5, force_complex=True)
    check_ones(int_bits=12314, frac_bits=1832, force_complex=True)

    # Test cases for APyFloatArray
    check_ones(exp_bits=13, man_bits=28)
    check_ones(exp_bits=16, man_bits=5, bias=8)

    # Test cases for APyCFloatArray
    check_ones(exp_bits=13, man_bits=28, force_complex=True)
    check_ones(exp_bits=16, man_bits=5, bias=8, force_complex=True)

    # Test raise
    with pytest.raises(
        ValueError,
        match="ones: could not determine array type",
    ):
        _ = ones((2, 2), int_bits=4, frac_bits=0, exp_bits=15)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_ones_like(shape: tuple[int, int]):
    def check_ones_like(
        ArrayType: type[APyArray],
        int_bits: int | None = None,
        frac_bits: int | None = None,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
    ):
        if ArrayType in (APyFixedArray, APyCFixedArray):
            b = ArrayType.from_float(
                [1] * (shape[0] * shape[1]), int_bits=int_bits, frac_bits=frac_bits
            ).reshape(shape)
            a = ones_like(b, int_bits=int_bits, frac_bits=frac_bits)
        elif ArrayType in (APyFloatArray, APyCFloatArray):
            assert exp_bits is not None
            assert man_bits is not None
            b = ArrayType.from_float(
                [1] * (shape[0] * shape[1]),
                exp_bits=exp_bits,
                man_bits=man_bits,
                bias=bias,
            ).reshape(shape)
            a = ones_like(b, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
        else:
            raise ValueError("This should not happen...")

        assert a.is_identical(b), (
            f"ones_like on {ArrayType.__name__} didn't work when shape={shape}."
        )
        c = ones_like(b)
        assert c.is_identical(b), (
            f"ones_like on {ArrayType.__name__} didn't work when shape={shape}."
        )

    # Test cases for APyFixedArray
    check_ones_like(APyFixedArray, int_bits=5, frac_bits=5)
    check_ones_like(APyFixedArray, int_bits=12314, frac_bits=1832)

    # Test cases for APyFloatArray
    check_ones_like(APyFloatArray, exp_bits=13, man_bits=28)
    check_ones_like(APyFloatArray, exp_bits=16, man_bits=5, bias=8)

    # Test cases for APyCFixedArray
    check_ones_like(APyCFixedArray, int_bits=5, frac_bits=5)
    check_ones_like(APyCFixedArray, int_bits=12314, frac_bits=1832)

    # Test cases for APyCFloatArray
    check_ones_like(APyCFloatArray, exp_bits=13, man_bits=28)
    check_ones_like(APyCFloatArray, exp_bits=16, man_bits=5, bias=8)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_full(shape: tuple[int, int]):
    def check_full(
        ArrayType: type[APyArray],
        fill_value: int,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
    ):
        if ArrayType in (APyFixedArray, APyCFixedArray):
            ScalarType = APyCFixed if ArrayType is APyCFixedArray else APyFixed
            b = ArrayType.from_float(
                [fill_value] * (shape[0] * shape[1]),
                int_bits=int_bits,
                frac_bits=frac_bits,
            ).reshape(shape)
            num = ScalarType.from_float(
                fill_value, int_bits=int_bits, frac_bits=frac_bits
            )
            a = full(shape, num, int_bits=int_bits, frac_bits=frac_bits)
        elif ArrayType in (APyFloatArray, APyCFloatArray):
            ScalarType = APyCFloat if ArrayType is APyCFloatArray else APyFloat
            assert exp_bits is not None
            assert man_bits is not None
            b = ArrayType.from_float(
                [fill_value] * (shape[0] * shape[1]),
                exp_bits=exp_bits,
                man_bits=man_bits,
                bias=bias,
            ).reshape(shape)
            num = ScalarType.from_float(
                fill_value, exp_bits=exp_bits, man_bits=man_bits, bias=bias
            )
            a = full(shape, num, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
        else:
            raise ValueError("This should not happen...")
        assert a.is_identical(b), (
            f"full on {ArrayType.__name__} didn't work when shape={shape}."
        )

    # Test cases for APyFixedArray
    check_full(APyFixedArray, shape[0] + shape[1], int_bits=5, frac_bits=5)
    check_full(APyFixedArray, shape[0] + shape[1], int_bits=12314, frac_bits=1832)
    check_full(APyFixedArray, shape[0] + shape[1], int_bits=0, frac_bits=1832)

    # Test cases for APyFloatArray
    check_full(APyFloatArray, shape[0] + shape[1], exp_bits=13, man_bits=28)
    check_full(APyFloatArray, shape[0] + shape[1], exp_bits=16, man_bits=5, bias=8)
    check_full(APyFloatArray, fill_value=shape[0] + shape[1], exp_bits=0, man_bits=5)

    # Test cases for APyCFixedArray
    check_full(APyCFixedArray, shape[0] + shape[1], int_bits=5, frac_bits=5)
    check_full(APyCFixedArray, shape[0] + shape[1], int_bits=12314, frac_bits=1832)
    check_full(APyCFixedArray, shape[0] + shape[1], int_bits=0, frac_bits=1832)

    # Test cases for APyCFloatArray
    check_full(APyCFloatArray, shape[0] + shape[1], exp_bits=13, man_bits=28)
    check_full(APyCFloatArray, shape[0] + shape[1], exp_bits=16, man_bits=5, bias=8)
    check_full(APyCFloatArray, fill_value=shape[0] + shape[1], exp_bits=0, man_bits=5)


@pytest.mark.parametrize("shape", [(i, j) for i in range(1, 5) for j in range(1, 5)])
def test_full_like(shape: tuple[int, int]):
    def check_full_like(
        ArrayType: type[APyArray],
        fill_value: int,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
    ):
        if ArrayType in (APyFixedArray, APyCFixedArray):
            ScalarType = APyCFixed if ArrayType is APyCFixedArray else APyFixed
            b = ArrayType.from_float(
                [fill_value] * (shape[0] * shape[1]),
                int_bits=int_bits,
                frac_bits=frac_bits,
            ).reshape(shape)
            num = ScalarType.from_float(
                fill_value, int_bits=int_bits, frac_bits=frac_bits
            )
            a = full_like(b, num, int_bits=int_bits, frac_bits=frac_bits)
            assert a.is_identical(b), (
                f"full_like on {ArrayType.__name__} didn't work when shape={shape}."
            )
            a = full_like(b, fill_value, int_bits=int_bits, frac_bits=frac_bits)
            assert a.is_identical(b), (
                f"full_like on {ArrayType.__name__} didn't work when shape={shape}."
            )
        elif ArrayType in (APyFloatArray, APyCFloatArray):
            ScalarType = APyCFloat if ArrayType is APyCFloatArray else APyFloat
            assert exp_bits is not None
            assert man_bits is not None
            b = ArrayType.from_float(
                [fill_value] * (shape[0] * shape[1]),
                exp_bits=exp_bits,
                man_bits=man_bits,
                bias=bias,
            ).reshape(shape)
            num = ScalarType.from_float(
                fill_value, exp_bits=exp_bits, man_bits=man_bits, bias=bias
            )
            a = full_like(b, num, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
            assert a.is_identical(b), (
                f"full_like on {ArrayType.__name__} didn't work when shape={shape}."
            )
            a = full_like(
                b, fill_value, exp_bits=exp_bits, man_bits=man_bits, bias=bias
            )
            assert a.is_identical(b), (
                f"full_like on {ArrayType.__name__} didn't work when shape={shape}."
            )
        else:
            raise ValueError("This should never happen...")

    # Test cases for APyFixedArray
    check_full_like(APyFixedArray, shape[0] + shape[1], int_bits=5, frac_bits=5)
    check_full_like(APyFixedArray, shape[0] + shape[1], int_bits=12314, frac_bits=1832)
    check_full_like(APyFixedArray, shape[0] + shape[1], int_bits=0, frac_bits=5)
    check_full_like(APyFixedArray, shape[0] + shape[1], int_bits=5, frac_bits=0)

    # Test cases for APyCFixedArray
    check_full_like(APyCFixedArray, shape[0] + shape[1], int_bits=5, frac_bits=5)
    check_full_like(APyCFixedArray, shape[0] + shape[1], int_bits=12314, frac_bits=1832)
    check_full_like(APyCFixedArray, shape[0] + shape[1], int_bits=0, frac_bits=5)
    check_full_like(APyCFixedArray, shape[0] + shape[1], int_bits=5, frac_bits=0)

    # Test cases for APyFloatArray
    check_full_like(APyFloatArray, shape[0] + shape[1], exp_bits=13, man_bits=28)
    check_full_like(APyFloatArray, shape[0] + shape[1], exp_bits=16, man_bits=5, bias=8)
    check_full_like(APyFloatArray, shape[0] + shape[1], exp_bits=16, man_bits=0)

    # Test cases for APyCFloatArray
    check_full_like(APyCFloatArray, shape[0] + shape[1], exp_bits=13, man_bits=28)
    check_full_like(
        APyCFloatArray, shape[0] + shape[1], exp_bits=16, man_bits=5, bias=8
    )
    check_full_like(APyCFloatArray, shape[0] + shape[1], exp_bits=16, man_bits=0)


def test_arange():
    # Test raises
    with pytest.raises(ValueError, match="Step size cannot be zero"):
        _ = arange(0, 10, 0, int_bits=4, frac_bits=0)

    with pytest.raises(ValueError, match="Undefined input"):
        _ = arange(10, 2, 1, int_bits=4, frac_bits=0)

    with pytest.raises(ValueError, match="Undefined input"):
        _ = arange(2, 10, -1, int_bits=4, frac_bits=0)

    with pytest.raises(ValueError, match="Cannot convert"):
        _ = arange(0, float("inf"), 2**5, exp_bits=5, man_bits=10)

    with pytest.raises(ValueError, match="Cannot convert"):
        _ = arange(float("nan"), 10, 2**5, exp_bits=5, man_bits=10)

    with pytest.raises(ValueError, match="Cannot convert"):
        _ = arange(0, 10, float("inf"), exp_bits=5, man_bits=10)

    with pytest.raises(ValueError, match="Non supported type"):
        _ = arange("0", 10, 1, exp_bits=5, man_bits=10)

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = arange(0, 10, 1)

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = arange(0, 10, 1, exp_bits=5, man_bits=10, bits=10, int_bits=10)

    with pytest.raises(ValueError, match="Could not determine bit specifiers"):
        _ = arange(APyFixed(1, 4, 0), 10, APyFixed(1, 4, 1))

    with pytest.raises(ValueError, match="Could not determine bit specifiers"):
        _ = arange(APyFloat(0, 0, 0, 4, 4), APyFloat(0, 7, 0, 4, 3))

    with pytest.raises(ValueError, match="Could not determine bit specifiers"):
        _ = arange(
            APyFloat(0, 0, 0, 4, 4), APyFloat(0, 7, 0, 4, 4), APyFloat(0, 7, 0, 4, 4, 5)
        )

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = arange(APyFloat(0, 0, 0, 4, 4), APyFloat(0, 7, 0, 4, 4), APyFixed(1, 4, 0))

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = arange(APyFixed(1, 4, 0), 10, APyFloat(0, 7, 0, 4, 3))

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = arange(10, exp_bits=5, man_bits=10, bits=20)

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = arange(10, int_bits=5, frac_bits=10, bias=15)

    with pytest.raises(
        ValueError,
        match="Fixed-point bit specification needs exactly two of three bit specifiers",
    ):
        _ = arange(
            APyFixed(0, int_bits=4, frac_bits=0),
            APyFixed(10, int_bits=4, frac_bits=0),
            bits=20,
        )

    with pytest.raises(
        ValueError,
        match="Fixed-point bit specification needs exactly two of three bit specifiers",
    ):
        _ = arange(
            APyFixed(0, int_bits=4, frac_bits=0),
            APyFixed(10, int_bits=4, frac_bits=0),
            bits=20,
        )

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

    # Test deducing the array type when one input is given
    a = arange(APyFixed.from_float(4, int_bits=5, frac_bits=0))
    assert a.is_identical(APyFixedArray.from_float(range(4), int_bits=5, frac_bits=0))

    a = arange(APyFloat.from_float(4, exp_bits=5, man_bits=4, bias=8))
    assert a.is_identical(
        APyFloatArray.from_float(range(4), exp_bits=5, man_bits=4, bias=8)
    )

    a = arange(0, APyFloat(0, 9, 0, 4, 3), bits=5, int_bits=4)
    assert a.is_identical(APyFixedArray([0, 2, 4, 6], bits=5, int_bits=4))

    # Test deducing the array type when two inputs are given
    a = arange(0, APyFixed.from_float(4, int_bits=5, frac_bits=0))
    assert a.is_identical(APyFixedArray.from_float(range(4), int_bits=5, frac_bits=0))

    a = arange(0, APyFloat.from_float(4, exp_bits=5, man_bits=4, bias=8))
    assert a.is_identical(
        APyFloatArray.from_float(range(4), exp_bits=5, man_bits=4, bias=8)
    )

    a = arange(
        APyFixed(0, int_bits=5, frac_bits=0),
        APyFixed.from_float(4, int_bits=5, frac_bits=0),
    )
    assert a.is_identical(APyFixedArray.from_float(range(4), int_bits=5, frac_bits=0))

    a = arange(
        APyFloat(0, 0, 0, exp_bits=5, man_bits=4, bias=8),
        APyFloat.from_float(4, exp_bits=5, man_bits=4, bias=8),
    )
    assert a.is_identical(
        APyFloatArray.from_float(range(4), exp_bits=5, man_bits=4, bias=8)
    )

    a = arange(APyFixed.from_float(0, int_bits=5, frac_bits=0), 4)
    assert a.is_identical(APyFixedArray.from_float(range(4), int_bits=5, frac_bits=0))

    a = arange(APyFloat.from_float(0, exp_bits=5, man_bits=4, bias=8), 4)
    assert a.is_identical(
        APyFloatArray.from_float(range(4), exp_bits=5, man_bits=4, bias=8)
    )

    # Test deducing the array type when three inputs are given
    a = arange(0, 4, APyFixed.from_float(1, int_bits=5, frac_bits=0))
    assert a.is_identical(APyFixedArray.from_float(range(4), int_bits=5, frac_bits=0))

    a = arange(0, 4, APyFloat.from_float(1, exp_bits=5, man_bits=4, bias=8))
    assert a.is_identical(
        APyFloatArray.from_float(range(4), exp_bits=5, man_bits=4, bias=8)
    )

    a = arange(0, APyFixed.from_float(4, int_bits=5, frac_bits=0), 1)
    assert a.is_identical(APyFixedArray.from_float(range(4), int_bits=5, frac_bits=0))

    a = arange(0, APyFloat.from_float(4, exp_bits=5, man_bits=4, bias=8), 1)
    assert a.is_identical(
        APyFloatArray.from_float(range(4), exp_bits=5, man_bits=4, bias=8)
    )

    a = arange(APyFixed.from_float(0, int_bits=5, frac_bits=0), 4, 1)
    assert a.is_identical(APyFixedArray.from_float(range(4), int_bits=5, frac_bits=0))

    a = arange(APyFloat.from_float(0, exp_bits=5, man_bits=4, bias=8), 4, 1)
    assert a.is_identical(
        APyFloatArray.from_float(range(4), exp_bits=5, man_bits=4, bias=8)
    )

    a = arange(
        APyFixed(0, int_bits=5, frac_bits=0),
        APyFixed(4, int_bits=5, frac_bits=0),
        APyFixed(1, int_bits=5, frac_bits=0),
    )
    assert a.is_identical(APyFixedArray.from_float(range(4), int_bits=5, frac_bits=0))

    a = arange(
        APyFloat(0, 0, 0, exp_bits=5, man_bits=4, bias=8),
        APyFloat.from_float(4, exp_bits=5, man_bits=4, bias=8),
        APyFloat(0, 8, 0, exp_bits=5, man_bits=4, bias=8),
    )
    assert a.is_identical(
        APyFloatArray.from_float(range(4), exp_bits=5, man_bits=4, bias=8)
    )

    # Test deducing bit-specifiers when only some are given
    a = arange(0, 4, APyFixed(4, int_bits=2, frac_bits=2), int_bits=3)
    assert a.is_identical(APyFixedArray([0, 4, 8, 12], int_bits=3, frac_bits=2))

    a = arange(
        APyFixed(0, int_bits=2, frac_bits=2),
        4,
        APyFloat(0, 15, 0, exp_bits=5, man_bits=2),
        man_bits=3,
    )
    assert a.is_identical(APyFloatArray.from_float(range(4), exp_bits=5, man_bits=3))

    a = arange(
        APyFixed(0, int_bits=2, frac_bits=2),
        4,
        APyFloat(0, 15, 0, exp_bits=5, man_bits=2),
        man_bits=3,
        bias=5,
    )
    assert a.is_identical(
        APyFloatArray.from_float(range(4), exp_bits=5, man_bits=3, bias=5)
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


def test_fullrange():
    # Test raises
    with pytest.raises(ValueError, match="Cannot convert"):
        _ = fullrange(0, float("inf"), bits=10, int_bits=5)

    with pytest.raises(ValueError, match="Cannot convert"):
        _ = fullrange(float("nan"), 10, bits=10, int_bits=5)

    with pytest.raises(ValueError, match="Non supported type"):
        _ = fullrange("0", 10, exp_bits=5, man_bits=10)

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = fullrange(0, 10)

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = fullrange(0, 10, exp_bits=5, man_bits=10, bits=10, int_bits=10)

    with pytest.raises(ValueError, match="Could not determine bit specifiers"):
        _ = fullrange(APyFixed(1, 4, 0), APyFixed(1, 4, 1))

    with pytest.raises(ValueError, match="Could not determine bit specifiers"):
        _ = fullrange(APyFloat(0, 0, 0, 4, 4), APyFloat(0, 7, 0, 4, 3))

    with pytest.raises(ValueError, match="Could not determine bit specifiers"):
        _ = fullrange(APyFloat(0, 0, 0, 4, 4), APyFloat(0, 7, 0, 5, 3))

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = fullrange(APyFloat(0, 0, 0, 4, 4), APyFixed(1, 4, 0))

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = fullrange(APyFixed(1, 4, 0), APyFloat(0, 7, 0, 4, 3))

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = fullrange(10, exp_bits=5, man_bits=10, bits=20)

    with pytest.raises(ValueError, match="Could not determine array type"):
        _ = fullrange(10, int_bits=5, frac_bits=10, bias=15)

    with pytest.raises(
        ValueError, match="APyFloatArray.fullrange: start or stop is NaN"
    ):
        _ = fullrange(0, float("nan"), exp_bits=5, man_bits=10)

    with pytest.raises(
        ValueError, match="APyFloatArray.fullrange: start or stop is NaN"
    ):
        _ = fullrange(float("nan"), 1, exp_bits=5, man_bits=10)

    with pytest.raises(
        ValueError,
        match="Fixed-point bit specification needs exactly two of three bit specifiers",
    ):
        _ = fullrange(
            APyFixed(0, int_bits=4, frac_bits=0),
            APyFixed(10, int_bits=4, frac_bits=0),
            bits=20,
        )

    with pytest.raises(
        ValueError,
        match="Fixed-point bit specification needs exactly two of three bit specifiers",
    ):
        _ = fullrange(
            APyFixed(0, int_bits=4, frac_bits=0),
            APyFixed(10, int_bits=4, frac_bits=0),
            bits=20,
        )

    # Test empty range, and test with APyFixed
    start, stop = 3, APyFixed(1, 5, 2)
    a = fullrange(start, stop, exp_bits=4, man_bits=3, bias=8)
    assert a.is_identical(APyFloatArray([], [], [], 4, 3, 8))

    # Test when both start and stop are positive
    start, stop = APyFloat.from_float(1, 4, 3), APyFloat.from_float(2, 4, 3)
    a = fullrange(start, stop)
    assert a.is_identical(
        APyFloatArray.from_bits(range(start.to_bits(), stop.to_bits()), 4, 3)
    )

    # Test with one argument
    start, stop = APyFloat(0, 0, 0, 4, 3), APyFloat.from_float(2, 4, 3)
    a = fullrange(stop)
    assert a.is_identical(
        APyFloatArray.from_bits(range(start.to_bits(), stop.to_bits()), 4, 3)
    )

    # Large exponent range, including subnormals
    start, stop = APyFloat.from_float(0, 4, 3), APyFloat.from_float(180, 4, 3)
    a = fullrange(start, stop)
    assert a.is_identical(
        APyFloatArray.from_bits(range(start.to_bits(), stop.to_bits()), 4, 3)
    )

    # Test with infinity
    start, stop = (
        APyFloat.from_float(180, 4, 3),
        APyFloat.from_float(float("inf"), 4, 3),
    )
    a = fullrange(start, stop)
    assert a[-1].is_finite
    assert a.is_identical(
        APyFloatArray.from_bits(range(start.to_bits(), stop.to_bits()), 4, 3)
    )

    # Test when both start and stop are negative
    start, stop = APyFloat.from_float(-16, 4, 2), APyFloat.from_float(-0.5, 4, 2)
    a = fullrange(start, stop)
    assert a.is_identical(
        APyFloatArray.from_bits(range(start.to_bits(), stop.to_bits(), -1), 4, 2)
    )

    # Test when both start and stop are negative, include negative zero
    start, stop = APyFloat.from_float(-1, 4, 2), APyFloat.from_float(0, 4, 2)
    a = fullrange(start, stop)
    assert a[-1].is_identical(APyFloat(1, 0, 0, 4, 2))
    bits = list(range(start.to_bits(), APyFloat(1, 0, 0, 4, 2).to_bits() - 1, -1))
    assert a.is_identical(APyFloatArray.from_bits(bits, 4, 2))

    # Test with infinity
    start, stop = (
        APyFloat.from_float(float("-inf"), 4, 2),
        APyFloat.from_float(-100, 4, 2),
    )
    a = fullrange(start, stop)
    assert a[0].is_identical(APyFloat(1, 15, 0, 4, 2))
    assert a.is_identical(
        APyFloatArray.from_bits(range(start.to_bits(), stop.to_bits(), -1), 4, 2)
    )

    # Test with negative start and positive stop
    start, stop = APyFloat.from_float(-2, 4, 2), APyFloat.from_float(1, 4, 2)
    a = fullrange(start, stop)
    bits = list(
        range(start.to_bits(), APyFloat(1, 0, 0, 4, 2).to_bits() - 1, -1)
    ) + list(range(0, stop.to_bits()))
    assert a.is_identical(APyFloatArray.from_bits(bits, 4, 2))

    # Similar tests for APyFixedArray

    # Test empty range, and test with APyFloat
    start, stop = 3, APyFloat.from_float(1, 5, 2)
    a = fullrange(start, stop, int_bits=5, frac_bits=2)
    assert a.is_identical(APyFixedArray([], 5, 2))

    # Test when both start and stop are positive
    start, stop = APyFixed.from_float(1, 5, 2), APyFixed.from_float(3, 5, 2)
    a = fullrange(start, stop)
    assert a.is_identical(APyFixedArray(range(start.to_bits(), stop.to_bits()), 5, 2))

    # Test with one argument
    start, stop = APyFixed(0, 5, 2), APyFixed.from_float(2, 5, 2)
    a = fullrange(stop)
    assert a.is_identical(APyFixedArray(range(start.to_bits(), stop.to_bits()), 5, 2))

    # Test when both start and stop are negative
    start, stop = APyFixed.from_float(-8, 5, 2), APyFixed.from_float(-0.5, 5, 2)
    a = fullrange(start, stop)
    assert a.is_identical(APyFixedArray(range(start.to_bits(), stop.to_bits()), 5, 2))

    # Test with negative start and positive stop
    start, stop = APyFixed.from_float(-3.5, 5, 2), APyFixed.from_float(-2, 5, 2)
    a = fullrange(start, stop)
    assert a.is_identical(APyFixedArray(range(start.to_bits(), stop.to_bits()), 5, 2))

    # Test with wraparound
    start, stop = APyFixed(128, 5, 2), APyFixed(133, 5, 2)
    a = fullrange(start, stop)
    assert a.is_identical(APyFixedArray(range(0, 5), 5, 2))
