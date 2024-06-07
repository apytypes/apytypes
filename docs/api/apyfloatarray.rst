``APyFloatArray``
=================

.. autoclass:: apytypes.APyFloatArray

   Constructor
   -----------

   .. automethod:: __init__

   Creation from other types
   -------------------------

   .. automethod:: from_array

   .. automethod:: from_float

   Change word length
   ------------------

   .. automethod:: cast

   Comparison
   ----------

   .. automethod:: is_identical

   Convert to NumPy
   ----------------

   .. automethod:: to_numpy

   Tranposition
   ------------

   .. automethod:: transpose

   Array shape manipulation
   -----------------------

   .. automethod:: squeeze

   .. automethod:: reshape

   Broadcasting
   ------------

   .. automethod:: broadcast_to

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

   Array properties
   ^^^^^^^^^^^^^^^^

   .. autoproperty:: ndim

   .. autoproperty:: shape

   Tranposition
   ^^^^^^^^^^^^

   .. autoproperty:: T
