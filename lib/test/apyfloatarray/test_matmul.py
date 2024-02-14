import re
from apytypes import (
    APyFloatArray,
    AccumulatorContext,
    QuantizationContext,
    QuantizationMode,
)

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


@pytest.mark.float_array
def test_matrix_multiplication_accumulator_context():
    A = APyFloatArray.from_float(
        [
            [1.25, 1.50],
            [1.75, 1.00],
        ],
        exp_bits=4,
        man_bits=3,
    )
    B = APyFloatArray.from_float(
        [
            [1.75, 1.25],
            [1.50, 2.50],
        ],
        exp_bits=4,
        man_bits=3,
    )

    # Using more mantissa bits, the result will be the same
    with AccumulatorContext(
        exp_bits=4, man_bits=10, quantization=QuantizationMode.TIES_EVEN
    ):
        assert (A @ B).is_identical(
            APyFloatArray.from_float([[4.4375, 5.3125], [4.5625, 4.6875]], 4, 3)
        )

    # Using more exponent bits, the intermediate result won't overflow.
    # The intermediate calculation here otherwise becomes inf-inf=nan
    a = APyFloatArray.from_float(
        [2**6, -(2**7)],
        exp_bits=4,
        man_bits=3,
    )
    b = APyFloatArray.from_float(
        [2**3, 2.5],
        exp_bits=4,
        man_bits=3,
    )

    with AccumulatorContext(exp_bits=10, man_bits=3):
        assert (a @ b).is_identical(APyFloatArray.from_float([192], 4, 3))
    # Truncating quantization mode
    # with AccumulatorContext(int_bits=2, frac_bits=4, quantization=QuantizationMode.TRN):
    #     assert (A @ B).is_identical(
    #         APyFixedArray.from_float([[0.3125, 0.5000], [0.8125, 1.2500]], 6, 2)
    #     )
    # with AccumulatorContext(int_bits=2, frac_bits=3, quantization=QuantizationMode.TRN):
    #     assert (A @ B).is_identical(
    #         APyFixedArray.from_float([[0.2500, 0.5000], [0.7500, 1.2500]], 5, 2)
    #     )
    # with AccumulatorContext(int_bits=2, frac_bits=2, quantization=QuantizationMode.TRN):
    #     assert (A @ B).is_identical(
    #         APyFixedArray.from_float([[0.0000, 0.5000], [0.7500, 1.2500]], 4, 2)
    #     )
    # with AccumulatorContext(int_bits=2, frac_bits=1, quantization=QuantizationMode.TRN):
    #     assert (A @ B).is_identical(
    #         APyFixedArray.from_float([[0.0000, 0.0000], [0.5000, 1.0000]], 3, 2)
    #     )

    # # Quantization, ties to plus infinity quantization
    # with AccumulatorContext(int_bits=2, frac_bits=4, quantization=QuantizationMode.RND):
    #     assert (A @ B).is_identical(
    #         APyFixedArray.from_float([[0.3125, 0.5000], [0.8125, 1.2500]], 6, 2)
    #     )
    # with AccumulatorContext(int_bits=2, frac_bits=3, quantization=QuantizationMode.RND):
    #     assert (A @ B).is_identical(
    #         APyFixedArray.from_float([[0.3750, 0.5000], [0.875, 1.2500]], 5, 2)
    #     )
    # with AccumulatorContext(int_bits=2, frac_bits=2, quantization=QuantizationMode.RND):
    #     assert (A @ B).is_identical(
    #         APyFixedArray.from_float([[0.5000, 0.5000], [0.7500, 1.2500]], 4, 2)
    #     )
    # with AccumulatorContext(int_bits=2, frac_bits=1, quantization=QuantizationMode.RND):
    #     assert (A @ B).is_identical(
    #         APyFixedArray.from_float([[0.0000, 1.0000], [1.0000, 1.5000]], 3, 2)
    #     )
