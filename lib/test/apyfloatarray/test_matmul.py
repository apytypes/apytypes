import pytest

from apytypes import (
    APyCFloat,
    APyCFloatArray,
    APyFloat,
    APyFloatAccumulatorContext,
    APyFloatArray,
    APyFloatQuantizationContext,
    QuantizationMode,
)


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_dimension_mismatch_raises(float_array: type[APyCFloatArray]):
    ##
    # Vector inner product mismatch
    #
    a = float_array([1, 0], [5, 8], [9, 10], exp_bits=10, man_bits=10)
    b = float_array([0, 0, 0], [5, 5, 9], [1, 2, 3], exp_bits=10, man_bits=10)

    with pytest.raises(
        ValueError,
        match="APyC?FloatArray.__matmul__: input shape mismatch",
    ):
        _ = a @ b

    with pytest.raises(
        ValueError,
        match="APyC?FloatArray.__matmul__: input shape mismatch",
    ):
        _ = b @ a

    ##
    # 2D matrix multiplication mismatch
    #
    a = float_array.from_float([[1, 2, 3], [4, 5, 6]], exp_bits=10, man_bits=10)
    b = float_array.from_float([[1, 2], [3, 4], [5, 6]], exp_bits=10, man_bits=10)

    # Should not throw...
    _ = a @ b

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray.__matmul__: input shape mismatch, "
        + r"lhs: \(2, 3\), rhs: \(2, 3\)",
    ):
        _ = a @ a

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray.__matmul__: input shape mismatch, "
        + r"lhs: \(3, 2\), rhs: \(3, 2\)",
    ):
        _ = b @ b


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array.from_float([1, 2, 3, 4, 5, 6, 7, 8], exp_bits=5, man_bits=8)
    b = float_array.from_float([9, 8, 7, 6, 5, 4, 3, 2], exp_bits=6, man_bits=7)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar.from_float(156, exp_bits=6, man_bits=8))


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_matrix_multiplication(float_array: type[APyCFloatArray]):
    a = float_array.from_float(
        [
            [1, 2, 3],
            [4, 5, 6],
        ],
        exp_bits=10,
        man_bits=23,
    )
    b = float_array.from_float(
        [
            [1, 1, 1, 1, 1],
            [2, 2, 2, 2, 2],
            [3, 3, 3, 3, 3],
        ],
        exp_bits=10,
        man_bits=23,
    )
    assert (a @ b).is_identical(
        float_array.from_float(
            [
                [14, 14, 14, 14, 14],
                [32, 32, 32, 32, 32],
            ],
            exp_bits=10,
            man_bits=23,
        )
    )
    with pytest.raises(ValueError, match=r"APyC?FloatArray.__matmul__: input shape"):
        _ = b @ a

    A = float_array.from_float([[1, 2, 3], [-2, -3, -4]], exp_bits=10, man_bits=23)
    b = float_array.from_float([-3, 4, 5], exp_bits=10, man_bits=23)
    assert (A @ b.T).is_identical(
        float_array.from_float([20, -26], exp_bits=10, man_bits=23)
    )


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_matrix_multiplication_accumulator_context(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    A = float_array.from_float(
        [
            [1.25, 1.50],
            [1.75, 1.00],
        ],
        exp_bits=4,
        man_bits=3,
    )
    B = float_array.from_float(
        [
            [1.75, 1.25],
            [1.50, 2.50],
        ],
        exp_bits=4,
        man_bits=3,
    )

    # Changing only the rounding mode will produce the same answer as the
    # APyFloatQuantizationContext
    answer = None
    with APyFloatQuantizationContext(QuantizationMode.TO_POS):
        answer = A @ B

    with APyFloatAccumulatorContext(
        exp_bits=4, man_bits=3, quantization=QuantizationMode.TO_POS
    ):
        print(answer)
        print(A @ B)
        assert answer.is_identical(A @ B)

    # Using more mantissa bits, the result will be the same
    for man_bits in range(6, 20):
        with APyFloatAccumulatorContext(
            exp_bits=4, man_bits=man_bits, quantization=QuantizationMode.TIES_EVEN
        ):
            assert (A @ B).is_identical(
                float_array.from_float(
                    [[4.4375, 5.3125], [4.5625, 4.6875]], 4, man_bits
                )
            )

    # Using more exponent bits, the intermediate result won't overflow.
    # The intermediate calculation here otherwise becomes inf-inf=nan
    a = float_array.from_float(
        [2**6, -(2**7)],
        exp_bits=4,
        man_bits=3,
    )
    b = float_array.from_float(
        [2**3, 2.5],
        exp_bits=4,
        man_bits=3,
    )

    with APyFloatAccumulatorContext(exp_bits=10, man_bits=3):
        assert (a @ b).is_identical(float_scalar.from_float(192, 10, 3))


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_with_subnormals(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array([0, 0, 0], [2, 3, 1], [3, 0, 2], exp_bits=4, man_bits=4)
    b = float_array([0, 1, 0], [5, 4, 6], [3, 0, 2], exp_bits=4, man_bits=4)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar(0, 0, 13, 4, 4))
    assert (a @ b).is_identical((a * b).sum())


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_with_zero_intermediate(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array([0, 0, 0], [7, 7, 8], [0, 3, 2], exp_bits=4, man_bits=4)
    b = float_array([0, 1, 0], [7, 7, 6], [3, 0, 2], exp_bits=4, man_bits=4)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar(0, 7, 4, 4, 4))
    assert (a @ b).is_identical((a * b).sum())


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_with_zero_intermediate_long(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array([0, 0, 0], [511, 511, 400], [0, 3, 2], exp_bits=10, man_bits=60)
    b = float_array([0, 1, 0], [511, 511, 500], [3, 0, 2], exp_bits=10, man_bits=60)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar(0, 389, 4, 10, 60))
    assert (a @ b).is_identical((a * b).sum())


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_with_inf_intermediate(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array([0, 0, 0], [7, 14, 8], [0, 3, 2], exp_bits=4, man_bits=4)
    b = float_array([1, 0, 0], [7, 14, 6], [0, 0, 2], exp_bits=4, man_bits=4)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar(0, 15, 0, 4, 4))
    assert (a @ b).is_identical((a * b).sum())


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_with_inf_intermediate_long(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array([0, 0, 0], [511, 1000, 520], [0, 3, 2], exp_bits=10, man_bits=60)
    b = float_array([1, 0, 0], [511, 1000, 500], [0, 0, 2], exp_bits=10, man_bits=60)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar(0, 1023, 0, 10, 60))
    assert (a @ b).is_identical((a * b).sum())


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_with_exponent_diff(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array([0, 0, 0], [7, 8, 5], [0, 3, 2], exp_bits=4, man_bits=4)
    b = float_array([1, 0, 0], [7, 13, 6], [0, 0, 2], exp_bits=4, man_bits=4)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar(0, 14, 3, 4, 4))
    assert (a @ b).is_identical((a * b).sum())


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_with_inf_from_summation(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array([0, 0, 0], [7, 14, 8], [0, 3, 2], exp_bits=4, man_bits=4)
    b = float_array([0, 0, 0], [14, 7, 10], [0, 0, 2], exp_bits=4, man_bits=4)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar(0, 15, 0, 4, 4))
    assert (a @ b).is_identical((a * b).sum())


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_with_inf_from_summation_long(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array(
        [0, 0, 0], [1020, 512, 1032], [0, 10000000, 200000000], exp_bits=10, man_bits=60
    )
    b = float_array(
        [0, 0, 0], [513, 1020, 500], [0, 10000000, 2000000000], exp_bits=10, man_bits=60
    )
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar(0, 1023, 0, 10, 60))
    assert (a @ b).is_identical((a * b).sum())


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_long(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array.from_float([1, 2, 3, 4, 5, 6, 7, 8], exp_bits=5, man_bits=61)
    b = float_array.from_float([9, 8, 7, 6, 5, 4, 3, 2], exp_bits=6, man_bits=61)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar.from_float(156, exp_bits=6, man_bits=61))


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_inner_product_with_cancellation_long(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    a = float_array([0, 0, 0], [511, 511, 500], [0, 3, 2], exp_bits=10, man_bits=60)
    b = float_array([0, 1, 0], [511, 511, 500], [3, 1, 2], exp_bits=10, man_bits=60)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(float_scalar(0, 488, 1152921504598458376, 10, 60))
    assert (a @ b).is_identical((a * b).sum())


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_array_matmul_mixed_bias(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    """Test that the implementation doesn't do "x.cast() @ y.cast()"."""
    x = float_array([0], [30], [0], exp_bits=5, man_bits=2, bias=14)
    y = float_array([0], [15], [0], exp_bits=5, man_bits=2, bias=16)
    assert (_ := x @ y).is_identical(
        float_scalar(0, 30, 0, exp_bits=5, man_bits=2, bias=15)
    )

    # Multiply two ones but with different formats
    x = float_array([0], [14], [0], exp_bits=5, man_bits=2, bias=14)
    y = float_array([0], [21], [0], exp_bits=5, man_bits=2, bias=21)
    assert (_ := x @ y).is_identical(
        float_scalar(0, 17, 0, exp_bits=5, man_bits=2, bias=17)
    )

    # Same tests again but with (2, 2) matrices
    x = float_array.from_float(
        [[65536, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=14,
    )
    y = float_array.from_float(
        [[0.5, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=16,
    )
    assert (_ := x @ y.T).is_identical(
        float_array.from_float(
            [[32768, 0], [0, 0]],
            exp_bits=5,
            man_bits=2,
            bias=15,
        )
    )

    x = float_array.from_float(
        [[1, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=14,
    )
    y = float_array.from_float(
        [[1, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=21,
    )
    assert (_ := x @ y.T).is_identical(
        float_array.from_float(
            [[1, 0], [0, 0]],
            exp_bits=5,
            man_bits=2,
            bias=17,
        )
    )


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_array_matmul_mixed_bias_overflow(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    """Test that a result can overflow to infinity due to a change in bias."""
    x = float_array([0], [21], [0], exp_bits=5, man_bits=2, bias=5)
    y = float_array([0], [25], [0], exp_bits=5, man_bits=2, bias=25)

    # Multiply with one
    assert (_ := x @ y).is_identical(
        float_scalar(0, 31, 0, exp_bits=5, man_bits=2, bias=15)
    )

    # Multiply with one but with larger bias difference
    y = float_array([0], [27], [0], exp_bits=5, man_bits=2, bias=27)
    assert (_ := x @ y).is_identical(
        float_scalar(0, 31, 0, exp_bits=5, man_bits=2, bias=16)
    )

    # Should become one
    x = float_array([0], [30], [0], exp_bits=5, man_bits=2, bias=3)
    y = float_array([0], [1], [0], exp_bits=5, man_bits=2, bias=28)
    assert (_ := x @ y).is_identical(float_scalar(0, 15, 0, exp_bits=5, man_bits=2))

    # Same tests again but now (2, 2) matrices
    x = float_array.from_float(
        [[65536, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=5,
    )
    y = float_array.from_float(
        [[1, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=25,
    )
    assert (_ := x @ y.T).is_identical(
        float_array.from_float(
            [[float("inf"), 0], [0, 0]],
            exp_bits=5,
            man_bits=2,
            bias=15,
        )
    )

    y = float_array.from_float(
        [[1, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=27,
    )
    assert (_ := x @ y.T).is_identical(
        float_array.from_float(
            [[float("inf"), 0], [0, 0]],
            exp_bits=5,
            man_bits=2,
            bias=16,
        )
    )


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_matrix_mul_acc_context_mixed_bias(float_array: type[APyCFloatArray]):
    """
    Test that the accumulator doesn't cast prematurely. All test cases here use an
    accumulator that shouldn't have any effect.
    """
    x = float_array.from_float(
        [[65536, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=14,
    )
    y = float_array.from_float(
        [[0.5, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=16,
    )

    with APyFloatAccumulatorContext(exp_bits=5, man_bits=2, bias=15):
        assert (_ := x @ y.T).is_identical(
            float_array.from_float(
                [[32768, 0], [0, 0]],
                exp_bits=5,
                man_bits=2,
                bias=15,
            )
        )

    x = float_array.from_float(
        [[1, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=14,
    )
    y = float_array.from_float(
        [[1, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=21,
    )

    with APyFloatAccumulatorContext(exp_bits=5, man_bits=2, bias=17):
        assert (_ := x @ y.T).is_identical(
            float_array.from_float(
                [[1, 0], [0, 0]],
                exp_bits=5,
                man_bits=2,
                bias=17,
            )
        )


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_matrix_mul_acc_context_mixed_bias_overflow(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat]
):
    """
    Test that the accumulator doesn't cast prematurely. All test cases here use an
    accumulator that shouldn't have any effect.
    """
    x = float_array([0], [21], [0], exp_bits=5, man_bits=2, bias=5)
    y = float_array([0], [25], [0], exp_bits=5, man_bits=2, bias=25)

    # Multiply with one
    with APyFloatAccumulatorContext(exp_bits=5, man_bits=2, bias=15):
        assert (_ := x @ y).is_identical(
            float_scalar(0, 31, 0, exp_bits=5, man_bits=2, bias=15)
        )

    # Multiply with one but with larger bias difference
    y = float_array([0], [27], [0], exp_bits=5, man_bits=2, bias=27)
    with APyFloatAccumulatorContext(exp_bits=5, man_bits=2, bias=16):
        assert (_ := x @ y).is_identical(
            float_scalar(0, 31, 0, exp_bits=5, man_bits=2, bias=16)
        )

    # Should become one
    x = float_array([0], [30], [0], exp_bits=5, man_bits=2, bias=3)
    y = float_array([0], [1], [0], exp_bits=5, man_bits=2, bias=28)
    with APyFloatAccumulatorContext(exp_bits=5, man_bits=2):
        assert (_ := x @ y).is_identical(float_scalar(0, 15, 0, exp_bits=5, man_bits=2))

    # Same tests again but now (2, 2) matrices
    x = float_array.from_float(
        [[65536, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=5,
    )
    y = float_array.from_float(
        [[1, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=25,
    )
    with APyFloatAccumulatorContext(exp_bits=5, man_bits=2):
        assert (_ := x @ y.T).is_identical(
            float_array.from_float(
                [[float("inf"), 0], [0, 0]],
                exp_bits=5,
                man_bits=2,
                bias=15,
            )
        )

    y = float_array.from_float(
        [[1, 0], [0, 0]],
        exp_bits=5,
        man_bits=2,
        bias=27,
    )
    with APyFloatAccumulatorContext(exp_bits=5, man_bits=2, bias=16):
        assert (_ := x @ y.T).is_identical(
            float_array.from_float(
                [[float("inf"), 0], [0, 0]],
                exp_bits=5,
                man_bits=2,
                bias=16,
            )
        )


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_to_wide_accumulator(
    float_array: type[APyCFloatArray],
    float_scalar: type[APyCFloat],
):
    """
    Related issue #742:
    https://github.com/apytypes/apytypes/issues/742
    """
    a = float_array.from_float([1, 2, 3, 4, 5, 6, 7, 8], exp_bits=6, man_bits=8)
    b = float_array.from_float([9, 8, 7, 6, 5, 4, 3, 2], exp_bits=6, man_bits=8)

    for q in filter(lambda q: q not in [QuantizationMode.JAM], QuantizationMode):
        for man_bits in range(8, 61):
            for man_bits_acc in range(man_bits, 61):
                ref = float_scalar.from_float(156, exp_bits=6, man_bits=man_bits_acc)
                with APyFloatAccumulatorContext(6, man_bits_acc, quantization=q):
                    assert (a @ b).is_identical(ref)


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_long_mul_short_add(float_array: type[APyCFloatArray]):
    A = float_array.from_float(range(3 * 3), exp_bits=10, man_bits=40).reshape((3, 3))
    b = float_array.from_float([1, 2, 3], exp_bits=10, man_bits=40)
    c = A @ b
    assert c.is_identical(float_array.from_float([8, 26, 44], exp_bits=10, man_bits=40))


@pytest.mark.float_array
def test_inner_product_with_nan():
    a = APyFloatArray([0, 0, 0], [15, 7, 8], [0, 3, 2], exp_bits=4, man_bits=4)
    b = APyFloatArray([0, 1, 0], [0, 7, 6], [0, 0, 2], exp_bits=4, man_bits=4)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(APyFloat(0, 15, 1, 4, 4))
    assert (a @ b).is_identical(sum(a * b))


@pytest.mark.float_array
def test_inner_product_with_nan_long():
    a = APyFloatArray([0, 0, 0], [1023, 511, 80], [0, 3, 2], exp_bits=10, man_bits=60)
    b = APyFloatArray([0, 1, 0], [0, 511, 60], [0, 0, 2], exp_bits=10, man_bits=60)
    assert (a @ b).is_identical(b @ a)
    assert (b @ a).is_identical(APyFloat(0, 1023, 1, 10, 60))
    assert (a @ b).is_identical(sum(a * b))
