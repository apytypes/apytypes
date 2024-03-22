import numfi


def create_scalar(val, total_bits, integer_bits):
    return numfi(val, 1, total_bits, total_bits - integer_bits)


def create_array(val, total_bits, integer_bits):
    return numfi(val, 1, total_bits, total_bits - integer_bits)
