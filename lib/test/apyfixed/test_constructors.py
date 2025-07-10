from math import pow

import pytest

from apytypes import APyFixed, APyFloat


def test_construction_raises():
    """
    Disallowed construction of `APyFixed` object raises `ValueError`
    """
    # Zero-bit fixed-point numbers cannot be created
    with pytest.raises(ValueError, match="Fixed-point bit specification needs a"):
        _ = APyFixed(0, bits=0, int_bits=0)

    # Exactly two of three bit-specifiers needs to be set when creating `APyFixed`
    with pytest.raises(ValueError, match="Fixed-point bit specification needs exactly"):
        _ = APyFixed(0, bits=1, int_bits=0, frac_bits=0)
    with pytest.raises(ValueError, match="Fixed-point bit specification needs exactly"):
        _ = APyFixed(0, frac_bits=1)
    with pytest.raises(ValueError, match="Fixed-point bit specification needs exactly"):
        _ = APyFixed(0, int_bits=1)
    with pytest.raises(ValueError, match="Fixed-point bit specification needs exactly"):
        _ = APyFixed(0, bits=1)


def test_constructor():
    """
    Test the base bit_pattern setting of the APyFixed constructor, along with some
    `APyFixed.to_bits()` tests
    """
    assert APyFixed(1234, int_bits=12, frac_bits=0).to_bits() == 1234
    assert APyFixed(1234, int_bits=10, frac_bits=0).to_bits() == 210
    assert APyFixed(-1234, int_bits=12, frac_bits=0).to_bits() == 2862
    assert APyFixed(-1234, int_bits=10, frac_bits=0).to_bits() == 814
    assert float(APyFixed(0x18, int_bits=4, frac_bits=4)) == 1.5

    # Integers that don't fit a single Python digit
    assert (
        APyFixed(1234567898765432123456789, int_bits=50, frac_bits=50).to_bits()
        == 1234567898765432123456789
    )
    assert (
        APyFixed(-1234567898765432123456789, int_bits=50, frac_bits=50).to_bits()
        == 2**100 - 1234567898765432123456789
    )


def test_floating_point_construction():
    """
    Test creation of `APyFixed` from floating-point objects with static method
    `APyFixed.from_float()`
    """
    assert float(APyFixed.from_float(1.234, int_bits=2, frac_bits=2)) == 1.25
    assert float(APyFixed.from_float(1.234, int_bits=2, frac_bits=1)) == 1.00
    assert float(APyFixed.from_float(-2.666, int_bits=3, frac_bits=1234)) == -2.666

    # Zero floating point numbers
    assert APyFixed.from_float(0.0, bits=1, int_bits=0).to_bits() == 0
    assert APyFixed.from_float(-0.0, bits=1234, int_bits=-1000).to_bits() == 0
    assert APyFixed.from_float(0.0, bits=1234, int_bits=1000).to_bits() == 0
    assert APyFixed.from_float(0.0, bits=1234, int_bits=1000).to_bits() == 0

    # Integer tests
    assert APyFixed.from_float(123.0, 64, 0).to_bits() == 123
    assert APyFixed.from_float(-123.0, 64, 0).to_bits() == 2**64 - 123
    assert APyFixed.from_float(pow(2.0, 52.0) + 0.0, 64, 0).to_bits() == (1 << 52) + 0
    assert APyFixed.from_float(pow(2.0, 52.0) + 1.0, 64, 0).to_bits() == (1 << 52) + 1
    assert APyFixed.from_float(pow(2.0, 53.0) + 0.0, 64, 0).to_bits() == (1 << 53) + 0
    assert (
        APyFixed.from_float(pow(2.0, 53.0) + 1.0, 64, 0).to_bits()
        == (1 << 53) + 0  # Precision was lost when adding 1 onto 2^53
    )
    assert (
        APyFixed.from_float(pow(2.0, 123.0) - pow(2.0, 152), 153, -123).to_bits()
        == (1 << 29) + 1
    )
    assert APyFixed.from_float(
        pow(2.0, 123.0) - pow(2.0, 152), 153, -124
    ).to_bits() == (1 << 28)

    # Fractional number testing
    assert float(APyFixed.from_float(123.125, 0, 64)) == 0.125
    assert APyFixed.from_float(-pow(2.0, -127.0), bits=1, int_bits=-126).to_bits() == 1
    assert (
        APyFixed.from_float(-1.0 + pow(2.0, -53), bits=55, int_bits=2).to_bits()
        == (1 << 54) + (1 << 53) + 1
    )

    # Round away from infinity on tie
    assert APyFixed.from_float(0.24, 1, 1).to_bits() == 0  #  0.00
    assert APyFixed.from_float(0.25, 1, 1).to_bits() == 1  #  0.25
    assert APyFixed.from_float(-0.25, 1, 1).to_bits() == 3  # -0.25
    assert APyFixed.from_float(-0.24, 1, 1).to_bits() == 0  #  0.00


