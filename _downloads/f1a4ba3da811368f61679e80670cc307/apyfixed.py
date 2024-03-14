"""
APyFixed
========

APyTypes provide a class for configurable floating-point numbers: :class:`APyFixed`.

The numbers are represented as the underlying integer, the total number of bits and integer bits.
However, there are also convenience methods to do this either from a float, a string, or a bit-pattern.
One can also supply the number of fractional bits; two out of the three (total, integer, and fractional
bits) must be supplied.
"""

from apytypes import APyFixed

a = APyFixed(7, 4, 2)
a

# %%
b = APyFixed.from_float(-2.25, int_bits=3, frac_bits=2)
b

# %%
c = APyFixed.from_str("3.75", bits=6, frac_bits=3)
c

# %%
# Standard arithmetic operations are supported. The resulting number of bits depends on the operation.
# For addition and subtraction, the maximum number of integer bits and fractional bits are used.
# A single integer bit is added to make sure that the computation does not overflow.
a + b

# %%
# For multiplication, the number of integer bits and fractional bits is the sum of the number of
# integer bits and fractional bits of the included operands, respectively.
a * c

# %%
# Standard string formatting is supported
str(a)

# %%
# Comparisons with integers and floats are supported
(b >= -2.25, a == 1.75)

# %%
# To change the word length of a APyFixed, the method :func:`~APyFixed.cast` can be used
(a + b).cast(4, 3)
