from fxpmath import Fxp


def create_scalar(val, integer_bits, fractional_bits):
    return Fxp(val, n_word=integer_bits + fractional_bits, n_frac=fractional_bits)


def create_array(val, integer_bits, fractional_bits):
    return Fxp(val, n_word=integer_bits + fractional_bits, n_frac=fractional_bits)
