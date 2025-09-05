from itertools import permutations

import pytest

from apytypes import (
    APyCFloat,
    APyCFloatArray,
    APyFixed,
    APyFloat,
    APyFloatArray,
    QuantizationMode,
)


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_is_identical(float_array: type[APyCFloatArray]):
    a = float_array([1], [2], [3], 4, 5)
    assert a.is_identical(a)

    assert not a.is_identical(float_array([0], [2], [3], 4, 5))
    assert not a.is_identical(float_array([1], [1], [3], 4, 5))
    assert not a.is_identical(float_array([1], [2], [2], 4, 5))
    assert not a.is_identical(float_array([1], [2], [3], 5, 5))
    assert not a.is_identical(float_array([1], [2], [3], 4, 6))
    assert not a.is_identical(float_array([1], [2], [6], 4, 6))  # Same value


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_copy(
    float_array: type[APyCFloatArray],
    float_scalar: type[APyCFloat],
):
    a = float_array([1, 0], [2, 3], [3, 1], 4, 5)
    b = a
    assert a.is_identical(b)
    c = a.copy()
    assert a.is_identical(c)
    a[0] = float_scalar(0, 1, 2, 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_python_copy(
    float_array: type[APyCFloatArray],
    float_scalar: type[APyCFloat],
):
    import copy

    a = float_array([1, 0], [2, 3], [3, 1], 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.copy(a)
    assert a.is_identical(c)
    a[0] = float_scalar(0, 1, 2, 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_python_deepcopy(
    float_array: type[APyCFloatArray],
    float_scalar: type[APyCFloat],
):
    import copy

    a = float_array([1, 0], [2, 3], [3, 1], 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.deepcopy(a)
    assert a.is_identical(c)
    a[0] = float_scalar(0, 1, 2, 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_array_from_float_raises(float_array: type[APyCFloatArray]):
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray.from_(float)|(complex): "
        + r"exponent bits must be a non-negative integer less or equal to .. but 300 "
        + r"was given",
    ):
        _ = float_array.from_float([0], 300, 5)

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray.from_(float)|(complex): "
        + r"exponent bits must be a non-negative integer less or equal to .. but -300 "
        + r"was given",
    ):
        _ = float_array.from_float([0], -300, 5)

    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray.from_(float)|(complex): "
        + r"mantissa bits must be a non-negative integer less or equal to .. but 300 "
        + "was given",
    ):
        _ = float_array.from_float([0], 5, 300)

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray.from_(float)|(complex): "
        + r"mantissa bits must be a non-negative integer less or equal to .. but -300 "
        + "was given",
    ):
        _ = float_array.from_float([0], 5, -300)

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray.from_(float)|(complex): "
        + r"unexpected type when traversing iterable sequence: <class 'str'>",
    ):
        _ = float_array.from_float(["0"], 5, 10)


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_from_float_with_non_floats(float_array: type[APyCFloatArray]):
    a = float_array.from_float([16], exp_bits=4, man_bits=2)
    assert a.is_identical(float_array([0], [11], [0], exp_bits=4, man_bits=2))

    # Should quantize to 16.0
    a = float_array.from_float([17], exp_bits=4, man_bits=2)
    assert a.is_identical(float_array([0], [11], [0], exp_bits=4, man_bits=2))

    # Should quantize to -20.0
    a = float_array.from_float([-19], exp_bits=4, man_bits=2)
    assert a.is_identical(float_array([1], [11], [1], exp_bits=4, man_bits=2))

    # Tie break, should quantize to 16.0
    a = float_array.from_float([18], exp_bits=4, man_bits=2)
    assert a.is_identical(float_array([0], [11], [0], exp_bits=4, man_bits=2))

    # Tie break, should quantize to 24.0
    a = float_array.from_float([22], exp_bits=4, man_bits=2)
    assert a.is_identical(float_array([0], [11], [2], exp_bits=4, man_bits=2))

    # Should quantize to 28.0
    a = float_array.from_float([29], exp_bits=4, man_bits=2)
    assert a.is_identical(float_array([0], [11], [3], exp_bits=4, man_bits=2))

    # Should quantize to 32.0
    a = float_array.from_float([31], exp_bits=4, man_bits=2)
    assert a.is_identical(float_array([0], [12], [0], exp_bits=4, man_bits=2))

    # 152-bit number, should become negative infinity
    a = float_array.from_float(
        [-0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF], exp_bits=4, man_bits=2
    )
    assert a.is_identical(float_array([1], [15], [0], exp_bits=4, man_bits=2))

    # Test with bool and non IEEE-like bias
    a = float_array.from_float([True], exp_bits=4, man_bits=2, bias=4)
    assert a.is_identical(float_array([0], [4], [0], exp_bits=4, man_bits=2, bias=4))

    # Test with big integer
    a = float_array.from_float([2**2047], exp_bits=28, man_bits=2)
    assert a.is_identical(
        float_array([0], [(2047 + (2**27) - 1)], [0], exp_bits=28, man_bits=2)
    )

    # From APyFixed
    assert float_array.from_float(
        [APyFixed(0, bits=3, int_bits=2)], 5, 2, 8
    ).is_identical(float_array([0], [0], [0], 5, 2, 8))  # 0

    assert float_array.from_float([APyFixed(3, bits=3, int_bits=2)], 5, 2).is_identical(
        float_array([0], [15], [2], 5, 2)
    )  # 1.5

    assert float_array.from_float(
        [APyFixed(13, bits=4, int_bits=3)], 5, 2
    ).is_identical(float_array([1], [15], [2], 5, 2))  # -1.5

    assert float_array.from_float(
        [APyFixed(3, bits=3, int_bits=-1)], 7, 3
    ).is_identical(float_array([0], [60], [4], 7, 3))  # 0.1875

    assert float_array.from_float(
        [APyFixed(1, bits=5, int_bits=40)], 5, 2
    ).is_identical(float_array([0], [31], [0], 5, 2))  # Saturate to infinity

    assert float_array.from_float(
        [APyFixed(1, bits=12, int_bits=2)], 4, 3
    ).is_identical(float_array([0], [0], [0], 4, 3))  # Quantize to zero

    assert float_array.from_float(
        [APyFixed(1, bits=10, int_bits=2)], 4, 3
    ).is_identical(float_array([0], [0], [2], 4, 3))  # Subnormal

    assert float_array.from_float(
        [APyFixed(1, int_bits=1, frac_bits=1076)], 11, 54
    ).is_identical(float_array([0], [0], [1], 11, 54))  # Smallest subnormal

    assert float_array.from_float(
        [APyFixed(1, bits=2, int_bits=-1072)], 11, 52
    ).is_identical(float_array([0], [0], [1], 11, 52))  # Smallest subnormal

    # From APyFloat

    # -0 to -0
    assert float_array.from_float([APyFloat(1, 0, 0, 5, 7)], 10, 24, 8).is_identical(
        float_array([1], [0], [0], 10, 24, 8)
    )

    # Cast -inf to -inf
    assert float_array.from_float([APyFloat(1, 31, 0, 5, 7)], 11, 34).is_identical(
        float_array([1], [2047], [0], 11, 34)
    )

    # Cast from big number becomes infinity
    assert float_array.from_float([APyFloat(1, 30, 4, 5, 3)], 4, 3).is_identical(
        float_array([1], [15], [0], 4, 3)
    )

    # Cast where result becomes subnormal
    assert float_array.from_float([APyFloat(0, 8, 0, 5, 2)], 4, 3).is_identical(
        float_array([0], [0], [4], 4, 3)
    )


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_array_cast_raises(float_array: type[APyCFloatArray]):
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray\.cast: "
        + r"exponent bits must be a non-negative integer less or equal to .. but 300 "
        + r"was given",
    ):
        _ = float_array([0], [0], [0], 5, 5).cast(300, 5)

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray\.cast: "
        + r"exponent bits must be a non-negative integer less or equal to .. but -300 "
        + r"was given",
    ):
        _ = float_array([0], [0], [0], 5, 5).cast(-300, 5)

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray\.cast: "
        + r"mantissa bits must be a non-negative integer less or equal to .. but 300 "
        + r"was given",
    ):
        _ = float_array([0], [0], [0], 5, 5).cast(5, 300)

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray\.cast: "
        + r"mantissa bits must be a non-negative integer less or equal to .. but -300 "
        + r"was given",
    ):
        _ = float_array([0], [0], [0], 5, 5).cast(5, -300)


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_array_from_array_raises(float_array: type[APyCFloatArray]):
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray\.cast: "
        + r"exponent bits must be a non-negative integer less or equal to .. but 300 "
        + r"was given",
    ):
        _ = float_array.from_array(np.array([[1, 2, 3, 4]]), 5, 5).cast(300, 5)

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray\.cast: "
        + r"exponent bits must be a non-negative integer less or equal to .. but -300 "
        + r"was given",
    ):
        _ = float_array.from_array(np.array([[1, 2, 3, 4]]), 5, 5).cast(-300, 5)

    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray\.cast: "
        + r"mantissa bits must be a non-negative integer less or equal to .. but 300 "
        + r"was given",
    ):
        _ = float_array.from_array(np.array([[1, 2, 3, 4]]), 5, 5).cast(5, 300)

    with pytest.raises(
        ValueError,
        match=r"APyC?FloatArray\.cast: "
        + r"mantissa bits must be a non-negative integer less or equal to .. but -300 "
        + r"was given",
    ):
        _ = float_array.from_array(np.array([[1, 2, 3, 4]]), 5, 5).cast(5, -300)


