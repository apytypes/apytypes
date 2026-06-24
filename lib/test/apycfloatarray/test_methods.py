import pytest

from apytypes import APyCFloatArray, APyFloatArray, QuantizationMode


@pytest.mark.float_array
def test_from_bits():
    # Test raises
    with pytest.raises(
        ValueError,
        match=r"APyCFloatArray\.from_bits: exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyCFloatArray.from_bits([0], 300, 5)

    with pytest.raises(
        ValueError, match=r"APyCFloatArray\.from_bits: unexpected type when traversing"
    ):
        APyCFloatArray.from_bits(["0"], 5, 10)

    # Only reals
    a = APyCFloatArray.from_bits([108, 112, 120, 4224], exp_bits=8, man_bits=4, bias=6)
    assert a.is_identical(
        APyCFloatArray(
            [(0, 0), (0, 0), (0, 0), (1, 0)],
            [(6, 0), (7, 0), (7, 0), (8, 0)],
            [(12, 0), (0, 0), (8, 0), (0, 0)],
            exp_bits=8,
            man_bits=4,
            bias=6,
        )
    )

    # Complex as well
    a = APyCFloatArray.from_bits([[108, 112], [120, 4224]], 8, 4)
    assert a.is_identical(
        APyCFloatArray(
            [(0, 0), (0, 1)],
            [(6, 7), (7, 8)],
            [(12, 0), (8, 0)],
            exp_bits=8,
            man_bits=4,
        )
    )


@pytest.mark.float_array
def test_from_bits_np():
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    with pytest.raises(
        TypeError,
        match=r"APyCFloatArray::_set_bits_from_ndarray\(\): unsupported `dtype` expecting integer",
    ):
        APyCFloatArray.from_bits(np.asarray([1.0]), 8, 7)

    # Only reals
    a = APyCFloatArray.from_bits(
        np.asarray([108, 112, 120, 4224]), exp_bits=8, man_bits=4, bias=6
    )
    assert a.is_identical(
        APyCFloatArray(
            [(0, 0), (0, 0), (0, 0), (1, 0)],
            [(6, 0), (7, 0), (7, 0), (8, 0)],
            [(12, 0), (0, 0), (8, 0), (0, 0)],
            exp_bits=8,
            man_bits=4,
            bias=6,
        )
    )

    # Complex as well
    a = APyCFloatArray.from_bits(
        np.asarray([[108, 112], [120, 4224]]), exp_bits=8, man_bits=4
    )
    assert a.is_identical(
        APyCFloatArray(
            [(0, 0), (0, 1)],
            [(6, 7), (7, 8)],
            [(12, 0), (8, 0)],
            exp_bits=8,
            man_bits=4,
        )
    )


@pytest.mark.float_array
def test_to_bits():
    a = APyCFloatArray(
        [(0, 0), (0, 0), (0, 0), (1, 0)],
        [(6, 0), (7, 0), (7, 0), (8, 0)],
        [(12, 0), (0, 0), (8, 0), (0, 0)],
        exp_bits=8,
        man_bits=4,
        bias=6,
    )
    assert a.to_bits() == [(108, 0), (112, 0), (120, 0), (4224, 0)]

    a = APyCFloatArray(
        [(0, 0), (0, 1)], [(6, 7), (7, 8)], [(12, 0), (8, 0)], exp_bits=8, man_bits=4
    )
    assert a.to_bits() == [(108, 112), (120, 4224)]

    a = APyCFloatArray(
        [(0, 0), (0, 1), (0, 0), (0, 1)],
        [(6, 7), (7, 8), (6, 7), (7, 8)],
        [(12, 0), (8, 0), (12, 0), (8, 0)],
        exp_bits=8,
        man_bits=4,
    ).reshape((2, 1, 2))
    assert a.to_bits() == [[[(108, 112), (120, 4224)]], [[(108, 112), (120, 4224)]]]


@pytest.mark.float_array
def test_to_bits_np():
    np = pytest.importorskip("numpy")

    a = APyCFloatArray(
        [(0, 0), (0, 0), (0, 0), (1, 0)],
        [(6, 0), (7, 0), (7, 0), (8, 0)],
        [(12, 0), (0, 0), (8, 0), (0, 0)],
        exp_bits=8,
        man_bits=4,
        bias=6,
    )
    assert np.array_equal(
        a.to_bits(True), np.asarray([[108, 0], [112, 0], [120, 0], [4224, 0]])
    )

    a = APyCFloatArray(
        [(0, 0), (0, 1)], [(6, 7), (7, 8)], [(12, 0), (8, 0)], exp_bits=8, man_bits=4
    )
    assert np.array_equal(a.to_bits(True), np.asarray([(108, 112), (120, 4224)]))

    a = APyCFloatArray(
        [(0, 0), (0, 1), (0, 0), (0, 1)],
        [(6, 7), (7, 8), (6, 7), (7, 8)],
        [(12, 0), (8, 0), (12, 0), (8, 0)],
        exp_bits=8,
        man_bits=4,
    ).reshape((2, 1, 2))
    assert np.array_equal(
        a.to_bits(True),
        np.asarray([[[(108, 112), (120, 4224)]], [[(108, 112), (120, 4224)]]]),
    )

    # Test with 8, 16, 32, and 64 bit
    a = APyCFloatArray([(0, 1)], [(2, 3)], [(4, 5)], exp_bits=3, man_bits=4).to_bits(
        True
    )
    assert np.array_equal(a, np.asarray([(36, 181)], dtype=np.uint8))
    assert a.dtype == np.uint8

    a = APyCFloatArray([(0, 1)], [(2, 3)], [(4, 5)], exp_bits=7, man_bits=8).to_bits(
        True
    )
    assert np.array_equal(a, np.asarray([(516, 33541)], dtype=np.uint16))
    assert a.dtype == np.uint16

    a = APyCFloatArray([(0, 1)], [(2, 3)], [(4, 5)], exp_bits=8, man_bits=23).to_bits(
        True
    )
    assert np.array_equal(a, np.asarray([(16777220, 2172649477)], dtype=np.uint32))
    assert a.dtype == np.uint32

    a = APyCFloatArray([(0, 1)], [(2, 3)], [(4, 5)], exp_bits=11, man_bits=52).to_bits(
        True
    )
    assert np.array_equal(
        a, np.asarray([(9007199254740996, 9236882835736887301)], dtype=np.uint64)
    )
    assert a.dtype == np.uint64

    with pytest.raises(
        ValueError,
        match=r"APyCFloatArray::to_bits_ndarray\(\): only supports",
    ):
        APyCFloatArray([0], [0], [0], 30, 60).to_bits(True)


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
