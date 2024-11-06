from apytypes._apytypes import (
    APyFixed as APyFixed,
    APyFixedArray as APyFixedArray,
    APyFloat as APyFloat,
    APyFloatArray as APyFloatArray,
)

def fp(val, exp_bits=None, man_bits=None, bias=None): ...
def fx(val, int_bits=None, frac_bits=None, bits=None): ...