@pytest.mark.float_array
def test_from_bits():
    # Test raises
    with pytest.raises(
        ValueError,
        match=r"APyFloatArray\.from_bits: exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        APyFloatArray.from_bits([0], 300, 5)

    with pytest.raises(
        ValueError, match=r"APyFloatArray\.from_bits: unexpected type when traversing"
    ):
        APyFloatArray.from_bits(["0"], 5, 10)

    with pytest.raises(
        ValueError,
        match="Invalid",
    ):
        APyFloatArray.from_bits([1, 10, 1.0, 5], 5, 10)

    # 1-D
    assert APyFloatArray.from_bits(range(2**8), 5, 2).to_bits() == list(range(2**8))

    # 2-D
    assert APyFloatArray.from_bits([[65, 66], [191, 190]], 5, 2, 9).is_identical(
        APyFloatArray([[0, 0], [1, 1]], [[16, 16], [15, 15]], [[1, 2], [3, 2]], 5, 2, 9)
    )

    # Test long Python integer
    val = 0xF000000FFFFFFFFFF
    APyFloatArray.from_bits([val], 27, 40).is_identical(
        APyFloatArray([1], [117440512], [1099511627775], 27, 40)
    )


@pytest.mark.float_array
def test_from_bits_numpy():
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    # Test raise
    with pytest.raises(
        TypeError,
        match="APyFloatArray::_set_bits_from_ndarray()",
    ):
        APyFloatArray.from_bits(np.asarray([1.0]), 8, 15)

    # 16-bit, (2, 2)
    vals = np.asarray([[65, 66], [191, 190]], dtype=np.int16)
    assert APyFloatArray.from_bits(vals, 5, 2, 9).is_identical(
        APyFloatArray([[0, 0], [1, 1]], [[16, 16], [15, 15]], [[1, 2], [3, 2]], 5, 2, 9)
    )

    # 64-bit, (4, 4, 4)
    a = APyFloatArray.from_float(
        [1.0, -8564651213, 2**-1070, float("inf")] * 16, 11, 52
    ).reshape((4, 4, 4))
    nparr = a.to_bits(numpy=True)
    assert APyFloatArray.from_bits(nparr, 11, 52).is_identical(a)


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_to_numpy(float_array: type[APyCFloatArray]):
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    assert np.array_equal(float_array([], [], [], 5, 5).to_numpy(), np.array([]))
    assert np.array_equal(
        float_array([0], [15], [1], 5, 2).to_numpy(), np.array([1.25])
    )

    float_seq = [
        [[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]],
        [[7.0, 8.0, 9.0], [10.0, 11.0, 12.0]],
    ]
    fp_arr = float_array.from_float(float_seq, 10, 10)
    assert fp_arr.to_numpy().shape == (2, 2, 3)
    assert np.array_equal(fp_arr.to_numpy(), np.array(float_seq))


@pytest.mark.parametrize(
    "dt",
    [
        "int8",
        "int16",
        "int32",
        "int64",
        "uint8",
        "uint16",
        "uint32",
        "uint64",
        "float32",
        "float64",
    ],
)
@pytest.mark.parametrize(
    "copy",
    [
        False,
        True,
    ],
)
def test_to_numpy_args_real(dt: str, copy: bool):
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    # Test with different dtypes
    fp_arr = APyFloatArray.from_float(range(5), 10, 10)

    if copy:
        assert np.array_equal(
            fp_arr.to_numpy(dtype=dt, copy=copy), np.array(range(5), dtype=dt)
        )
    else:
        with pytest.raises(
            ValueError,
            match=r"APyFloatArray\.to_numpy: copy must be True",
        ):
            _ = fp_arr.to_numpy(dtype=dt, copy=copy)


@pytest.mark.parametrize(
    "dt",
    [
        "complex64",
        "complex128",
    ],
)
@pytest.mark.parametrize(
    "copy",
    [
        False,
        True,
    ],
)
def test_to_numpy_args_complex(dt: str, copy: bool):
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    # Test with different dtypes
    fp_arr = APyCFloatArray.from_float(range(5), 10, 10)

    if copy:
        assert np.array_equal(
            fp_arr.to_numpy(dtype=dt, copy=copy), np.array(range(5), dtype=dt)
        )
    else:
        with pytest.raises(
            ValueError,
            match=r"APyCFloatArray\.to_numpy: copy must be True",
        ):
            _ = fp_arr.to_numpy(dtype=dt, copy=copy)


@pytest.mark.float_array
def test_iterator():
    fx_array = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)
    iterator = iter(fx_array)
    assert next(iterator).is_identical(APyFloat.from_float(1, exp_bits=10, man_bits=10))
    assert next(iterator).is_identical(APyFloat.from_float(2, exp_bits=10, man_bits=10))


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_len(float_array: type[APyCFloatArray]):
    fp_array = float_array.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)
    assert len(fp_array) == 6


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_cast(float_array: type[APyCFloatArray]):
    fp_array = float_array.from_float([[5, 4], [1, -7]], exp_bits=10, man_bits=15)
    answer = float_array.from_float([[5, 4], [1, -7]], exp_bits=5, man_bits=2)
    assert fp_array.cast(5, 2).is_identical(answer)

    fp_array = float_array.from_float(
        [[1000, -1000], [1.015625, float("nan")]], exp_bits=10, man_bits=15
    )
    answer = float_array.from_float(
        [[float("inf"), float("-inf")], [1, float("nan")]], exp_bits=4, man_bits=3
    )
    assert fp_array.cast(4, 3).is_identical(answer)

    fp_array = float_array.from_float([1.125, -1.875], exp_bits=10, man_bits=15)
    answer = float_array.from_float([1, -1.75], exp_bits=5, man_bits=2)
    assert fp_array.cast(5, 2, quantization=QuantizationMode.TO_ZERO).is_identical(
        answer
    )

    fp_array = float_array.from_float([[5, 4], [1, -7]], exp_bits=10, man_bits=15)
    answer = float_array.from_float([[5, 4], [1, -7]], exp_bits=4, man_bits=15)
    assert fp_array.cast(4).is_identical(answer)
    answer = float_array.from_float([[5, 4], [1, -7]], exp_bits=10, man_bits=5)
    assert fp_array.cast(man_bits=5).is_identical(answer)


@pytest.mark.float_array
@pytest.mark.parametrize("exp_bits", range(4, 8))
@pytest.mark.parametrize("man_bits", range(7, 12))
@pytest.mark.parametrize(
    "quantization",
    [
        QuantizationMode.RND_CONV,
        QuantizationMode.JAM_UNBIASED,
        QuantizationMode.RND_CONV_ODD,
        QuantizationMode.RND_ZERO,
        QuantizationMode.RND_INF,
        QuantizationMode.RND_MIN_INF,
        QuantizationMode.TIES_AWAY,
        QuantizationMode.TIES_EVEN,
        QuantizationMode.TIES_NEG,
        QuantizationMode.TIES_POS,
        QuantizationMode.TO_AWAY,
        QuantizationMode.TO_ZERO,
        QuantizationMode.TO_NEG,
        QuantizationMode.TO_POS,
    ],
)
@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_cast_array_parametrized(
    exp_bits: int,
    man_bits: int,
    quantization: QuantizationMode,
    float_array: type[APyCFloatArray],
    float_scalar: type[APyCFloat],
):
    vals = [0, float("inf"), float("nan"), 1e-2, 1, 100, -1e-2, -1, -100]
    fpvals = [float_scalar.from_float(v, exp_bits, man_bits) for v in vals]
    fparray = float_array.from_float(vals, exp_bits, man_bits)

    # Check that starting values are the same
    for i, v in enumerate(fpvals):
        assert v.is_identical(fparray[i])

    # Decrease word length
    casted_fparray = fparray.cast(exp_bits - 1, man_bits - 2, quantization=quantization)
    for i, v in enumerate(fpvals):
        assert v.cast(
            exp_bits - 1, man_bits - 2, quantization=quantization
        ).is_identical(casted_fparray[i])

    # Decrease mantissa word length
    casted_fparray = fparray.cast(exp_bits, man_bits - 2, quantization=quantization)
    for i, v in enumerate(fpvals):
        assert v.cast(exp_bits, man_bits - 2, quantization=quantization).is_identical(
            casted_fparray[i]
        )

    # Decrease exponent word length
    casted_fparray = fparray.cast(exp_bits - 1, man_bits, quantization=quantization)
    for i, v in enumerate(fpvals):
        assert v.cast(exp_bits - 1, man_bits, quantization=quantization).is_identical(
            casted_fparray[i]
        )

    # Increase word length
    casted_fparray = fparray.cast(exp_bits + 1, man_bits + 2, quantization=quantization)
    for i, v in enumerate(fpvals):
        assert v.cast(
            exp_bits + 1, man_bits + 2, quantization=quantization
        ).is_identical(casted_fparray[i])

    # No change in word length
    casted_fparray = fparray.cast(exp_bits, man_bits, quantization=quantization)
    for i, v in enumerate(fpvals):
        assert v.cast(exp_bits, man_bits, quantization=quantization).is_identical(
            casted_fparray[i]
        )


@pytest.mark.float_array
def test_python_sum():
    fx_array = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)
    assert sum(fx_array) == 21
    assert sum(fx_array).is_identical(APyFloat.from_float(21, 10, 10))


