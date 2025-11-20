``APyCFixed``
=============

.. autoclass:: apytypes.APyCFixed

   Constructor
   -----------

   .. automethod:: __init__

   Creation from other types
   -------------------------

   .. automethod:: from_complex

   .. automethod:: from_float

   .. automethod:: from_number

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

   .. automethod:: conj

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

   .. autoproperty:: leading_ones

   .. autoproperty:: leading_signs

   .. autoproperty:: leading_zeros

   .. autoproperty:: trailing_zeros

   Real and imaginary part
   ^^^^^^^^^^^^^^^^^^^^^^^

   .. autoproperty:: real

   .. autoproperty:: imag
