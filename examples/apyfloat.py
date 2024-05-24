"""
APyFloat
========

APyTypes provide a class for configurable floating-point numbers: :class:`APyFloat`.

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

# %%
# Standard string formatting is supported
str(a)

# %%
# Comparisons with integers and floats are supported
(b >= 7, a < -1.2)

# %%
# Arithmetic with Python integers and floats is supported, although *not recommended*, by
# automatically casting the number to the same format of the APyType-object
b + 1.125

# %%
# Which is equivalent to
b + APyFloat.from_float(1.125, exp_bits=b.exp_bits, man_bits=b.man_bits, bias=b.bias)

# %%
# However, while it is convenient to let APyTypes convert numbers automatically,
# it is not recommended as it can yield unexpected results. Consider the expression
2 + 2 + APyFloat.from_float(8, exp_bits=4, man_bits=1)

# %%
# which would yield a different result from
APyFloat.from_float(8, exp_bits=4, man_bits=1) + 2 + 2

# %%
# To change the word length of a APyFloat, the method :func:`~APyFloat.cast` can be used
(a + b).cast(4, 3)
