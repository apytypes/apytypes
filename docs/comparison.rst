Comparison
==========

Comparison matrix
-----------------

.. list-table::
    :header-rows: 1
    :stub-columns: 1

    * - Library
      - APyTypes
      - `fixedpoint <https://github.com/Schweitzer-Engineering-Laboratories/fixedpoint>`_
      - `fpbinary <https://github.com/smlgit/fpbinary>`_
      - `fxpmath <https://github.com/francof2a/fxpmath>`_
      - `pyfixp <https://github.com/chipmuenk/pyfixp>`_
      - `spfpm <https://github.com/rwpenney/spfpm>`_
      - `gmpy2 <https://github.com/aleaxit/gmpy>`_
    * - Custom fixed-point formats
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes
      - No
    * - Custom floating-point formats
      - Yes
      - No
      - No
      - No
      - No
      - No
      - Yes
    * - Arrays
      - Yes
      - No
      - No
      - Yes
      - Yes [#1]_
      - No
      - No
    * - Resizing of fixed-point results
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes [#1]_
      - No
      - N/A
    * - Matrix multiplication (``@``)
      - Yes
      - N/A
      - N/A
      - No
      - Yes [#1]_
      - N/A
      - N/A
    * - Written in
      - C++/Python
      - Python
      - C/Python
      - Python
      - Python [#1]_
      - Python
      - C/Python


.. rubric:: Footnotes

.. [#1] ``pyfixp`` is essentially a package to simply quantize NumPy-arrays with doubles.


Benchmark
---------

This benchmark compares relevant fixed-point libraries from the list above.
It is updated every documentation build and may vary over time.

.. image:: _static/benchmark.png
    :alt: Benchmark numbers for different fixed-point libraries.
