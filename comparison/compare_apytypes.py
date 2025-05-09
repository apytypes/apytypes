from apytypes import APyFixed, APyFixedArray


def create_scalar(val, integer_bits, fractional_bits):
    return APyFixed.from_float(val, integer_bits, fractional_bits)


def create_array(val, integer_bits, fractional_bits):
    return APyFixedArray.from_float(val, integer_bits, fractional_bits)
