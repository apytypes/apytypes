from apytypes import APyFixedArray

import pytest


def test_homogeneous_shape():
    """
    Test that array shapes work as intended (must be homogeneous)
    """
    APyFixedArray([], bits=10, int_bits=10)
    APyFixedArray([1], bits=10, int_bits=10)
    APyFixedArray([range(3), (3, 4, 5), [6, 7, 8]], bits=10, int_bits=10)
    APyFixedArray([[range(2)], [range(2)], [range(2)]], bits=10, int_bits=10)

    with pytest.raises(ValueError, match="Inhomogeneous sequence shape"):
        APyFixedArray([[1, 2], [3]], bits=10, int_bits=10)
    with pytest.raises(ValueError, match="Inhomogeneous sequence shape"):
        APyFixedArray([[1, 2], 3], bits=10, int_bits=10)
    with pytest.raises(ValueError, match="Inhomogeneous sequence shape"):
        APyFixedArray([range(3), [3, 4], (6, 7, 8)], bits=10, int_bits=10)
    with pytest.raises(ValueError, match="Inhomogeneous sequence shape"):
        APyFixedArray([range(3), [3, 4, 5], (7, 8)], bits=10, int_bits=10)
    with pytest.raises(ValueError, match="Inhomogeneous sequence shape"):
        APyFixedArray([range(4), [3, 4, 5], (6, 7, 8)], bits=10, int_bits=10)
    with pytest.raises(ValueError, match="python_sequence_extract_shape"):
        APyFixedArray(["a", 5], bits=10, int_bits=10)


def test_array_floating_point_construction():
    """
    Test the `APyFixedArray.from_float` construction method
    """
    a = APyFixedArray.from_float([1.0, 1.25, 2.99], bits=4, frac_bits=1)
    assert a.is_identical(APyFixedArray([2, 3, 6], bits=4, frac_bits=1))

    a = APyFixedArray.from_float([-1.0, -1.25, -2.99], bits=4, frac_bits=1)
    assert a.is_identical(APyFixedArray([-2, -3, -6], bits=4, frac_bits=1))

    with pytest.raises(ValueError, match="Non <type>/sequence found when walking"):
        APyFixedArray.from_float([1.0, 2.0, None], int_bits=13, frac_bits=12)

    a = APyFixedArray.from_float([-1.0, -2.0, -3.0, -4.0], bits=4, frac_bits=0)
    b = APyFixedArray.from_float([-1, -2, -3.0, -4], bits=4, frac_bits=0)
    assert a.is_identical(APyFixedArray([15, 14, 13, 12], bits=4, frac_bits=0))
    assert a.is_identical(b)

    a = APyFixedArray.from_float([-1.0, -2.0, -3.0, -4.0], bits=4, frac_bits=1)
    b = APyFixedArray.from_float([-1, -2, -3.0, -4], bits=4, frac_bits=1)
    assert a.is_identical(APyFixedArray([14, 12, 10, 8], bits=4, frac_bits=1))
    assert a.is_identical(b)

    a = APyFixedArray.from_float([-1.0, -2.0, -3.0, -4.0], bits=4, frac_bits=-1)
    b = APyFixedArray.from_float([-1, -2, -3.0, -4], bits=4, frac_bits=-1)
    assert a.is_identical(b)

    a = APyFixedArray.from_float([100.0, -200.0, -300.0, -400.0], bits=4, frac_bits=0)
    b = APyFixedArray.from_float([100, -200, -300, -400], bits=4, frac_bits=0)
    assert a.is_identical(b)


@pytest.mark.parametrize(
    "dt",
    [
        "int64",
        "int32",
        "int16",
        "int8",
        "uint64",
        "uint32",
        "uint16",
        "uint8",
        "float64",
        "float32",
    ],
)
def test_numpy_creation(dt):
    np = pytest.importorskip("numpy")
    anp = np.array([[1, 2, 3, 4]], dtype=dt)

    a = APyFixedArray.from_float(anp, 5, 4)

    assert np.all(a.to_numpy() == anp.astype(np.double))


def test_incorrect_double_construction():
    with pytest.raises(ValueError, match="Cannot convert nan to fixed-point"):
        APyFixedArray.from_float([float("NaN"), 0.3], 4, 4)

    with pytest.raises(ValueError, match="Cannot convert inf to fixed-point"):
        APyFixedArray.from_float([0.3, float("inf")], 4, 4)


def test_incorrect_double_construction_from_numpy():
    np = pytest.importorskip("numpy")
    with pytest.raises(ValueError, match="Cannot convert nan to fixed-point"):
        APyFixedArray.from_float(np.asarray([float("NaN"), 0.3]), 4, 4)

    with pytest.raises(ValueError, match="Cannot convert inf to fixed-point"):
        APyFixedArray.from_float(np.asarray([0.3, float("inf")]), 4, 4)


@pytest.mark.parametrize(
    "dt",
    [
        "int64",
        "int32",
        "int16",
        "int8",
        "uint64",
        "uint32",
        "uint16",
        "uint8",
    ],
)
def test_base_constructor_ndarray(dt):
    np = pytest.importorskip("numpy")
    np_ndarray = np.array(
        [
            [1, 2, 3],
            [4, 5, 6],
        ],
        dtype=dt,
    )
    assert APyFixedArray(np_ndarray, bits=100, int_bits=50).is_identical(
        APyFixedArray(
            [
                [1, 2, 3],
                [4, 5, 6],
            ],
            bits=100,
            int_bits=50,
        )
    )


def test_from_numpy_raises():
    np = pytest.importorskip("numpy")
    a = np.asarray([[0.0, 1.0]], dtype="half")
    with pytest.raises(
        TypeError, match="APyFixedArray::_set_values_from_numpy_ndarray"
    ):
        b = APyFixedArray.from_float(a, 14, 4)
    a = np.asarray([[0.0, 1.0]])
    with pytest.raises(TypeError, match="APyFixedArray::_set_bits_from_numpy_ndarray"):
        b = APyFixedArray(a, 14, 4)
