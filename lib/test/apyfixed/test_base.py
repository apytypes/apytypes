from apytypes import APyFixed
import pytest


def test_floating_point_construction():
    """
    Test creation of `APyFixed` from floating-point objects with static method
    APyFixed.from_float()
    """
    assert float(APyFixed.from_float(1.234, int_bits=2, frac_bits=2)) == 1.25
    assert float(APyFixed.from_float(1.234, int_bits=2, frac_bits=1)) == 1.00
    assert float(APyFixed.from_float(2.666, int_bits=3, frac_bits=1234)) == 2.666

    assert (
        APyFixed.from_float(2.666, int_bits=3, frac_bits=52).__repr__()
        == "APyFixed(bit_pattern=12006596606569742, bits=55, int_bits=3)"
    )


def test_construction_raises():
    """
    Disallowed construction of `APyFixed` object
    """

    # Zero-bit fixed-point nubmers can not be created
    with pytest.raises(ValueError):
        APyFixed(0, bits=0, int_bits=0)

    # Exactly two of three bit-specifiers needs to be set when creating `APyFixed`
    with pytest.raises(ValueError):
        APyFixed(0, bits=1, int_bits=0, frac_bits=0)

    with pytest.raises(ValueError):
        APyFixed(0, frac_bits=1)

    with pytest.raises(ValueError):
        APyFixed(0, int_bits=1)

    with pytest.raises(ValueError):
        APyFixed(0, bits=1)
