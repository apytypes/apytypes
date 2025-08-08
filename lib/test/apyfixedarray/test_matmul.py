import pytest

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyFixed,
    APyFixedAccumulatorContext,
    APyFixedArray,
    QuantizationMode,
)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_dimension_mismatch_raises(fixed_array: type[APyCFixedArray]):
    ##
    # Vector inner product mismatch
    #
    a = fixed_array([1, 2, 3], bits=10, int_bits=10)
    b = fixed_array([4, 5, 6, 7], bits=10, int_bits=10)

    with pytest.raises(
        ValueError,
        match=r"APyC?FixedArray.__matmul__: input shape mismatch, "
        + r"lhs: \(3,\), rhs: \(4,\)",
    ):
        _ = a @ b

    with pytest.raises(
        ValueError,
        match=r"APyC?FixedArray.__matmul__: input shape mismatch, "
        + r"lhs: \(4,\), rhs: \(3,\)",
    ):
        _ = b @ a

    ##
    # 2D matrix multiplication mismatch
    #
    a = fixed_array([[1, 2, 3], [4, 5, 6]], bits=10, int_bits=10)
    b = fixed_array.from_float([[1, 2], [3, 4], [5, 6]], bits=10, int_bits=10)

    # Should not throw...
    _ = a @ b

    with pytest.raises(
        ValueError,
        match=r"APyC?FixedArray.__matmul__: input shape mismatch, "
        + r"lhs: \(2, 3\), rhs: \(2, 3\)",
    ):
        _ = a @ a

    with pytest.raises(
        ValueError,
        match=r"APyC?FixedArray.__matmul__: input shape mismatch, "
        + r"lhs: \(3, 2\), rhs: \(3, 2\)",
    ):
        _ = b @ b


@pytest.mark.parametrize(
    ("fx_array", "fx_scalar"), [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)]
)
def test_inner_product(fx_array: type[APyCFixedArray], fx_scalar: type[APyCFixed]):
    # Complex mult results in one additional bit compared to real mult
    cb = fx_array == APyCFixedArray

    a = fx_array([1, 2, 3, 4, 5, 6, 7, 8], bits=10, int_bits=10)
    b = fx_array([9, 8, 7, 6, 5, 4, 3, 2], bits=10, int_bits=5)
    assert (a @ b).is_identical(fx_scalar(156, int_bits=18 + cb, frac_bits=5))
    assert (b @ a).is_identical(fx_scalar(156, int_bits=18 + cb, frac_bits=5))

    a = fx_array.from_float([1.2345, 5.4321], bits=256, int_bits=128)
    b = fx_array.from_float([9.8765, 5.6789], bits=256, int_bits=128)
    assert (a @ b).is_identical(b @ a)
    assert (a @ b).is_identical(
        fx_scalar(
            0x2B0A77E4E8F8510588CC16EF5AC000000000000000000000000000000000000000,
            bits=513 + cb,
            int_bits=257 + cb,
        )
    )

    a = fx_array([-1, 2, -3, 4, -5, 6, -7, 8], bits=10, int_bits=10)
    b = fx_array([-9, -8, 7, 6, -5, -4, 3, 2], bits=10, int_bits=5)
    assert (a @ b).is_identical(
        fx_scalar.from_float(-0.25, int_bits=18 + cb, frac_bits=5)
    )
    assert (b @ a).is_identical(
        fx_scalar.from_float(-0.25, int_bits=18 + cb, frac_bits=5)
    )

    a = fx_array.from_float([[-1, 2], [-3, 4], [-5, 6], [-7, 8]], bits=10, int_bits=10)
    b = fx_array([[-9, -8, 7, 6], [-5, -4, 3, 2]], bits=10, int_bits=5)
    assert (a @ b).is_identical(
        fx_array.from_float(
            [
                [-0.03125, 0, -0.03125, -0.0625],
                [0.21875, 0.25, -0.28125, -0.3125],
                [0.46875, 0.5, -0.53125, -0.5625],
                [0.71875, 0.75, -0.78125, -0.8125],
            ],
            int_bits=16 + cb,
            frac_bits=5,
        )
    )
    assert (b @ a).is_identical(
        fx_array.from_float(
            [[-1.375, 1.25], [-0.375, 0.25]], int_bits=17 + cb, frac_bits=5
        )
    )


@pytest.mark.parametrize(
    ("fx_array", "fx_scalar"), [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)]
)
def test_inner_product_accumulator_context(
    fx_array: type[APyCFixedArray], fx_scalar: type[APyCFixed]
):
    # Complex mult results in one additional bit compared to real mult
    cb = fx_array == APyCFixedArray

    a = fx_array.from_float([0.25, 0.50], int_bits=1, frac_bits=2)
    b = fx_array.from_float([0.75, 0.25], int_bits=1, frac_bits=2)

    assert (a @ b).is_identical(
        fx_scalar.from_float(0.3125, int_bits=3 + cb, frac_bits=4)
    )

    with APyFixedAccumulatorContext(int_bits=1, frac_bits=4):
        assert (a @ b).is_identical(
            fx_scalar.from_float(0.3125, int_bits=1, frac_bits=4)
        )
    with APyFixedAccumulatorContext(int_bits=1, frac_bits=3):
        assert (a @ b).is_identical(fx_scalar.from_float(0.25, int_bits=1, frac_bits=3))
    with APyFixedAccumulatorContext(int_bits=1, frac_bits=2):
        assert (a @ b).is_identical(fx_scalar.from_float(0.0, int_bits=1, frac_bits=2))


