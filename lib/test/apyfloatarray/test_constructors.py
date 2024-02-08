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


@pytest.mark.float_array
def test_from_float():
    a = APyFloatArray.from_float([1.0, 1.25, 1.5], exp_bits=5, man_bits=2)
    assert a.is_identical(APyFloatArray([0, 0, 0], [15, 15, 15], [0, 1, 2], 5, 2))

    # (2 x 3 matrix) initialized from floating-point values.
    b = APyFloatArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=5,
        man_bits=2
    )

    assert b.is_identical(APyFloatArray([[0, 0, 0], [1, 1, 1]], 
                                        [[15, 16, 16], [17, 17, 17]],
                                        [[0, 0, 2], [0, 1, 2]], 5, 2))
    