def test_round_trip_conversion():
    # Skip this test if `NumPy` is not present on the machine
    pytest.importorskip("numpy")
    for exp in reversed(range(15)):  # Skip nan/inf
        for man in reversed(range(16)):
            a = APyFloatArray([0], [exp], [man], 4, 4)
            assert (APyFloatArray.from_float(a.to_numpy(), 4, 4)).is_identical(a)


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_squeeze(float_array: type[APyCFloatArray]):
    a = float_array.from_float([[1], [2], [3]], exp_bits=10, man_bits=10)
    b = a.squeeze()
    assert (b).is_identical(float_array.from_float([1, 2, 3], exp_bits=10, man_bits=10))
    c = float_array.from_float(
        [[[1, 2], [3, 4], [5, 6], [7, 8]]], exp_bits=10, man_bits=10
    )
    d = c.squeeze()
    assert (d).is_identical(
        float_array.from_float(
            [[1, 2], [3, 4], [5, 6], [7, 8]], exp_bits=10, man_bits=10
        )
    )
    e = float_array.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    f = e.squeeze()
    assert f.is_identical(e)
    g = float_array.from_float([[[[[[[[2]]]]]]]], exp_bits=10, man_bits=10)
    h = g.squeeze()
    assert h.is_identical(float_array.from_float([2], exp_bits=10, man_bits=10))
    i = float_array.from_float([], exp_bits=10, man_bits=10)
    j = float_array.from_float([[]], exp_bits=10, man_bits=10)
    k = i.squeeze()
    z = j.squeeze()
    assert k.is_identical(i)
    assert z.is_identical(i)
    m = float_array.from_float([[1], [2], [3]], exp_bits=10, man_bits=10)
    with pytest.raises(ValueError, match=r"APyC?FloatArray.squeeze: cannot squeeze"):
        _ = m.squeeze(axis=0)
    m1 = m.squeeze(axis=1)
    assert m1.is_identical(float_array.from_float([1, 2, 3], exp_bits=10, man_bits=10))
    with pytest.raises(IndexError):
        _ = m.squeeze(axis=2)

    n = float_array.from_float([[[[[[[[2]]]]]]]], exp_bits=10, man_bits=10)
    o = n.squeeze((0, 1, 2, 3))
    assert o.is_identical(float_array.from_float([[[[2]]]], exp_bits=10, man_bits=10))
    p = n.squeeze((0, 1, 3))
    assert p.is_identical(float_array.from_float([[[[[2]]]]], exp_bits=10, man_bits=10))
    q = float_array.from_float([[[1]], [[2]], [[3]], [[4]]], exp_bits=10, man_bits=10)
    with pytest.raises(ValueError, match="APyC?FloatArray.squeeze: cannot squeeze"):
        _ = q.squeeze((0, 1, 2))
    with pytest.raises(IndexError):
        _ = m.squeeze((1, 4))


