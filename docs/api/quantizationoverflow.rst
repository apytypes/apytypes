Quantization and overflow handling
==================================

.. autoclass:: apytypes.QuantizationMode
   :members:
   :undoc-members:

The following figure illustrates the effect of the different quantization modes
on quantizing a floating-point number with three fractional bits to none.
The dots corresponds to different values, where the red dots correspond to
numbers that were initially an integer and yellow dots to numbers that are
exactly between two integers (ties). Below each plot is an error distribution
histogram, where the red line indicates the bias (average error) of the quantization. Note that
the bias will converge towards zero the more bits are quantized away (except for
:py:attr:`QuantizationMode.TRN`, which will converge towards a half).

.. image:: ../_static/quantization.png
   :alt: Illustration of the different quantization modes

.. autoclass:: apytypes.OverflowMode
   :members:
   :undoc-members:

.. autofunction:: apytypes.get_quantization_mode

.. autofunction:: apytypes.set_quantization_mode

.. autofunction:: apytypes.get_quantization_seed

.. autofunction:: apytypes.set_quantization_seed
