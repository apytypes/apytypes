from apytypes import APyCFloatArray, APyFloatArray, QuantizationMode


def test_real_imag():
    real_values = [0.0, 0.5, 0.75, 1.0, 1.25, -0.5, -1.0, -1.25]
    imag_values = [0.0, -0.25, -1.75, 1.25, 1.25, -0.5, -1.0, -1.25]
    complex_values = [real + 1j * imag for real, imag in zip(real_values, imag_values)]
    array = APyCFloatArray.from_complex(
        complex_values, exp_bits=7, man_bits=12, bias=14
    )

    assert array.real.is_identical(
        APyFloatArray.from_float(real_values, exp_bits=7, man_bits=12, bias=14)
    )
    assert array.imag.is_identical(
        APyFloatArray.from_float(imag_values, exp_bits=7, man_bits=12, bias=14)
    )

    # Test reshaped
    array = array.reshape((2, 4))

    assert array.real.is_identical(
        APyFloatArray.from_float(real_values, exp_bits=7, man_bits=12, bias=14).reshape(
            (2, 4)
        )
    )
    assert array.imag.is_identical(
        APyFloatArray.from_float(imag_values, exp_bits=7, man_bits=12, bias=14).reshape(
            (2, 4)
        )
    )


def test_convenience_cast():
    a = APyCFloatArray.from_complex(
        [0.893820 + 3e20j, complex(-float("inf"), float("nan"))],
        exp_bits=10,
        man_bits=50,
    )
    assert a.cast_to_double().is_identical(
        APyCFloatArray(
            [[0, 0], [1, 0]],
            [[1022, 1091], [2047, 2047]],
            [[3547215210502096, 74037091379504], [0, 1]],
            exp_bits=11,
            man_bits=52,
        )
    )
    assert a.cast_to_single().is_identical(
        APyCFloatArray(
            [[0, 0], [1, 0]],
            [[126, 195], [255, 255]],
            [[6607203, 137905], [0, 1]],
            exp_bits=8,
            man_bits=23,
        )
    )
    assert a.cast_to_half().is_identical(
        APyCFloatArray(
            [[0, 0], [1, 0]],
            [[14, 31], [31, 31]],
            [[807, 0], [0, 1]],
            exp_bits=5,
            man_bits=10,
        )
    )
    assert a.cast_to_bfloat16().is_identical(
        APyCFloatArray(
            [[0, 0], [1, 0]],
            [[126, 195], [255, 255]],
            [[101, 2], [0, 1]],
            exp_bits=8,
            man_bits=7,
        )
    )
    a = APyCFloatArray([1, 0], [0, 0], [1, 1], exp_bits=10, man_bits=40)
    b = a.cast_to_bfloat16(QuantizationMode.TO_POS)
    assert b.is_identical(
        APyCFloatArray([1, 0], [0, 0], [0, 1], exp_bits=8, man_bits=7)
    )
