``APyCFloatArray``
==================

.. autoclass:: apytypes.APyCFloatArray

   Constructor
   -----------

   .. automethod:: __init__

   Conversion to other types
   -------------------------

   .. automethod:: to_numpy

   .. automethod:: to_bits

   Creation from other types
   -------------------------

   .. automethod:: from_array

   .. automethod:: from_float

   .. automethod:: from_complex

   .. automethod:: from_bits

   Other creation functions
   ------------------------

   .. automethod:: copy

   .. automethod:: eye

   .. automethod:: full

   .. automethod:: identity

   .. automethod:: ones

   .. automethod:: zeros

   Change word length
   ------------------

   .. automethod:: cast

   Comparison
   ----------

   .. automethod:: is_identical

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

   .. automethod:: max

   .. automethod:: min

   .. automethod:: nanmax

   .. automethod:: nanmin

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
