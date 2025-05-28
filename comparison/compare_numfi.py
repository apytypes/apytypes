import numfi


def create_scalar(val, integer_bits, fractional_bits):
    return numfi.numfi(val, 1, integer_bits + fractional_bits, fractional_bits)


def create_array(val, integer_bits, fractional_bits):
    return numfi.numfi(val, 1, integer_bits + fractional_bits, fractional_bits)
