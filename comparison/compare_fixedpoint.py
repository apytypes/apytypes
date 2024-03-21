from fixedpoint import FixedPoint


def create_scalar(val, total_bits, integer_bits):
    return FixedPoint(val, m=integer_bits, n=total_bits - integer_bits)


def create_array(val, total_bits, integer_bits):
    return FixedPoint(val, m=integer_bits, n=total_bits - integer_bits)
