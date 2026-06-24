import math

import pytest

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyCFloat,
    APyCFloatArray,
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
    fn,
    fp,
    from_bits,
    fx,
)


def test_fx():
    assert fx(0.3, 4, 6).is_identical(APyFixed.from_float(0.3, 4, 6))

    assert fx([0.3, -0.4], 4, 6).is_identical(
        APyFixedArray.from_float([0.3, -0.4], 4, 6)
    )

    assert fx([0.3, -0.4j], 4, 6).is_identical(
        APyCFixedArray.from_float([0.3, -0.4j], 4, 6)
    )

    assert fx([0.3j, 0.4], 4, 6).is_identical(
        APyCFixedArray.from_float([0.3j, 0.4], 4, 6)
    )

    assert fx([0.3, -0.4j], 4, 6, force_complex=True).is_identical(
        APyCFixedArray.from_float([0.3, -0.4j], 4, 6)
    )

    assert fx([0.3j, 0.4], 4, 6, force_complex=True).is_identical(
        APyCFixedArray.from_float([0.3j, 0.4], 4, 6)
    )

    assert fx(0.3, 4, 6, force_complex=True).is_identical(
        APyCFixed.from_complex(0.3, 4, 6)
    )

    assert fx(0.3, 4, 6, force_complex=False).is_identical(
        APyFixed.from_float(0.3, 4, 6)
    )

    assert fx(0.3j, 4, 6).is_identical(APyCFixed.from_complex(0.3j, 4, 6))

    assert fx(0.3j, 4, 6, force_complex=False).is_identical(
        APyCFixed.from_complex(0.3j, 4, 6)
    )

    assert fx(0.3j, 4, 6, force_complex=True).is_identical(
        APyCFixed.from_complex(0.3j, 4, 6)
    )

    assert fx([0.3j, -0.4], 4, 6, force_complex=True).is_identical(
        APyCFixedArray.from_complex([0.3j, -0.4], 4, 6)
    )


def test_fp():
    assert fp(0.3, 4, 6).is_identical(APyFloat.from_float(0.3, 4, 6))

    assert fp(0.3, 4, 6, force_complex=True).is_identical(
        APyCFloat.from_complex(0.3, 4, 6)
    )

    assert fp(0.3, 4, 6, force_complex=False).is_identical(
        APyFloat.from_float(0.3, 4, 6)
    )

    assert fp(0.3j, 4, 6, force_complex=False).is_identical(
        APyCFloat.from_complex(0.3j, 4, 6)
    )

    assert fp(0.3j, 4, 6, force_complex=True).is_identical(
        APyCFloat.from_complex(0.3j, 4, 6)
    )

    assert fp([0.3, -0.4], 4, 6).is_identical(
        APyFloatArray.from_float([0.3, -0.4], 4, 6)
    )

    assert fp([0.3, -0.4j], 4, 6).is_identical(
        APyCFloatArray.from_float([0.3, -0.4j], 4, 6)
    )

    assert fp([0.3j, -0.4j], 4, 6).is_identical(
        APyCFloatArray.from_float([0.3j, -0.4j], 4, 6)
    )

    assert fp([0.3j, -0.4], 4, 6, force_complex=True).is_identical(
        APyCFloatArray.from_complex([0.3j, -0.4], 4, 6)
    )


def test_fx_numpy():
    np = pytest.importorskip("numpy")
    a = np.array([[0.1, 0.2], [0.3, 0.4]])
    assert fx(a, 0, 9).is_identical(APyFixedArray.from_array(a, 0, 9))

    assert fx(a[0, 0], 0, 9).is_identical(APyFixed.from_float(a[0, 0], 0, 9))


def test_fp_numpy():
    np = pytest.importorskip("numpy")
    a = np.array([[0.1, 0.2], [0.3, 0.4]])
    assert fp(a, 3, 9).is_identical(APyFloatArray.from_array(a, 3, 9))

    assert fp(a[0, 0], 3, 9).is_identical(APyFloat.from_float(a[0, 0], 3, 9))


def test_fn():
    a = APyFixed(30, 4, 6)
    assert fn(math.sqrt, a).is_identical(APyFixed(44, bits=10, int_bits=4))

    b = APyFloat(0, 3, 30, 4, 6)
    assert fn(math.sqrt, b).is_identical(APyFloat(0, 5, 14, 4, 6))

    with pytest.warns(UserWarning, match="Mix of APyFixed"):
        assert fn(math.atan2, a, b).is_identical(APyFixed(88, 4, 6))

    with pytest.warns(UserWarning, match="Mix of APyFloat"):
        assert fn(math.atan2, b, a).is_identical(APyFloat(0, 4, 35, 4, 6))


def test_fn_numpy():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([[1, 2], [3, 4]], 2, 8)
    assert fn(np.sqrt, a).is_identical(
        APyFixedArray([[16, 23], [28, 32]], bits=10, int_bits=2)
    )

    a = APyFloatArray([[0, 1], [1, 0]], [[1, 2], [3, 4]], [[1, 2], [3, 4]], 3, 4)
    assert fn(np.sin, a).is_identical(
        APyFloatArray(
            [[0, 1], [1, 0]],
            [[1, 2], [2, 2]],
            [[1, 1], [14, 3]],
            exp_bits=3,
            man_bits=4,
            bias=3,
        )
    )


def test_fn_warnings():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([3], 3, 6)
    b = APyFloatArray([1], [2], [4], 3, 6)

    with pytest.warns(UserWarning, match="Mix of APyFixed"):
        fn(np.hypot, a, b)

    with pytest.warns(UserWarning, match="Mix of APyFloat"):
        fn(np.hypot, b, a)


def test_from_bits():
    with pytest.raises(
        ValueError, match="from_bits: could not determine data type from bit-specifiers"
    ):
        from_bits(1)

    with pytest.raises(
        ValueError, match="from_bits: could not determine data type from bit-specifiers"
    ):
        from_bits(1, exp_bits=2, man_bits=3, bits=2)

    assert from_bits(0b1_01111_10, exp_bits=5, man_bits=2, bias=6).is_identical(
        APyFloat.from_bits(0b1_01111_10, exp_bits=5, man_bits=2, bias=6)
    )

    assert from_bits(13, int_bits=3, frac_bits=1).is_identical(
        APyFixed(13, int_bits=3, frac_bits=1)
    )

    assert from_bits([0, 1, 2, 3], int_bits=3, frac_bits=0).is_identical(
        APyFixedArray([0, 1, 2, 3], int_bits=3, frac_bits=0)
    )

    assert from_bits([60, 61, 80, 82], exp_bits=5, man_bits=2).is_identical(
        APyFloatArray.from_bits([60, 61, 80, 82], exp_bits=5, man_bits=2)
    )

    assert from_bits([1, 2], int_bits=10, frac_bits=0, force_complex=True).is_identical(
        APyCFixedArray.from_bits([1, 2], int_bits=10, frac_bits=0)
    )

    assert from_bits(
        [[108, 112], [120, 4224]], exp_bits=8, man_bits=4, force_complex=True
    ).is_identical(APyCFloatArray.from_bits([[108, 112], [120, 4224]], 8, 4))
