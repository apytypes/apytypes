``APyFixed``
============

.. autoclass:: apytypes.APyFixed

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
