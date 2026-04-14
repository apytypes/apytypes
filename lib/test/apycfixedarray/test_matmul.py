import pytest

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyFixedAccumulatorContext,
    APyFixedArray,
    QuantizationMode,
    fx,
    outer,
)


def test_inner_product():
    a = APyCFixedArray.from_complex(
        [1 - 2j, -3 + 3j, 4, 5j, 6 - 7j, 8], int_bits=10, frac_bits=10
    )
    b = APyCFixedArray.from_complex(
        [-9 + 8j, 7 + 6j, 5j, 4, 3 - 2j, 1], int_bits=10, frac_bits=5
    )
    assert (a @ b).is_identical(
        APyCFixed.from_complex(-20 + 36j, int_bits=24, frac_bits=15)
    )
    assert (b @ a).is_identical(
        APyCFixed.from_complex(-20 + 36j, int_bits=24, frac_bits=15)
    )

    a = APyCFixedArray.from_complex(
        [1.2345 - 3.567j, -14.001 + 3j], int_bits=128, frac_bits=128
    )
    b = APyCFixedArray.from_complex(
        [-7.628 + 3.7j, 14.001 + 3j], int_bits=128, frac_bits=128
    )
    assert (a @ b).is_identical(b @ a)
    assert (a @ b).is_identical(
        APyCFixed(
            (
                53631231719770388398296099992823384509917463282369573510894245774887056120270885112025102890838648527032594664757219069441635306238819856146171761522638848,
                3679493492661745910627696086324642733534098305366894010910293498023895212490752,
            ),
            bits=514,
            int_bits=258,
        )
    )


def test_inner_product_accumulator_context():
    a = APyCFixedArray.from_float([0.25 - 0.5j, -0.50 - 0.25j], int_bits=1, frac_bits=2)
    b = APyCFixedArray.from_float([0.75 + 0.5j, 0.25j], int_bits=1, frac_bits=2)

    assert (a @ b).is_identical(
        APyCFixed.from_complex(0.5 - 0.375j, int_bits=4, frac_bits=4)
    )

    with APyFixedAccumulatorContext(int_bits=1, frac_bits=4):
        assert (a @ b).is_identical(
            APyCFixed.from_complex(0.5 - 0.375j, int_bits=1, frac_bits=4)
        )
    with APyFixedAccumulatorContext(int_bits=1, frac_bits=3):
        assert (a @ b).is_identical(
            APyCFixed.from_complex(0.375 - 0.375j, int_bits=1, frac_bits=3)
        )
    with APyFixedAccumulatorContext(int_bits=1, frac_bits=2):
        assert (a @ b).is_identical(
            APyCFixed.from_complex(0.25 - 0.5j, int_bits=1, frac_bits=2)
        )


def test_matrix_multiplication():
    a = APyCFixedArray.from_complex(
        [
            [1, 2 + 3j, 3],
            [4j, 5, 6 - 4j],
        ],
        bits=10,
        int_bits=10,
    )
    b = APyCFixedArray.from_complex(
        [
            [1, 1, 1, 1, 1 + 4j],
            [2, -2, 2, 2, 2],
            [3, 3, -3, 3j, 3],
        ],
        bits=10,
        int_bits=7,
    )
    assert (a @ b).is_identical(
        APyCFixedArray.from_complex(
            [
                [14 + 6j, 6 - 6j, -4 + 6j, 5 + 15j, 14 + 10j],
                [28 - 8j, 8 - 8j, -8 + 16j, 22 + 22j, 12 - 8j],
            ],
            bits=23,
            int_bits=20,
        )
    )
    with pytest.raises(ValueError, match=r"APyCFixedArray\.__matmul__: input shape"):
        _ = b @ a


def test_mixed_inner_product_real_complex_and_complex_real():
    cplx = APyCFixedArray.from_complex([1 + 2j, 3 - 1j], int_bits=5, frac_bits=0)
    real = fx([4, -2], int_bits=4, frac_bits=0)
    res_int_bits = cplx.int_bits + real.int_bits + (len(real) - 1).bit_length()

    assert (cplx @ real).is_identical(
        APyCFixed.from_complex(-2 + 10j, int_bits=res_int_bits, frac_bits=0)
    )
    assert (real @ cplx).is_identical(
        APyCFixed.from_complex(-2 + 10j, int_bits=res_int_bits, frac_bits=0)
    )


