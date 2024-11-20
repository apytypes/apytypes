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

   Change word length
   ------------------

   .. automethod:: cast

   Get bit representation
   ----------------------

   .. automethod:: to_bits

   Comparison
   ----------

   .. automethod:: is_identical

   Properties
   ----------

   Real and imaginary part
   ^^^^^^^^^^^^^^^^^^^^^^^

   .. autoproperty:: real

   .. autoproperty:: imag

   Word length
   ^^^^^^^^^^^

   .. autoproperty:: bits

   .. autoproperty:: int_bits

   .. autoproperty:: frac_bits

   Bit pattern information
   ^^^^^^^^^^^^^^^^^^^^^^^

   .. autoproperty:: is_zero
