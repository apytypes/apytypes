.. APyTypes documentation master file.

Welcome to APyTypes' documentation!
===================================

APyTypes is a Python library providing algorithmic data types, both fixed- and
floating-point in Python. The main purpose is to simplify finite word length design and
simulation. Written in C++, the focus is on performance and flexibility.

The repository can be found at `GitHub <https://github.com/apytypes/apytypes>`_. Please
report any issues or suggestions there. Contributions are also welcome!

Installation
------------

Releases of APyTypes come with pre-build wheels for Linux, Windows, and MacOS (ARM64 and
x86-64). Release wheels are made available through `PyPi
<https://pypi.org/project/apytypes/>`_. Using a pre-built wheel is the recommended way
of installing APyTypes.

.. admonition:: Install APyTypes

    .. code-block:: console

        $ pip install apytypes

It is also possible to build APyTypes from source code. Unreleased content not yet
available through a release is made available in this way.

.. admonition:: Install APyTypes from source

    .. code-block:: console

        $ pip install git+https://github.com/apytypes/apytypes.git

    Installing APyTypes from source requires:
      * A C++17-compatible compiler
      * `Git <https://git-scm.com/>`_
      * `CMake <https://cmake.org/>`_
      * `Meson <https://mesonbuild.com/>`_

.. seealso::

    :ref:`development`
        Detailed information on building APyTypes and its documentation from source.

Now, have a look at the examples and play around. Using an enhanced terminal, like
Jupyter Notebooks or Spyder, will provide a nicer output of the numbers.

.. toctree::
   :maxdepth: 1
   :caption: Contents:

   api/index
   cpp_api/index
   examples/index
   playground
   performance
   comparison
   development
   publications
   changelog
   licenses

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

APyTypes is developed by the Division of Electronics and Computer Engineering at
Linköping University, Sweden.

.. image:: _static/liu-white.svg
   :class: only-dark

.. image:: _static/liu-black.svg
   :class: only-light