def test_mixed_matrix_multiplication_real_complex_and_complex_real():
    cplx = APyCFixedArray.from_complex(
        [
            [1 + 2j, 3 - 1j],
            [2j, -4],
        ],
        int_bits=5,
        frac_bits=0,
    )
    real_r = fx(
        [
            [2, -1, 3],
            [0, 5, -2],
        ],
        int_bits=4,
        frac_bits=0,
    )
    real_l = fx(
        [
            [2, -1],
            [0, 5],
            [3, -2],
        ],
        int_bits=4,
        frac_bits=0,
    )
    pad_bits = (len(real_r) - 1).bit_length()
    res_int_bits = cplx.int_bits + real_r.int_bits + pad_bits

    assert (cplx @ real_r).is_identical(
        APyCFixedArray.from_complex(
            [
                [2 + 4j, 14 - 7j, -3 + 8j],
                [4j, -20 - 2j, 8 + 6j],
            ],
            int_bits=res_int_bits,
            frac_bits=0,
        )
    )

    assert (real_l @ cplx).is_identical(
        APyCFixedArray.from_complex(
            [
                [2 + 2j, 10 - 2j],
                [10j, -20],
                [3 + 2j, 17 - 3j],
            ],
            int_bits=res_int_bits,
            frac_bits=0,
        )
    )


def _python_matrix_product(lhs, rhs):
    return [
        [
            sum(lhs_row[k] * rhs[k][j] for k in range(len(rhs)))
            for j in range(len(rhs[0]))
        ]
        for lhs_row in lhs
    ]


def _pad_bits(n_terms):
    return (n_terms - 1).bit_length() if n_terms else 0


@pytest.mark.parametrize(
    (
        "cplx_int_bits",
        "cplx_frac_bits",
        "real_int_bits",
        "real_frac_bits",
    ),
    [
        (9, 3, 7, 4),
        (30, 8, 18, 6),
        (58, 12, 34, 7),
        (72, 10, 66, 4),
        (140, 9, 90, 6),
        (140, 9, 7, 4),
        (9, 2, 90, 4),
    ],
)
def test_mixed_matmul_mismatched_word_lengths(
    cplx_int_bits: int,
    cplx_frac_bits: int,
    real_int_bits: int,
    real_frac_bits: int,
):
    complex_left_values = [
        [1 + 2j, -3 + 1j, 4j, 5 - 2j, -1],
        [2 - 1j, 3, -2 + 2j, 1 + 3j, 4 - 4j],
        [-1 + 1j, 2 - 3j, 6, -4 + 2j, 3 + 1j],
        [-2j, -5 + 1j, 2 + 2j, 7, -3 - 1j],
    ]
    real_right_values = [
        [2, -1, 3, 4],
        [0, 5, -2, 1],
        [-3, 2, 1, -4],
        [6, -2, 0, 3],
        [1, 1, -1, 2],
    ]
    real_left_values = [
        [2, -1, 0, 3],
        [4, 2, -2, 1],
        [-3, 5, 1, 0],
        [1, 0, 2, -4],
        [2, 3, -1, 1],
    ]
    complex_right_values = [
        [1 + 2j, -3 + 1j, 4j, 5 - 2j, -1],
        [2 - 1j, 3, -2 + 2j, 1 + 3j, 4 - 4j],
        [-1 + 1j, 2 - 3j, 6, -4 + 2j, 3 + 1j],
        [-2j, -5 + 1j, 2 + 2j, 7, -3 - 1j],
    ]

    complex_left = APyCFixedArray.from_complex(
        complex_left_values,
        int_bits=cplx_int_bits,
        frac_bits=cplx_frac_bits,
    )
    real_right = fx(
        real_right_values,
        int_bits=real_int_bits,
        frac_bits=real_frac_bits,
    )
    real_left = fx(
        real_left_values,
        int_bits=real_int_bits,
        frac_bits=real_frac_bits,
    )
    complex_right = APyCFixedArray.from_complex(
        complex_right_values,
        int_bits=cplx_int_bits,
        frac_bits=cplx_frac_bits,
    )

    complex_real_res_int_bits = (
        cplx_int_bits + real_int_bits + _pad_bits(len(real_right_values))
    )
    real_complex_res_int_bits = (
        cplx_int_bits + real_int_bits + _pad_bits(len(complex_right_values))
    )
    res_frac_bits = cplx_frac_bits + real_frac_bits

    assert (complex_left @ real_right).is_identical(
        APyCFixedArray.from_complex(
            _python_matrix_product(complex_left_values, real_right_values),
            int_bits=complex_real_res_int_bits,
            frac_bits=res_frac_bits,
        )
    )
    assert (real_left @ complex_right).is_identical(
        APyCFixedArray.from_complex(
            _python_matrix_product(real_left_values, complex_right_values),
            int_bits=real_complex_res_int_bits,
            frac_bits=res_frac_bits,
        )
    )


