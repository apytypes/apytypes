from typing import cast

from apytypes import APyFixedArray


def is_repr_identical(a: APyFixedArray):
    """
    Tests that an :class:`APyFloat` is identical to its representation.
    """
    return a.is_identical(cast(APyFixedArray, eval(repr(a))))


def test_array_representation():
    a = APyFixedArray([], bits=1, int_bits=-5)
    assert repr(a) == "APyFixedArray([], int_bits=-5, frac_bits=6)"
    assert is_repr_identical(a)

    b = APyFixedArray([[[[]]]], int_bits=1010, frac_bits=123)
    assert repr(b) == "APyFixedArray([[[[]]]], int_bits=1010, frac_bits=123)"
    assert is_repr_identical(b)

    c = APyFixedArray([1, 2, 3], bits=2, int_bits=2)
    assert repr(c) == "APyFixedArray([1, 2, 3], int_bits=2, frac_bits=0)"
    assert is_repr_identical(c)

    d = APyFixedArray([range(5, 8), range(3)], bits=10, int_bits=20)

    assert (
        repr(d)
        == """\
APyFixedArray([[5, 6, 7],
               [0, 1, 2]], int_bits=20, frac_bits=-10)"""
    )
    assert is_repr_identical(d)

    e = APyFixedArray([[range(5, 8), range(3)]], bits=64, int_bits=32)
    assert (
        repr(e)
        == """\
APyFixedArray([[[5, 6, 7],
                [0, 1, 2]]], int_bits=32, frac_bits=32)"""
    )
    assert is_repr_identical(e)

    f = APyFixedArray([[0, -1, -2, 1, 2]], bits=10, int_bits=0)
    assert (
        repr(f)
        == """\
APyFixedArray([[   0, 1023, 1022,    1,    2]], int_bits=0, frac_bits=10)"""
    )
    assert is_repr_identical(f)

    g = APyFixedArray([[0, -1, -2, 1, 2]], bits=11, int_bits=-5)
    assert (
        repr(g)
        == """\
APyFixedArray([[   0, 2047, 2046,    1,    2]], int_bits=-5, frac_bits=16)"""
    )
    assert is_repr_identical(g)

    h = APyFixedArray([[range(5, 8), range(3)]], bits=1000, int_bits=500)
    assert (
        repr(h)
        == """\
APyFixedArray([[[5, 6, 7],
                [0, 1, 2]]], int_bits=500, frac_bits=500)"""
    )
    assert is_repr_identical(h)
