import pytest

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyFixedAccumulatorContext,
    QuantizationMode,
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
    with pytest.raises(ValueError, match="APyCFixedArray.__matmul__: input shape"):
        _ = b @ a


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
