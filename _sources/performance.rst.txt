Computational performance
=========================

One of the key design criteria of APyTypes is computational performance. Now, using custom word lengths will always
be much slower compared to using word lengths directly supported in hardware. By far. Hence, do not expect
APyTypes to compete with, e.g., NumPy in pure computational speed. Also, pure C++-libraries like ac-types
and ap-types are template-based. Hence, they know the word length at compile time and can therefore optimize
the code. APyTypes must do this dynamically.

Still, there are some ways to improve the performance when running APyTypes. This section will list best
practices and expand over time.

To track the computational speed of some selected operations over time, as the code evolves and hopefully improves,
see `here <https://apytypes.github.io/apytypes/dev/bench/>`_.
These graphs are updated after every commit. However, as they are executed as part of the CI, one can expect
a bit of variation. It should still be possible to note significant changes though.

Use arrays if possible
----------------------

As the overhead of going between Python and C++ is significant for a single number (about 90%),
it very much makes sense to use arrays when possible.

See the figure below for the effect of using arrays and the current operation speeds available.

.. note::
    This figure is generated as part of the documentation build and is continuously updated.
    The actual performance may differ depending on compiler, processor, etc.

.. image:: _static/performancescale.png
   :alt: Illustration of the obtained performance as a function of array size

Inplace shifting for fixed-point
--------------------------------

Shifting is used to move the binary point of fixed-point numbers. Hence, this will only involve updating the
number of integer/fractional bits. However, it may also involve creating a new object, so it is preferred to
use inplace shifting if possible.

.. code-block:: Python

    a = APyFixed(5, bits=6, int_bits=4)
    a = a >> 1   # slower
    a >>= 1   # faster
