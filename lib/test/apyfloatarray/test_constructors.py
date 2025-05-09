import pytest

from apytypes import APyFloatArray


@pytest.mark.float_array
def test_constructor_raises():
    with pytest.raises(ValueError, match=r"APyFloatArray\.__init__: shape mismatch"):
        _ = APyFloatArray([1], [5, 2], [4], 10, 10)
    with pytest.raises(ValueError, match=r"APyFloatArray\.__init__: unexpected type"):
        _ = APyFloatArray([1, 2], [5, 2], [4, "str"], 10, 10)
    with pytest.raises(ValueError, match=r"APyFloatArray\.__init__: unexpected type"):
        _ = APyFloatArray(["foo"], [5], [4], 10, 10)
    with pytest.raises(ValueError, match=r"APyFloatArray\.__init__: unexpected type"):
        _ = APyFloatArray([1], ["foo"], [4], 10, 10)
    with pytest.raises(ValueError, match=r"APyFloatArray\.__init__: unexpected type"):
        _ = APyFloatArray([1], [5], ["foo"], 10, 10)
    with pytest.raises(
        ValueError, match=r"APyFloatArray\.__init__: unexpected type when traversing"
    ):
        _ = APyFloatArray([1.0], [4], [4], 10, 10)
    with pytest.raises(
        ValueError, match=r"APyFloatArray\.__init__: unexpected type when traversing"
    ):
        _ = APyFloatArray([True], [range], [4], 10, 10)
    with pytest.raises(
        ValueError, match=r"APyFloatArray\.__init__: unexpected type when traversing"
    ):
        _ = APyFloatArray([True], [4], [APyFloatArray], 10, 10)
    with pytest.raises(
        ValueError,
        match=r"APyFloatArray\.__init__: exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = APyFloatArray([0], [0], [0], 300, 5)
    with pytest.raises(
        ValueError,
        match=r"APyFloatArray\.__init__: exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = APyFloatArray([0], [0], [0], -300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match=r"APyFloatArray.__init__: mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = APyFloatArray([0], [0], [0], 5, 300)
    with pytest.raises(
        ValueError,
        match=r"APyFloatArray.__init__: mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = APyFloatArray([0], [0], [0], 5, -300)


@pytest.mark.float_array
def test_explicit_constructor():
    arr = APyFloatArray([1], [2], [3], 4, 5)
    assert len(arr) == 1
    assert arr.shape == (1,)
    assert arr.exp_bits == 4
    assert arr.man_bits == 5
    assert arr.bias == 7  # Default when using 4 exponent bits

    arr2d = APyFloatArray(
        [[False, 1], [0, True]], [[2, 3], [2, 3]], [[4, 5], [4, 5]], 6, 7
    )
    assert len(arr2d) == 2
    assert arr2d.shape == (2, 2)
    assert arr2d.exp_bits == 6
    assert arr2d.man_bits == 7
    assert arr2d.bias == 31  # Default for 6 bits


@pytest.mark.float_array
def test_from_float():
    a = APyFloatArray.from_float([1.0, 1.25, 1.5], exp_bits=5, man_bits=2)
    assert a.is_identical(APyFloatArray([0, 0, 0], [15, 15, 15], [0, 1, 2], 5, 2))

    # (2 x 3 matrix) initialized from floating-point values.
    b = APyFloatArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=5,
        man_bits=2,
    )

    assert b.is_identical(
        APyFloatArray(
            [[0, 0, 0], [1, 1, 1]],
            [[15, 16, 16], [17, 17, 17]],
            [[0, 0, 2], [0, 1, 2]],
            5,
            2,
        )
    )


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

    a = APyFloatArray.from_float(anp, 4, 2)

    assert np.all(a.to_numpy() == anp.astype(np.float64))


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
def test_numpy_array_creation(dt):
    np = pytest.importorskip("numpy")
    anp = np.array([[1, 2, 3, 4]], dtype=dt)

    a = APyFloatArray.from_array(anp, 4, 2)

    assert np.all(a.to_numpy() == anp.astype(np.float64))


def test_special_value_numpy_from_double():
    np = pytest.importorskip("numpy")
    a = np.asarray([[1e-323, float("inf")], [float("nan"), 0.0]])
    b = APyFloatArray.from_float(a, 14, 60)
    c = APyFloatArray(
        [[0, 0], [0, 0]],
        [[7118, 16383], [16383, 0]],
        [[0, 0], [1, 0]],
        exp_bits=14,
        man_bits=60,
        bias=8191,
    )
    assert b.is_identical(c)


def test_from_numpy_raises():
    np = pytest.importorskip("numpy")
    a = np.asarray([[1e-323, float("inf")], [float("nan"), 0.0]], dtype="half")
    with pytest.raises(TypeError, match="APyFloatArray::_set_values_from_ndarray"):
        APyFloatArray.from_float(a, 14, 60)


def test_c_striding():
    np = pytest.importorskip("numpy")
    a = np.array([[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]])

    # `from_float()`
    assert APyFloatArray.from_float(a, man_bits=10, exp_bits=10).is_identical(
        APyFloatArray.from_float([[1, 2, 3], [4, 5, 6]], man_bits=10, exp_bits=10)
    )
    assert APyFloatArray.from_float(a.T, man_bits=10, exp_bits=10).is_identical(
        APyFloatArray.from_float([[1, 4], [2, 5], [3, 6]], man_bits=10, exp_bits=10)
    )

    # `from_array()`
    assert APyFloatArray.from_array(a, man_bits=10, exp_bits=10).is_identical(
        APyFloatArray.from_float([[1, 2, 3], [4, 5, 6]], man_bits=10, exp_bits=10)
    )
    assert APyFloatArray.from_array(a.T, man_bits=10, exp_bits=10).is_identical(
        APyFloatArray.from_float([[1, 4], [2, 5], [3, 6]], man_bits=10, exp_bits=10)
    )
