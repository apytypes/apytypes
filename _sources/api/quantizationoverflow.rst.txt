Quantization and overflow handling
==================================

Quantization modes
------------------

The following figure illustrates the effect of the different quantization modes
on quantizing a fixed-point number with three fractional bits to none.
The dots corresponds to different values, where the red dots correspond to
numbers that were initially an integer and yellow dots to numbers that are
exactly between two integers (ties). Below each plot is an error distribution
histogram, where the red line indicates the bias (average error) of the quantization. Note that
the bias will converge towards zero the more bits are quantized away (except for
:py:attr:`QuantizationMode.TRN`, which will converge towards a half).

.. image:: ../_static/quantization.png
   :alt: Illustration of the different quantization modes.

.. autoclass:: apytypes.QuantizationMode

   .. autoattribute:: RND

      Round to nearest, ties toward positive infinity.

   .. image:: ../_static/RND.png
      :alt: Illustration of round to nearest, ties toward positive infinity

   .. autoattribute:: RND_CONV

      Round to nearest, ties to even.

   .. image:: ../_static/RND_CONV.png
      :alt: Illustration of round to nearest, ties to even

   .. autoattribute:: RND_CONV_ODD

      Round to nearest, ties to odd.

   .. image:: ../_static/RND_CONV_ODD.png
      :alt: Illustration of round to nearest, ties to odd

   .. autoattribute:: RND_INF

      Round to nearest, ties away from zero.

   .. image:: ../_static/RND_INF.png
      :alt: Illustration of round to nearest, ties away from zero

   .. autoattribute:: RND_MIN_INF

      Round to nearest, ties toward negative infinity.

   .. image:: ../_static/RND_MIN_INF.png
      :alt: Illustration of round to nearest, ties toward negative infinity

   .. autoattribute:: RND_ZERO

      Round to nearest, ties towards zero.

   .. image:: ../_static/RND_ZERO.png
      :alt: Illustration of round to nearest, ties toward zero

   .. autoattribute:: TRN

      Round towards negative infinity (truncation).

      Implementation: remove additional bits.

   .. image:: ../_static/TRN.png
      :alt: Illustration of tounding towards negative infinity (truncation)

   .. autoattribute:: TRN_INF

      Round towards positive infinity.

   .. image:: ../_static/TRN_INF.png
      :alt: Illustration of rounding towards positive infinity

   .. autoattribute:: TRN_ZERO

      Round towards zero (unbiased magnitude truncation).

   .. image:: ../_static/TRN_ZERO.png
      :alt: Illustration of round towards zero (unbiased magnitude truncation)

   .. autoattribute:: TRN_AWAY

      Round away from zero.

   .. image:: ../_static/TRN_AWAY.png
      :alt: Illustration of round away from zero

   .. autoattribute:: TRN_MAG

      Fixed-point magnitude truncation (add sign-bit).

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

   .. autoattribute:: TO_NEG

      Fixed-point truncation. Round towards negative infinity. Alias for :attr:`TRN`.

   .. autoattribute:: TO_POS

      Round towards positive infinity. Alias for :attr:`TRN_INF`.

   .. autoattribute:: TO_ZERO

      Unbiased magnitude truncation. Round towards zero. Alias for :attr:`TRN_ZERO`.

   .. autoattribute:: TO_AWAY

      Round away from zero. Alias for :attr:`TRN_AWAY`.

   .. autoattribute:: TIES_POS

      Fixed-point rounding. Round to nearest, ties toward positive infinity. Alias for :attr:`RND`.

   .. autoattribute:: TIES_EVEN

      Unbiased fixed-point rounding. Round to nearest, ties to even. Alias for :attr:`RND_CONV`.

   .. autoattribute:: TIES_ODD

      Alternate unbiased fixed-point rounding. Round to nearest, ties to odd. Alias for :attr:`RND_CONV_ODD`.

   .. autoattribute:: TIES_AWAY

      Round to nearest, ties away from zero. Alias for :attr:`RND_INF`.

   .. autoattribute:: TIES_NEG

      Round to nearest, ties toward negative infinity. Alias for :attr:`RND_MIN_INF`.

   .. autoattribute:: TIES_ZERO

      Round to nearest, ties toward zero. Alias for :attr:`RND_ZERO`.

   Utility functions
   -----------------

.. autofunction:: apytypes.get_float_quantization_mode

.. autofunction:: apytypes.set_float_quantization_mode

.. autofunction:: apytypes.get_float_quantization_seed

.. autofunction:: apytypes.set_float_quantization_seed

   Sign of zero for floating-point
   -------------------------------

   For multiplication and division the sign is always the XOR of the operands' signs, but for
   addition and subtraction the sign depends on the quantization mode.
   Below is a table showing what the sign of zero will be in different cases.
   Using this table one can derive the sign for subtraction as well.

.. list-table:: Sign of zero in floating-point addition
   :widths: 30 25 40
   :header-rows: 1

   * - :math:`x + y`
     - TO_NEG
     - Other modes
   * - :math:`(+0) + (+0)`
     - :math:`+0`
     - :math:`+0`
   * - :math:`(+0) + (-0)`
     - :math:`-0`
     - :math:`+0`
   * - :math:`(-0) + (+0)`
     - :math:`-0`
     - :math:`+0`
   * - :math:`(-0) + (-0)`
     - :math:`-0`
     - :math:`-0`
   * - :math:`x + y, x = -y`
     - :math:`-0`
     - :math:`+0`

Overflow modes
--------------

.. autoclass:: apytypes.OverflowMode

   .. autoattribute:: WRAP

      Two's complement wrapping.

   .. image:: ../_static/WRAP.png
      :alt: Illustration of two's complement wrapping

   .. autoattribute:: SAT

      Two's complement saturation.

   .. image:: ../_static/SAT.png
      :alt: Illustration of two's complement saturation

   .. autoattribute:: NUMERIC_STD

      Keep sign bit and remove intermediate bits.

      As ``resize`` for ``signed`` in ``ieee.numeric_std``.

   .. image:: ../_static/NUMERIC_STD.png
      :alt: Illustration of numeric_std overflowing
