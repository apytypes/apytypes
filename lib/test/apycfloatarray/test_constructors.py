import pytest

from apytypes import APyCFixed, APyCFloat, APyCFloatArray, APyFixed, APyFloat


@pytest.mark.float_array
def test_constructor_raises():
    with pytest.raises(ValueError, match=r"APyCFloatArray\.__init__: shape mismatch"):
        _ = APyCFloatArray([1], [5, 2, 5], [4], 10, 10)
    with pytest.raises(ValueError, match=r"APyCFloatArray\.__init__: unexpected type"):
        _ = APyCFloatArray([1, 2], [5, 2], [4, "str"], 10, 10)
    with pytest.raises(ValueError, match=r"APyCFloatArray\.__init__: unexpected type"):
        _ = APyCFloatArray(["foo"], [5], [4], 10, 10)
    with pytest.raises(ValueError, match=r"APyCFloatArray\.__init__: unexpected type"):
        _ = APyCFloatArray([1], ["foo"], [4], 10, 10)
    with pytest.raises(ValueError, match=r"APyCFloatArray\.__init__: unexpected type"):
        _ = APyCFloatArray([1], [5], ["foo"], 10, 10)
    with pytest.raises(
        ValueError, match=r"APyCFloatArray\.__init__: unexpected type when traversing"
    ):
        _ = APyCFloatArray([1.0], [4], [4], 10, 10)
    with pytest.raises(
        ValueError, match=r"APyCFloatArray\.__init__: unexpected type when traversing"
    ):
        _ = APyCFloatArray([True], [range], [4], 10, 10)
    with pytest.raises(
        ValueError, match=r"APyCFloatArray\.__init__: unexpected type when traversing"
    ):
        _ = APyCFloatArray([True], [4], [APyCFloatArray], 10, 10)
    with pytest.raises(
        ValueError,
        match=r"APyCFloatArray\.__init__: exponent bits must be a non-negative integer "
        + r"less or equal to .. but 300 was given",
    ):
        _ = APyCFloatArray([0], [0], [0], 300, 5)
    with pytest.raises(
        ValueError,
        match=r"APyCFloatArray\.__init__: exponent bits must be a non-negative integer "
        + "less or equal to .. but -300 was given",
    ):
        _ = APyCFloatArray([0], [0], [0], -300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match=r"APyCFloatArray.__init__: mantissa bits must be a non-negative integer "
        + "less or equal to .. but 300 was given",
    ):
        _ = APyCFloatArray([0], [0], [0], 5, 300)
    with pytest.raises(
        ValueError,
        match=r"APyCFloatArray.__init__: mantissa bits must be a non-negative integer "
        + "less or equal to .. but -300 was given",
    ):
        _ = APyCFloatArray([0], [0], [0], 5, -300)


@pytest.mark.float_array
def test_explicit_constructor():
    arr = APyCFloatArray([1], [2], [3], 4, 5)
    assert len(arr) == 1
    assert arr.shape == (1,)
    assert arr.exp_bits == 4
    assert arr.man_bits == 5
    assert arr.bias == 7  # Default when using 4 exponent bits

    arr2d = APyCFloatArray(
        [[False, 1, 1], [0, True, 1]],
        [[2, 3, 4], [2, 3, 4]],
        [[4, 5, 4], [4, 5, 4]],
        6,
        7,
    )
    assert len(arr2d) == 2
    assert arr2d.shape == (2, 3)
    assert arr2d.exp_bits == 6
    assert arr2d.man_bits == 7
    assert arr2d.bias == 31  # Default for 6 bits


