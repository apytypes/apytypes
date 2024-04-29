from fpbinary import FpBinary


def create_scalar(val, integer_bits, fractional_bits):
    return FpBinary(value=val, int_bits=integer_bits, frac_bits=fractional_bits)


def create_array(val, integer_bits, fractional_bits):
    return FpBinary(value=val, int_bits=integer_bits, frac_bits=fractional_bits)
