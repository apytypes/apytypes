import pytest
from apytypes import APyFloatArray

@pytest.mark.float_array
def test_is_identical():
    a = APyFloatArray([1], [2], [3], 4, 5)
    assert a.is_identical(a)

    assert not a.is_identical(APyFloatArray([0], [2], [3], 4, 5))
    assert not a.is_identical(APyFloatArray([1], [1], [3], 4, 5))
    assert not a.is_identical(APyFloatArray([1], [2], [2], 4, 5))
    assert not a.is_identical(APyFloatArray([1], [2], [3], 5, 5))
    assert not a.is_identical(APyFloatArray([1], [2], [3], 4, 6))
    assert not a.is_identical(APyFloatArray([1], [2], [6], 4, 6)) # Same value
    assert not a.is_identical(APyFloatArray([1], [2], [3], 4, 5, 9))
