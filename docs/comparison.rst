Comparison Between Available Libraries
======================================

.. list-table:: Comparison matrix
    :header-rows: 1
    :stub-columns: 1

    * - Library
      - APyTypes
      - `fixedpoint <https://github.com/Schweitzer-Engineering-Laboratories/fixedpoint>`
      - `fxpmath <https://github.com/francof2a/fxpmath>`
      - `pyfixp <https://github.com/chipmuenk/pyfixp>`
      - `spfpm <https://github.com/rwpenney/spfpm>`
      - `gmpy2 <https://github.com/aleaxit/gmpy>`
    * - Fixed-point support
      - Yes
      - Yes
      - Yes
      - Yes
      - Yes
      - No
    * - Floating-point support
      - Yes
      - No
      - No
      - No
      - No
      - Yes
    * - Arrays
      - Yes
      - ?
      - Yes
      - Yes [#1]_
      - No
      - ?
    * - Explicit resizing of fixed-point values
      - Yes
      - ?
      - Yes
      - Yes [#1]_
      - ?
      - N/A
    * - Matrix multiplication (``@``)
      - Yes
      - ?
      - No
      - Yes [#1]_
      - N/A
      - ?
    * - Written in
      - C++/Python
      - Python
      - Python
      - Python [#1]_
      - Python
      - C/Python


.. rubric:: Footnotes

.. [#1] ``pyfixp`` is essentially a package to simply quantize NumPy-arrays with doubles.
