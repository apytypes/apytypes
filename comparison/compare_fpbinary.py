from fpbinary import FpBinary


def create_scalar(val, total_bits, integer_bits):
    return FpBinary(
        value=val, int_bits=integer_bits, frac_bits=total_bits - integer_bits
    )


def create_array(val, total_bits, integer_bits):
    return FpBinary(
        value=val, int_bits=integer_bits, frac_bits=total_bits - integer_bits
    )
