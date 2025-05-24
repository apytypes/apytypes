from typing import cast

from apytypes import APyCFixedArray


def is_repr_identical(a: APyCFixedArray):
    """
    Tests that an :class:`APyCFixedArray` is identical to its representation.
    """
    try:
        return a.is_identical(cast(APyCFixedArray, eval(repr(a))))
    except ValueError:
        return False


def test_array_representation():
    a = APyCFixedArray([], bits=1, int_bits=-5)
    assert is_repr_identical(a)
    assert repr(a) == "APyCFixedArray([], int_bits=-5, frac_bits=6)"

    b = APyCFixedArray([1, 2, 3], bits=2, int_bits=2)
    assert is_repr_identical(b)
    assert (
        repr(b)
        == """\
APyCFixedArray([(1, 0), (2, 0), (3, 0)], int_bits=2, frac_bits=0)"""
    )

    c = APyCFixedArray([range(5, 8), range(3)], bits=10, int_bits=20)
    assert is_repr_identical(c)
    assert (
        repr(c)
        == """\
APyCFixedArray([[(5, 0), (6, 0), (7, 0)],
                [(0, 0), (1, 0), (2, 0)]], int_bits=20, frac_bits=-10)"""
    )

    d = APyCFixedArray([[range(5, 8), range(3)]], bits=64, int_bits=32)
    assert is_repr_identical(d)
    assert (
        repr(d)
        == """\
APyCFixedArray([[[(5, 0), (6, 0), (7, 0)],
                 [(0, 0), (1, 0), (2, 0)]]], int_bits=32, frac_bits=32)"""
    )

    e = APyCFixedArray([[0, -1, -2, 1, 2]], bits=10, int_bits=0)
    assert is_repr_identical(d)
    assert (
        repr(e)
        == """\
APyCFixedArray(
    [[   (0, 0), (1023, 0), (1022, 0),    (1, 0),    (2, 0)]], int_bits=0, frac_bits=10
)"""
    )

    f = APyCFixedArray([[0, -1, -2, 1, 2]], bits=11, int_bits=-5)
    assert is_repr_identical(d)
    assert (
        repr(f)
        == """\
APyCFixedArray(
    [[   (0, 0), (2047, 0), (2046, 0),    (1, 0),    (2, 0)]], int_bits=-5, frac_bits=16
)"""
    )

    g = APyCFixedArray([[range(5, 8), range(3)]], bits=1000, int_bits=500)
    assert is_repr_identical(g)
    assert (
        repr(g)
        == """\
APyCFixedArray([[[(5, 0), (6, 0), (7, 0)],
                 [(0, 0), (1, 0), (2, 0)]]], int_bits=500, frac_bits=500)"""
    )

    # Summary view
    h = APyCFixedArray(range(10000), int_bits=20, frac_bits=0)
    assert not (is_repr_identical(h))
    assert (
        repr(h)
        == """\
APyCFixedArray(
    [   (0, 0),    (1, 0),    (2, 0),       ..., (9997, 0), (9998, 0), (9999, 0)],
    int_bits=20,
    frac_bits=0
)"""
    )

    for i in range(995, 1005):
        x = APyCFixedArray.from_complex(range(i), int_bits=100, frac_bits=200)
        if i <= 1000:
            # Less then a thousand element produces exact repr
            assert is_repr_identical(x)
        else:
            # More than a thousand elements produces a summary view
            assert not (is_repr_identical(x))
