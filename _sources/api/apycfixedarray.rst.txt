``APyCFixedArray``
==================

.. autoclass:: apytypes.APyCFixedArray

   Constructor
   -----------

   .. automethod:: __init__

   Conversion to other types
   -------------------------

   .. automethod:: to_numpy

   Creation from other types
   -------------------------

   .. automethod:: from_array

   .. automethod:: from_complex

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

   Transposition
   -------------

   .. automethod:: transpose

   .. automethod:: hermitian_transpose

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

   .. automethod:: conj

   Convolution
   -----------

   .. automethod:: convolve

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

   .. autoproperty:: H

   Real and imaginary part
   ^^^^^^^^^^^^^^^^^^^^^^^

   .. autoproperty:: real

   .. autoproperty:: imag
