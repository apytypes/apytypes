import pytest
from apytypes import APyFloatArray

@pytest.mark.float_array
def test_explicit_constructor():
    arr = APyFloatArray([1], [2], [3], 4, 5)
    assert len(arr) == 1
    assert arr.shape == (1,)
    assert arr.exp_bits == 4
    assert arr.man_bits == 5
    assert arr.bias == 7 # Default when using 4 exponent bits

    arr2d = APyFloatArray([[0, 1], [0, 1]], [[2, 3], [2, 3]], [[4, 5], [4, 5]], 6, 7, 8)
    assert len(arr2d) == 2
    assert arr2d.shape == (2, 2)
    assert arr2d.exp_bits == 6
    assert arr2d.man_bits == 7
    assert arr2d.bias == 8