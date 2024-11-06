from apytypes import fx, fp, APyFixed, APyFixedArray, APyFloat, APyFloatArray


def test_fx():
    assert fx(0.3, 4, 6).is_identical(APyFixed.from_float(0.3, 4, 6))

    assert fx([0.3, -0.4], 4, 6).is_identical(
        APyFixedArray.from_float([0.3, -0.4], 4, 6)
    )


def test_fp():
    assert fp(0.3, 4, 6).is_identical(APyFloat.from_float(0.3, 4, 6))

    assert fp([0.3, -0.4], 4, 6).is_identical(
        APyFloatArray.from_float([0.3, -0.4], 4, 6)
    )
