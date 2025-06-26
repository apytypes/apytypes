from typing import cast

import pytest

from apytypes import APyFloatArray


def is_repr_identical(a: APyFloatArray):
    """
    Tests that an :class:`APyFixed` is identical to its representation.
    """
    return a.is_identical(cast(APyFloatArray, eval(repr(a))))


@pytest.mark.float_array
def test_repr():
    arr = APyFloatArray([], [], [], 6, 7)
    assert is_repr_identical(arr)
    assert repr(arr) == "APyFloatArray([], [], [], exp_bits=6, man_bits=7)"

    arr = APyFloatArray([1], [5], [3], 5, 2)
    assert is_repr_identical(arr)
    assert repr(arr) == "APyFloatArray([1], [5], [3], exp_bits=5, man_bits=2)"

    arr = APyFloatArray([1, 0], [5, 10], [3, 1], 5, 2)
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == "APyFloatArray([ 1,  0], [ 5, 10], [ 3,  1], exp_bits=5, man_bits=2)"
    )

    arr = APyFloatArray([[1, 0]], [[5, 10]], [[3, 1]], 5, 2)
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == "APyFloatArray([[ 1,  0]], [[ 5, 10]], [[ 3,  1]], exp_bits=5, man_bits=2)"
    )

    arr = APyFloatArray([[1, 0]], [[5, 10]], [[3, 1]], 5, 2, bias=2)
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == """\
APyFloatArray([[ 1,  0]], [[ 5, 10]], [[ 3,  1]], exp_bits=5, man_bits=2, bias=2)"""
    )

    arr = APyFloatArray.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    assert (
        repr(arr)
        == """\
APyFloatArray(
    [  0,   0,   0], [511, 512, 512], [  0,   0, 512], exp_bits=10, man_bits=10
)"""
    )

    arr = APyFloatArray.from_float([1, 2, 3, 4, 5], exp_bits=10, man_bits=10)
    assert (
        repr(arr)
        == """\
APyFloatArray(
    [  0,   0,   0,   0,   0],
    [511, 512, 512, 513, 513],
    [  0,   0, 512,   0, 256],
    exp_bits=10,
    man_bits=10
)"""
    )

    arr = APyFloatArray([[1, 0], [0, 1]], [[5, 10], [3, 17]], [[3, 1], [0, 2]], 4, 3)
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == """\
APyFloatArray(
    [[ 1,  0],
     [ 0,  1]],

    [[ 5, 10],
     [ 3, 17]],

    [[ 3,  1],
     [ 0,  2]],
    exp_bits=4,
    man_bits=3
)"""
    )

    arr = APyFloatArray(
        [[[1, 0], [0, 1], [1, 1]], [[0, 0], [1, 0], [0, 1]]],
        [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 10], [11, 12]]],
        [[[13, 14], [15, 16], [17, 18]], [[19, 20], [21, 22], [23, 24]]],
        5,
        10,
    )
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == """\
APyFloatArray(
    [[[ 1,  0],
      [ 0,  1],
      [ 1,  1]],

     [[ 0,  0],
      [ 1,  0],
      [ 0,  1]]],


    [[[ 1,  2],
      [ 3,  4],
      [ 5,  6]],

     [[ 7,  8],
      [ 9, 10],
      [11, 12]]],


    [[[13, 14],
      [15, 16],
      [17, 18]],

     [[19, 20],
      [21, 22],
      [23, 24]]],
    exp_bits=5,
    man_bits=10
)"""
    )


@pytest.mark.float_array
def test_repr_summary():
    a = APyFloatArray.from_float((range(10000), range(10000)), exp_bits=10, man_bits=10)
    assert (
        repr(a)
        == """\
APyFloatArray(
    [[   0,    0,    0,  ...,    0,    0,    0],
     [   0,    0,    0,  ...,    0,    0,    0]],

    [[   0,  511,  512,  ...,  524,  524,  524],
     [   0,  511,  512,  ...,  524,  524,  524]],

    [[   0,    0,    0,  ...,  226,  226,  226],
     [   0,    0,    0,  ...,  226,  226,  226]],
    exp_bits=10,
    man_bits=10
)"""
    )

    b = APyFloatArray.from_float(
        [range(4) for _ in range(10000)], exp_bits=10, man_bits=10
    )
    assert (
        repr(b)
        == """\
APyFloatArray(
    [[  0,   0,   0,   0],
     [  0,   0,   0,   0],
     [  0,   0,   0,   0],
     ...,
     [  0,   0,   0,   0],
     [  0,   0,   0,   0],
     [  0,   0,   0,   0]],

    [[  0, 511, 512, 512],
     [  0, 511, 512, 512],
     [  0, 511, 512, 512],
     ...,
     [  0, 511, 512, 512],
     [  0, 511, 512, 512],
     [  0, 511, 512, 512]],

    [[  0,   0,   0, 512],
     [  0,   0,   0, 512],
     [  0,   0,   0, 512],
     ...,
     [  0,   0,   0, 512],
     [  0,   0,   0, 512],
     [  0,   0,   0, 512]],
    exp_bits=10,
    man_bits=10
)"""
    )

    c = APyFloatArray.from_float(
        [range(200) for _ in range(200)], exp_bits=10, man_bits=10
    )
    assert (
        repr(c)
        == """\
APyFloatArray(
    [[   0,    0,    0,  ...,    0,    0,    0],
     [   0,    0,    0,  ...,    0,    0,    0],
     [   0,    0,    0,  ...,    0,    0,    0],
     ...,
     [   0,    0,    0,  ...,    0,    0,    0],
     [   0,    0,    0,  ...,    0,    0,    0],
     [   0,    0,    0,  ...,    0,    0,    0]],

    [[   0,  511,  512,  ...,  518,  518,  518],
     [   0,  511,  512,  ...,  518,  518,  518],
     [   0,  511,  512,  ...,  518,  518,  518],
     ...,
     [   0,  511,  512,  ...,  518,  518,  518],
     [   0,  511,  512,  ...,  518,  518,  518],
     [   0,  511,  512,  ...,  518,  518,  518]],

    [[   0,    0,    0,  ...,  552,  560,  568],
     [   0,    0,    0,  ...,  552,  560,  568],
     [   0,    0,    0,  ...,  552,  560,  568],
     ...,
     [   0,    0,    0,  ...,  552,  560,  568],
     [   0,    0,    0,  ...,  552,  560,  568],
     [   0,    0,    0,  ...,  552,  560,  568]],
    exp_bits=10,
    man_bits=10
)"""
    )


@pytest.mark.float_array
def test_str():
    arr = APyFloatArray.from_float(
        [[float("nan"), 1.0], [2.0, 3.0]], exp_bits=10, man_bits=10
    )
    with pytest.raises(ValueError, match=r"APyFloatArray\.__str__: base=9 is not"):
        _ = arr.__str__(9)
    assert (
        str(arr)
        == """\
[[nan,   1],
 [  2,   3]]"""
    )
