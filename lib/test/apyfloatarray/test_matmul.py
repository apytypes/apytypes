import re
from apytypes import APyFloatArray

import pytest


@pytest.mark.float_array
def test_dimension_missmatch_raises():

    ##
    # Vector inner product missmatch
    #
    a = APyFloatArray([1, 0], [5, 8], [9, 10], exp_bits=10, man_bits=10)
    b = APyFloatArray([0, 0, 0], [5, 5, 9], [1, 2, 3], exp_bits=10, man_bits=10)

    with pytest.raises(
        ValueError,
        match="APyFloatArray.__matmul__: input shape missmatch",
    ):
        _ = a @ b

    with pytest.raises(
        ValueError,
        match="APyFloatArray.__matmul__: input shape missmatch",
    ):
        _ = b @ a

    ##
    # 2D matrix multiplication missmatch
    #
    a = APyFloatArray.from_float([[1, 2, 3], [4, 5, 6]], exp_bits=10, man_bits=10)
    b = APyFloatArray.from_float([[1, 2], [3, 4], [5, 6]], exp_bits=10, man_bits=10)

    # Should not throw...
    _ = a @ b

    with pytest.raises(
        ValueError,
        match=re.escape(
            "APyFloatArray.__matmul__: input shape missmatch, lhs: (2, 3), rhs: (2, 3)"
        ),
    ):
        _ = a @ a

    with pytest.raises(
        ValueError,
        match=re.escape(
            "APyFloatArray.__matmul__: input shape missmatch, lhs: (3, 2), rhs: (3, 2)"
        ),
    ):
        _ = b @ b


@pytest.mark.float_array
def test_inner_product():
    a = APyFloatArray.from_float([1, 2, 3, 4, 5, 6, 7, 8], exp_bits=5, man_bits=8)
    b = APyFloatArray.from_float([9, 8, 7, 6, 5, 4, 3, 2], exp_bits=6, man_bits=7)
    print(a @ b)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(APyFloatArray.from_float([156], exp_bits=6, man_bits=8))


@pytest.mark.float_array
def test_matrix_multiplication():
    a = APyFloatArray.from_float(
        [
            [1, 2, 3],
            [4, 5, 6],
        ],
        exp_bits=10,
        man_bits=23,
    )
    b = APyFloatArray.from_float(
        [
            [1, 1, 1, 1, 1],
            [2, 2, 2, 2, 2],
            [3, 3, 3, 3, 3],
        ],
        exp_bits=10,
        man_bits=23,
    )
    assert (a @ b).is_identical(
        APyFloatArray.from_float(
            [
                [14, 14, 14, 14, 14],
                [32, 32, 32, 32, 32],
            ],
            exp_bits=10,
            man_bits=23,
        )
    )
    with pytest.raises(ValueError, match="APyFloatArray.__matmul__: input shape"):
        _ = b @ a

    A = APyFloatArray.from_float([[1, 2, 3], [-2, -3, -4]], exp_bits=10, man_bits=23)
    b = APyFloatArray.from_float([-3, 4, 5], exp_bits=10, man_bits=23)
    assert (A @ b.T).is_identical(
        APyFloatArray.from_float([20, -26], exp_bits=10, man_bits=23)
    )
