Comparison
==========

Comparison matrix
-----------------

.. list-table:: Fixed-point libraries
    :header-rows: 1
    :stub-columns: 1

    * - Library
      - APyTypes
      - `en_cl_fix <https://github.com/enclustra/en_cl_fix/>`_
      - `fixedpoint <https://github.com/Schweitzer-Engineering-Laboratories/fixedpoint>`_
      - `fpbinary <https://github.com/smlgit/fpbinary>`_
      - `fxpmath <https://github.com/francof2a/fxpmath>`_
      - `numfi <https://github.com/ZZZZzzzzac/numfi>`_
      - `pyfixp <https://github.com/chipmuenk/pyfixp>`_
      - `spfpm <https://github.com/rwpenney/spfpm>`_
    * - Custom fixed-point formats
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes [#numfifootnote]_
      - Yes
      - Yes
    * - Custom floating-point formats
      - Yes
      - No
      - No
      - No
      - No
      - No
      - No
      - No
    * - Maximum wordlength [#wlfootnote]_
      - None
      - 53/None [#enclfixfootnote]_
      - None
      - None
      - None
      - 64 [#numfifootnote]_
      - 53 [#pyfixpfootnote]_
      - None
    * - Resizing of fixed-point results
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes [#numfifootnote]_
      - Yes [#pyfixpfootnote]_
      - No
    * - Arrays
      - Yes
      - Yes
      - No
      - No [#fpbinaryfootnote]_
      - Yes
      - Yes
      - Yes [#pyfixpfootnote]_
      - No
    * - Matrix multiplication (``@``)
      - Yes
      - No
      - N/A
      - N/A
      - No
      - No
      - Yes [#pyfixpfootnote]_
      - N/A
    * - Written in
      - C++/Python
      - Python
      - Python
      - C/Python
      - Python
      - Python
      - Python [#pyfixpfootnote]_
      - Python


.. rubric:: Footnotes

.. [#enclfixfootnote] ``en_cl_fix`` has two different fixed-point classes, one handles up to 53 bits, the other has no limitation. However, an operation involving the shorter format cannot produce the longer etc. In the comparison, the longer format was used for all tests.
.. [#numfifootnote] ``numfi`` is limited to 64 total bits (including results of e.g. multiplication).
.. [#pyfixpfootnote] ``pyfixp`` is essentially a package to simply quantize NumPy-arrays with doubles.
.. [#fpbinaryfootnote] ``fpbinary`` provides a helper function to convert an array to a list with fixed-point numbers.
.. [#wlfootnote] Note that this holds for intermediate results as well. For example, multiplying two 27-bit numbers will create a 54-bit result which will not give bit-accurate results with ``pyfixp``.

.. list-table:: Floating-point libraries
    :header-rows: 1
    :stub-columns: 1

    * - Library
      - APyTypes
      - `gmpy2 <https://github.com/aleaxit/gmpy>`_
    * - Custom fixed-point formats
      - Yes
      - No
    * - Custom floating-point formats
      - Yes
      - Yes
    * - Binary representation of number [#binaryfootnote]_
      - Yes
      - No
    * - Maximum wordlength
      - 32 bits for exponent, 64 bits for mantissa [#apytypesfootnote]_
      - None
    * - Stochastic rounding
      - Yes
      - No
    * - Arrays
      - Yes
      - No
    * - Matrix multiplication (``@``)
      - Yes
      - N/A
    * - Written in
      - C++/Python
      - C/Python

.. rubric:: Footnotes

.. [#binaryfootnote] This means that it is possible to get a binary representation for comparison with, e.g., a hardware implementation.
.. [#apytypesfootnote] This is a design choice since ``apytypes``  is aimed at simulating shorter custom formats. However, there is unlimited word length support for fixed-point values which can, if deemed suitable at a later time, be used for floating-point as well.


Performance Comparison
----------------------

This benchmark compares relevant fixed-point libraries from the list above.
It is updated every documentation build and may vary over time as the code is further optimized.

.. image:: _static/comparison.png
    :alt: Benchmark numbers for different fixed-point libraries.

.. image:: _static/comparison_relative.png
    :alt: Benchmark relative to APyTypes for different fixed-point libraries.

Feature Comparison
------------------

.. image:: _static/comparison_matrix.png
    :alt: Feature matrix for different fixed-point libraries.
