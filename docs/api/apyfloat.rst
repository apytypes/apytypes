``APyFloat``
============

.. autoclass:: apytypes.APyFloat

   Constructor
   -----------

   .. automethod:: __init__

   Creation from other types
   -------------------------

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

   Convenience casting methods
   ---------------------------

   .. automethod:: cast_to_bfloat16

   .. automethod:: cast_to_double

   .. automethod:: cast_to_half

   .. automethod:: cast_to_single

   Properties
   ----------

   Word length
   ^^^^^^^^^^^

   .. autoproperty:: bits

   .. autoproperty:: exp_bits

   .. autoproperty:: man_bits

   .. autoproperty:: bias

   Values
   ^^^^^^
   .. autoproperty:: sign

   .. autoproperty:: exp

   .. autoproperty:: man

   .. autoproperty:: true_sign

   .. autoproperty:: true_exp

   .. autoproperty:: true_man

   Bit pattern information
   ^^^^^^^^^^^^^^^^^^^^^^^

   .. autoproperty:: is_inf

   .. autoproperty:: is_nan

   .. autoproperty:: is_finite

   .. autoproperty:: is_normal

   .. autoproperty:: is_subnormal
