import pytest

from apytypes import APyCFixedArray, APyFixed, APyFixedArray, APyFloat


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_homogeneous_shape(fixed_array: type[APyCFixedArray]):
    """
    Test that array shapes work as intended (must be homogeneous)
    """
    _ = fixed_array([], bits=10, int_bits=10)
    _ = fixed_array([1], bits=10, int_bits=10)
    _ = fixed_array([range(3), (3, 4, 5), [6, 7, 8]], bits=10, int_bits=10)
    _ = fixed_array([[range(2)], [range(2)], [range(2)]], bits=10, int_bits=10)

    with pytest.raises(ValueError, match=r"APyC?FixedArray.__init__: inhomogeneous"):
        _ = fixed_array([[1, 2], [3]], bits=10, int_bits=10)
    with pytest.raises(ValueError, match=r"APyC?FixedArray.__init__: inhomogeneous"):
        _ = fixed_array([[1, 2], 3], bits=10, int_bits=10)
    with pytest.raises(ValueError, match=r"APyC?FixedArray.__init__: inhomogeneous"):
        _ = fixed_array([range(3), [3, 4], (6, 7, 8)], bits=10, int_bits=10)
    with pytest.raises(ValueError, match=r"APyC?FixedArray.__init__: inhomogeneous"):
        _ = fixed_array([range(3), [3, 4, 5], (7, 8)], bits=10, int_bits=10)
    with pytest.raises(ValueError, match=r"APyC?FixedArray.__init__: inhomogeneous"):
        _ = fixed_array([range(4), [3, 4, 5], (6, 7, 8)], bits=10, int_bits=10)
    with pytest.raises(ValueError, match=r"APyC?FixedArray.__init__: unexpected type"):
        _ = fixed_array(["a", 5], bits=10, int_bits=10)
    with pytest.raises(ValueError, match=r"APyC?FixedArray.__init__: inhomogeneous"):
        _ = fixed_array([1, 2, 3, [4, 5]], bits=10, int_bits=10)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_array_floating_point_construction(fixed_array: type[APyCFixedArray]):
    """
    Test the `APyFixedArray.from_float` construction method
    """
    a = fixed_array.from_float([1.0, 1.25, 2.99], bits=4, frac_bits=1)
    assert a.is_identical(fixed_array([2, 3, 6], bits=4, frac_bits=1))

    a = fixed_array.from_float([-1.0, -1.25, -2.99], bits=4, frac_bits=1)
    assert a.is_identical(fixed_array([-2, -3, -6], bits=4, frac_bits=1))

    with pytest.raises(ValueError, match=r"APyC?FixedArray.from_[a-z]+: unexpected"):
        _ = fixed_array.from_float([1.0, 2.0, None], int_bits=13, frac_bits=12)

    a = fixed_array.from_float([-1.0, -2.0, -3.0, -4.0], bits=4, frac_bits=0)
    b = fixed_array.from_float([-1, -2, -3.0, -4], bits=4, frac_bits=0)
    assert a.is_identical(fixed_array([15, 14, 13, 12], bits=4, frac_bits=0))
    assert a.is_identical(b)

    a = fixed_array.from_float([-1.0, -2.0, -3.0, -4.0], bits=4, frac_bits=1)
    b = fixed_array.from_float([-1, -2, -3.0, -4], bits=4, frac_bits=1)
    assert a.is_identical(fixed_array([14, 12, 10, 8], bits=4, frac_bits=1))
    assert a.is_identical(b)

    a = fixed_array.from_float([-1.0, -2.0, -3.0, -4.0], bits=4, frac_bits=-1)
    b = fixed_array.from_float([-1, -2, -3.0, -4], bits=4, frac_bits=-1)
    assert a.is_identical(b)

    a = fixed_array.from_float([100.0, -200.0, -300.0, -400.0], bits=4, frac_bits=0)
    b = fixed_array.from_float([100, -200, -300, -400], bits=4, frac_bits=0)
    assert a.is_identical(b)

    # Python long integer (256 bits)
    int_val = 2**257 - 1
    assert fixed_array.from_float([int_val], bits=257, int_bits=257).is_identical(
        fixed_array([int_val], bits=257, int_bits=257)
    )

    int_val = 2**64 - 1
    assert fixed_array.from_float([int_val], bits=65, int_bits=65).is_identical(
        fixed_array([int_val], bits=65, int_bits=65)
    )

    ##
    # From APyFloat
    #

    assert fixed_array.from_float(
        [APyFloat(0, 0, 0, 5, 7)], bits=4, int_bits=3
    ).is_identical(fixed_array([0], bits=4, int_bits=3))  # 0

    assert fixed_array.from_float(
        [APyFloat(1, 0, 0, 5, 7)], bits=4, int_bits=3
    ).is_identical(fixed_array([0], bits=4, int_bits=3))  # 0

    assert fixed_array.from_float(
        [APyFloat(0, 15, 2, 5, 2)], bits=3, int_bits=2
    ).is_identical(fixed_array([3], bits=3, int_bits=2))  # 1.5

    assert fixed_array.from_float(
        [APyFloat(1, 15, 2, 5, 2)], bits=4, int_bits=3
    ).is_identical(fixed_array([13], bits=4, int_bits=3))  # -1.5

    assert fixed_array.from_float(
        [APyFloat(0, 60, 4, 7, 3)], bits=3, int_bits=-1
    ).is_identical(fixed_array([3], bits=3, int_bits=-1))  # 0.1875

    assert fixed_array.from_float(
        [APyFloat(0, 1, 1, 8, 10)], bits=12, int_bits=2
    ).is_identical(fixed_array([0], bits=12, int_bits=2))  # Quantize to zero

    assert fixed_array.from_float(
        [APyFloat(0, 0, 1, 11, 54)],
        int_bits=1,
        frac_bits=1076,  # Smallest subnormal
    ).is_identical(fixed_array([1], int_bits=1, frac_bits=1076))

    ##
    # From APyFixed
    #

    assert fixed_array.from_float(
        [APyFixed(0, bits=10, int_bits=8)], bits=4, int_bits=3
    ).is_identical(fixed_array([0], bits=4, int_bits=3))  # 0

    assert fixed_array.from_float(
        [APyFixed(8, bits=4, int_bits=4)], bits=2, int_bits=2
    ).is_identical(fixed_array([0], bits=2, int_bits=2))  # 0

    assert fixed_array.from_float(
        [APyFixed(9, bits=4, int_bits=4)], bits=2, int_bits=2
    ).is_identical(fixed_array([1], bits=2, int_bits=2))  # Too big, becomes 1

    assert fixed_array.from_float(
        [APyFixed(13, int_bits=2, frac_bits=3)], int_bits=2, frac_bits=2
    ).is_identical(fixed_array([7], int_bits=2, frac_bits=2))  # Rounds to 1.75


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
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
def test_numpy_creation(fixed_array: type[APyCFixedArray], dt: str):
    np = pytest.importorskip("numpy")
    if np.issubdtype(dt, np.signedinteger) or np.issubdtype(dt, np.floating):
        anp = np.array([[1, 2, -3, 4]], dtype=dt)
    else:
        anp = np.array([[1, 2, 3, 4]], dtype=dt)
    a = fixed_array.from_float(anp, 5, 4)
    assert np.all(a.to_numpy() == anp.astype(np.double))

    if dt == "uint64":
        anp = np.asarray([2**64 - 1], dtype=dt)
        assert fixed_array.from_array(anp, bits=65, int_bits=65).is_identical(
            fixed_array([2**64 - 1], bits=65, int_bits=65)
        )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
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
def test_numpy_creation_from_array(fixed_array: type[APyCFixedArray], dt: str):
    np = pytest.importorskip("numpy")
    if np.issubdtype(dt, np.signedinteger) or np.issubdtype(dt, np.floating):
        anp = np.array([[1, 2, -3, 4]], dtype=dt)
    else:
        anp = np.array([[1, 2, 3, 4]], dtype=dt)
    a = fixed_array.from_array(anp, 5, 4)
    assert np.all(a.to_numpy() == anp.astype(np.double))


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_incorrect_double_construction(fixed_array: type[APyCFixedArray]):
    with pytest.raises(ValueError, match="Cannot convert nan to fixed-point"):
        fixed_array.from_float([float("NaN"), 0.3], 4, 4)

    with pytest.raises(ValueError, match="Cannot convert inf to fixed-point"):
        fixed_array.from_float([0.3, float("inf")], 4, 4)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_incorrect_double_construction_from_numpy(fixed_array):
    np = pytest.importorskip("numpy")
    with pytest.raises(ValueError, match="Cannot convert nan to fixed-point"):
        fixed_array.from_float(np.asarray([float("NaN"), 0.3]), 4, 4)

    with pytest.raises(ValueError, match="Cannot convert inf to fixed-point"):
        fixed_array.from_float(np.asarray([0.3, float("inf")]), 4, 4)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
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
def test_base_constructor_ndarray(fixed_array: type[APyCFixedArray], dt: str):
    np = pytest.importorskip("numpy")
    if np.issubdtype(dt, np.signedinteger) or np.issubdtype(dt, np.floating):
        np_ndarray = np.array(
            [
                [1, 2, -3],
                [4, 5, 6],
            ],
            dtype=dt,
        )
        assert fixed_array(np_ndarray, bits=100, int_bits=50).is_identical(
            fixed_array(
                [
                    [1, 2, -3],
                    [4, 5, 6],
                ],
                bits=100,
                int_bits=50,
            )
        )
    else:
        np_ndarray = np.array(
            [
                [1, 2, 3],
                [4, 5, 6],
            ],
            dtype=dt,
        )
        assert fixed_array(np_ndarray, bits=100, int_bits=50).is_identical(
            fixed_array(
                [
                    [1, 2, 3],
                    [4, 5, 6],
                ],
                bits=100,
                int_bits=50,
            )
        )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_from_numpy_raises(fixed_array: type[APyCFixedArray]):
    np = pytest.importorskip("numpy")
    a = np.asarray([[0.0, 1.0]], dtype="half")
    with pytest.raises(TypeError, match=r"APyC?FixedArray\.from_array: unsupported"):
        fixed_array.from_array(a, 14, 4)
    a = np.asarray([[0.0, 1.0]])
    with pytest.raises(TypeError, match=r"APyC?FixedArray\.__init__: unsupported"):
        fixed_array(a, 14, 4)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_c_striding(fixed_array: type[APyCFixedArray]):
    np = pytest.importorskip("numpy")
    a = np.array([[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]])

    # `from_float()`
    assert fixed_array.from_float(a, int_bits=10, frac_bits=0).is_identical(
        fixed_array(
            [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]], int_bits=10, frac_bits=0
        )
    )
    assert fixed_array.from_float(a.T, int_bits=10, frac_bits=0).is_identical(
        fixed_array(
            [[1, 5, 9], [2, 6, 10], [3, 7, 11], [4, 8, 12]], int_bits=10, frac_bits=0
        )
    )

    # `from_array()`
    assert fixed_array.from_array(a, int_bits=10, frac_bits=0).is_identical(
        fixed_array(
            [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]], int_bits=10, frac_bits=0
        )
    )
    assert fixed_array.from_array(a.T, int_bits=10, frac_bits=0).is_identical(
        fixed_array(
            [[1, 5, 9], [2, 6, 10], [3, 7, 11], [4, 8, 12]], int_bits=10, frac_bits=0
        )
    )

    # `__init__()`
    assert fixed_array(a, int_bits=10, frac_bits=0).is_identical(
        fixed_array(
            [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]], int_bits=10, frac_bits=0
        )
    )
    assert fixed_array(a.T, int_bits=10, frac_bits=0).is_identical(
        fixed_array(
            [[1, 5, 9], [2, 6, 10], [3, 7, 11], [4, 8, 12]], int_bits=10, frac_bits=0
        )
    )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_issue_487(fixed_array: type[APyCFixedArray]):
    """
    Test for GitHub issue #487
    https://github.com/apytypes/apytypes/issues/487
    """
    np = pytest.importorskip("numpy")

    fl = 1.2124003311558797e-16
    fl2fxa = fixed_array.from_float([fl], int_bits=1, frac_bits=11)
    npa2fxa = fixed_array.from_array(np.array([fl]), int_bits=1, frac_bits=11)
    assert fl2fxa.is_identical(fixed_array([0], int_bits=1, frac_bits=11))
    assert npa2fxa.is_identical(fixed_array([0], int_bits=1, frac_bits=11))

    fl = 1.25e16
    fl2fxa = fixed_array.from_float([fl], int_bits=60, frac_bits=0)
    npa2fxa = fixed_array.from_array(np.array([fl]), int_bits=60, frac_bits=0)
    assert fl2fxa.to_numpy() == np.array([fl])
    assert npa2fxa.to_numpy() == np.array([fl])

    fl = 1.25e160
    fl2fxa = fixed_array.from_float([fl], int_bits=60, frac_bits=0)
    npa2fxa = fixed_array.from_array(np.array([fl]), int_bits=60, frac_bits=0)
    assert fl2fxa.to_numpy() == np.array([0])
    assert npa2fxa.to_numpy() == np.array([0])


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_fizz_fuzz(fixed_array: type[APyCFixedArray]):
    """
    Tests discovered through some fuzzing
    """
    np = pytest.importorskip("numpy")

    # 1
    fl = [4.06997806e-09]
    a = fixed_array.from_float(fl, int_bits=16, frac_bits=16)
    b = fixed_array.from_array(np.array(fl), int_bits=16, frac_bits=16)
    assert a.is_identical(b)
