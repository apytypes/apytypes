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

    a = APyFixedArray.from_float([1.2345, 5.4321], bits=256, int_bits=128)
    b = APyFixedArray.from_float([9.8765, 5.6789], bits=256, int_bits=128)
    assert (a @ b).is_identical(b @ a)
    assert (a @ b).is_identical(
        APyFixedArray(
            [
                0x2B0A77E4E8F8510588CC16EF5AC000000000000000000000000000000000000000,
            ],
            bits=513,
            int_bits=257,
        )
    )


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


def test_wide_matmul():
    a = APyFixedArray.from_float(
        [
            [1.2345, 5.4321],
            [9.8765, 5.6789],
        ],
        bits=256,
        int_bits=128,
    )
    b = APyFixedArray.from_float(
        [
            [1.2345, 5.4321],
            [9.8765, 5.6789],
        ],
        bits=256,
        int_bits=128,
    )
    assert (a @ b).is_identical(
        APyFixedArray(
            [
                [
                    0x372C9383D5E2F6791E87AA83CCA900000000000000000000000000000000000000,
                    0x258DE54DA21F4EBEDB57D4FE1EA800000000000000000000000000000000000000,
                ],
                [
                    0x4447BADDB6FFDAC6C1DD8581B4A000000000000000000000000000000000000000,
                    0x55E66913EAC3821267C40984B91000000000000000000000000000000000000000,
                ],
            ],
            bits=513,
            int_bits=257,
        )
    )
