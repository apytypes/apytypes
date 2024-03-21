Development
===========

APyTypes is an open source project and contributions are very welcome!
From bug reports, through enhancement suggestions, to better documentation and code.

Here, there will be some devleopment information to be expanded over time.

Installation
------------

To install APyTypes from a cloned git-version, do:

.. code-block:: bash

    python -m pip install .

Any change in the C++-code will require a reinstallation, so this is something done frequently
during development.

If you want to run tests, instead doing

.. code-block:: bash

    python -m pip install .[test]

will install the test dependencies required. (This only needs to be done once, or when the
dependencies are updated.)