@pytest.mark.float_array
def test_from_float():
    a = APyCFloatArray.from_float([1.0, 1.25, 1.5], exp_bits=5, man_bits=2)
    assert a.is_identical(APyCFloatArray([0, 0, 0], [15, 15, 15], [0, 1, 2], 5, 2))

    # (2 x 3 matrix) initialized from floating-point values.
    b = APyCFloatArray.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=5,
        man_bits=2,
    )

    assert b.is_identical(
        APyCFloatArray(
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
def test_numpy_creation(dt: str):
    np = pytest.importorskip("numpy")
    anp = np.array([[1, 2, 3, 4]], dtype=dt)

    a = APyCFloatArray.from_float(anp, 4, 2)

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
        "complex64",
        "complex128",
    ],
)
def test_numpy_array_creation(dt):
    np = pytest.importorskip("numpy")
    anp = np.array([[1, 2, 3, 4]], dtype=dt)

    a = APyCFloatArray.from_array(anp, 4, 2)

    assert np.all(a.to_numpy() == anp.astype(np.complex128))


def test_special_value_numpy_from_double():
    np = pytest.importorskip("numpy")
    a = np.asarray([[1e-323, float("inf")], [float("nan"), 0.0]])
    b = APyCFloatArray.from_float(a, 14, 60)
    c = APyCFloatArray(
        [[(0, 0), (0, 0)], [(0, 0), (0, 0)]],
        [[(7118, 0), (16383, 0)], [(16383, 0), (0, 0)]],
        [[(0, 0), (0, 0)], [(1, 0), (0, 0)]],
        exp_bits=14,
        man_bits=60,
    )
    assert b.is_identical(c)


def test_from_numpy_raises():
    np = pytest.importorskip("numpy")
    a = np.asarray([[1e-323, float("inf")], [float("nan"), 0.0]], dtype="half")
    with pytest.raises(TypeError, match="APyCFloatArray.from_array"):
        APyCFloatArray.from_float(a, 14, 60)


def test_c_striding():
    np = pytest.importorskip("numpy")
    a = np.array([[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]])

    # `from_float()`
    assert APyCFloatArray.from_float(a, man_bits=10, exp_bits=10).is_identical(
        APyCFloatArray.from_float([[1, 2, 3], [4, 5, 6]], man_bits=10, exp_bits=10)
    )
    assert APyCFloatArray.from_float(a.T, man_bits=10, exp_bits=10).is_identical(
        APyCFloatArray.from_float([[1, 4], [2, 5], [3, 6]], man_bits=10, exp_bits=10)
    )

    # `from_array()`
    assert APyCFloatArray.from_array(a, man_bits=10, exp_bits=10).is_identical(
        APyCFloatArray.from_float([[1, 2, 3], [4, 5, 6]], man_bits=10, exp_bits=10)
    )
    assert APyCFloatArray.from_array(a.T, man_bits=10, exp_bits=10).is_identical(
        APyCFloatArray.from_float([[1, 4], [2, 5], [3, 6]], man_bits=10, exp_bits=10)
    )


def test_from_complex():
    fx = APyFixed.from_float(-1.25, int_bits=10, frac_bits=5)
    cfx = APyCFixed.from_complex(-2.0 + 3.5j, int_bits=7, frac_bits=3)
    fl = APyFloat.from_float(-4.125, exp_bits=7, man_bits=8)
    cfl = APyCFloat.from_complex(5.0 - 2.0j, exp_bits=7, man_bits=8)
    arr = APyCFloatArray.from_complex(
        [[1, 2.0, (3.0 + 4.0j), fx], [cfx, fl, cfl, -1.0]], exp_bits=20, man_bits=20
    )
    assert arr.is_identical(
        APyCFloatArray(
            [[(0, 0), (0, 0), (0, 0), (1, 0)], [(1, 0), (1, 0), (0, 1), (1, 0)]],
            [
                [(524287, 0), (524288, 0), (524288, 524289), (524287, 0)],
                [(524288, 524288), (524289, 0), (524289, 524288), (524287, 0)],
            ],
            [
                [(0, 0), (0, 0), (524288, 0), (262144, 0)],
                [(0, 786432), (32768, 0), (262144, 0), (0, 0)],
            ],
            exp_bits=20,
            man_bits=20,
        )
    )
