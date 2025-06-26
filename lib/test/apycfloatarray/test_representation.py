from typing import cast

import pytest

from apytypes import APyCFloatArray


def is_repr_identical(a: APyCFloatArray) -> bool:
    """Returns true if `a` is identical to its representation"""
    return cast(APyCFloatArray, eval(repr(a))).is_identical(a)


@pytest.mark.float_array
def test_repr():
    arr = APyCFloatArray([], [], [], 6, 7)
    assert is_repr_identical(arr)
    assert repr(arr) == "APyCFloatArray([], [], [], exp_bits=6, man_bits=7)"

    arr = APyCFloatArray([1], [5], [3], 5, 2)
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == """\
APyCFloatArray([(1, 0)], [(5, 0)], [(3, 0)], exp_bits=5, man_bits=2)"""
    )

    arr = APyCFloatArray([1, 0], [5, 10], [3, 1], 5, 2)
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == "APyCFloatArray([ (1, 0)], [(5, 10)], [ (3, 1)], exp_bits=5, man_bits=2)"
    )

    arr = APyCFloatArray([[1, 0]], [[5, 10]], [[3, 1]], 5, 2)
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == "APyCFloatArray([ (1, 0)], [(5, 10)], [ (3, 1)], exp_bits=5, man_bits=2)"
    )

    arr = APyCFloatArray([[1, 0]], [[5, 10]], [[3, 1]], 5, 2, bias=2)
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == """\
APyCFloatArray([ (1, 0)], [(5, 10)], [ (3, 1)], exp_bits=5, man_bits=2, bias=2)"""
    )

    arr = APyCFloatArray.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    assert (
        repr(arr)
        == """\
APyCFloatArray(
    [  (0, 0),   (0, 0),   (0, 0)],
    [(511, 0), (512, 0), (512, 0)],
    [  (0, 0),   (0, 0), (512, 0)],
    exp_bits=10,
    man_bits=10
)"""
    )

    arr = APyCFloatArray.from_float([1, 2, 3, 4, 5], exp_bits=10, man_bits=10)
    assert (
        repr(arr)
        == """\
APyCFloatArray(
    [  (0, 0),   (0, 0),   (0, 0),   (0, 0),   (0, 0)],
    [(511, 0), (512, 0), (512, 0), (513, 0), (513, 0)],
    [  (0, 0),   (0, 0), (512, 0),   (0, 0), (256, 0)],
    exp_bits=10,
    man_bits=10
)"""
    )

    arr = APyCFloatArray([[1, 0], [0, 1]], [[5, 10], [3, 17]], [[3, 1], [0, 2]], 4, 3)
    assert is_repr_identical(arr)
    assert (
        repr(arr)
        == """\
APyCFloatArray(
    [ (1, 0),  (0, 1)], [(5, 10), (3, 17)], [ (3, 1),  (0, 2)], exp_bits=4, man_bits=3
)"""
    )

    arr = APyCFloatArray(
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
APyCFloatArray(
    [[  (1, 0),   (0, 1),   (1, 1)],
     [  (0, 0),   (1, 0),   (0, 1)]],

    [[  (1, 2),   (3, 4),   (5, 6)],
     [  (7, 8),  (9, 10), (11, 12)]],

    [[(13, 14), (15, 16), (17, 18)],
     [(19, 20), (21, 22), (23, 24)]],
    exp_bits=5,
    man_bits=10
)"""
    )


@pytest.mark.float_array
def test_repr_summary():
    a = APyCFloatArray.from_float(
        (range(10000), range(10000)), exp_bits=10, man_bits=10
    )
    assert (
        repr(a)
        == """\
APyCFloatArray(
    [[   (0, 0),    (0, 0),    (0, 0),       ...,    (0, 0),    (0, 0),    (0, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,    (0, 0),    (0, 0),    (0, 0)]],

    [[   (0, 0),  (511, 0),  (512, 0),       ...,  (524, 0),  (524, 0),  (524, 0)],
     [   (0, 0),  (511, 0),  (512, 0),       ...,  (524, 0),  (524, 0),  (524, 0)]],

    [[   (0, 0),    (0, 0),    (0, 0),       ...,  (226, 0),  (226, 0),  (226, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,  (226, 0),  (226, 0),  (226, 0)]],
    exp_bits=10,
    man_bits=10
)"""
    )

    b = APyCFloatArray.from_float(
        [range(4) for _ in range(10000)], exp_bits=10, man_bits=10
    )
    assert (
        repr(b)
        == """\
APyCFloatArray(
    [[  (0, 0),   (0, 0),   (0, 0),   (0, 0)],
     [  (0, 0),   (0, 0),   (0, 0),   (0, 0)],
     [  (0, 0),   (0, 0),   (0, 0),   (0, 0)],
     ...,
     [  (0, 0),   (0, 0),   (0, 0),   (0, 0)],
     [  (0, 0),   (0, 0),   (0, 0),   (0, 0)],
     [  (0, 0),   (0, 0),   (0, 0),   (0, 0)]],

    [[  (0, 0), (511, 0), (512, 0), (512, 0)],
     [  (0, 0), (511, 0), (512, 0), (512, 0)],
     [  (0, 0), (511, 0), (512, 0), (512, 0)],
     ...,
     [  (0, 0), (511, 0), (512, 0), (512, 0)],
     [  (0, 0), (511, 0), (512, 0), (512, 0)],
     [  (0, 0), (511, 0), (512, 0), (512, 0)]],

    [[  (0, 0),   (0, 0),   (0, 0), (512, 0)],
     [  (0, 0),   (0, 0),   (0, 0), (512, 0)],
     [  (0, 0),   (0, 0),   (0, 0), (512, 0)],
     ...,
     [  (0, 0),   (0, 0),   (0, 0), (512, 0)],
     [  (0, 0),   (0, 0),   (0, 0), (512, 0)],
     [  (0, 0),   (0, 0),   (0, 0), (512, 0)]],
    exp_bits=10,
    man_bits=10
)"""
    )

    c = APyCFloatArray.from_float(
        [range(200) for _ in range(200)], exp_bits=10, man_bits=10
    )
    assert (
        repr(c)
        == """\
APyCFloatArray(
    [[   (0, 0),    (0, 0),    (0, 0),       ...,    (0, 0),    (0, 0),    (0, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,    (0, 0),    (0, 0),    (0, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,    (0, 0),    (0, 0),    (0, 0)],
     ...,
     [   (0, 0),    (0, 0),    (0, 0),       ...,    (0, 0),    (0, 0),    (0, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,    (0, 0),    (0, 0),    (0, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,    (0, 0),    (0, 0),    (0, 0)]],

    [[   (0, 0),  (511, 0),  (512, 0),       ...,  (518, 0),  (518, 0),  (518, 0)],
     [   (0, 0),  (511, 0),  (512, 0),       ...,  (518, 0),  (518, 0),  (518, 0)],
     [   (0, 0),  (511, 0),  (512, 0),       ...,  (518, 0),  (518, 0),  (518, 0)],
     ...,
     [   (0, 0),  (511, 0),  (512, 0),       ...,  (518, 0),  (518, 0),  (518, 0)],
     [   (0, 0),  (511, 0),  (512, 0),       ...,  (518, 0),  (518, 0),  (518, 0)],
     [   (0, 0),  (511, 0),  (512, 0),       ...,  (518, 0),  (518, 0),  (518, 0)]],

    [[   (0, 0),    (0, 0),    (0, 0),       ...,  (552, 0),  (560, 0),  (568, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,  (552, 0),  (560, 0),  (568, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,  (552, 0),  (560, 0),  (568, 0)],
     ...,
     [   (0, 0),    (0, 0),    (0, 0),       ...,  (552, 0),  (560, 0),  (568, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,  (552, 0),  (560, 0),  (568, 0)],
     [   (0, 0),    (0, 0),    (0, 0),       ...,  (552, 0),  (560, 0),  (568, 0)]],
    exp_bits=10,
    man_bits=10
)"""
    )


@pytest.mark.float_array
def test_str():
    arr = APyCFloatArray.from_float(
        [[float("nan"), 1.0], [2.0, 3.0]], exp_bits=10, man_bits=10
    )
    with pytest.raises(ValueError, match=r"APyCFloatArray\.__str__: base=9 is not"):
        _ = arr.__str__(9)
    assert (
        str(arr)
        == """\
[[nan+0j,   1+0j],
 [  2+0j,   3+0j]]"""
    )
