from apytypes import APyFixed


def test_arithmetic_operations():
    a = APyFixed.from_float(-1.25, int_bits=2, frac_bits=4)
    b = APyFixed.from_float(0.75, int_bits=2, frac_bits=3)
    assert a * b == APyFixed.from_float(-0.9375, int_bits=4, frac_bits=7)
    assert a * b == APyFixed.from_float(-0.9375, int_bits=4, frac_bits=5)
    assert a + b == APyFixed.from_float(-0.5, int_bits=3, frac_bits=4)
    assert b + a == APyFixed.from_float(-0.5, int_bits=3, frac_bits=4)
    assert a - b == APyFixed.from_float(-2, int_bits=3, frac_bits=4)
    assert b - a == APyFixed.from_float(2, int_bits=3, frac_bits=4)
    assert a / b == APyFixed.from_float(-1.625, int_bits=6, frac_bits=3)
    assert b / a == APyFixed.from_float(-0.5, int_bits=7, frac_bits=3)