@pytest.mark.parametrize("fx_array", [APyFixedArray, APyCFixedArray])
def test_matrix_multiplication(fx_array: type[APyCFixedArray]):
    # Complex mult results in one additional bit compared to real mult
    cb = fx_array == APyCFixedArray

    a = fx_array(
        [
            [1, 2, 3],
            [4, 5, 6],
        ],
        bits=10,
        int_bits=10,
    )
    b = fx_array(
        [
            [1, 1, 1, 1, 1],
            [2, 2, 2, 2, 2],
            [3, 3, 3, 3, 3],
        ],
        bits=10,
        int_bits=7,
    )
    assert (a @ b).is_identical(
        fx_array(
            [
                [14, 14, 14, 14, 14],
                [32, 32, 32, 32, 32],
            ],
            int_bits=19 + cb,
            frac_bits=3,
        )
    )
    with pytest.raises(ValueError, match=r"APyC?FixedArray.__matmul__: input shape"):
        _ = b @ a

    A = fx_array.from_float(
        [[0.1, 0.2, 0.3], [-0.2, -0.3, -0.4]], int_bits=0, frac_bits=7
    )
    b = fx_array.from_float([-3.2, 1.4, 4.5], int_bits=3, frac_bits=3)
    assert (A @ b.T).is_identical(
        fx_array.from_float([-1.08984375, 1.646484375], int_bits=5 + cb, frac_bits=10)
    )


@pytest.mark.parametrize("fx_array", [APyFixedArray, APyCFixedArray])
def test_wide_matmul(fx_array: type[APyCFixedArray]):
    # Complex mult results in one additional bit compared to real mult
    cb = fx_array == APyCFixedArray

    a = fx_array.from_float(
        [
            [1.2345, 5.4321],
            [9.8765, 5.6789],
        ],
        bits=256,
        int_bits=128,
    )
    b = fx_array.from_float(
        [
            [1.2345, 5.4321],
            [9.8765, 5.6789],
        ],
        bits=256,
        int_bits=128,
    )
    assert (a @ b).is_identical(
        fx_array.from_float(
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
            ),
            bits=513 + cb,
            int_bits=257 + cb,
        )
    )

    A = fx_array.from_float(
        [
            [0.25, 0.50],
            [-0.75, 1.00],
        ],
        int_bits=512,
        frac_bits=256,
    )
    B = fx_array.from_float(
        [
            [0.75, 1.00],
            [0.25, 0.50],
        ],
        int_bits=256,
        frac_bits=512,
    )
    assert (A @ B).is_identical(
        fx_array.from_float(
            [[0.3125, 0.5], [-0.3125, -0.25]],
            bits=1537 + cb,
            int_bits=768 + 1 + cb,
        )
    )
    assert (B @ A).is_identical(
        fx_array.from_float(
            [[-0.5625, 1.375], [-0.3125, 0.625]],
            bits=1537 + cb,
            int_bits=768 + 1 + cb,
        )
    )


@pytest.mark.parametrize("fx_array", [APyFixedArray, APyCFixedArray])
def test_matrix_multiplication_accumulator_context(fx_array: type[APyCFixedArray]):
    # Complex mult results in one additional bit compared to real mult
    cb = fx_array == APyCFixedArray

    A = fx_array.from_float(
        [
            [0.25, 0.50],
            [0.75, 1.00],
        ],
        int_bits=2,
        frac_bits=2,
    )
    B = fx_array.from_float(
        [
            [0.75, 1.00],
            [0.25, 0.50],
        ],
        int_bits=2,
        frac_bits=2,
    )

    # True result
    assert (A @ B).is_identical(
        fx_array.from_float(
            [[0.3125, 0.50], [0.8125, 1.25]], int_bits=5 + cb, frac_bits=4
        )
    )

    # Truncating quantization mode
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=4, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            fx_array.from_float([[0.3125, 0.5000], [0.8125, 1.2500]], 2, 4)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=3, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            fx_array.from_float([[0.2500, 0.5000], [0.7500, 1.2500]], 2, 3)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=2, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            fx_array.from_float([[0.0000, 0.5000], [0.7500, 1.2500]], 2, 2)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=1, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            fx_array.from_float([[0.0000, 0.0000], [0.5000, 1.0000]], 2, 1)
        )

    # Quantization, ties to plus infinity quantization
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=4, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            fx_array.from_float([[0.3125, 0.5000], [0.8125, 1.2500]], 2, 4)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=3, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            fx_array.from_float([[0.3750, 0.5000], [0.875, 1.2500]], 2, 3)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=2, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            fx_array.from_float([[0.5000, 0.5000], [0.7500, 1.2500]], 2, 2)
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=1, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            fx_array.from_float([[0.0000, 1.0000], [1.0000, 1.5000]], 2, 1)
        )


