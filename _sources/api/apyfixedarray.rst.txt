``APyFixedArray``
=================

.. autoclass:: apytypes.APyFixedArray

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
