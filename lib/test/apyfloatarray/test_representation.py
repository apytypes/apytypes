import pytest
from apytypes import APyFloatArray


@pytest.mark.float_array
def test_repr():
    arr = APyFloatArray([], [], [], 6, 7)
    assert (
        repr(arr)
        == "APyFloatArray([], [], [], shape=(0,), exp_bits=6, man_bits=7, bias=31)"
    )

    arr = APyFloatArray([1], [5], [3], 5, 2)
    assert (
        repr(arr)
        == "APyFloatArray([1], [5], [3], shape=(1,), exp_bits=5, man_bits=2, bias=15)"
    )

    arr = APyFloatArray([1, 0], [5, 10], [3, 1], 5, 2)
    assert (
        repr(arr)
        == "APyFloatArray([1, 0], [5, 10], [3, 1], shape=(2,), exp_bits=5, man_bits=2, bias=15)"
    )

    arr = APyFloatArray([[1, 0]], [[5, 10]], [[3, 1]], 5, 2)
    assert (
        repr(arr)
        == "APyFloatArray([1, 0], [5, 10], [3, 1], shape=(1, 2), exp_bits=5, man_bits=2, bias=15)"
    )

    arr = APyFloatArray([[1, 0], [0, 1]], [[5, 10], [3, 17]], [[3, 1], [0, 2]], 4, 3)
    assert (
        repr(arr)
        == "APyFloatArray([1, 0, 0, 1], [5, 10, 3, 17], [3, 1, 0, 2], shape=(2, 2), exp_bits=4, man_bits=3, bias=7)"
    )

    arr = APyFloatArray(
        [[[1, 0], [0, 1], [1, 1]], [[0, 0], [1, 0], [0, 1]]],
        [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 10], [11, 12]]],
        [[[13, 14], [15, 16], [17, 18]], [[19, 20], [21, 22], [23, 24]]],
        5,
        10,
    )
    print(arr)
    assert (
        repr(arr)
        == "APyFloatArray([1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1], [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12], [13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24], shape=(2, 3, 2), exp_bits=5, man_bits=10, bias=15)"
    )
