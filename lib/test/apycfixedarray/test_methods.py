import pytest

from apytypes import APyCFixed, APyCFixedArray, APyFixed, APyFixedArray, APyFloat


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


def test_from_complex():
    a = APyCFixedArray.from_complex(
        [
            [1 - 2j, 1 / 2 + 3 / 4j],
            [-(2**-5) + 17j, -3j],
        ],
        int_bits=10,
        frac_bits=10,
    )
    assert a.is_identical(
        APyCFixedArray(
            [[(1024, 1046528), (512, 1047808)], [(1048544, 17408), (0, 1045504)]],
            int_bits=10,
            frac_bits=10,
        )
    )

    # From APyCFixed
    assert APyCFixedArray.from_complex(
        [APyCFixed((13, 11), int_bits=2, frac_bits=3)], int_bits=2, frac_bits=2
    ).is_identical(
        APyCFixedArray.from_complex([1.75 + 1.5j], int_bits=2, frac_bits=2)
    )  # Rounds to (1.75, 1.5)
    # Testing using "from_complex([1.75+1.5j]" is currently required

    # From APyFixed
    assert APyCFixedArray.from_complex(
        [APyFixed(13, int_bits=2, frac_bits=3)], int_bits=2, frac_bits=2
    ).is_identical(APyCFixedArray([7], int_bits=2, frac_bits=2))  # Rounds to (1.75, 0)

    # From APyFloat
    assert APyCFixedArray.from_complex(
        [APyFloat(1, 15, 2, 5, 2)], bits=4, int_bits=3
    ).is_identical(APyCFixedArray([13], bits=4, int_bits=3))  # (-1.5, 0)


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


@pytest.mark.parametrize("bits", [16, 32, 64, 128])
def test_issue_615_complex(bits: int):
    """
    Specialized test for complex-valued array: no overflowing/carry between
    real/imaginary parts.
    """
    A_fx = APyCFixedArray.from_complex(
        [1 + 1j, 2 - 2j, -3 + 3j, 4 - 4j], bits=bits, frac_bits=bits // 2
    )
    assert A_fx.cumsum().is_identical(
        APyCFixedArray.from_complex(
            [1 + 1j, 3 - 1j, 0 + 2j, 4 - 2j], bits=bits + 2, frac_bits=bits // 2
        )
    )

    A_fx = APyCFixedArray.from_complex(
        [1 + 2j, 2 - 3j, -5 + 3j, 4 - 5j], bits=bits, frac_bits=bits // 2
    )
    assert A_fx.cumprod().is_identical(
        APyCFixedArray.from_complex(
            [1 + 2j, 8 + 1j, -43 + 19j, -77 + 291j],
            bits=4 * bits + 3,
            frac_bits=2 * bits,
        )
    )


def test_copy():
    a = APyCFixedArray([(3, 1), (0, 3)], 4, 5)
    b = a
    assert a.is_identical(b)
    c = a.copy()
    assert a.is_identical(c)
    a[0] = APyCFixed((2, 1), 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


def test_python_copy():
    import copy

    a = APyCFixedArray([(3, 1), (0, 3)], 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.copy(a)
    assert a.is_identical(c)
    a[0] = APyCFixed((2, 1), 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


def test_python_deepcopy():
    import copy

    a = APyCFixedArray([(3, 1), (0, 3)], 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.deepcopy(a)
    assert a.is_identical(c)
    a[0] = APyCFixed((2, 1), 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)
