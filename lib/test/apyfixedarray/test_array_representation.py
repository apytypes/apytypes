from apytypes import APyFixedArray

import pytest


def test_array_representation():
    assert (
        APyFixedArray([], 1, -5).__repr__()
        == "APyFixedArray([], shape=(0), bits=1, int_bits=-5)"
    )
    assert (
        APyFixedArray([1, 2, 3], 1, 2).__repr__()
        == "APyFixedArray([1, 2, 3], shape=(3), bits=1, int_bits=2)"
    )
    assert (
        APyFixedArray([range(5, 8), range(3)], 10, 20).__repr__()
        == "APyFixedArray([5, 6, 7, 0, 1, 2], shape=(2, 3), bits=10, int_bits=20)"
    )
    assert (
        APyFixedArray([[range(5, 8), range(3)]], 64, 32).__repr__()
        == "APyFixedArray([5, 6, 7, 0, 1, 2], shape=(1, 2, 3), bits=64, int_bits=32)"
    )
    assert (
        APyFixedArray([[range(5, 8), range(3)]], 1000, 500).__repr__()
        == "APyFixedArray([5, 6, 7, 0, 1, 2], shape=(1, 2, 3), bits=1000, int_bits=500)"
    )
