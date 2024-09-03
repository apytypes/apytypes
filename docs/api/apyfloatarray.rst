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

   Other creation functions
   ------------------------

   .. automethod:: zeros

   .. automethod:: ones

   .. automethod:: eye

   .. automethod:: identity

   .. automethod:: full

   Change word length
   ------------------

   .. automethod:: cast

   Comparison
   ----------

   .. automethod:: is_identical

   Convert to NumPy
   ----------------

   .. automethod:: to_numpy

   Convolution
   -----------

   .. automethod:: convolve

   Transposition
   -------------

   .. automethod:: transpose

   Array shape manipulation
   ------------------------

   .. automethod:: flatten

   .. automethod:: ravel

   .. automethod:: reshape

   .. automethod:: squeeze

   .. automethod:: swapaxes

   Mathematical functions
   ----------------------

   .. automethod:: sum

   .. automethod:: prod

   .. automethod:: nansum

   .. automethod:: nanprod

   .. automethod:: cumsum

   .. automethod:: cumprod

   .. automethod:: nancumsum

   .. automethod:: nancumprod

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

   Transposition
   ^^^^^^^^^^^^^

   .. autoproperty:: T