@pytest.mark.parametrize(
    ("int_bits", "frac_bits"),
    [
        (10, 0),
        (20, 0),
        (40, 0),
        (80, 0),
        (6, 3),
    ],
)
def test_mixed_matmul_bit_sweeps(int_bits: int, frac_bits: int):
    cplx_vec = APyCFixedArray.from_complex(
        [1 + 2j, 3 - 1j], int_bits=int_bits, frac_bits=frac_bits
    )
    real_vec = fx([4, -2], int_bits=int_bits, frac_bits=frac_bits)

    cplx_mat = APyCFixedArray.from_complex(
        [
            [1 + 2j, 3 - 1j],
            [2j, -4],
        ],
        int_bits=int_bits,
        frac_bits=frac_bits,
    )
    real_rhs = fx(
        [
            [2, -1, 3],
            [0, 5, -2],
        ],
        int_bits=int_bits,
        frac_bits=frac_bits,
    )
    real_lhs = fx(
        [
            [2, -1],
            [0, 5],
            [3, -2],
        ],
        int_bits=int_bits,
        frac_bits=frac_bits,
    )

    res_int_bits = 2 * int_bits + _pad_bits(len(real_vec))
    res_frac_bits = 2 * frac_bits

    assert (cplx_vec @ real_vec).is_identical(
        APyCFixed.from_complex(-2 + 10j, int_bits=res_int_bits, frac_bits=res_frac_bits)
    )
    assert (real_vec @ cplx_vec).is_identical(
        APyCFixed.from_complex(-2 + 10j, int_bits=res_int_bits, frac_bits=res_frac_bits)
    )

    assert (cplx_mat @ real_rhs).is_identical(
        APyCFixedArray.from_complex(
            [
                [2 + 4j, 14 - 7j, -3 + 8j],
                [4j, -20 - 2j, 8 + 6j],
            ],
            int_bits=res_int_bits,
            frac_bits=res_frac_bits,
        )
    )
    assert (real_lhs @ cplx_mat).is_identical(
        APyCFixedArray.from_complex(
            [
                [2 + 2j, 10 - 2j],
                [10j, -20],
                [3 + 2j, 17 - 3j],
            ],
            int_bits=res_int_bits,
            frac_bits=res_frac_bits,
        )
    )


def test_wide_matmul():
    a = APyCFixedArray.from_float(
        [
            [-1.2345 + 4.24j, 5.4321 + 0.24j],
            [-9.8765 - 0.15j, 5.7891 - 1.522j],
        ],
        bits=256,
        int_bits=128,
    )
    b = APyCFixedArray.from_float(
        [
            [1.2345 - 9.2834j, 5.4321j],
            [9.8765, -5.6789 - 123.56j],
        ],
        bits=256,
        int_bits=128,
    )
    assert (a @ b).is_identical(
        APyCFixedArray(
            [
                [
                    (
                        10593559032151092676518474359595726550001345526051303136008440410996821396029440,
                        2207575868670792283909905912147011845716485059797410517162830805590142722506752,
                    ),
                    (
                        53631231719770388398296099992823384509917463282369573510894245774887056120291382721489380431525087936649247635372950595812274703299381981791639191886495744,
                        53631231719770388398296099992823384509917463282369573510894245774887056120215535072317040183389892650332647035957719391301258237531395125424456476095676416,
                    ),
                ],
                [
                    (
                        5047492602988374156593858532451808032419247413221644566258610518768947436716032,
                        8854654886846836078878250559316836161156872787895992017215336145994287327215616,
                    ),
                    (
                        53631231719770388398296099992823384509917463282369573510894245774887056120268699842264606230885853826564088760110457464065309894259608960317229403601895424,
                        53631231719770388398296099992823384509917463282369573510894245774887056120206150250114584492336062398409331877828718262713595373009631918184080548409901056,
                    ),
                ],
            ],
            int_bits=258,
            frac_bits=256,
        )
    )


