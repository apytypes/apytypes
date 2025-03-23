``APyFixedArray``
=================

.. autoclass:: apytypes.APyFixedArray

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

   .. automethod:: cumsum

   .. automethod:: cumprod

   .. automethod:: max

   .. automethod:: min

   Broadcasting
   ------------

   .. automethod:: broadcast_to

   Properties
   ----------

   Word length
   ^^^^^^^^^^^

   .. autoproperty:: bits

   .. autoproperty:: int_bits

   .. autoproperty:: frac_bits

   Array properties
   ^^^^^^^^^^^^^^^^

   .. autoproperty:: ndim

   .. autoproperty:: shape

   Transposition
   ^^^^^^^^^^^^^

   .. autoproperty:: T
