from en_cl_fix_pkg import FixFormat, WideFix


def create_scalar(val, integer_bits, fractional_bits):
    f = FixFormat(1, integer_bits - 1, fractional_bits)
    return WideFix.from_real(val, f)


def create_array(val, integer_bits, fractional_bits):
    f = FixFormat(1, integer_bits - 1, fractional_bits)
    return WideFix.from_real(val, f)
