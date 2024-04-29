from fixedpoint import FixedPoint


def create_scalar(val, integer_bits, fractional_bits):
    return FixedPoint(val, m=integer_bits, n=fractional_bits)


def create_array(val, integer_bits, fractional_bits):
    return FixedPoint(val, m=integer_bits, n=fractional_bits)
