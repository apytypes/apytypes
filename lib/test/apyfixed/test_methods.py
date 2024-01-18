from apytypes import APyFixed


def test_repr():
    assert (
        APyFixed(-1, int_bits=12, frac_bits=4).__repr__()
        == "APyFixed(65535, bits=16, int_bits=12)"
    )
    assert (
        APyFixed(1234, int_bits=12, frac_bits=0).__repr__()
        == "APyFixed(1234, bits=12, int_bits=12)"
    )
    assert (
        APyFixed.from_float(-2.666, int_bits=3, frac_bits=52).__repr__()
        == "APyFixed(24022200412394226, bits=55, int_bits=3)"
    )

    #
    # Related GitHub issue: #2
    # `Excecive ones when printing bit-pattern for negative fixed-point number`
    #
    assert (
        APyFixed.from_float(-5.75, int_bits=4, frac_bits=4).__repr__()
        == "APyFixed(164, bits=8, int_bits=4)"
    )
    assert (
        APyFixed.from_float(-5.75, int_bits=4, frac_bits=4).bit_pattern_to_int() == 164
    )
    assert (
        APyFixed.from_float(-5.75, int_bits=4, frac_bits=4).bit_pattern_to_string_dec()
        == "164"
    )


def test_bit_pattern_to_int():
    assert (
        APyFixed.from_float(1.00, int_bits=4, frac_bits=4).bit_pattern_to_int()
        == 2**4
    )
    assert (
        APyFixed.from_float(-1.00, int_bits=4, frac_bits=4).bit_pattern_to_int()
        == 2**7 + 2**6 + 2**5 + 2**4
    )
    assert APyFixed.from_float(-1.00, int_bits=4, frac_bits=4).bit_pattern_to_int(
        allow_negative_return_value=True
    ) == -(2**4)
