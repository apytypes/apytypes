from FixedPoint import FXfamily, FXnum


def create_scalar(val, integer_bits, fractional_bits):
    family = FXfamily(n_bits=fractional_bits, n_intbits=integer_bits)
    return FXnum(val, family=family)


def create_array(val, integer_bits, fractional_bits):
    family = FXfamily(n_bits=fractional_bits, n_intbits=integer_bits)
    return FXnum(val, family=family)
