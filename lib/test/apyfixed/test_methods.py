from apytypes import APyFixed

import pytest


def test_to_bits():
    assert APyFixed(0, 1, 0).to_bits() == 0
    # Python short int
    assert APyFixed.from_float(1.00, int_bits=4, frac_bits=4).to_bits() == 2**4
    assert (
        APyFixed.from_float(-1.00, int_bits=4, frac_bits=4).to_bits()
        == 2**7 + 2**6 + 2**5 + 2**4
    )


def test_bit_specifier_getters():
    """
    Bit specifier getter functions:
      * `bits()`
      * `int_bits()`
      * `frac_bits()`
    """
    assert APyFixed(0, bits=12345, int_bits=12).bits == 12345
    assert APyFixed(0, bits=12345, int_bits=12).int_bits == 12
    assert APyFixed(0, bits=12345, int_bits=12).frac_bits == 12345 - 12


def test_to_float():
    assert float(APyFixed(0, bits=1, int_bits=0)) == 0.0
    assert float(APyFixed(0, bits=1234, int_bits=123)) == 0.0
    assert float(APyFixed(0, bits=1234, int_bits=-123)) == 0.0
    assert float(APyFixed(0, bits=1234, int_bits=12345)) == 0.0
    assert float(APyFixed(0b00100, int_bits=1, frac_bits=4)) == 0.25
    assert float(APyFixed(0b11100, int_bits=1, frac_bits=4)) == -0.25
    assert float(APyFixed(0x48, int_bits=4, frac_bits=4)) == 4.5
    assert float(APyFixed(0x558 << 128, bits=128 + 12, int_bits=8)) == 85.5

    ##
    # Positive subnormals
    #

    # Smallest positive normal number
    assert float(APyFixed(1, int_bits=0, frac_bits=1022)) == 2.2250738585072014e-308

    # Greatest positive subnormal number
    assert float(APyFixed(1, int_bits=0, frac_bits=1023)) == 1.1125369292536007e-308

    # Smallest positive subnormal number
    assert float(APyFixed(1, int_bits=0, frac_bits=1074)) == 5e-324

    # 1/2 of the smallest positive subnormal number (ties to the smallest positive
    # subnormal number)
    assert float(APyFixed(1, int_bits=0, frac_bits=1075)) == 5e-324

    # 1/4 of the smallest subnormal number, rounds towards zero
    assert float(APyFixed(1, int_bits=0, frac_bits=1076)) == 0.0

    ##
    # Negative subnormals
    #

    # Greatest negative normal number
    assert float(APyFixed(-1, int_bits=0, frac_bits=1022)) == -2.2250738585072014e-308

    # Smallest negative subnormal number
    assert float(APyFixed(-1, int_bits=0, frac_bits=1023)) == -1.1125369292536007e-308

    # Greatest negative subnormal number
    assert float(APyFixed(-1, int_bits=0, frac_bits=1074)) == -5e-324

    # 1/2 of the greatest negative subnormal number (ties to the greatest negative
    # subnormal number)
    assert float(APyFixed(-1, int_bits=0, frac_bits=1075)) == -5e-324

    # 1/4 of the greatest negative subnormal number, rounds towards zero
    assert float(APyFixed(-1, int_bits=0, frac_bits=1076)) == 0.0


def test_from_float():
    ##
    # Positive subnormals
    #

    # Smallest positive normal number
    assert APyFixed.from_float(2**-1022, int_bits=0, frac_bits=1024).is_identical(
        APyFixed(4, int_bits=0, frac_bits=1024)
    )

    # Greatest positive subnormal number
    assert APyFixed.from_float(2**-1023, int_bits=0, frac_bits=1024).is_identical(
        APyFixed(2, int_bits=0, frac_bits=1024)
    )

    # Smallest positive subnormal number
    assert APyFixed.from_float(2**-1074, int_bits=0, frac_bits=1074).is_identical(
        APyFixed(1, int_bits=0, frac_bits=1074)
    )

    ##
    # Negative subnormals
    #

    # Greatest negative normal number
    assert APyFixed.from_float(-(2**-1022), int_bits=0, frac_bits=1024).is_identical(
        APyFixed(-4, int_bits=0, frac_bits=1024)
    )

    # Smallest negative subnormal number
    assert APyFixed.from_float(-(2**-1023), int_bits=0, frac_bits=1024).is_identical(
        APyFixed(-2, int_bits=0, frac_bits=1024)
    )

    # Greatest negative subnormal number
    assert APyFixed.from_float(-(2**-1074), int_bits=0, frac_bits=1074).is_identical(
        APyFixed(-1, int_bits=0, frac_bits=1074)
    )


def test_is_zero():
    for bit_pattern in range(0, 1 << 4):
        fx = APyFixed(bit_pattern, 4, 2)
        if bit_pattern == 0:
            assert fx.is_zero
        else:
            assert not (fx.is_zero)


