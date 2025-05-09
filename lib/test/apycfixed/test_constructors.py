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
