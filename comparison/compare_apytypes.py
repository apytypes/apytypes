from apytypes import APyFixedArray, APyFixed


def create_scalar(val, total_bits, integer_bits):
    return APyFixed.from_float(val, total_bits, integer_bits)


def create_array(val, total_bits, integer_bits):
    return APyFixedArray.from_float(val, total_bits, integer_bits)