def test_to_string():
    """
    Test `__str__` and `to_string` methods
    """
    assert (
        str(APyFixed((1 << 512) + (1 << 123) + (1 << 64) + (1 << 63), 1000, 0))
        == "13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486061487577849091273580847006241813168128"
    )
    assert (
        str(APyFixed((1 << 512) + (1 << 123) + (1 << 64) + (1 << 63), 900, 100))
        == "10576895500643977583230644928524336637254474927428499508554380724390492659780981533203027367035444557561459392400373741256704.0000000000218278728425502777099609375"
    )
    assert (
        str(APyFixed((1 << 999) + (1 << 123) + (1 << 64) + (1 << 63), 1000, 0))
        == "-5357543035931336604742125245300009052807024058527668037218751941851755255624680612465991894078479290637973364587765734125935726428461570217992288787349287401967283887412115492710537302531185570938977091076523237491790970633699383779582771973038531457285598238843260450006248636499301292846010026950656"
    )
    assert (
        str(APyFixed((1 << 999) + (1 << 123) + (1 << 64) + (1 << 63), 900, 100))
        == "-4226356249085321970818718279332132852150778608288972177023685672213391220453798875795338047101257503157395159946057029431058780476021484298004311827703516615267093471992040673349852141411411528424193863265689507233184226342012493910707175190136291811916308647181895598079.9999999999781721271574497222900390625"
    )


def test_is_positive():
    a = APyFixed(4, int_bits=2, frac_bits=1)
    assert not a._is_positive
    a = APyFixed(3, int_bits=2, frac_bits=1)
    assert a._is_positive
    a = APyFixed(0, int_bits=2, frac_bits=1)
    assert not a._is_positive
    a = APyFixed(-3, int_bits=2, frac_bits=1)
    assert not a._is_positive


def test_is_negative():
    a = APyFixed(4, int_bits=2, frac_bits=1)
    assert a._is_negative
    a = APyFixed(3, int_bits=2, frac_bits=1)
    assert not a._is_negative
    a = APyFixed(0, int_bits=2, frac_bits=1)
    assert not a._is_negative
    a = APyFixed(-3, int_bits=2, frac_bits=1)
    assert a._is_negative


def test_round_trip_conversion():
    for bits in range(256):
        a = APyFixed(bits, bits=8, int_bits=4)
        assert (APyFixed.from_float(float(a), bits=8, int_bits=4)).is_identical(a)


def test_fixed_cast_throws():
    # Only specifying `bits` in a cast raises
    with pytest.raises(ValueError, match="Fixed-point casting bit specification needs"):
        APyFixed(0, int_bits=10, frac_bits=5).cast(bits=7)

    # Specifying `bits`, `int_bits` and `frac_bits` in a cast raises
    with pytest.raises(ValueError, match="Fixed-point casting bit specification needs"):
        APyFixed(0, int_bits=10, frac_bits=5).cast(bits=7, int_bits=2, frac_bits=5)

    # Casting `bits` to zero or less raises exception
    for i in range(-5, 0 + 1):
        with pytest.raises(ValueError, match="Fixed-point casting bit specification"):
            APyFixed(0, int_bits=10, frac_bits=5).cast(bits=0, int_bits=0)

    # Only specifying `int_bits` or `frac_bits` does *not* throw
    APyFixed(0, int_bits=10, frac_bits=5).cast(7)
    APyFixed(0, int_bits=10, frac_bits=5).cast(int_bits=7)
    APyFixed(0, int_bits=10, frac_bits=5).cast(frac_bits=7)

    # Any two-combination of bit-specifiers does not throw
    APyFixed(0, int_bits=10, frac_bits=5).cast(bits=5, int_bits=2)
    APyFixed(0, int_bits=10, frac_bits=5).cast(bits=5, frac_bits=2)
    APyFixed(0, int_bits=10, frac_bits=5).cast(int_bits=5, frac_bits=2)

    # Casting to a negative number of `bits` casts


def test_from_float_with_non_floats():
    assert APyFixed.from_float(3, 5, 0).is_identical(
        APyFixed(3, int_bits=5, frac_bits=0)
    )
    assert APyFixed.from_float(3, 5, 1).is_identical(
        APyFixed(6, int_bits=5, frac_bits=1)
    )
    assert APyFixed.from_float(-3, 5, 0).is_identical(APyFixed(29, bits=5, int_bits=5))

    # from_float currently wraps around
    assert APyFixed.from_float(17, 5, 0).is_identical(
        APyFixed(17, int_bits=5, frac_bits=0)
    )
    assert APyFixed.from_float(17, 4, 0).is_identical(
        APyFixed(1, int_bits=4, frac_bits=0)
    )

    # Test long integer (256 bits)
    int_val = 0xFF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF_FF
    assert APyFixed.from_float(int_val, 257, 0).is_identical(
        APyFixed(int_val, int_bits=257, frac_bits=0)
    )

    # Test negative number of fractioal bits
    assert APyFixed.from_float(14, int_bits=6, frac_bits=-1).is_identical(
        APyFixed(7, bits=5, int_bits=6)
    )

    # Test from bool
    APyFixed.from_float(True, 5, 0).is_identical(APyFixed(1, int_bits=5, frac_bits=0))

    # Raise for string
    with pytest.raises(ValueError, match="Non supported type"):
        APyFixed.from_float("14", 5, 0)
