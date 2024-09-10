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
