import re
from apytypes import APyFixedArray

import pytest


def test_dimension_missmatch_raises():

    ##
    # Vector inner product missmatch
    #
    a = APyFixedArray([1, 2, 3], bits=10, int_bits=10)
    b = APyFixedArray([4, 5], bits=10, int_bits=10)

    with pytest.raises(
        RuntimeError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape missmatch, lhs: (3), rhs: (2)"
        ),
    ):
        _ = a @ b

    with pytest.raises(
        RuntimeError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape missmatch, lhs: (2), rhs: (3)"
        ),
    ):
        _ = b @ a

    ##
    # 2D matrix multiplication missmatch
    #
    a = APyFixedArray([[1, 2, 3], [4, 5, 6]], bits=10, int_bits=10)
    b = APyFixedArray([[1, 2], [3, 4], [5, 6]], bits=10, int_bits=10)

    # Should not throw...
    _ = a @ b

    with pytest.raises(
        RuntimeError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape missmatch, lhs: (2, 3), rhs: (2, 3)"
        ),
    ):
        _ = a @ a

    with pytest.raises(
        RuntimeError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape missmatch, lhs: (3, 2), rhs: (3, 2)"
        ),
    ):
        _ = b @ b


def test_inner_product():
    a = APyFixedArray([1, 2, 3, 4, 5, 6, 7, 8], bits=10, int_bits=10)
    b = APyFixedArray([9, 8, 7, 6, 5, 4, 3, 2], bits=10, int_bits=10)
    assert (a @ b).is_identical(APyFixedArray([156], bits=23, int_bits=23))
    assert (b @ a).is_identical(APyFixedArray([156], bits=23, int_bits=23))


def test_matrix_multiplication():
    a = APyFixedArray(
        [
            [1, 2, 3],
            [4, 5, 6],
        ],
        bits=10,
        int_bits=10,
    )
    b = APyFixedArray(
        [
            [1, 1, 1, 1, 1],
            [2, 2, 2, 2, 2],
            [3, 3, 3, 3, 3],
        ],
        bits=10,
        int_bits=10,
    )
    assert (a @ b).is_identical(
        APyFixedArray(
            [
                [14, 14, 14, 14, 14],
                [32, 32, 32, 32, 32],
            ],
            bits=22,
            int_bits=22,
        )
    )
    with pytest.raises(RuntimeError, match="APyFixedArray.__matmul__: input shape"):
        _ = b @ a
