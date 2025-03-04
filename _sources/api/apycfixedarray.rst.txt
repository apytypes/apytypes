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

   Broadcasting
   ------------

   .. automethod:: broadcast_to

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

   Array properties
   ^^^^^^^^^^^^^^^^

   .. autoproperty:: ndim

   .. autoproperty:: shape

   Transposition
   ^^^^^^^^^^^^^

   .. autoproperty:: T
