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
def test_numpy_creation_from_array(dt):
    np = pytest.importorskip("numpy")
    anp = np.array([[1, 2, 3, 4]], dtype=dt)

    a = APyFixedArray.from_array(anp, 5, 4)

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
    with pytest.raises(TypeError, match="APyFixedArray::_set_values_from_ndarray"):
        APyFixedArray.from_float(a, 14, 4)
    a = np.asarray([[0.0, 1.0]])
    with pytest.raises(TypeError, match="APyFixedArray::_set_bits_from_ndarray"):
        APyFixedArray(a, 14, 4)


def test_c_striding():
    np = pytest.importorskip("numpy")
    a = np.array([[1, 2, 3], [4, 5, 6]])

    # `from_float()`
    assert APyFixedArray.from_float(a, int_bits=10, frac_bits=0).is_identical(
        APyFixedArray([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)
    )
    assert APyFixedArray.from_float(a.T, int_bits=10, frac_bits=0).is_identical(
        APyFixedArray([[1, 4], [2, 5], [3, 6]], int_bits=10, frac_bits=0)
    )

    # `from_array()`
    assert APyFixedArray.from_array(a, int_bits=10, frac_bits=0).is_identical(
        APyFixedArray([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)
    )
    assert APyFixedArray.from_array(a.T, int_bits=10, frac_bits=0).is_identical(
        APyFixedArray([[1, 4], [2, 5], [3, 6]], int_bits=10, frac_bits=0)
    )

    # `__init__()`
    assert APyFixedArray(a, int_bits=10, frac_bits=0).is_identical(
        APyFixedArray([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)
    )
    assert APyFixedArray(a.T, int_bits=10, frac_bits=0).is_identical(
        APyFixedArray([[1, 4], [2, 5], [3, 6]], int_bits=10, frac_bits=0)
    )


def test_issue_487():
    """
    Test for GitHub issue #487
    https://github.com/apytypes/apytypes/issues/487
    """
    np = pytest.importorskip("numpy")

    fl = 1.2124003311558797e-16
    fl2fxa = APyFixedArray.from_float([fl], int_bits=1, frac_bits=11)
    npa2fxa = APyFixedArray.from_array(np.array([fl]), int_bits=1, frac_bits=11)
    assert fl2fxa.is_identical(APyFixedArray([0], int_bits=1, frac_bits=11))
    assert npa2fxa.is_identical(APyFixedArray([0], int_bits=1, frac_bits=11))

    fl = 1.25e16
    fl2fxa = APyFixedArray.from_float([fl], int_bits=60, frac_bits=0)
    npa2fxa = APyFixedArray.from_array(np.array([fl]), int_bits=60, frac_bits=0)
    assert fl2fxa.to_numpy() == np.array([fl])
    assert npa2fxa.to_numpy() == np.array([fl])

    fl = 1.25e160
    fl2fxa = APyFixedArray.from_float([fl], int_bits=60, frac_bits=0)
    npa2fxa = APyFixedArray.from_array(np.array([fl]), int_bits=60, frac_bits=0)
    assert fl2fxa.to_numpy() == np.array([0])
    assert npa2fxa.to_numpy() == np.array([0])


def test_fizz_fuzz():
    """
    Tests discovered through some fuzzing
    """
    np = pytest.importorskip("numpy")

    # 1
    fl = [4.06997806e-09]
    a = APyFixedArray.from_float(fl, int_bits=16, frac_bits=16)
    b = APyFixedArray.from_array(np.array(fl), int_bits=16, frac_bits=16)
    assert a.is_identical(b)
