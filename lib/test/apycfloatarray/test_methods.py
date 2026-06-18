from apytypes import APyCFloatArray, APyFloatArray


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
