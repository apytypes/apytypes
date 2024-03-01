from fxpmath import Fxp


def create_scalar(val, total_bits, integer_bits):
    return Fxp(val, n_word=total_bits, n_frac=total_bits - integer_bits)


def create_array(val, total_bits, integer_bits):
    return Fxp(val, n_word=total_bits, n_frac=total_bits - integer_bits)
