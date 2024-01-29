from apytypes import APyFixedArray

import pytest


def test_array_add():
    with pytest.raises(RuntimeError, match="APyFixedArray.__add__: shape missmatch"):
        a = APyFixedArray([1, 2], bits=10, int_bits=5)
        b = APyFixedArray([1, 2, 3], bits=7, int_bits=2)
        c = a + b
