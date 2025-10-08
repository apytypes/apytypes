``APyCFloat``
=============

.. autoclass:: apytypes.APyCFloat

   Constructor
   -----------

   .. automethod:: __init__

   Creation from other types
   -------------------------

   .. automethod:: from_complex

   .. automethod:: from_float

   .. automethod:: from_bits

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

   .. autoproperty:: real

   .. autoproperty:: imag

   .. autoproperty:: is_zero

   Word length
   ^^^^^^^^^^^

   .. autoproperty:: bits

   .. autoproperty:: exp_bits

   .. autoproperty:: man_bits

   .. autoproperty:: bias