def test_matrix_multiplication_accumulator_context():
    A = APyCFixedArray.from_complex(
        [
            [-0.25, 0.50 + 0.25j],
            [0.75j, 1.00 - 1.25j],
        ],
        int_bits=2,
        frac_bits=2,
    )
    B = APyCFixedArray.from_complex(
        [
            [0.75j, -1.00j],
            [0.25, 0.50 + 0.25j],
        ],
        int_bits=2,
        frac_bits=2,
    )

    # True result
    assert (A @ B).is_identical(
        APyCFixedArray.from_complex(
            [[0.125 - 0.125j, 0.1875 + 0.5j], [-0.3125 - 0.3125j, 1.5625 - 0.375j]],
            int_bits=6,
            frac_bits=4,
        )
    )

    # Truncating quantization mode
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=4, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            APyCFixedArray.from_float(
                [[0.125 - 0.125j, 0.1875 + 0.5j], [-0.3125 - 0.3125j, 1.5625 - 0.375j]],
                int_bits=2,
                frac_bits=4,
            )
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=3, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            APyCFixedArray.from_float(
                [[0.125 - 0.25j, 0.125 + 0.5j], [-0.375 - 0.375j, 1.5 - 0.375j]],
                int_bits=2,
                frac_bits=3,
            )
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=2, quantization=QuantizationMode.TRN
    ):
        assert (A @ B).is_identical(
            APyCFixedArray.from_float(
                [[0 - 0.25j, 0 + 0.5j], [-0.5 - 0.5j, 1.5 - 0.5j]],
                int_bits=2,
                frac_bits=2,
            )
        )

    # Quantization, ties to plus infinity quantization
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=4, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            APyCFixedArray.from_float(
                [[0.125 - 0.125j, 0.1875 + 0.5j], [-0.3125 - 0.3125j, 1.5625 - 0.375j]],
                int_bits=2,
                frac_bits=4,
            )
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=3, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            APyCFixedArray.from_float(
                [[0.125 + 0j, 0.25 + 0.5j], [-0.25 - 0.25j, 1.625 - 0.375j]],
                int_bits=2,
                frac_bits=3,
            )
        )
    with APyFixedAccumulatorContext(
        int_bits=2, frac_bits=2, quantization=QuantizationMode.RND
    ):
        assert (A @ B).is_identical(
            APyCFixedArray.from_float(
                [[0.25 - 0.25j, 0.25 + 0.5j], [-0.25 - 0.25j, 1.5 - 0.25j]],
                int_bits=2,
                frac_bits=2,
            )
        )


@pytest.mark.parametrize("bits", [20, 40, 60, 80])
def test_matrix_multiplication_threadpool(bits: int):
    np = pytest.importorskip("numpy")
    array_np = np.array(range(200 * 200 * 2)).reshape((200, 200, 2))
    array_fx = APyCFixedArray(array_np, int_bits=bits, frac_bits=0)

    array_cnp = np.zeros((200, 200), dtype=np.complex128)
    array_cnp.real = array_np[:, :, 0]
    array_cnp.imag = array_np[:, :, 1]
    assert np.all((array_cnp @ array_cnp) == (array_fx @ array_fx).to_numpy())


