from apytypes import APyFixed

import pytest


def test_bit_pattern_to_int():
    assert APyFixed(0, 1, 0).bit_pattern_to_int(False) == 0
    assert APyFixed(0, 1, 0).bit_pattern_to_int(True) == 0
    # Python short int
    assert (
        APyFixed.from_float(1.00, int_bits=4, frac_bits=4).bit_pattern_to_int() == 2**4
    )
    assert (
        APyFixed.from_float(-1.00, int_bits=4, frac_bits=4).bit_pattern_to_int()
        == 2**7 + 2**6 + 2**5 + 2**4
    )
    assert APyFixed.from_float(-1.00, int_bits=4, frac_bits=4).bit_pattern_to_int(
        allow_negative_return_value=True
    ) == -(2**4)


def test_bit_specifier_getters():
    """
    Bit specifier getter functions:
      * `bits()`
      * `int_bits()`
      * `frac_bits()`
    """
    assert APyFixed(0, 12345, 12).bits == 12345
    assert APyFixed(0, 12345, 12).int_bits == 12
    assert APyFixed(0, 12345, 12).frac_bits == 12345 - 12


def test_to_float():
    assert float(APyFixed(0, 1, 0)) == 0.0
    assert float(APyFixed(0, 1234, 123)) == 0.0
    assert float(APyFixed(0, 1234, -123)) == 0.0
    assert float(APyFixed(0, 1234, 12345)) == 0.0
    assert float(APyFixed(0b00100, 5, 1)) == 0.25
    assert float(APyFixed(0b11100, 5, 1)) == -0.25
    assert float(APyFixed(0x48, 8, 4)) == 4.5
    assert float(APyFixed(0x558 << 128, 128 + 12, 8)) == 85.5


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
        str(APyFixed((1 << 512) + (1 << 123) + (1 << 64) + (1 << 63), 1000, 1000))
        == "13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486061487577849091273580847006241813168128"
    )
    assert (
        str(APyFixed((1 << 512) + (1 << 123) + (1 << 64) + (1 << 63), 1000, 900))
        == "10576895500643977583230644928524336637254474927428499508554380724390492659780981533203027367035444557561459392400373741256704.0000000000218278728425502777099609375"
    )
    assert (
        str(APyFixed((1 << 999) + (1 << 123) + (1 << 64) + (1 << 63), 1000, 1000))
        == "-5357543035931336604742125245300009052807024058527668037218751941851755255624680612465991894078479290637973364587765734125935726428461570217992288787349287401967283887412115492710537302531185570938977091076523237491790970633699383779582771973038531457285598238843260450006248636499301292846010026950656"
    )
    assert (
        str(APyFixed((1 << 999) + (1 << 123) + (1 << 64) + (1 << 63), 1000, 900))
        == "-4226356249085321970818718279332132852150778608288972177023685672213391220453798875795338047101257503157395159946057029431058780476021484298004311827703516615267093471992040673349852141411411528424193863265689507233184226342012493910707175190136291811916308647181895598079.9999999999781721271574497222900390625"
    )


def test_is_positive():
    a = APyFixed(4, 3, 2)
    assert not a.is_positive
    a = APyFixed(3, 3, 2)
    assert a.is_positive
    a = APyFixed(0, 3, 2)
    assert not a.is_positive
    a = APyFixed(-3, 3, 2)
    assert not a.is_positive


def test_is_negative():
    a = APyFixed(4, 3, 2)
    assert a.is_negative
    a = APyFixed(3, 3, 2)
    assert not a.is_negative
    a = APyFixed(0, 3, 2)
    assert not a.is_negative
    a = APyFixed(-3, 3, 2)
    assert a.is_negative
