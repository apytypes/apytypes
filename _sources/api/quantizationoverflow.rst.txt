Quantization and overflow handling
==================================

The following figure illustrates the effect of the different quantization modes
on quantizing a fixed-point number with three fractional bits to none.
The dots corresponds to different values, where the red dots correspond to
numbers that were initially an integer and yellow dots to numbers that are
exactly between two integers (ties). Below each plot is an error distribution
histogram, where the red line indicates the bias (average error) of the quantization. Note that
the bias will converge towards zero the more bits are quantized away (except for
:py:attr:`QuantizationMode.TRN`, which will converge towards a half).

.. image:: ../_static/quantization.png
   :alt: Illustration of the different quantization modes

.. autoclass:: apytypes.QuantizationMode

   .. autoattribute:: TIES_POS

      Round to nearest, ties towards positive infinity.

   .. image:: ../_static/TIES_POS.png
      :alt: Illustration of round to nearest, ties towards positive

   .. autoattribute:: TIES_EVEN

      Round to nearest, ties towards even.

   .. image:: ../_static/TIES_EVEN.png
      :alt: Illustration of round to nearest, ties to even

   .. autoattribute:: TIES_ODD

      Round to nearest, ties towards odd.

   .. image:: ../_static/TIES_ODD.png
      :alt: Illustration of round to nearest, ties to odd

   .. autoattribute:: TIES_AWAY

      Round to nearest, ties away from zero.

   .. image:: ../_static/TIES_AWAY.png
      :alt: Illustration of round to nearest, ties away from zero

   .. autoattribute:: TIES_NEG

      Round to nearest, ties towards negative infinity.

   .. image:: ../_static/TIES_NEG.png
      :alt: Illustration of round to nearest, ties towards negative inifinity

   .. autoattribute:: TIES_ZERO

      Round to nearest, ties towards zero.

   .. image:: ../_static/TIES_ZERO.png
      :alt: Illustration of round to nearest, ties towards zero

   .. autoattribute:: TO_NEG

      Rounding towards negative infinity (truncation).

      Implementation: remove additional bits.

   .. image:: ../_static/TO_NEG.png
      :alt: Illustration of tounding towards negative infinity (truncation)

   .. autoattribute:: TO_POS

      Rounding away from zero.

   .. image:: ../_static/TO_POS.png
      :alt: Illustration of rounding away from zero

   .. autoattribute:: TO_ZERO

      Rounding towards zero (unbiased magnitude truncation).

   .. image:: ../_static/TO_ZERO.png
      :alt: Illustration of rounding towards zero (unbiased magnitude truncation)

   .. autoattribute:: TRN_MAG

      Magnitude truncation.

   .. image:: ../_static/TRN_MAG.png
      :alt: Illustration of magnitude truncation

   .. autoattribute:: JAM

      Jamming/von Neumann rounding.

   .. image:: ../_static/JAM.png
      :alt: Illustration of jamming/von Neumann rounding

   .. autoattribute:: JAM_UNBIASED

      Unbiased jamming/von Neumann rounding.

   .. image:: ../_static/JAM_UNBIASED.png
      :alt: Illustration of unbiased jamming/von Neumann rounding

   Aliases
   -------

   .. autoattribute:: TRN

      Fixed-point truncation. Round towards negative infinity. Alias for :attr:`TO_NEG`.

   .. autoattribute:: TRN_INF

      Round towards positive infinity. Alias for :attr:`TO_POS`.

   .. autoattribute:: TRN_ZERO

      Unbiased magnitude truncation. Round towards zero. Alias for :attr:`TO_ZERO`.

   .. autoattribute:: RND

      Fixed-point rounding. Round to nearest, ties towards positive infinity. Alias for :attr:`TIES_POS`.

   .. autoattribute:: RND_CONV

      Unbiased fixed-point rounding. Round to nearest, ties to even. Alias for :attr:`TIES_EVEN`.

   .. autoattribute:: RND_CONV_ODD

      Altenate unbiased fixed-point rounding. Round to nearest, ties to odd. Alias for :attr:`TIES_ODD`.

   .. autoattribute:: RND_INF

      Round to nearest, ties away from zero. Alias for :attr:`TIES_AWAY`.

   .. autoattribute:: RND_MIN_INF

      Round to nearest, ties towards negative infinity. Alias for :attr:`TIES_NEG`.

   .. autoattribute:: RND_ZERO

      Round to nearest, ties towards zero. Alias for :attr:`TIES_ZERO`.

.. autoclass:: apytypes.OverflowMode

   .. autoattribute:: WRAP

      Two's complement wrapping.

   .. autoattribute:: SAT

      Two's complement saturation.

   .. autoattribute:: NUMERIC_STD

      Keep sign bit and remove intermediate bits.

      As ``resize`` for ``signed`` in ``ieee.numeric_std``.

.. autofunction:: apytypes.get_quantization_mode

.. autofunction:: apytypes.set_quantization_mode

.. autofunction:: apytypes.get_quantization_seed

.. autofunction:: apytypes.set_quantization_seed
