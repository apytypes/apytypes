from apytypes import APyFixedArray

import pytest


def test_array_add_raises():
    with pytest.raises(RuntimeError, match="APyFixedArray.__add__: shape missmatch"):
        a = APyFixedArray([5, 6], bits=10, int_bits=5)
        b = APyFixedArray([1, 2, 3], bits=7, int_bits=2)
        _ = a + b


def test_array_add():
    a = APyFixedArray([-5, -6, 7], bits=10, int_bits=5)
    b = APyFixedArray([1, -2, 3], bits=7, int_bits=2)
    c = a + b
    assert c.is_identical(APyFixedArray([-4, -8, 10], bits=11, int_bits=6))
