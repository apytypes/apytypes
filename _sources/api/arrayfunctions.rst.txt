Array functions
===============

.. _array-assignment:

Array assignment
----------------

Assigning an APyTypes object to an APyTypes array requires matching format specifiers.
For convenience, Python numeric values can be assigned directly to APyTypes arrays
without an explicit conversion.
The value is converted using the destination array's format, just as with the
corresponding scalar type's ``from_float(value, spec=array)`` method.

For example::

	import apytypes as apy

	array = apy.APyFloatArray.from_float([0, 0, 0], exp_bits=7, man_bits=14, bias=13)
	array[0] = 1.3
	# Equivalent to: array[0] = apy.APyFloat.from_float(1.3, spec=array)

For complex arrays, a Python ``int`` or ``float`` sets the real part and leaves the
imaginary part as zero. Python ``complex`` values can also be assigned to complex
arrays, with both parts converted to the array's format.

Array creation functions
------------------------

.. autofunction:: apytypes.zeros

.. autofunction:: apytypes.ones

.. autofunction:: apytypes.eye

.. autofunction:: apytypes.identity

.. autofunction:: apytypes.full

.. autofunction:: apytypes.zeros_like

.. autofunction:: apytypes.ones_like

.. autofunction:: apytypes.full_like

.. autofunction:: apytypes.arange

.. autofunction:: apytypes.fullrange

.. autofunction:: apytypes.meshgrid


Array modification functions
----------------------------

.. autofunction:: apytypes.expand_dims

.. autofunction:: apytypes.flatten

.. autofunction:: apytypes.moveaxis

.. autofunction:: apytypes.ravel

.. autofunction:: apytypes.reshape

.. autofunction:: apytypes.squeeze

.. autofunction:: apytypes.swapaxes

.. autofunction:: apytypes.transpose


Array utility functions
-----------------------

.. autofunction:: apytypes.convolve

.. autofunction:: apytypes.export_csv

.. autofunction:: apytypes.import_csv

.. autofunction:: apytypes.outer

.. autofunction:: apytypes.shape

.. autofunction:: apytypes.nextafter