@pytest.mark.parametrize("fx_array", [APyFixedArray, APyCFixedArray])
def test_matrix_multiplication_narrow_accumulator(fx_array: type[APyCFixedArray]):
    # Complex mult results in one additional bit compared to real mult
    cb = fx_array == APyCFixedArray

    A = fx_array.from_float(
        [
            [0.25, 0.50],
            [0.75, 1.00],
        ],
        int_bits=512,
        frac_bits=256,
    )
    B = fx_array.from_float(
        [
            [0.75, 1.00],
            [0.25, 0.50],
        ],
        int_bits=256,
        frac_bits=512,
    )

    assert (A @ B).is_identical(
        fx_array.from_float(
            [[0.3125, 0.5], [0.8125, 1.25]],
            bits=1537 + cb,
            int_bits=768 + 1 + cb,
        )
    )

    with APyFixedAccumulatorContext(bits=10, int_bits=5):
        C = A @ B
        assert C.is_identical(
            fx_array.from_float([[0.3125, 0.5], [0.8125, 1.25]], bits=10, int_bits=5)
        )


@pytest.mark.parametrize(
    ("fx_array", "fx_scalar"), [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)]
)
def test_inner_product_accumulator_edge(
    fx_array: type[APyCFixedArray], fx_scalar: type[APyCFixed]
):
    A = fx_array.from_float(range(1024), int_bits=64, frac_bits=64)
    B = (-A).cast(int_bits=64, frac_bits=64)

    with APyFixedAccumulatorContext(int_bits=128 + 5, frac_bits=128):
        assert (A @ A).is_identical(
            fx_scalar.from_float(357389824, int_bits=128 + 5, frac_bits=128)
        )
        assert (A @ B).is_identical(
            fx_scalar.from_float(-357389824, int_bits=128 + 5, frac_bits=128)
        )
        assert (B @ A).is_identical(
            fx_scalar.from_float(-357389824, int_bits=128 + 5, frac_bits=128)
        )
        assert (B @ B).is_identical(
            fx_scalar.from_float(357389824, int_bits=128 + 5, frac_bits=128)
        )


@pytest.mark.parametrize("bits", range(20, 80))
@pytest.mark.parametrize("fx_array", [APyFixedArray, APyCFixedArray])
def test_matrix_multiplication_varying_wordlength(
    bits: int, fx_array: type[APyCFixedArray]
):
    # Complex mult results in one additional bit compared to real mult
    cb = fx_array == APyCFixedArray

    a = fx_array(
        [
            [1, 2, 3],
            [4, 5, 6],
        ],
        bits=bits,
        int_bits=10,
    )
    b = fx_array(
        [
            [1, 1, 1, 1, 1],
            [2, 2, 2, 2, 2],
            [3, 3, 3, 3, 3],
        ],
        bits=bits,
        int_bits=7,
    )
    assert (a @ b).is_identical(
        fx_array(
            [
                [14, 14, 14, 14, 14],
                [32, 32, 32, 32, 32],
            ],
            bits=2 * bits + 2 + cb,
            int_bits=19 + cb,
        )
    )


@pytest.mark.parametrize("fx_array", [APyFixedArray, APyCFixedArray])
def test_matrix_multiplication_two_64bit_limb_result(fx_array: type[APyCFixedArray]):
    # Complex mult results in one additional bit compared to real mult
    cb = fx_array == APyCFixedArray

    A = fx_array.from_float(
        [[0.1, 0.2, 0.3], [-0.2, -0.3, -0.4]], int_bits=0, frac_bits=37
    )
    b = fx_array.from_float([-3.2, 1.4, 4.5], int_bits=3, frac_bits=35)
    assert (A @ b.T).is_identical(
        fx_array.from_float(
            APyFixedArray(
                [145968347985444383584420, 7650233702228896597934], bits=77, int_bits=5
            ),
            int_bits=5 + cb,
            bits=77 + cb,
        )
    )


@pytest.mark.parametrize("fx_array", [APyFixedArray, APyCFixedArray])
def test_matrix_multiplication_two_32bit_limb_result(fx_array: type[APyCFixedArray]):
    # Complex mult results in one additional bit compared to real mult
    cb = fx_array == APyCFixedArray

    A = fx_array.from_float(
        [[0.1, 0.2, 0.3], [-0.2, -0.3, -0.4]], int_bits=0, frac_bits=17
    )
    b = fx_array.from_float([-3.2, 1.4, 4.5], int_bits=3, frac_bits=20)
    assert (A @ b.T).is_identical(
        fx_array.from_float(
            APyFixedArray([4248236659835, 222649919734], bits=42, int_bits=5),
            int_bits=5 + cb,
            bits=42 + cb,
        )
    )
