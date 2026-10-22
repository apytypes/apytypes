from apytypes import APyUFixed


def test_unsigned_repr_and_string():
    value = APyUFixed(45, int_bits=3, frac_bits=3)

    assert repr(value) == "APyUFixed(45, int_bits=3, frac_bits=3)"
    assert str(value) == "5.625"
    assert APyUFixed(value.to_bits(), value.int_bits, value.frac_bits).is_identical(
        value
    )


def test_unsigned_latex_representation():
    value = APyUFixed(45, int_bits=3, frac_bits=3)

    assert value._repr_latex_() == r"$\frac{45}{2^{3}} = 5.625$"
