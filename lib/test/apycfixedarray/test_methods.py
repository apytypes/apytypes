import pytest

from apytypes import APyCFixed, APyCFixedArray, APyFixed, APyFixedArray, APyFloat


def test_transpose():
    a = APyCFixedArray.from_complex(
        [
            [1 - 1j, 2 + 1j],
            [-1 - 1j, -5 + 1j],
        ],
        int_bits=10,
        frac_bits=0,
    )
    assert a.T.is_identical(
        APyCFixedArray.from_complex(
            [
                [1 - 1j, -1 - 1j],
                [2 + 1j, -5 + 1j],
            ],
            int_bits=10,
            frac_bits=0,
        )
    )


def test_from_complex():
    a = APyCFixedArray.from_complex(
        [
            [1 - 2j, 1 / 2 + 3 / 4j],
            [-(2**-5) + 17j, -3j],
        ],
        int_bits=10,
        frac_bits=10,
    )
    assert a.is_identical(
        APyCFixedArray(
            [[(1024, 1046528), (512, 1047808)], [(1048544, 17408), (0, 1045504)]],
            int_bits=10,
            frac_bits=10,
        )
    )

    # From APyCFixed
    assert APyCFixedArray.from_complex(
        [APyCFixed((13, 11), int_bits=2, frac_bits=3)], int_bits=2, frac_bits=2
    ).is_identical(
        APyCFixedArray.from_complex([1.75 + 1.5j], int_bits=2, frac_bits=2)
    )  # Rounds to (1.75, 1.5)
    # Testing using "from_complex([1.75+1.5j]" is currently required

    # From APyFixed
    assert APyCFixedArray.from_complex(
        [APyFixed(13, int_bits=2, frac_bits=3)], int_bits=2, frac_bits=2
    ).is_identical(APyCFixedArray([7], int_bits=2, frac_bits=2))  # Rounds to (1.75, 0)

    # From a sequence of APyFixed (must be treated as scalar elements, not iterables)
    assert APyCFixedArray.from_complex(
        [
            APyFixed(13, int_bits=2, frac_bits=3),
            APyFixed(3, int_bits=2, frac_bits=3),
            APyFixed(15, int_bits=2, frac_bits=3),
        ],
        int_bits=2,
        frac_bits=2,
    ).is_identical(
        APyCFixedArray.from_complex(
            [1.75 + 0j, 0.5 + 0j, 2.0 + 0j], int_bits=2, frac_bits=2
        )
    )

    # From APyFloat
    assert APyCFixedArray.from_complex(
        [APyFloat(1, 15, 2, 5, 2)], bits=4, int_bits=3
    ).is_identical(APyCFixedArray([13], bits=4, int_bits=3))  # (-1.5, 0)


def test_real_imag():
    real_values = [0.0, 0.5, 0.75, 1.0, 1.25, -0.5, -1.0, -1.25]
    imag_values = [0.0, -0.25, -1.75, 1.25, 1.25, -0.5, -1.0, -1.25]
    complex_values = [real + 1j * imag for real, imag in zip(real_values, imag_values)]
    array = APyCFixedArray.from_complex(complex_values, int_bits=128, frac_bits=128)

    assert array.real.is_identical(
        APyFixedArray.from_float(real_values, int_bits=128, frac_bits=128)
    )
    assert array.imag.is_identical(
        APyFixedArray.from_float(imag_values, int_bits=128, frac_bits=128)
    )


@pytest.mark.parametrize("bits", [16, 32, 64, 128])
def test_issue_615_complex(bits: int):
    """
    Specialized test for complex-valued array: no overflowing/carry between
    real/imaginary parts.
    """
    A_fx = APyCFixedArray.from_complex(
        [1 + 1j, 2 - 2j, -3 + 3j, 4 - 4j], bits=bits, frac_bits=bits // 2
    )
    assert A_fx.cumsum().is_identical(
        APyCFixedArray.from_complex(
            [1 + 1j, 3 - 1j, 0 + 2j, 4 - 2j], bits=bits + 2, frac_bits=bits // 2
        )
    )

    A_fx = APyCFixedArray.from_complex(
        [1 + 2j, 2 - 3j, -5 + 3j, 4 - 5j], bits=bits, frac_bits=bits // 2
    )
    assert A_fx.cumprod().is_identical(
        APyCFixedArray.from_complex(
            [1 + 2j, 8 + 1j, -43 + 19j, -77 + 291j],
            bits=4 * bits,
            frac_bits=2 * bits,
        )
    )