@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_sum(
    float_array: type[APyCFloatArray],
    float_scalar: type[APyCFloat],
):
    a = float_array.from_float(
        [[1, 2], [3, 4], [5, 6], [7, 8]], exp_bits=10, man_bits=10
    )
    b = a.sum()
    assert b.is_identical(float_scalar.from_float(36, exp_bits=10, man_bits=10))
    c = float_array.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    d = c.sum((0, 1))
    e = c.sum(0)
    f = c.sum(1)
    assert d.is_identical(float_scalar.from_float(15, exp_bits=10, man_bits=10))
    assert e.is_identical(float_array.from_float([3, 5, 7], exp_bits=10, man_bits=10))
    assert f.is_identical(float_array.from_float([3, 12], exp_bits=10, man_bits=10))

    g = float_array.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    h = g.sum(0)
    assert h.is_identical(float_array.from_float([3, 5, 7], exp_bits=10, man_bits=10))
    j = float_array.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    k = j.sum(0)
    assert k.is_identical(float_array.from_float([3, 5, 7], exp_bits=10, man_bits=10))

    # test some float and negative summation
    j = float_array.from_float([0.25, 1.375, 3.25], exp_bits=10, man_bits=10)
    k = j.sum()
    assert k.is_identical(float_scalar.from_float(4.875, exp_bits=10, man_bits=10))
    m = float_array.from_float([0.25, 1.25, 3.25], exp_bits=10, man_bits=10)
    n = m.sum()
    assert n.is_identical(float_scalar.from_float(4.75, exp_bits=10, man_bits=10))

    o = float_array.from_float([[-1, -2], [-3, -4]], exp_bits=10, man_bits=10)
    p = o.sum(1)
    assert p.is_identical(float_array.from_float([-3, -7], exp_bits=10, man_bits=10))

    q = float_array.from_float([[-1, -2], [1, 2]], exp_bits=10, man_bits=10)
    r = q.sum(0)
    assert r.is_identical(float_array.from_float([0, 0], exp_bits=10, man_bits=10))

    m = float_array.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    with pytest.raises(IndexError):
        _ = m.sum(1)

    n = float_array.from_float(
        [[[1, 2], [3, 4]], [[5, 6], [7, 8]]], exp_bits=10, man_bits=10
    )
    o = n.sum((0, 2))
    assert o.is_identical(float_array.from_float([14, 22], exp_bits=10, man_bits=10))

    x = float_array.from_float(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    y = x.sum(1)
    z = x.sum(2)
    assert y.is_identical(
        float_array.from_float(
            [[[4, 6], [8, 10]], [[20, 22], [24, 26]]], exp_bits=10, man_bits=10
        )
    )
    assert z.is_identical(
        float_array.from_float(
            [[[2, 4], [10, 12]], [[18, 20], [26, 28]]], exp_bits=10, man_bits=10
        )
    )
    w = x.sum((1, 3))
    assert w.is_identical(
        float_array.from_float([[10, 18], [42, 50]], exp_bits=10, man_bits=10)
    )


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_cumsum(
    float_array: type[APyCFloatArray],
):
    a = float_array.from_float([[1, 2, 3], [4, 5, 6]], exp_bits=10, man_bits=10)
    b = a.cumsum()
    assert b.is_identical(
        float_array.from_float([1, 3, 6, 10, 15, 21], exp_bits=10, man_bits=10)
    )
    c = a.cumsum(0)
    assert c.is_identical(
        float_array.from_float([[1, 2, 3], [5, 7, 9]], exp_bits=10, man_bits=10)
    )
    d = a.cumsum(1)
    assert d.is_identical(
        float_array.from_float([[1, 3, 6], [4, 9, 15]], exp_bits=10, man_bits=10)
    )
    e = float_array.from_float(
        [[[1, 2], [3, 4]], [[5, 6], [7, 8]]], exp_bits=10, man_bits=10
    )
    f = e.cumsum()
    g = e.cumsum(0)
    h = e.cumsum(1)
    i = e.cumsum(2)
    assert f.is_identical(
        float_array.from_float([1, 3, 6, 10, 15, 21, 28, 36], exp_bits=10, man_bits=10)
    )
    assert g.is_identical(
        float_array.from_float(
            [[[1, 2], [3, 4]], [[6, 8], [10, 12]]], exp_bits=10, man_bits=10
        )
    )
    assert h.is_identical(
        float_array.from_float(
            [[[1, 2], [4, 6]], [[5, 6], [12, 14]]], exp_bits=10, man_bits=10
        )
    )
    assert i.is_identical(
        float_array.from_float(
            [[[1, 3], [3, 7]], [[5, 11], [7, 15]]], exp_bits=10, man_bits=10
        )
    )
    with pytest.raises(IndexError):
        _ = e.cumsum(4)

    k = float_array.from_float([[0.25, 0.25], [0.25, 0.25]], exp_bits=10, man_bits=10)
    m = k.cumsum()
    assert m.is_identical(
        float_array.from_float([0.25, 0.5, 0.75, 1], exp_bits=10, man_bits=10)
    )

    o = float_array.from_float([[-1, 2], [-3, 4]], exp_bits=10, man_bits=10)
    p = o.cumsum()
    assert p.is_identical(
        float_array.from_float([-1, 1, -2, 2], exp_bits=10, man_bits=10)
    )

    x = float_array.from_float(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    y = x.cumsum(1)
    z = x.cumsum(2)
    assert y.is_identical(
        float_array.from_float(
            [
                [[[0, 1], [2, 3]], [[4, 6], [8, 10]]],
                [[[8, 9], [10, 11]], [[20, 22], [24, 26]]],
            ],
            exp_bits=10,
            man_bits=10,
        )
    )
    assert z.is_identical(
        float_array.from_float(
            [
                [[[0, 1], [2, 4]], [[4, 5], [10, 12]]],
                [[[8, 9], [18, 20]], [[12, 13], [26, 28]]],
            ],
            exp_bits=10,
            man_bits=10,
        )
    )


@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_nansum(
    float_array: type[APyCFloatArray],
    float_scalar: type[APyCFloat],
):
    nan = float("nan")
    a = float_array.from_float(
        [[nan, 2], [3, 4], [5, 6], [7, 8]], exp_bits=10, man_bits=10
    )
    b = a.nansum()
    assert b.is_identical(float_scalar.from_float(35, exp_bits=10, man_bits=10))
    c = float_array.from_float([[0, 1, nan], [3, 4, 5]], exp_bits=10, man_bits=10)
    d = c.nansum((0, 1))
    e = c.nansum(0)
    f = c.nansum(1)
    assert d.is_identical(float_scalar.from_float(13, exp_bits=10, man_bits=10))
    assert e.is_identical(float_array.from_float([3, 5, 5], exp_bits=10, man_bits=10))
    assert f.is_identical(float_array.from_float([1, 12], exp_bits=10, man_bits=10))

    g = float_array.from_float([[0, 1, 2], [nan, 4, 5]], exp_bits=10, man_bits=10)
    h = g.nansum(0)
    assert h.is_identical(float_array.from_float([0, 5, 7], exp_bits=10, man_bits=10))
    j = float_array.from_float([[0, 1, 2], [nan, nan, nan]], exp_bits=10, man_bits=10)
    k = j.nansum(0)
    assert k.is_identical(float_array.from_float([0, 1, 2], exp_bits=10, man_bits=10))

    # test some float and negative summation
    j = float_array.from_float([0.2, 1.4, 3.3], exp_bits=10, man_bits=10)
    k = j.nansum()
    assert k.is_identical(float_scalar.from_float(4.904, exp_bits=10, man_bits=10))
    m = float_array.from_float([0.333333, 1.333333, 3.33333], exp_bits=10, man_bits=10)
    n = m.nansum()
    assert n.is_identical(float_scalar.from_float(5, exp_bits=10, man_bits=10))

    o = float_array.from_float([[-1, -2], [-3, nan]], exp_bits=10, man_bits=10)
    p = o.nansum(1)
    assert p.is_identical(float_array.from_float([-3, -3], exp_bits=10, man_bits=10))

    q = float_array.from_float([[-1, -2], [1, nan]], exp_bits=10, man_bits=10)
    r = q.nansum(0)
    assert r.is_identical(float_array.from_float([0, -2], exp_bits=10, man_bits=10))

    m = float_array.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    with pytest.raises(IndexError):
        _ = m.nansum(1)

    n = float_array.from_float(
        [[[1, 2], [3, nan]], [[5, 6], [7, 8]]], exp_bits=10, man_bits=10
    )
    o = n.nansum((0, 2))
    assert o.is_identical(float_array.from_float([14, 18], exp_bits=10, man_bits=10))

    x = float_array.from_float(
        [
            [[[nan, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    y = x.nansum(1)
    z = x.nansum(2)
    assert y.is_identical(
        float_array.from_float(
            [[[4, 6], [8, 10]], [[20, 22], [24, 26]]], exp_bits=10, man_bits=10
        )
    )
    assert z.is_identical(
        float_array.from_float(
            [[[2, 4], [10, 12]], [[18, 20], [26, 28]]], exp_bits=10, man_bits=10
        )
    )
    w = x.nansum((1, 3))
    assert w.is_identical(
        float_array.from_float([[10, 18], [42, 50]], exp_bits=10, man_bits=10)
    )


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_nancumsum(
    float_array: type[APyCFloatArray],
):
    nan = float("nan")
    a = float_array.from_float([[1, 2, 3], [4, 5, nan]], exp_bits=10, man_bits=10)
    b = a.nancumsum()
    assert b.is_identical(
        float_array.from_float([1, 3, 6, 10, 15, 15], exp_bits=10, man_bits=10)
    )
    c = a.nancumsum(0)
    assert c.is_identical(
        float_array.from_float([[1, 2, 3], [5, 7, 3]], exp_bits=10, man_bits=10)
    )
    d = a.nancumsum(1)
    assert d.is_identical(
        float_array.from_float([[1, 3, 6], [4, 9, 9]], exp_bits=10, man_bits=10)
    )
    e = float_array.from_float(
        [[[1, 2], [3, 4]], [[nan, nan], [7, 8]]], exp_bits=10, man_bits=10
    )
    f = e.nancumsum()
    g = e.nancumsum(0)
    h = e.nancumsum(1)
    i = e.nancumsum(2)
    assert f.is_identical(
        float_array.from_float([1, 3, 6, 10, 10, 10, 17, 25], exp_bits=10, man_bits=10)
    )
    assert g.is_identical(
        float_array.from_float(
            [[[1, 2], [3, 4]], [[1, 2], [10, 12]]], exp_bits=10, man_bits=10
        )
    )
    assert h.is_identical(
        float_array.from_float(
            [[[1, 2], [4, 6]], [[0, 0], [7, 8]]], exp_bits=10, man_bits=10
        )
    )
    assert i.is_identical(
        float_array.from_float(
            [[[1, 3], [3, 7]], [[0, 0], [7, 15]]], exp_bits=10, man_bits=10
        )
    )
    with pytest.raises(IndexError):
        _ = e.nancumsum(4)

    k = float_array.from_float([[0.25, 0.25], [0.25, nan]], exp_bits=10, man_bits=10)
    m = k.nancumsum()
    assert m.is_identical(
        float_array.from_float([0.25, 0.5, 0.75, 0.75], exp_bits=10, man_bits=10)
    )

    o = float_array.from_float([[-1, 2], [nan, 4]], exp_bits=10, man_bits=10)
    p = o.nancumsum()
    assert p.is_identical(
        float_array.from_float([-1, 1, 1, 5], exp_bits=10, man_bits=10)
    )

    x = float_array.from_float(
        [
            [[[nan, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    y = x.nancumsum(1)
    z = x.nancumsum(2)
    assert y.is_identical(
        float_array.from_float(
            [
                [[[0, 1], [2, 3]], [[4, 6], [8, 10]]],
                [[[8, 9], [10, 11]], [[20, 22], [24, 26]]],
            ],
            exp_bits=10,
            man_bits=10,
        )
    )
    assert z.is_identical(
        float_array.from_float(
            [
                [[[0, 1], [2, 4]], [[4, 5], [10, 12]]],
                [[[8, 9], [18, 20]], [[12, 13], [26, 28]]],
            ],
            exp_bits=10,
            man_bits=10,
        )
    )


@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_prod(
    float_array: type[APyCFloatArray],
    float_scalar: type[APyCFloat],
):
    a = float_array.from_float(
        [[1, 2], [3, 4], [5, 6], [7, 8]], exp_bits=10, man_bits=10
    )
    b = a.prod()
    assert b.is_identical(float_scalar.from_float(40320, exp_bits=10, man_bits=10))
    c = float_array.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    d = c.prod((0, 1))
    e = c.prod(0)
    f = c.prod(1)
    assert d.is_identical(float_scalar.from_float(0, exp_bits=10, man_bits=10))
    assert e.is_identical(float_array.from_float([0, 4, 10], exp_bits=10, man_bits=10))
    assert f.is_identical(float_array.from_float([0, 60], exp_bits=10, man_bits=10))
    g = float_array.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    h = g.prod(0)
    assert h.is_identical(float_array.from_float([0, 4, 10], exp_bits=10, man_bits=10))
    j = float_array.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    k = j.prod(0)
    assert k.is_identical(float_array.from_float([0, 4, 10], exp_bits=10, man_bits=10))

    # test some float and negative multiplication
    j = float_array.from_float([0.25, 8], exp_bits=10, man_bits=10)
    k = j.prod()
    assert k.is_identical(float_scalar.from_float(2, exp_bits=10, man_bits=10))

    o = float_array.from_float([[-1, -2], [-3, -4]], exp_bits=10, man_bits=10)
    p = o.prod(1)
    assert p.is_identical(
        float_array.from_float([2, 12], exp_bits=10, man_bits=10), ignore_zero_sign=True
    )

    q = float_array.from_float([[-1, -2], [1, 2]], exp_bits=10, man_bits=10)
    r = q.prod(0)
    assert r.is_identical(float_array.from_float([-1, -4], exp_bits=10, man_bits=10))

    m = float_array.from_float([1, 2, 3], exp_bits=10, man_bits=10)
    with pytest.raises(IndexError):
        _ = m.prod(1)

    n = float_array.from_float([[0.25, 0.5]], exp_bits=10, man_bits=10)
    o = n.prod(1)
    assert o.is_identical(float_array.from_float([0.125], exp_bits=10, man_bits=10))

    n = float_array.from_float(
        [[[1, 2], [3, 4]], [[5, 6], [7, 8]]], exp_bits=10, man_bits=10
    )
    o = n.prod((0, 2))
    assert o.is_identical(float_array.from_float([60, 672], exp_bits=10, man_bits=10))

    x = float_array.from_float(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    y = x.prod(1)
    z = x.prod(2)
    assert y.is_identical(
        float_array.from_float(
            [[[0, 5], [12, 21]], [[96, 117], [140, 165]]], exp_bits=10, man_bits=10
        )
    )
    assert z.is_identical(
        float_array.from_float(
            [[[0, 3], [24, 35]], [[80, 99], [168, 195]]], exp_bits=10, man_bits=10
        )
    )
    w = x.prod((1, 3))
    assert w.is_identical(
        float_array.from_float([[0, 252], [11232, 23100]], exp_bits=10, man_bits=10)
    )


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_cumprod(float_array: type[APyCFloatArray]):
    a = float_array.from_float([[1, 2, 3], [4, 5, 6]], exp_bits=10, man_bits=10)
    b = a.cumprod()
    assert b.is_identical(
        float_array.from_float([1, 2, 6, 24, 120, 720], exp_bits=10, man_bits=10)
    )
    c = a.cumprod(0)
    assert c.is_identical(
        float_array.from_float([[1, 2, 3], [4, 10, 18]], exp_bits=10, man_bits=10)
    )
    d = a.cumprod(1)
    assert d.is_identical(
        float_array.from_float([[1, 2, 6], [4, 20, 120]], exp_bits=10, man_bits=10)
    )
    e = float_array.from_float(
        [[[1, 2], [3, 4]], [[5, 6], [7, 8]]], exp_bits=10, man_bits=10
    )
    f = e.cumprod()
    g = e.cumprod(0)
    h = e.cumprod(1)
    i = e.cumprod(2)
    assert f.is_identical(
        float_array.from_float(
            [1, 2, 6, 24, 120, 720, 5040, 40320], exp_bits=10, man_bits=10
        )
    )
    assert g.is_identical(
        float_array.from_float(
            [[[1, 2], [3, 4]], [[5, 12], [21, 32]]], exp_bits=10, man_bits=10
        )
    )
    assert h.is_identical(
        float_array.from_float(
            [[[1, 2], [3, 8]], [[5, 6], [35, 48]]], exp_bits=10, man_bits=10
        )
    )
    assert i.is_identical(
        float_array.from_float(
            [[[1, 2], [3, 12]], [[5, 30], [7, 56]]], exp_bits=10, man_bits=10
        )
    )
    with pytest.raises(IndexError):
        _ = e.cumprod(4)

    k = float_array.from_float([[0.25, 0.5], [1, 2]], exp_bits=10, man_bits=10)
    m = k.cumprod()
    assert m.is_identical(
        float_array.from_float([0.25, 0.125, 0.125, 0.25], exp_bits=10, man_bits=10)
    )

    g = float_array.from_float([[0, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    h = g.cumprod(0)
    assert h.is_identical(
        float_array.from_float([[0, 1, 2], [0, 4, 10]], exp_bits=10, man_bits=10)
    )
    j = float_array.from_float([[1, -1, 2], [-3, 4, -5]], exp_bits=10, man_bits=10)
    k = j.cumprod()
    assert k.is_identical(
        float_array.from_float([1, -1, -2, 6, 24, -120], exp_bits=10, man_bits=10),
        ignore_zero_sign=True,
    )

    o = float_array.from_float([[-1, 2], [-3, 4]], exp_bits=10, man_bits=10)
    p = o.cumprod()
    assert p.is_identical(
        float_array.from_float([-1, -2, 6, 24], exp_bits=10, man_bits=10),
        ignore_zero_sign=True,
    )

    x = float_array.from_float(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    y = x.cumprod(1)
    z = x.cumprod(2)
    assert y.is_identical(
        float_array.from_float(
            [
                [[[0, 1], [2, 3]], [[0, 5], [12, 21]]],
                [[[8, 9], [10, 11]], [[96, 117], [140, 165]]],
            ],
            exp_bits=10,
            man_bits=10,
        )
    )
    assert z.is_identical(
        float_array.from_float(
            [
                [[[0, 1], [0, 3]], [[4, 5], [24, 35]]],
                [[[8, 9], [80, 99]], [[12, 13], [168, 195]]],
            ],
            exp_bits=10,
            man_bits=10,
        )
    )


@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
def test_nanprod(
    float_array: type[APyCFloatArray],
    float_scalar: type[APyCFloat],
):
    nan = float("nan")
    a = float_array.from_float(
        [[1, 2], [3, 4], [5, 6], [7, nan]], exp_bits=10, man_bits=10
    )
    b = a.nanprod()
    assert b.is_identical(float_scalar.from_float(5040, exp_bits=10, man_bits=10))
    c = float_array.from_float([[0, 1, 2], [3, nan, 5]], exp_bits=10, man_bits=10)
    d = c.nanprod((0, 1))
    e = c.nanprod(0)
    f = c.nanprod(1)
    assert d.is_identical(float_scalar.from_float(0, exp_bits=10, man_bits=10))
    assert e.is_identical(float_array.from_float([0, 1, 10], exp_bits=10, man_bits=10))
    assert f.is_identical(float_array.from_float([0, 15], exp_bits=10, man_bits=10))
    g = float_array.from_float([[nan, 1, 2], [3, 4, 5]], exp_bits=10, man_bits=10)
    h = g.nanprod(0)
    assert h.is_identical(float_array.from_float([3, 4, 10], exp_bits=10, man_bits=10))

    # test some float and negative multiplication
    j = float_array.from_float([0.25, 8, nan], exp_bits=10, man_bits=10)
    k = j.nanprod()
    assert k.is_identical(float_scalar.from_float(2, exp_bits=10, man_bits=10))

    o = float_array.from_float([[-1, nan], [-3, -4]], exp_bits=10, man_bits=10)
    p = o.nanprod(1)
    assert p.is_identical(
        float_array.from_float([-1, 12], exp_bits=10, man_bits=10),
        ignore_zero_sign=True,
    )

    q = float_array.from_float([[-1, -2], [1, nan]], exp_bits=10, man_bits=10)
    r = q.nanprod(0)
    assert r.is_identical(float_array.from_float([-1, -2], exp_bits=10, man_bits=10))

    m = float_array.from_float([1, 2, nan], exp_bits=10, man_bits=10)
    with pytest.raises(IndexError):
        _ = m.nanprod(1)

    n = float_array.from_float(
        [[[1, 2], [3, 4]], [[5, nan], [7, 8]]], exp_bits=10, man_bits=10
    )
    o = n.nanprod((0, 2))
    assert o.is_identical(float_array.from_float([10, 672], exp_bits=10, man_bits=10))

    x = float_array.from_float(
        [
            [[[0, nan], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    y = x.nanprod(1)
    z = x.nanprod(2)
    assert y.is_identical(
        float_array.from_float(
            [[[0, 5], [12, 21]], [[96, 117], [140, 165]]], exp_bits=10, man_bits=10
        )
    )
    assert z.is_identical(
        float_array.from_float(
            [[[0, 3], [24, 35]], [[80, 99], [168, 195]]], exp_bits=10, man_bits=10
        )
    )
    w = x.nanprod((1, 3))
    assert w.is_identical(
        float_array.from_float([[0, 252], [11232, 23100]], exp_bits=10, man_bits=10)
    )


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_nancumprod(float_array: type[APyCFloatArray]):
    nan = float("nan")
    a = float_array.from_float([[1, 2, 3], [4, 5, nan]], exp_bits=10, man_bits=10)
    b = a.nancumprod()
    assert b.is_identical(
        float_array.from_float([1, 2, 6, 24, 120, 120], exp_bits=10, man_bits=10)
    )
    c = a.nancumprod(0)
    assert c.is_identical(
        float_array.from_float([[1, 2, 3], [4, 10, 3]], exp_bits=10, man_bits=10)
    )
    d = a.nancumprod(1)
    assert d.is_identical(
        float_array.from_float([[1, 2, 6], [4, 20, 20]], exp_bits=10, man_bits=10)
    )
    e = float_array.from_float(
        [[[1, 2], [3, nan]], [[nan, 6], [7, 8]]], exp_bits=10, man_bits=10
    )
    f = e.nancumprod()
    g = e.nancumprod(0)
    h = e.nancumprod(1)
    i = e.nancumprod(2)
    assert f.is_identical(
        float_array.from_float([1, 2, 6, 6, 6, 36, 252, 2016], exp_bits=10, man_bits=10)
    )
    assert g.is_identical(
        float_array.from_float(
            [[[1, 2], [3, 1]], [[1, 12], [21, 8]]], exp_bits=10, man_bits=10
        )
    )
    assert h.is_identical(
        float_array.from_float(
            [[[1, 2], [3, 2]], [[1, 6], [7, 48]]], exp_bits=10, man_bits=10
        )
    )
    assert i.is_identical(
        float_array.from_float(
            [[[1, 2], [3, 3]], [[1, 6], [7, 56]]], exp_bits=10, man_bits=10
        )
    )
    with pytest.raises(IndexError):
        _ = e.nancumprod(4)

    k = float_array.from_float([[0.25, 0.5], [nan, 2]], exp_bits=10, man_bits=10)
    m = k.nancumprod()
    assert m.is_identical(
        float_array.from_float([0.25, 0.125, 0.125, 0.25], exp_bits=10, man_bits=10)
    )

    g = float_array.from_float([[0, 1, nan], [3, 4, 5]], exp_bits=10, man_bits=10)
    h = g.nancumprod(0)
    assert h.is_identical(
        float_array.from_float([[0, 1, 1], [0, 4, 5]], exp_bits=10, man_bits=10)
    )
    j = float_array.from_float([[0, nan, 2], [3, nan, 5]], exp_bits=10, man_bits=10)
    k = j.nancumprod(0)
    assert k.is_identical(
        float_array.from_float([[0, 1, 2], [0, 1, 10]], exp_bits=10, man_bits=10)
    )

    x = float_array.from_float([[1, -1, nan], [-3, 4, -5]], exp_bits=10, man_bits=10)
    y = x.nancumprod()
    assert y.is_identical(
        float_array.from_float([1, -1, -1, 3, 12, -60], exp_bits=10, man_bits=10),
        ignore_zero_sign=True,
    )

    o = float_array.from_float([[-1, 2], [nan, 4]], exp_bits=10, man_bits=10)
    p = o.nancumprod()
    assert p.is_identical(
        float_array.from_float([-1, -2, -2, -8], exp_bits=10, man_bits=10)
    )

    x = float_array.from_float(
        [
            [[[0, nan], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    y = x.nancumprod(1)
    z = x.nancumprod(2)
    assert y.is_identical(
        float_array.from_float(
            [
                [[[0, 1], [2, 3]], [[0, 5], [12, 21]]],
                [[[8, 9], [10, 11]], [[96, 117], [140, 165]]],
            ],
            exp_bits=10,
            man_bits=10,
        )
    )
    assert z.is_identical(
        float_array.from_float(
            [
                [[[0, 1], [0, 3]], [[4, 5], [24, 35]]],
                [[[8, 9], [80, 99]], [[12, 13], [168, 195]]],
            ],
            exp_bits=10,
            man_bits=10,
        )
    )


@pytest.mark.parametrize("max_func", ["max", "nanmax"])
def test_max(max_func: str):
    a = APyFloatArray.from_float([[0, 1], [2, 3]], exp_bits=10, man_bits=10)
    b = getattr(a, max_func)()
    c = getattr(a, max_func)(0)
    d = getattr(a, max_func)(1)
    assert b.is_identical(APyFloat.from_float(3, exp_bits=10, man_bits=10))
    assert c.is_identical(APyFloatArray.from_float([2, 3], exp_bits=10, man_bits=10))
    assert d.is_identical(APyFloatArray.from_float([1, 3], exp_bits=10, man_bits=10))
    e = APyFloatArray.from_float(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    f = getattr(e, max_func)(1)
    g = getattr(e, max_func)((0, 1))
    h = getattr(e, max_func)((1, 3))
    i = getattr(e, max_func)()
    k = getattr(e, max_func)((0, 1, 2, 3))
    assert f.is_identical(
        APyFloatArray.from_float(
            [[[4, 5], [6, 7]], [[12, 13], [14, 15]]], exp_bits=10, man_bits=10
        )
    )
    assert g.is_identical(
        APyFloatArray.from_float([[12, 13], [14, 15]], exp_bits=10, man_bits=10)
    )
    assert h.is_identical(
        APyFloatArray.from_float([[5, 7], [13, 15]], exp_bits=10, man_bits=10)
    )
    assert i.is_identical(APyFloat.from_float(15, exp_bits=10, man_bits=10))
    assert k.is_identical(i)
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple\: dimension 4 out of range \(ndim = 4\)",
    ):
        _ = getattr(e, max_func)(4)


@pytest.mark.parametrize("max_func", ["max", "nanmax"])
def test_max_negative(max_func):
    # test negative numbers
    z = APyFloatArray.from_float([[0, -1], [-2, -3]], exp_bits=10, man_bits=10)
    m = getattr(z, max_func)()
    n = getattr(z, max_func)(0)
    o = getattr(z, max_func)(1)
    assert m.is_identical(APyFloat.from_float(0, exp_bits=10, man_bits=10))
    assert n.is_identical(APyFloatArray.from_float([0, -1], exp_bits=10, man_bits=10))
    assert o.is_identical(APyFloatArray.from_float([0, -2], exp_bits=10, man_bits=10))


@pytest.mark.parametrize("max_func", ["max", "nanmax"])
def test_max_mixed(max_func):
    # test a mix of negative and positive numbers
    p = APyFloatArray.from_float([[4, -1], [-2, -3]], exp_bits=10, man_bits=10)
    q = getattr(p, max_func)()
    assert q.is_identical(APyFloat.from_float(4, exp_bits=10, man_bits=10))


@pytest.mark.parametrize("max_func", ["max", "nanmax"])
def test_max_float(max_func):
    # test float numbers
    r = APyFloatArray.from_float([1.0, 1.25, 2.875], exp_bits=10, man_bits=10)
    s = getattr(r, max_func)()
    assert s.is_identical(APyFloat.from_float(2.875, exp_bits=10, man_bits=10))
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple\: dimension 4 out of range \(ndim = 1\)",
    ):
        _ = getattr(r, max_func)(4)


def test_max_nan():
    nan = float("nan")
    a = APyFloatArray.from_float([0, 1, 2, 3, nan], exp_bits=10, man_bits=10)
    b = a.max()
    assert b.is_identical(APyFloat.from_float(nan, exp_bits=10, man_bits=10))
    a = APyFloatArray.from_float(
        [[0, 1, 2, 3, nan], [nan, nan, nan, nan, 0]], exp_bits=10, man_bits=10
    )
    b = a.max(0)
    assert b.is_identical(
        APyFloatArray.from_float([nan, nan, nan, nan, nan], exp_bits=10, man_bits=10)
    )


def test_nanmax_nan():
    nan = float("nan")
    a = APyFloatArray.from_float([0, 1, 2, 3, nan], exp_bits=10, man_bits=10)
    b = a.nanmax()
    assert b.is_identical(APyFloat.from_float(3, exp_bits=10, man_bits=10))
    a = APyFloatArray.from_float(
        [[0, 1, 2, 3, nan], [nan, nan, nan, nan, 0]], exp_bits=10, man_bits=10
    )
    b = a.nanmax(0)
    assert b.is_identical(
        APyFloatArray.from_float([0, 1, 2, 3, 0], exp_bits=10, man_bits=10)
    )
    a = APyFloatArray.from_float([nan, nan], exp_bits=10, man_bits=10)
    b = a.nanmax(0)
    assert b.is_identical(APyFloat.from_float(nan, exp_bits=10, man_bits=10))


@pytest.mark.parametrize("min_func", ["min", "nanmin"])
def test_min(min_func):
    a = APyFloatArray.from_float([[0, 1], [2, 3]], exp_bits=10, man_bits=10)
    b = getattr(a, min_func)()
    c = getattr(a, min_func)(0)
    d = getattr(a, min_func)(1)
    assert b.is_identical(APyFloat.from_float(0, exp_bits=10, man_bits=10))
    assert c.is_identical(APyFloatArray.from_float([0, 1], exp_bits=10, man_bits=10))
    assert d.is_identical(APyFloatArray.from_float([0, 2], exp_bits=10, man_bits=10))
    e = APyFloatArray.from_float(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        exp_bits=10,
        man_bits=10,
    )
    f = getattr(e, min_func)(1)
    g = getattr(e, min_func)((0, 1))
    h = getattr(e, min_func)((1, 3))
    i = getattr(e, min_func)()
    k = getattr(e, min_func)((0, 1, 2, 3))
    assert f.is_identical(
        APyFloatArray.from_float(
            [[[0, 1], [2, 3]], [[8, 9], [10, 11]]], exp_bits=10, man_bits=10
        )
    )
    assert g.is_identical(
        APyFloatArray.from_float([[0, 1], [2, 3]], exp_bits=10, man_bits=10)
    )
    assert h.is_identical(
        APyFloatArray.from_float([[0, 2], [8, 10]], exp_bits=10, man_bits=10)
    )
    assert i.is_identical(APyFloat.from_float(0, exp_bits=10, man_bits=10))
    assert k.is_identical(i)
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple\: dimension 4 out of range \(ndim = 4\)",
    ):
        _ = getattr(e, min_func)(4)


@pytest.mark.parametrize("min_func", ["min", "nanmin"])
def test_min_negative(min_func):
    # test negative numbers
    z = APyFloatArray.from_float([[0, -1], [-2, -3]], exp_bits=10, man_bits=10)
    m = getattr(z, min_func)()
    n = getattr(z, min_func)(0)
    o = getattr(z, min_func)(1)
    assert m.is_identical(APyFloat.from_float(-3, exp_bits=10, man_bits=10))
    assert n.is_identical(APyFloatArray.from_float([-2, -3], exp_bits=10, man_bits=10))
    assert o.is_identical(APyFloatArray.from_float([-1, -3], exp_bits=10, man_bits=10))


@pytest.mark.parametrize("min_func", ["min", "nanmin"])
def test_min_mixed_signs(min_func):
    # test a mix of negative and positive numbers
    p = APyFloatArray.from_float([[4, -1], [-2, -3]], exp_bits=10, man_bits=10)
    q = getattr(p, min_func)()
    assert q.is_identical(APyFloat.from_float(-3, exp_bits=10, man_bits=10))


@pytest.mark.parametrize("min_func", ["min", "nanmin"])
def test_min_float(min_func):
    # test float numbers
    r = APyFloatArray.from_float([1.0, 1.25, 2.875], exp_bits=10, man_bits=10)
    s = getattr(r, min_func)()
    assert s.is_identical(APyFloat.from_float(1.0, exp_bits=10, man_bits=10))
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple\: dimension 4 out of range \(ndim = 1\)",
    ):
        _ = getattr(r, min_func)(4)


def test_min_nan():
    nan = float("nan")
    a = APyFloatArray.from_float([0, 1, 2, 3, nan], exp_bits=10, man_bits=10)
    b = a.min()
    assert b.is_identical(APyFloat.from_float(nan, exp_bits=10, man_bits=10))
    a = APyFloatArray.from_float(
        [[0, 1, 2, 3, nan], [nan, nan, nan, nan, 0]], exp_bits=10, man_bits=10
    )
    b = a.min(0)
    assert b.is_identical(
        APyFloatArray.from_float([nan, nan, nan, nan, nan], exp_bits=10, man_bits=10)
    )


def test_nanmin_nan():
    nan = float("nan")
    a = APyFloatArray.from_float([0, 1, 2, 3, nan], exp_bits=10, man_bits=10)
    b = a.nanmin()
    assert b.is_identical(APyFloat.from_float(0, exp_bits=10, man_bits=10))
    a = APyFloatArray.from_float(
        [[0, 1, 2, 3, nan], [nan, nan, nan, nan, 0]], exp_bits=10, man_bits=10
    )
    b = a.nanmin(0)
    assert b.is_identical(
        APyFloatArray.from_float([0, 1, 2, 3, 0], exp_bits=10, man_bits=10)
    )
    a = APyFloatArray.from_float([nan, nan], exp_bits=10, man_bits=10)
    b = a.nanmin(0)
    assert b.is_identical(APyFloat.from_float(nan, exp_bits=10, man_bits=10))


def test_convenience_cast():
    a = APyFloatArray.from_float([0.893820, 3e20, -float("inf"), float("nan")], 10, 50)
    assert a.cast_to_double().is_identical(
        APyFloatArray(
            [0, 0, 1, 0],
            [1022, 1091, 2047, 2047],
            [3547215210502096, 74037091379504, 0, 1],
            exp_bits=11,
            man_bits=52,
            bias=1023,
        )
    )
    assert a.cast_to_single().is_identical(
        APyFloatArray(
            [0, 0, 1, 0],
            [126, 195, 255, 255],
            [6607203, 137905, 0, 1],
            exp_bits=8,
            man_bits=23,
            bias=127,
        )
    )
    assert a.cast_to_half().is_identical(
        APyFloatArray(
            [0, 0, 1, 0],
            [14, 31, 31, 31],
            [807, 0, 0, 1],
            exp_bits=5,
            man_bits=10,
            bias=15,
        )
    )
    assert a.cast_to_bfloat16().is_identical(
        APyFloatArray(
            [0, 0, 1, 0],
            [126, 195, 255, 255],
            [101, 2, 0, 1],
            exp_bits=8,
            man_bits=7,
            bias=127,
        )
    )


@pytest.mark.float_array
@pytest.mark.parametrize(
    ("shape", "is_valid", "is_invalid", "test_neg_one"),
    [
        ((6,), True, False, False),  # Valid shapes
        ((3, 2), True, False, False),
        ((1, 6), True, False, False),
        ((6, 1), True, False, False),
        ((2, 3), True, False, False),
        ((-1,), True, False, False),
        ((-1, 2), True, False, False),
        ((1, -1), True, False, False),
        ((6, -1), True, False, False),
        ((3, -1), True, False, False),
        ((1, 2, 50), False, True, False),  # Invalid shapes
        ((1, 2), False, True, False),
        ((1, 4, 3), False, True, False),
        ((2, 4), False, True, False),
        ((2, 2), False, True, False),
        ((-2,), False, False, True),  # Invalid negatives
        ((-1, -1), False, False, True),
        ((4, -1), False, False, True),
        ((-2, 3), False, False, True),
        ((-999, 12), False, False, True),
    ],
)
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_reshape(
    shape: tuple[int, ...],
    is_valid: bool,
    is_invalid: bool,
    test_neg_one: bool,
    float_array: type[APyCFloatArray],
):
    signs = [[52, 15, 32], [12, 43, 5]]
    exps = [[5, 10, 6], [15, 20, 3]]
    mans = [[3, 1, 2], [4, 2, 8]]

    arr = float_array(signs, exps, mans, exp_bits=8, man_bits=23)

    if is_invalid:
        with pytest.raises(
            ValueError,
            match=r"APyC?FloatArray\.reshape: target array number of elements does not",
        ):
            _ = arr.reshape(shape)
    elif test_neg_one:
        with pytest.raises(
            ValueError,
            match=r"APyC?FloatArray\.reshape: array dimensions must be greater than|"
            + r"APyC?FloatArray\.reshape: only one dimension can be -1|"
            + r"APyC?FloatArray\.reshape: the size of target array must be unchanged",
        ):
            _ = arr.reshape(shape)
    elif is_valid:
        try:
            g = arr.reshape(shape)
            if g is arr:
                pytest.fail(
                    "Reshape should return a new array, now returns a modified one"
                )
        except ValueError:
            pytest.fail(f"Unexpected ValueError for shape {shape}")


@pytest.mark.float_array
@pytest.mark.parametrize(
    "shape",
    [
        (1, 12),
        (2, 6),
        (3, 4),
        (4, 3),
        (6, 2),
        (12, 1),
        (-1, 12),
        (2, -1),
        (3, -1),
        (4, -1),
        (-1, 3),
        (-1, 2),
        (12, -1),
    ],
)
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_reshape_2d(shape: tuple[int, ...], float_array: type[APyCFloatArray]):
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # Creating 1D array to be reshaped
    arr = float_array(signs, exps, mans, exp_bits=5, man_bits=2)

    reshaped_arr = arr.reshape(shape)
    for i, row in enumerate(reshaped_arr):
        for j, float_ in enumerate(row):
            arr_index = i * reshaped_arr.shape[1] + j
            if not arr[arr_index].is_identical(float_):
                pytest.fail(f"Mismatch at index {arr_index} during reshape")

    go_back = reshaped_arr.reshape(arr.shape)
    if not go_back.is_identical(arr):
        pytest.fail(f"Mismatch after reshaping back at index {arr_index}")


@pytest.mark.float_array
@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
        (1, 12, 1),
        (2, 1, 6),
        (2, 2, 3),
        (2, 3, 2),
        (2, 6, 1),
        (3, 1, 4),
        (3, 2, 2),
        (3, 4, 1),
        (4, 1, 3),
        (4, 3, 1),
        (6, 1, 2),
        (6, 2, 1),
        (12, 1, 1),
        (-1, 2, 2),
        (1, -1, 2),
        (6, 1, -1),
    ],
)
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_reshape_3d(shape: tuple[int, ...], float_array: type[APyCFloatArray]):
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # Creating 1D array to be reshaped
    arr = float_array(signs, exps, mans, exp_bits=5, man_bits=2)

    reshaped_arr = arr.reshape(shape)
    for i, matrix in enumerate(reshaped_arr):
        for j, row in enumerate(matrix):
            for k, float_ in enumerate(row):
                arr_index = (
                    i * reshaped_arr.shape[1] * reshaped_arr.shape[2]
                    + j * reshaped_arr.shape[2]
                    + k
                )
                if not arr[arr_index].is_identical(float_):
                    pytest.fail(f"Mismatch at index {arr_index} during reshape")

    go_back = reshaped_arr.reshape(arr.shape)
    if not go_back.is_identical(arr):
        pytest.fail(f"Mismatch after reshaping back at index {arr_index}")


@pytest.mark.float_array
@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
    ],
)
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_flatten(shape: tuple[int, ...], float_array: type[APyCFloatArray]):
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # manually create 1d arr
    arr = float_array(signs, exps, mans, exp_bits=5, man_bits=2)
    reshaped = arr.reshape(shape)
    if not reshaped.flatten().is_identical(arr):
        pytest.fail(f"Flatten didn't return to original 1d list after reshape {shape}")


@pytest.mark.float_array
@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
    ],
)
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_ravel(shape: tuple[int, ...], float_array: type[APyCFloatArray]):
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    # manually create 1d arr
    arr = float_array(signs, exps, mans, exp_bits=5, man_bits=2)
    reshaped = arr.reshape(shape)
    if not reshaped.ravel().is_identical(arr):
        pytest.fail(f"Flatten didn't return to original 1d list after reshape {shape}")


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_swapaxes(float_array: type[APyCFloatArray]):
    signs = [1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0]
    exps = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    mans = [3, 1, 4, 2, 6, 5, 8, 7, 9, 0, 2, 3]

    a = float_array(signs, exps, mans, exp_bits=5, man_bits=2).reshape((6, 2))

    if not a.swapaxes(0, 1).is_identical(a.T):
        pytest.fail("swapaxes didn't correctly swap axis")

    if not a.swapaxes(1, 0).is_identical(a.T):
        pytest.fail("swapaxes didn't correctly swap axis")

    a = APyFloatArray.from_float([0] * 24, exp_bits=5, man_bits=2).reshape((4, 3, 2))

    if a.swapaxes(0, 1).shape != (3, 4, 2):
        pytest.fail("swapaxes didn't correctly swap axis")

    if a.swapaxes(1, 0).shape != (3, 4, 2):
        pytest.fail("swapaxes didn't correctly swap axis")

    if a.swapaxes(2, 0).shape != (2, 3, 4):
        pytest.fail("swapaxes didn't correctly swap axis")

    if a.swapaxes(0, 2).shape != (2, 3, 4):
        pytest.fail("swapaxes didn't correctly swap axis")


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_transpose(float_array: type[APyCFloatArray]):
    # 1-D transposition simply returns the input (just like NumPy-arrays)
    assert float_array([], [], [], 4, 3).T.is_identical(float_array([], [], [], 4, 3))
    assert float_array([1, 0, 1], [15, 4, 20], [7, 2, 3], 5, 5).T.is_identical(
        float_array([1, 0, 1], [15, 4, 20], [7, 2, 3], 5, 5)
    )

    # # 2-D transposition returns the matrix transposition
    a = float_array.from_float(
        [
            [1.0, 2.0, 3.0],
            [-4.0, -5.0, -6.0],
        ],
        exp_bits=5,
        man_bits=2,
    )
    assert a.T.T.is_identical(a)
    assert a.T.is_identical(
        float_array.from_float(
            [
                [1.0, -4.0],
                [2.0, -5.0],
                [3.0, -6.0],
            ],
            exp_bits=5,
            man_bits=2,
        )
    )


@pytest.mark.float_array
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_transpose_highdim_np(float_array: type[APyCFloatArray]):
    def _generate_dimensions(n: int):
        result: set[tuple[int, ...]] = set()  # Use a set to store unique combinations

        def factor_combinations(target: int, factors: list[int]):
            if target == 1:
                result.add(tuple(sorted(factors)))  # Add sorted tuple to set
                return
            for i in range(2, target + 1):
                if target % i == 0:
                    factor_combinations(target // i, [*factors, i])

        factor_combinations(n, [])
        return list(result)  # Convert set back to list

    np = pytest.importorskip("numpy")

    # can be any, does not matter
    num_elems = 48
    elements = np.arange(num_elems)
    # Generate all possible axis permutations for a 3D array
    possible_shapes = _generate_dimensions(num_elems)

    # Test each permutation
    for shape in possible_shapes:
        if len(shape) < 3:
            axes_permutations = [None]
        else:
            axes_permutations = list(permutations(list(range(len(shape)))))

        for perm in axes_permutations:
            apy_array = float_array.from_array(np.reshape(elements, shape), 5, 5)
            numpy_transposed = np.transpose(np.reshape(elements, shape), perm)

            apy_transposed = apy_array.transpose(perm)
            numpy_array = float_array.from_array(numpy_transposed, 5, 5)

            assert apy_transposed.is_identical(numpy_array), (
                f"Failed for shape {shape} and permutation {perm}. "
                f"Original array = \n{np.reshape(elements, shape)}\n "
                f"ApyFloat array = \n{apy_transposed.to_numpy()}\n "
                f"Numpy created array = \n{numpy_array.to_numpy()}"
            )


@pytest.mark.parametrize("start_val", [2**i for i in range(0, 130, 13)])
@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_transpose_highdim(start_val: int, float_array: type[APyCFloatArray]):
    a = float_array.from_float(
        [[start_val + 1, start_val + 2], [start_val + 3, start_val + 4]], 30, 5
    )
    b = float_array.from_float(
        [[start_val + 1, start_val + 3], [start_val + 2, start_val + 4]], 30, 5
    )
    assert a.transpose().is_identical(b)
    # 1  2  3
    # 4  5  6
    # 7  8  9

    a = float_array.from_float(
        [
            [start_val + 1, start_val + 2, start_val + 3],
            [start_val + 4, start_val + 5, start_val + 6],
            [start_val + 7, start_val + 8, start_val + 9],
        ],
        30,
        5,
    )

    # 1  4  7
    # 2  5  8
    # 3  6  9
    b = float_array.from_float(
        [
            [start_val + 1, start_val + 4, start_val + 7],
            [start_val + 2, start_val + 5, start_val + 8],
            [start_val + 3, start_val + 6, start_val + 9],
        ],
        30,
        5,
    )

    assert a.transpose().is_identical(b)


@pytest.mark.parametrize("float_array", [APyFloatArray, APyCFloatArray])
def test_transpose_negative_dim(float_array: type[APyCFloatArray]):
    a = float_array.from_float([1.0] * 6, man_bits=5, exp_bits=6).reshape((1, 2, 3))
    assert a.transpose((1, 0, 2)).shape == (2, 1, 3)
    assert a.transpose((-2, -3, -1)).shape == (2, 1, 3)


def test_to_bits():
    #
    # 1-D:
    #
    assert APyFloatArray.from_float(range(4), 5, 2).to_bits() == [
        0,
        0b00111100,
        0b01000000,
        0b01000010,
    ]
    assert APyFloatArray.from_float(range(4), 4, 3).to_bits() == [
        0,
        0b00111000,
        0b01000000,
        0b01000100,
    ]
    b = 2**19 - 1
    assert APyFloatArray.from_float(range(4), 20, 40, b).to_bits() == [
        0,
        b << 40,
        (b + 1) << 40,
        ((b + 1) << 40) | (1 << 39),
    ]
    assert APyFloatArray.from_float(range(-3, 3), 5, 2).to_bits() == [
        0b11000010,
        0b11000000,
        0b10111100,
        0,
        0b00111100,
        0b01000000,
    ]
    assert APyFloatArray.from_float(range(-3, 3), 4, 3).to_bits() == [
        0b11000100,
        0b11000000,
        0b10111000,
        0,
        0b00111000,
        0b01000000,
    ]

    #
    # 2-D:
    #
    assert APyFloatArray.from_float([[1, 2], [4, 3]], 5, 2).to_bits() == [
        [0b00111100, 0b01000000],
        [0b01000100, 0b01000010],
    ]
    assert APyFloatArray.from_float([[1, 2], [4, 3]], 4, 3).to_bits() == [
        [0b00111000, 0b01000000],
        [0b01001000, 0b01000100],
    ]

    #
    # 3-D:
    #
    assert APyFloatArray.from_float(
        [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]], 5, 2
    ).to_bits() == [[[60, 64], [66, 68], [69, 70]], [[71, 72], [72, 72], [71, 70]]]
    assert APyFloatArray.from_float(
        [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]], 4, 3
    ).to_bits() == [[[56, 64], [68, 72], [74, 76]], [[78, 80], [81, 80], [78, 76]]]


def test_to_bits_numpy():
    np = pytest.importorskip("numpy")

    #
    # Numpy currently does not support long conversions
    #
    with pytest.raises(ValueError, match=r"APyFloatArray::to_bits_ndarray"):
        APyFloatArray([0], [0], [0], 25, 40).to_bits(True)

    #
    # Correct Numpy dtypes
    #
    assert APyFloatArray([0], [0], [0], 2, 2).to_bits(True).dtype == np.dtype("uint8")
    assert APyFloatArray([0], [0], [0], 4, 3).to_bits(True).dtype == np.dtype("uint8")
    assert APyFloatArray([0], [0], [0], 5, 5).to_bits(True).dtype == np.dtype("uint16")
    assert APyFloatArray([0], [0], [0], 8, 7).to_bits(True).dtype == np.dtype("uint16")
    assert APyFloatArray([0], [0], [0], 20, 5).to_bits(True).dtype == np.dtype("uint32")
    assert APyFloatArray([0], [0], [0], 16, 15).to_bits(True).dtype == np.dtype(
        "uint32"
    )
    assert APyFloatArray([0], [0], [0], 20, 30).to_bits(True).dtype == np.dtype(
        "uint64"
    )
    assert APyFloatArray([0], [0], [0], 30, 33).to_bits(True).dtype == np.dtype(
        "uint64"
    )

    #
    # 1-D:
    #
    assert np.all(
        APyFloatArray.from_float(range(4), 5, 2).to_bits(True)
        == np.array([0, 0b00111100, 0b01000000, 0b01000010])
    )
    assert np.all(
        APyFloatArray.from_float(range(4), 4, 3).to_bits(True)
        == np.array([0, 0b00111000, 0b01000000, 0b01000100])
    )
    assert np.all(
        APyFloatArray.from_float(range(-3, 3), 5, 2).to_bits(True)
        == np.array([0b11000010, 0b11000000, 0b10111100, 0, 0b00111100, 0b01000000])
    )
    assert np.all(
        APyFloatArray.from_float(range(-3, 3), 4, 3).to_bits(True)
        == np.array([0b11000100, 0b11000000, 0b10111000, 0, 0b00111000, 0b01000000])
    )

    #
    # 2-D:
    #
    assert np.all(
        APyFloatArray.from_float([[1, 2], [4, 3]], 5, 2).to_bits(True)
        == np.array([[0b00111100, 0b01000000], [0b01000100, 0b01000010]])
    )
    assert np.all(
        APyFloatArray.from_float([[1, 2], [4, 3]], 4, 3).to_bits(True)
        == np.array([[0b00111000, 0b01000000], [0b01001000, 0b01000100]])
    )

    #
    # 3-D:
    #
    assert np.all(
        APyFloatArray.from_float(
            [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]], 5, 2
        ).to_bits(True)
        == np.array([[[60, 64], [66, 68], [69, 70]], [[71, 72], [72, 72], [71, 70]]])
    )
    assert np.all(
        APyFloatArray.from_float(
            [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]], 4, 3
        ).to_bits(True)
        == np.array([[[56, 64], [68, 72], [74, 76]], [[78, 80], [81, 80], [78, 76]]])
    )


@pytest.mark.parametrize(
    ("float_array", "float_scalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
@pytest.mark.parametrize("bits", [16, 32, 48])
def test_issue_623(
    float_array: type[APyCFloatArray], float_scalar: type[APyCFloat], bits: int
):
    """
    Test for GitHub issue #623: `cumsum` (and `cumprod`) of empty array crashing. Should
    use same semantics as Numpy.
    """
    exp_bits = bits // 2
    man_bits = bits // 2
    empty = float_array.from_float([], exp_bits=exp_bits, man_bits=man_bits)
    zero_scalar = float_scalar(0, 0, 0, exp_bits=exp_bits, man_bits=man_bits)
    one_scalar = float_scalar.from_float(1.0, exp_bits=exp_bits, man_bits=man_bits)
    assert empty.sum().is_identical(zero_scalar)
    assert empty.sum(0).is_identical(zero_scalar)
    assert empty.prod().is_identical(one_scalar)
    assert empty.prod(0).is_identical(one_scalar)
    assert empty.cumsum().is_identical(empty)
    assert empty.cumsum(0).is_identical(empty)
    assert empty.cumprod().is_identical(empty)
    assert empty.cumprod(0).is_identical(empty)
