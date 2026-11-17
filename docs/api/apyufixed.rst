``APyUFixed``
=============

``APyUFixed`` represents an unsigned fixed-point value. Its ``bits``, ``int_bits``,
and ``frac_bits`` properties describe the bit pattern without a sign bit, so the most
significant integer bit contributes to the value's magnitude.

Operations between two ``APyUFixed`` values return ``APyUFixed`` for addition,
multiplication, and division. Subtraction returns ``APyFixed`` so negative results are
preserved. Mixing with Python integers, floats, or ``APyFixed`` returns ``APyFixed``;
operations with complex values return ``APyCFixed``.

.. autoclass:: apytypes.APyUFixed

   Constructor
   -----------

   .. automethod:: __init__

   Creation from other types
   -------------------------

   .. automethod:: from_float

   .. automethod:: from_number

   .. automethod:: from_str

   Change word length
   ------------------

   .. automethod:: cast

   Get bit representation
   ----------------------

   .. automethod:: to_bits

   Comparison
   ----------

   .. automethod:: is_identical

   Other
   -----

   .. automethod:: as_integer_ratio

   .. automethod:: to_fraction

   .. automethod:: copy

   Properties
   ----------

   Word length
   ^^^^^^^^^^^

   .. autoproperty:: bits

   .. autoproperty:: int_bits

   .. autoproperty:: frac_bits

   Bit pattern information
   ^^^^^^^^^^^^^^^^^^^^^^^

   .. autoproperty:: is_zero

   .. autoproperty:: leading_fractional_zeros

   .. autoproperty:: leading_ones

   .. autoproperty:: leading_signs

   .. autoproperty:: leading_zeros

   .. autoproperty:: trailing_zeros

   Real and imaginary part
   ^^^^^^^^^^^^^^^^^^^^^^^

   .. autoproperty:: real

   .. autoproperty:: imag
