Comparison
==========

Comparison matrix
-----------------

.. list-table:: Fixed-point libraries
    :header-rows: 1
    :stub-columns: 1

    * - Library
      - APyTypes
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
      - No
    * - Resizing of fixed-point results
      - Yes
      - Yes
      - Yes
      - Yes [#numfifootnote]_
      - Yes
      - Yes [#pyfixpfootnote]_
      - No
    * - Arrays
      - Yes
      - No
      - No [#fpbinaryfootnote]_
      - Yes
      - Yes
      - Yes [#pyfixpfootnote]_
      - No
    * - Matrix multiplication (``@``)
      - Yes
      - N/A
      - N/A
      - No
      - No
      - Yes [#pyfixpfootnote]_
      - N/A
    * - Written in
      - C++/Python
      - Python
      - C/Python
      - Python
      - Python
      - Python [#pyfixpfootnote]_
      - Python


.. rubric:: Footnotes

.. [#numfifootnote] ``numfi`` is limited to 64 total bits (including results of e.g. multiplication).
.. [#pyfixpfootnote] ``pyfixp`` is essentially a package to simply quantize NumPy-arrays with doubles.
.. [#fpbinaryfootnote] ``fpbinary`` provides a helper function to convert an array to a list with fixed-point numbers.

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
    * - Arrays
      - Yes
      - No
    * - Matrix multiplication (``@``)
      - Yes
      - N/A
    * - Written in
      - C++/Python
      - C/Python



Benchmark
---------

This benchmark compares relevant fixed-point libraries from the list above.
It is updated every documentation build and may vary over time.

.. image:: _static/benchmark.png
    :alt: Benchmark numbers for different fixed-point libraries.
