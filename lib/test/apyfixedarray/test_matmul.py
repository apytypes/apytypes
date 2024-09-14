import re
from apytypes import APyFixedArray
from apytypes import APyFixedAccumulatorContext
from apytypes import QuantizationMode

import pytest


def test_dimension_mismatch_raises():
    ##
    # Vector inner product mismatch
    #
    a = APyFixedArray([1, 2, 3], bits=10, int_bits=10)
    b = APyFixedArray([4, 5], bits=10, int_bits=10)

    with pytest.raises(
        ValueError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape mismatch, lhs: (3,), rhs: (2,)"
        ),
    ):
        _ = a @ b

    with pytest.raises(
        ValueError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape mismatch, lhs: (2,), rhs: (3,)"
        ),
    ):
        _ = b @ a

    ##
    # 2D matrix multiplication mismatch
    #
    a = APyFixedArray([[1, 2, 3], [4, 5, 6]], bits=10, int_bits=10)
    b = APyFixedArray([[1, 2], [3, 4], [5, 6]], bits=10, int_bits=10)

    # Should not throw...
    _ = a @ b

    with pytest.raises(
        ValueError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape mismatch, lhs: (2, 3), rhs: (2, 3)"
        ),
    ):
        _ = a @ a

    with pytest.raises(
        ValueError,
        match=re.escape(
            "APyFixedArray.__matmul__: input shape mismatch, lhs: (3, 2), rhs: (3, 2)"
        ),
    ):
        _ = b @ b


def test_inner_product():
    a = APyFixedArray([1, 2, 3, 4, 5, 6, 7, 8], bits=10, int_bits=10)
    b = APyFixedArray([9, 8, 7, 6, 5, 4, 3, 2], bits=10, int_bits=5)
    assert (a @ b).is_identical(APyFixedArray([156], bits=23, int_bits=18))
    assert (b @ a).is_identical(APyFixedArray([156], bits=23, int_bits=18))

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

    a = APyFixedArray([-1, 2, -3, 4, -5, 6, -7, 8], bits=10, int_bits=10)
    b = APyFixedArray([-9, -8, 7, 6, -5, -4, 3, 2], bits=10, int_bits=5)
    assert (a @ b).is_identical(APyFixedArray([8388600], bits=23, int_bits=18))
    assert (b @ a).is_identical(APyFixedArray([8388600], bits=23, int_bits=18))

    a = APyFixedArray([[-1, 2], [-3, 4], [-5, 6], [-7, 8]], bits=10, int_bits=10)
    b = APyFixedArray([[-9, -8, 7, 6], [-5, -4, 3, 2]], bits=10, int_bits=5)
    assert (a @ b).is_identical(
        APyFixedArray(
            [
                [2097151, 0, 2097151, 2097150],
                [7, 8, 2097143, 2097142],
                [15, 16, 2097135, 2097134],
                [23, 24, 2097127, 2097126],
            ],
            bits=21,
            int_bits=16,
        )
    )
    assert (b @ a).is_identical(
        APyFixedArray([[4194260, 40], [4194292, 8]], bits=22, int_bits=17)
    )


def test_inner_product_accumulator_context():
    a = APyFixedArray.from_float([0.25, 0.50], int_bits=1, frac_bits=2)
    b = APyFixedArray.from_float([0.75, 0.25], int_bits=1, frac_bits=2)

    assert (a @ b).is_identical(
        APyFixedArray.from_float([0.3125], int_bits=3, frac_bits=4)
    )

    with APyFixedAccumulatorContext(int_bits=1, frac_bits=4):
        assert (a @ b).is_identical(
            APyFixedArray.from_float([0.3125], int_bits=1, frac_bits=4)
        )
    with APyFixedAccumulatorContext(int_bits=1, frac_bits=3):
        assert (a @ b).is_identical(
            APyFixedArray.from_float([0.25], int_bits=1, frac_bits=3)
        )
    with APyFixedAccumulatorContext(int_bits=1, frac_bits=2):
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
        int_bits=7,
    )
    assert (a @ b).is_identical(
        APyFixedArray(
            [
                [14, 14, 14, 14, 14],
                [32, 32, 32, 32, 32],
            ],
            bits=22,
            int_bits=19,
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

    A = APyFixedArray.from_float(
        [
            [0.25, 0.50],
            [-0.75, 1.00],
        ],
        int_bits=512,
        frac_bits=256,
    )
    B = APyFixedArray.from_float(
        [
            [0.75, 1.00],
            [0.25, 0.50],
        ],
        int_bits=256,
        frac_bits=512,
    )
    assert (A @ B).is_identical(
        APyFixedArray.from_float(
            [[0.3125, 0.5], [-0.3125, -0.25]],
            bits=1537,
            int_bits=768 + 1,
        )
    )
    assert (B @ A).is_identical(
        APyFixedArray.from_float(
            [[-0.5625, 1.375], [-0.3125, 0.625]],
            bits=1537,
            int_bits=768 + 1,
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

    # Truncating quantization mode
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=4, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.3125, 0.5000], [0.8125, 1.2500]], 2, 4)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=3, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.2500, 0.5000], [0.7500, 1.2500]], 2, 3)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=2, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.0000, 0.5000], [0.7500, 1.2500]], 2, 2)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=1, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.0000, 0.0000], [0.5000, 1.0000]], 2, 1)
        )

    # Quantization, ties to plus infinity quantization
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=4, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.3125, 0.5000], [0.8125, 1.2500]], 2, 4)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=3, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.3750, 0.5000], [0.875, 1.2500]], 2, 3)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=2, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.5000, 0.5000], [0.7500, 1.2500]], 2, 2)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=1, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            APyFixedArray.from_float([[0.0000, 1.0000], [1.0000, 1.5000]], 2, 1)
        )


def test_matrix_multiplication_narrow_accumulator():
    A = APyFixedArray.from_float(
        [
            [0.25, 0.50],
            [0.75, 1.00],
        ],
        int_bits=512,
        frac_bits=256,
    )
    B = APyFixedArray.from_float(
        [
            [0.75, 1.00],
            [0.25, 0.50],
        ],
        int_bits=256,
        frac_bits=512,
    )

    assert (A @ B).is_identical(
        APyFixedArray.from_float(
            [[0.3125, 0.5], [0.8125, 1.25]],
            bits=1537,
            int_bits=768 + 1,
        )
    )

    with APyFixedAccumulatorContext(bits=10, int_bits=5):
        C = A @ B
        assert C.is_identical(
            APyFixedArray.from_float(
                [[0.3125, 0.5], [0.8125, 1.25]], bits=10, int_bits=5
            )
        )


def test_inner_product_accumulator_edge():
    A = APyFixedArray.from_float(range(1024), int_bits=64, frac_bits=64)
    B = (-A).cast(int_bits=64, frac_bits=64)

    with APyFixedAccumulatorContext(int_bits=128 + 5, frac_bits=128):
        assert (A @ A).is_identical(
            APyFixedArray.from_float([357389824], int_bits=128 + 5, frac_bits=128)
        )
        assert (A @ B).is_identical(
            APyFixedArray.from_float([-357389824], int_bits=128 + 5, frac_bits=128)
        )
        assert (B @ A).is_identical(
            APyFixedArray.from_float([-357389824], int_bits=128 + 5, frac_bits=128)
        )
        assert (B @ B).is_identical(
            APyFixedArray.from_float([357389824], int_bits=128 + 5, frac_bits=128)
        )