def test_string_construction():
    assert APyFixed.from_str("511", 10, 0).to_bits() == 0x1FF
    assert APyFixed.from_str("512", 10, 0).to_bits() == 0x200
    assert APyFixed.from_str("-511", 10, 0).to_bits() == 0x201
    assert APyFixed.from_str("-512", 10, 0).to_bits() == 0x200
    assert APyFixed.from_str("512", 14, -4).to_bits() == 0x20
    assert APyFixed.from_str("1.0", 9, 1).to_bits() == 2
    assert APyFixed.from_str("1.000000", 9, 1).to_bits() == 2
    assert APyFixed.from_str("10.00", 9, 1).to_bits() == 20
    assert APyFixed.from_str(".0625000", 2, 8).to_bits() == 16

    fx_a = APyFixed.from_str(
        "28948022309329048855892746252171976963317496166410141009892066118088846737408",
        bits=256,
        int_bits=256,
    )
    fx_b = APyFixed.from_str(
        "-1376018206341311063223476816643087998331620501540496640."
        + "021222579872958058370179355618716816066859017361262100333952697594702"
        + "314679773970519809467311447652539955943903993200932791396783892142688"
        + "708904952458654442554723081083186210082207584128592922850820472478833"
        + "257136662269306798708182072507551281664490003441493733349403017982015"
        + "56238154807942919433116912841796875",
        bits=511,
        int_bits=199,
    )
    assert (
        fx_a.to_bits()
        == 28948022309329048855892746252171976963317496166410141009892066118088846737408
    )
    assert (
        str(fx_b)
        == "-1376018206341311063223476816643087998331620501540496640."
        + "021222579872958058370179355618716816066859017361262100333952697594702"
        + "314679773970519809467311447652539955943903993200932791396783892142688"
        + "708904952458654442554723081083186210082207584128592922850820472478833"
        + "257136662269306798708182072507551281664490003441493733349403017982015"
        + "56238154807942919433116912841796875"
    )

    with pytest.raises(ValueError, match="Not a valid decimal numeric string"):
        _ = APyFixed.from_str("Foo", 4, 4)


@pytest.mark.parametrize("bits", [4, 8, 100, 2000])
def test_incorrect_double_construction(bits: int):
    with pytest.raises(ValueError, match="Cannot convert nan to fixed-point"):
        _ = APyFixed.from_float(float("NaN"), bits=bits, frac_bits=4)
    with pytest.raises(ValueError, match="Cannot convert inf to fixed-point"):
        _ = APyFixed.from_float(float("inf"), bits=bits, frac_bits=4)
    with pytest.raises(ValueError, match="Cannot convert nan to fixed-point"):
        _ = APyFixed.from_float(APyFloat(0, 15, 3, 4, 3), bits=bits, frac_bits=4)
    with pytest.raises(ValueError, match="Cannot convert inf to fixed-point"):
        _ = APyFixed.from_float(APyFloat(0, 15, 0, 4, 3), bits=bits, frac_bits=4)


def test_issue_487():
    """
    Test for GitHub issue #487
    https://github.com/apytypes/apytypes/issues/487
    """
    np = pytest.importorskip("numpy")
    fl = 1.2124003311558797e-16
    npa = np.array([fl])
    fx = APyFixed.from_float(fl, 1, 11)
    npa2fx = APyFixed.from_float(npa[0], 1, 11)
    assert fx.is_identical(APyFixed(0, 1, 11))
    assert npa2fx.is_identical(APyFixed(0, 1, 11))


def test_conversion_to_and_from_python_int():
    for k in range(30000):
        a = (1 << (100 + k)) + k
        afx = APyFixed(a, bits=k + 101, int_bits=k // 2)
        assert afx.to_bits() == a
        a -= 1 + 2 * k
        afx = APyFixed(a, bits=k + 101, int_bits=k // 2)
        assert afx.to_bits() == a
