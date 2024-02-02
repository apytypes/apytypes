"""
APyFloat
========

APyTypes provide a class for configurable floating-point numbers.

The numbers are created from the sign-bit, the exponent and the mantissa
(without hidden one) and the corresponding wordlengths. However, there are
also convenience methods to do this either from a float or from a bit-pattern.
"""

from apytypes import APyFloat

a = APyFloat(True, 3, 5, 4, 5)
a

# %%
b = APyFloat.from_float(2.25, 3, 2)
b

# %%
c = APyFloat.from_bits(int("10110101"), 3, 4)
c

# %%
# Standard arithmetic operations are supported. The resulting number of exponent
# and mantissa bits are the maximum of the involved terms.

a + b

# %%
a * c
