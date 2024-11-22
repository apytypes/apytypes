from apytypes import APyCFixedArray, APyFixedArray


def test_transpose():
    a = APyCFixedArray.from_complex(
        [
            [1 - 1j, 2 + 1j],
            [-1 - 1j, -5 + 1j],
        ],
        int_bits=10,
        frac_bits=0,
    )
    assert a.T.is_identical(
        APyCFixedArray.from_complex(
            [
                [1 - 1j, -1 - 1j],
                [2 + 1j, -5 + 1j],
            ],
            int_bits=10,
            frac_bits=0,
        )
    )


def test_from_complex_with_float():
    a = APyCFixedArray.from_complex(
        [
            [1, 2],
            [-1, -5],
        ],
        int_bits=10,
        frac_bits=0,
    )
    assert a.is_identical(
        APyCFixedArray(
            [
                [1, 2],
                [-1, -5],
            ],
            int_bits=10,
            frac_bits=0,
        )
    )


def test_real_imag():
    real_values = [0.0, 0.5, 0.75, 1.0, 1.25, -0.5, -1.0, -1.25]
    imag_values = [0.0, -0.25, -1.75, 1.25, 1.25, -0.5, -1.0, -1.25]
    complex_values = [real + 1j * imag for real, imag in zip(real_values, imag_values)]
    array = APyCFixedArray.from_complex(complex_values, int_bits=128, frac_bits=128)

    assert array.real.is_identical(
        APyFixedArray.from_float(real_values, int_bits=128, frac_bits=128)
    )
    assert array.imag.is_identical(
        APyFixedArray.from_float(imag_values, int_bits=128, frac_bits=128)
    )