@pytest.mark.parametrize("int_bits", [20, 40, 200, 2000])
@pytest.mark.parametrize("frac_bits", [10, 40, 100, 2500])
def test_outer_product(int_bits: int, frac_bits: int):
    a = fx(
        [1j, 1.25j, 0, -4, -3.25 + 2j, 0 + 1j, 99 + 2j],
        int_bits=int_bits,
        frac_bits=frac_bits,
        force_complex=True,
    )
    b = fx(
        [-1 + 2j, 1.25 - 1j, 0 - 0.25j, -4j, -3.25j],
        int_bits=int_bits,
        frac_bits=frac_bits,
        force_complex=True,
    )

    assert outer(a, b).is_identical(
        fx(
            [
                [-2 - 1j, 1 + 1.25j, 0.25 + 0j, 4 + 0j, 3.25 + 0j],
                [-2.5 - 1.25j, 1.25 + 1.5625j, 0.3125 + 0j, 5 + 0j, 4.0625 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [4 - 8j, -5 + 4j, 0 + 1j, 0 + 16j, 0 + 13j],
                [-0.75 - 8.5j, -2.0625 + 5.75j, 0.5 + 0.8125j, 8 + 13j, 6.5 + 10.5625j],
                [-2 - 1j, 1 + 1.25j, 0.25 + 0j, 4 + 0j, 3.25 + 0j],
                [-103 + 196j, 125.75 - 96.5j, 0.5 - 24.75j, 8 - 396j, 6.5 - 321.75j],
            ],
            int_bits=2 * int_bits + 1,
            frac_bits=2 * frac_bits,
            force_complex=True,
        )
    )

    assert outer(b, a).is_identical(
        fx(
            [
                [-2 - 1j, -2.5 - 1.25j, 0, 4 - 8j, -0.75 - 8.5j, -2 - 1j, -103 + 196j],
                [
                    1 + 1.25j,
                    1.25 + 1.5625j,
                    0,
                    -5 + 4j,
                    -2.0625 + 5.75j,
                    1 + 1.25j,
                    125.75 - 96.5j,
                ],
                [
                    0.25 + 0j,
                    0.3125 + 0j,
                    0,
                    0 + 1j,
                    0.5 + 0.8125j,
                    0.25 + 0j,
                    0.5 - 24.75j,
                ],
                [4 + 0j, 5 + 0j, 0, 0 + 16j, 8 + 13j, 4 + 0j, 8 - 396j],
                [
                    3.25 + 0j,
                    4.0625 + 0j,
                    0,
                    0 + 13j,
                    6.5 + 10.5625j,
                    3.25 + 0j,
                    6.5 - 321.75j,
                ],
            ],
            int_bits=2 * int_bits + 1,
            frac_bits=2 * frac_bits,
            force_complex=True,
        )
    )


def test_outer_product_raises():
    a = fx([[1, 1.25, 0, 4, -3.25, 0]], int_bits=10, frac_bits=10, force_complex=True)
    b = fx([-1, 1.25, 0, -4, -3.25], int_bits=10, frac_bits=10, force_complex=True)

    with pytest.raises(
        ValueError, match=r"APyCFixedArray\.outer: both `self` and `rhs`"
    ):
        _ = outer(a, b)

    with pytest.raises(
        ValueError, match=r"APyCFixedArray\.outer: both `self` and `rhs`"
    ):
        _ = outer(b, a)


def test_complex_real_matrix_multiplication():
    a = APyCFixedArray.from_complex(
        [
            [1, 2 + 3j, 3],
            [4j, 5, 6 - 4j],
        ],
        bits=10,
        int_bits=10,
    )
    b = APyFixedArray.from_float(
        [
            [1, 1, 1, 1],
            [2, -2, 2, 2],
            [3, 3, -3, 3],
        ],
        bits=10,
        int_bits=7,
    )
    assert (a @ b).is_identical(
        APyCFixedArray.from_complex(
            [[14 + 6j, 6 - 6j, -4 + 6j, 14 + 6j], [28 - 8j, 8 - 8j, -8 + 16j, 28 - 8j]],
            bits=22,
            int_bits=19,
        )
    )
    with pytest.raises(ValueError, match=r"APyCFixedArray\.__rmatmul__: input shape"):
        _ = b @ a


def test_complex_real_matrix_vector_multiplication():
    a = APyCFixedArray.from_complex(
        [
            [1, 2 + 3j, 3],
            [4j, 5, 6 - 4j],
        ],
        bits=10,
        int_bits=10,
    )
    b = APyFixedArray.from_float(
        [1, 2, 3],
        bits=10,
        int_bits=7,
    )
    assert (a @ b).is_identical(
        APyCFixedArray.from_complex(
            [14 + 6j, 28 - 8j],
            bits=22,
            int_bits=19,
        )
    )
    with pytest.raises(ValueError, match=r"APyCFixedArray\.__rmatmul__: input shape"):
        _ = b @ a


def test_real_complex_matrix_multiplication():
    a = APyFixedArray.from_float(
        [
            [1, 2, 3],
            [4, 5, 6],
        ],
        bits=10,
        int_bits=10,
    )
    b = APyCFixedArray.from_complex(
        [
            [1, 1 + 1j, 1, -1j],
            [2, -2 - 2j, 2j, 2],
            [3j, 3 - 1j, -3, 3],
        ],
        bits=10,
        int_bits=7,
    )
    assert (a @ b).is_identical(
        APyCFixedArray.from_complex(
            [
                [5 + 9j, 6 - 6j, -8 + 4j, 13 - 1j],
                [14 + 18j, 12 - 12j, -14 + 10j, 28 - 4j],
            ],
            bits=22,
            int_bits=19,
        )
    )
    with pytest.raises(ValueError, match=r"APyCFixedArray\.__matmul__: input shape"):
        _ = b @ a


def test_real_complex_matrix_vector_multiplication():
    a = APyFixedArray.from_float(
        [
            [1, 2, 3],
            [4, 5, 6],
        ],
        bits=10,
        int_bits=10,
    )
    b = APyCFixedArray.from_complex(
        [1, 1 + 1j, -1j],
        bits=10,
        int_bits=7,
    )
    assert (a @ b).is_identical(
        APyCFixedArray.from_complex(
            [3 - 1j, 9 - 1j],
            bits=22,
            int_bits=19,
        )
    )
    with pytest.raises(ValueError, match=r"APyCFixedArray\.__matmul__: input shape"):
        _ = b @ a


@pytest.mark.parametrize("left_int_bits", [20, 40, 200])
@pytest.mark.parametrize("left_frac_bits", [10, 40, 100])
@pytest.mark.parametrize("right_int_bits", [20, 40, 200])
@pytest.mark.parametrize("right_frac_bits", [10, 40, 100])
def test_outer_product_different_word_lengths(
    left_int_bits: int, left_frac_bits: int, right_int_bits: int, right_frac_bits: int
):
    a = fx(
        [1j, 1.25j, 0, -4, -3.25 + 2j, 0 + 1j, 99 + 2j],
        int_bits=left_int_bits,
        frac_bits=left_frac_bits,
        force_complex=True,
    )
    b = fx(
        [-1 + 2j, 1.25 - 1j, 0 - 0.25j, -4j, -3.25j],
        int_bits=right_int_bits,
        frac_bits=right_frac_bits,
        force_complex=True,
    )

    assert outer(a, b).is_identical(
        fx(
            [
                [-2 - 1j, 1 + 1.25j, 0.25 + 0j, 4 + 0j, 3.25 + 0j],
                [-2.5 - 1.25j, 1.25 + 1.5625j, 0.3125 + 0j, 5 + 0j, 4.0625 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [4 - 8j, -5 + 4j, 0 + 1j, 0 + 16j, 0 + 13j],
                [-0.75 - 8.5j, -2.0625 + 5.75j, 0.5 + 0.8125j, 8 + 13j, 6.5 + 10.5625j],
                [-2 - 1j, 1 + 1.25j, 0.25 + 0j, 4 + 0j, 3.25 + 0j],
                [-103 + 196j, 125.75 - 96.5j, 0.5 - 24.75j, 8 - 396j, 6.5 - 321.75j],
            ],
            int_bits=left_int_bits + right_int_bits + 1,
            frac_bits=left_frac_bits + right_frac_bits,
            force_complex=True,
        )
    )

    assert outer(b, a).is_identical(
        fx(
            [
                [-2 - 1j, -2.5 - 1.25j, 0, 4 - 8j, -0.75 - 8.5j, -2 - 1j, -103 + 196j],
                [
                    1 + 1.25j,
                    1.25 + 1.5625j,
                    0,
                    -5 + 4j,
                    -2.0625 + 5.75j,
                    1 + 1.25j,
                    125.75 - 96.5j,
                ],
                [
                    0.25 + 0j,
                    0.3125 + 0j,
                    0,
                    0 + 1j,
                    0.5 + 0.8125j,
                    0.25 + 0j,
                    0.5 - 24.75j,
                ],
                [4 + 0j, 5 + 0j, 0, 0 + 16j, 8 + 13j, 4 + 0j, 8 - 396j],
                [
                    3.25 + 0j,
                    4.0625 + 0j,
                    0,
                    0 + 13j,
                    6.5 + 10.5625j,
                    3.25 + 0j,
                    6.5 - 321.75j,
                ],
            ],
            int_bits=left_int_bits + right_int_bits + 1,
            frac_bits=left_frac_bits + right_frac_bits,
            force_complex=True,
        )
    )
