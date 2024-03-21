from FixedPoint import FXfamily, FXnum


def create_scalar(val, total_bits, integer_bits):
    family = FXfamily(n_bits=total_bits - integer_bits, n_intbits=integer_bits)
    return FXnum(val, family=family)


def create_array(val, total_bits, integer_bits):
    family = FXfamily(n_bits=total_bits - integer_bits, n_intbits=integer_bits)
    return FXnum(val, family=family)
