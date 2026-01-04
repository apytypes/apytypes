Third-Party Array Libraries
===========================

APyTypes can interoperate with several third-party array libraries.
These can be used as input to APyTypes functions.
APyTypes arrays can be converted to these formats and comparisons can generate results in these formats.

It is required to install these third-party libraries separately if you want to use them.

The following third-party array libraries are supported:
- NumPy
- PyTorch
- TensorFlow
- JAX
- CuPy

To choose which third-party library to use for output arrays, the following functions are provided:

.. autofunction:: apytypes.set_array_library

.. autofunction:: apytypes.get_array_library
