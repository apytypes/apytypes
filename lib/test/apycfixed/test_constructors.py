import pytest

from apytypes import APyCFixed


def test_raises():
    with pytest.raises(ValueError, match=r"APyCFixed.__init__: tuple initializer"):
        APyCFixed((), int_bits=10, frac_bits=10)

    with pytest.raises(ValueError, match=r"APyCFixed.__init__: tuple initializer"):
        APyCFixed((1, 2, 3), int_bits=10, frac_bits=10)

    with pytest.raises(ValueError, match=r"APyCFixed.__init__: tuple initializer"):
        APyCFixed((1.0,), int_bits=10, frac_bits=10)

    with pytest.raises(ValueError, match=r"APyCFixed.__init__: tuple initializer"):
        APyCFixed((1.0, 1), int_bits=10, frac_bits=10)

    with pytest.raises(ValueError, match=r"APyCFixed.__init__: tuple initializer"):
        APyCFixed((1, 1.0), int_bits=10, frac_bits=10)

    with pytest.raises(ValueError, match=r"Non-supported type: "):
        APyCFixed.from_float({"abc": 7, "cde": 8}, int_bits=7, frac_bits=9)


def test_constructor():
    assert APyCFixed((0x14,), int_bits=4, frac_bits=4) == 1.25
    assert APyCFixed((0x00, 0x14), int_bits=4, frac_bits=4) == 0.0 + 1.25j


def test_from_float():
    np = pytest.importorskip("numpy")
    types = [
        np.float16,
        np.float32,
        np.float64,
        np.int8,
        np.int16,
        np.int32,
        np.int64,
        np.uint8,
        np.uint16,
        np.uint32,
        np.uint64,
    ]
    for t in types:
        a = APyCFixed.from_float(t(125), int_bits=30, frac_bits=30)
        assert a.is_identical(APyCFixed.from_complex(125, int_bits=30, frac_bits=30))


def test_from_complex():
    np = pytest.importorskip("numpy")
    types = [
        np.complex64,
        np.complex128,
    ]
    for t in types:
        a = APyCFixed.from_float(t(923.75), int_bits=30, frac_bits=30)
        assert a.is_identical(APyCFixed.from_complex(923.75, int_bits=30, frac_bits=30))
