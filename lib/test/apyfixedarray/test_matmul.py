import re
from apytypes import APyFixedArray
from apytypes import AccumulatorContext
from apytypes import RoundingMode

import pytest


def test_dimension_missmatch_raises():

    ##
    # Vector inner product missmatch
    #
    a = APyFixedArray([1, 2, 3], bits=10, int_bits=10)
    b = APyFixedArray([4, 5], bits=10, int_bits=10)

    with pytest.raises(
        ValueError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape missmatch, lhs: (3), rhs: (2)"
        ),
    ):
        _ = a @ b

    with pytest.raises(
        ValueError,
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
        ValueError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape missmatch, lhs: (2, 3), rhs: (2, 3)"
        ),
    ):
        _ = a @ a

    with pytest.raises(
        ValueError,
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


def test_inner_product_accumulator_context():
    a = APyFixedArray.from_float([0.25, 0.50], int_bits=1, frac_bits=2)
    b = APyFixedArray.from_float([0.75, 0.25], int_bits=1, frac_bits=2)

    assert (a @ b).is_identical(
        APyFixedArray.from_float([0.3125], int_bits=3, frac_bits=4)
    )

    with AccumulatorContext(int_bits=1, frac_bits=4):
        assert (a @ b).is_identical(
            APyFixedArray.from_float([0.3125], int_bits=1, frac_bits=4)
        )
    with AccumulatorContext(int_bits=1, frac_bits=3):
        assert (a @ b).is_identical(
            APyFixedArray.from_float([0.25], int_bits=1, frac_bits=3)
        )
    with AccumulatorContext(int_bits=1, frac_bits=2):
        assert (a @ b).is_identical(
            APyFixedArray.from_float([0.0], int_bits=1, frac_bits=2)
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
    with pytest.raises(ValueError, match="APyFixedArray.__matmul__: input shape"):
        _ = b @ a

    A = APyFixedArray.from_float(
        [[0.1, 0.2, 0.3], [-0.2, -0.3, -0.4]], int_bits=0, frac_bits=7
    )
    b = APyFixedArray.from_float([-3.2, 1.4, 4.5], int_bits=3, frac_bits=3)
    assert (A @ b.T).is_identical(
        APyFixedArray([31652, 1686], int_bits=5, frac_bits=10)
    )


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


def test_matrix_multiplication_accumulator_context():
    A = APyFixedArray.from_float(
        [
            [0.25, 0.50],
            [0.75, 1.00],
        ],
        int_bits=2,
        frac_bits=2,
    )
    B = APyFixedArray.from_float(
        [
            [0.75, 1.00],
            [0.25, 0.50],
        ],
        int_bits=2,
        frac_bits=2,
    )

    # True result
    assert (A @ B).is_identical(
        APyFixedArray.from_float([[0.3125, 0.50], [0.8125, 1.25]], bits=9, int_bits=5)
    )

    # Truncating rounding mode
    with AccumulatorContext(int_bits=2, frac_bits=4, rounding=RoundingMode.TRN):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.3125, 0.5000], [0.8125, 1.2500]], 6, 2)
        )
    with AccumulatorContext(int_bits=2, frac_bits=3, rounding=RoundingMode.TRN):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.2500, 0.5000], [0.7500, 1.2500]], 5, 2)
        )
    with AccumulatorContext(int_bits=2, frac_bits=2, rounding=RoundingMode.TRN):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.0000, 0.5000], [0.7500, 1.2500]], 4, 2)
        )
    with AccumulatorContext(int_bits=2, frac_bits=1, rounding=RoundingMode.TRN):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.0000, 0.0000], [0.5000, 1.0000]], 3, 2)
        )

    # Rounding, ties to plus infinity rounding
    with AccumulatorContext(int_bits=2, frac_bits=4, rounding=RoundingMode.RND):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.3125, 0.5000], [0.8125, 1.2500]], 6, 2)
        )
    with AccumulatorContext(int_bits=2, frac_bits=3, rounding=RoundingMode.RND):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.3750, 0.5000], [0.875, 1.2500]], 5, 2)
        )
    with AccumulatorContext(int_bits=2, frac_bits=2, rounding=RoundingMode.RND):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.5000, 0.5000], [0.7500, 1.2500]], 4, 2)
        )
    with AccumulatorContext(int_bits=2, frac_bits=1, rounding=RoundingMode.RND):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.0000, 1.0000], [1.0000, 1.5000]], 3, 2)
        )
