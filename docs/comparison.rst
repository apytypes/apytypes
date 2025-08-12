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
      - `pychop <https://github.com/inEXASCALE/pychop>`_
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
      - Yes
    * - Custom floating-point formats
      - Yes
      - No
      - No
      - No
      - No
      - No
      - Yes
      - No
      - No
    * - Maximum wordlength [#wlfootnote]_
      - None
      - 53/None [#enclfixfootnote]_
      - None
      - None
      - None
      - 64 [#numfifootnote]_
      - 53 [#pychopfootnote]_
      - 53 [#pyfixpfootnote]_
      - None
    * - Resizing of fixed-point results
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes [#numfifootnote]_
      - Yes [#pychopfootnote]_
      - Yes [#pyfixpfootnote]_
      - No
    * - Arrays
      - Yes
      - Yes
      - No
      - No [#fpbinaryfootnote]_
      - Yes
      - Yes
      - Yes [#pychopfootnote]_
      - Yes [#pyfixpfootnote]_
      - No
    * - Matrix multiplication (``@``)
      - Yes
      - No
      - N/A
      - N/A
      - No
      - No
      - Yes [#pychopfootnote]_
      - Yes [#pyfixpfootnote]_
      - N/A
    * - Written in
      - C++/Python
      - Python
      - Python
      - C/Python
      - Python
      - Python
      - Python [#pychopfootnote]_
      - Python [#pyfixpfootnote]_
      - Python


.. rubric:: Footnotes

.. [#enclfixfootnote] ``en_cl_fix`` has two different fixed-point classes, one handles up to 53 bits, the other has no limitation. However, an operation involving the shorter format cannot produce the longer etc. In the comparison, the longer format was used for all tests.
.. [#numfifootnote] ``numfi`` is limited to 64 total bits (including results of e.g. multiplication).
.. [#pychopfootnote] ``pychop`` is essentially a package to simply quantize floating-point NumPy, PyTorch, and JAX-arrays to a corresponding fixed-point or floating-point representation. Computation still happens at the precision of the underlying array.
.. [#pyfixpfootnote] ``pyfixp`` is essentially a package to simply quantize floating-point NumPy-arrays with doubles to a corresponding fixed-point representation. Computation still happens at the precision of the underlying array.
.. [#fpbinaryfootnote] ``fpbinary`` provides a helper function to convert an array to a list with fixed-point numbers.
.. [#wlfootnote] Note that this holds for intermediate results as well. For example, multiplying two 27-bit numbers will create a 54-bit result which will not give bit-accurate results with ``pyfixp``.

.. list-table:: Floating-point libraries
    :header-rows: 1
    :stub-columns: 1

    * - Library
      - APyTypes
      - `bigfloat <https://github.com/mdickinson/bigfloat>`_ [#bigfloatfootnote]_
      - `gmpy2 <https://github.com/aleaxit/gmpy>`_ [#gmpy2footnote]_
      - `mpmath <https://github.com/mpmath/mpmath>`_
      - `PyChop <https://github.com/inEXASCALE/pychop>`_
      - `PyMPF <https://github.com/florianschanda/PyMPF>`_
      - `gfloat <https://github.com/graphcore-research/gfloat>`_
    * - Custom fixed-point formats
      - Yes
      - No
      - No
      - No
      - Yes [#pychopfootnotefp]_
      - No
      - No
    * - Custom floating-point formats
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes [#pychopfootnotefp]_
      - Yes
      - Yes [#gfloatfootnote]_
    * - Binary representation of number [#binaryfootnote]_
      - Yes
      - No
      - No
      - No
      - No
      - Yes
      - Yes
    * - Maximum wordlength
      - 32 bits for exponent, 64 bits for mantissa [#apytypesfootnote]_
      - None
      - None
      - None
      - Yes [#pychopfootnotefp]_
      - 18 bits for exponent, none for mantissa
      - Yes [#gfloatfootnote]_
    * - Stochastic rounding
      - Yes
      - No
      - No
      - No
      - Yes
      - No
      - Yes [#gfloatfootnote]_
    * - Arrays
      - Yes
      - No
      - No
      - Yes
      - Yes [#pychopfootnotefp]_
      - No
      - Yes [#gfloatfootnote]_
    * - Matrix multiplication (``@``)
      - Yes
      - N/A
      - N/A
      - Yes
      - Yes [#pychopfootnotefp]_
      - N/A
      - Yes [#gfloatfootnote]_
    * - Written in
      - C++/Python
      - C/Python
      - C/Python
      - Python [#mpmathfootnote]_
      - Python [#pychopfootnotefp]_
      - Python
      - Python [#gfloatfootnote]_

.. rubric:: Footnotes

.. [#bigfloatfootnote] ``bigfloat`` is a Python wrapper around the `GNU MPFR <https://www.mpfr.org/>`_ library.
.. [#gmpy2footnote] ``gmpy2`` is a Python wrapper around the `GNU MPFR <https://www.mpfr.org/>`_ and `GNU MPC <https://www.multiprecision.org/>`_ libraries.
.. [#pychopfootnotefp] ``pychop`` is essentially a package to simply quantize floating-point NumPy, PyTorch, and JAX-arrays to a given fixed-point or floating-point representation. Computation still happens at the precision of the underlying array.
.. [#gfloatfootnote] ``gfloat`` is a package for encoding/decoding floating-point numbers. Floating-point NumPy, PyTorch, and JAX-arrays are quantized to a given floating-point representation, and computation still happens at the precision of the underlying array.
.. [#binaryfootnote] This means that it is possible to get a binary representation for comparison with, e.g., a hardware implementation.
.. [#apytypesfootnote] This is a design choice since ``apytypes``  is aimed at simulating shorter custom formats. However, there is unlimited word length support for fixed-point values which can, if deemed suitable at a later time, be used for floating-point as well.
.. [#mpmathfootnote] ``mpmath`` can use ``gmpy`` as a backend to do faster arithmetic, but uses Python integers by default.


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