def test_copy():
    a = APyCFixedArray([(3, 1), (0, 3)], 4, 5)
    b = a
    assert a.is_identical(b)
    c = a.copy()
    assert a.is_identical(c)
    a[0] = APyCFixed((2, 1), 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


def test_python_copy():
    import copy

    a = APyCFixedArray([(3, 1), (0, 3)], 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.copy(a)
    assert a.is_identical(c)
    a[0] = APyCFixed((2, 1), 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


def test_python_deepcopy():
    import copy

    a = APyCFixedArray([(3, 1), (0, 3)], 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.deepcopy(a)
    assert a.is_identical(c)
    a[0] = APyCFixed((2, 1), 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


@pytest.mark.parametrize(
    ("lst", "ref"),
    [
        ([1], APyCFixedArray.from_complex([1], int_bits=10, frac_bits=0)),
        ([1, 2], APyCFixedArray.from_complex([1 + 2j], int_bits=10, frac_bits=0)),
        ([1, 3, 4], APyCFixedArray.from_complex([1, 3, 4], int_bits=10, frac_bits=0)),
        ([[1], [2]], APyCFixedArray.from_complex([[1], [2]], int_bits=10, frac_bits=0)),
        (
            [[1, 2], [3, 4]],
            APyCFixedArray.from_complex([1 + 2j, 3 + 4j], int_bits=10, frac_bits=0),
        ),
        (
            [[[1, 2]], [[3, 4]]],
            APyCFixedArray.from_complex([[1 + 2j], [3 + 4j]], int_bits=10, frac_bits=0),
        ),
    ],
)
def test_from_bits(lst: list[list[int] | int], ref: APyCFixedArray):
    assert APyCFixedArray.from_bits(lst, int_bits=10, frac_bits=0).is_identical(ref)


@pytest.mark.parametrize(
    ("lst", "ref"),
    [
        ([1], APyCFixedArray.from_complex([1], int_bits=10, frac_bits=0)),
        ([1, 2], APyCFixedArray.from_complex([1 + 2j], int_bits=10, frac_bits=0)),
        ([1, 3, 4], APyCFixedArray.from_complex([1, 3, 4], int_bits=10, frac_bits=0)),
        ([[1], [2]], APyCFixedArray.from_complex([[1], [2]], int_bits=10, frac_bits=0)),
        (
            [[1, 2], [3, 4]],
            APyCFixedArray.from_complex([1 + 2j, 3 + 4j], int_bits=10, frac_bits=0),
        ),
        (
            [[[1, 2]], [[3, 4]]],
            APyCFixedArray.from_complex([[1 + 2j], [3 + 4j]], int_bits=10, frac_bits=0),
        ),
    ],
)
def test_from_bits_np(lst: list[list[int] | int], ref: APyCFixedArray):
    np = pytest.importorskip("numpy")
    assert APyCFixedArray.from_bits(
        np.asarray(lst), int_bits=10, frac_bits=0
    ).is_identical(ref)


@pytest.mark.parametrize(
    "lst",
    [
        [(1, 1)],
        [(1, 2), (2, 0)],
        [(1, 3), (3, 8), (4, 1)],
        [[(1, 7)], [(2, 8)]],
        [[(1, 3), (2, 0)], [(3, 5), (4, 6)]],
        [[[(1, 2**80 - 7), (2**90, 4)]], [[(3, 1), (2**65 - 4, 2**70)]]],
    ],
)
def test_to_bits(lst: list[list[tuple[int, int]] | tuple[int, int]]):
    assert APyCFixedArray(lst, int_bits=91, frac_bits=0).to_bits() == lst


@pytest.mark.parametrize(
    "lst",
    [
        [(1, 1)],
        [(1, 2), (2, 0)],
        [(1, 3), (3, 8), (4, 1)],
        [[(1, 7)], [(2, 2**16 - 9)]],
        [[(1, 3), (2, 0)], [(3, 7), (2**30 - 7, 6)]],
        [[[(1, 1), (2**62 - 5, 4)]], [[(3, 1), (4, 0)]]],
    ],
)
def test_to_bits_np(lst: list[list[tuple[int, int]] | tuple[int, int]]):
    np = pytest.importorskip("numpy")
    assert np.array_equal(
        APyCFixedArray(lst, int_bits=63, frac_bits=0).to_bits(True), np.asarray(lst)
    )


def test_to_bits_np_neg():
    np = pytest.importorskip("numpy")
    assert np.array_equal(
        APyCFixedArray.from_float(range(-3, 3), int_bits=4, frac_bits=1).to_bits(True),
        np.array(
            [
                (0b11010, 0),
                (0b11100, 0),
                (0b11110, 0),
                (0b00000, 0),
                (0b00010, 0),
                (0b00100, 0),
            ]
        ),
    )

    data = np.arange(-3, 3) + 1j * np.arange(2, -4, -1)
    assert np.array_equal(
        APyCFixedArray.from_complex(data, int_bits=4, frac_bits=1).to_bits(True),
        np.array(
            [
                (0b11010, 0b00100),
                (0b11100, 0b00010),
                (0b11110, 0b00000),
                (0b00000, 0b11110),
                (0b00010, 0b11100),
                (0b00100, 0b11010),
            ]
        ),
    )


@pytest.mark.parametrize(
    "bits",
    [8, 16, 32, 64],
)
def test_to_bits_np_sizes(bits: int):
    np = pytest.importorskip("numpy")
    dtype_map = {
        8: np.uint8,
        16: np.uint16,
        32: np.uint32,
        64: np.uint64,
    }
    dtype = dtype_map[bits]
    a = APyCFixedArray([(2**bits - 7, 2 ** (bits - 1) + 9)], bits, 0).to_bits(True)
    expected = np.asarray([(2**bits - 7, 2 ** (bits - 1) + 9)], dtype=dtype)
    assert np.array_equal(a, expected)
    assert a.dtype == dtype


def test_to_bits_np_raise():
    _ = pytest.importorskip("numpy")
    with pytest.raises(
        ValueError,
        match=r"APyCFixedArray::to_bits_ndarray\(\): only supports",
    ):
        APyCFixedArray([0], 30, 60).to_bits(True)


def test_iterator():
    fx_array = APyCFixedArray([1, 2, 3, 4, 5, 6], bits=10, int_bits=10)
    iterator = iter(fx_array)
    assert next(iterator).is_identical(APyCFixed((1, 0), bits=10, int_bits=10))
    assert next(iterator).is_identical(APyCFixed((2, 0), bits=10, int_bits=10))

    fx_array = APyCFixedArray([(1, 2), (3, 4)], bits=10, int_bits=10)
    iterator = iter(fx_array)
    assert next(iterator).is_identical(APyCFixed((1, 2), bits=10, int_bits=10))
    assert next(iterator).is_identical(APyCFixed((3, 4), bits=10, int_bits=10))


def test_iterator_2d():
    fx_array = APyCFixedArray(
        [[(1, 2), (3, 4)], [(5, 6), (7, 8)]], bits=10, int_bits=10
    )
    iterator = iter(fx_array)
    assert next(iterator).is_identical(
        APyCFixedArray([(1, 2), (3, 4)], bits=10, int_bits=10)
    )
    assert next(iterator).is_identical(
        APyCFixedArray([(5, 6), (7, 8)], bits=10, int_bits=10)
    )
