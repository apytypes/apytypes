Development
===========

APyTypes is an open source project and contributions are very welcome!
From bug reports, through enhancement suggestions, to better documentation and code.

The development happens at `https://github.com/apytypes/apytypes <https://github.com/apytypes/apytypes>`_
where you also can provide suggestions and report bugs.

Here, there will be some development information to be expanded over time.

Pre-requisites
--------------

Most required packages will be installed during the installation process, but a few things are required:

* A C/C++ compiler supporting at least C++ 17.
* CMake

Clone the repository
--------------------

To clone the repository, do

.. code-block:: bash

   git clone https://github.com/apytypes/apytypes.git

However, if you want to contribute to the development, it is preferred to first
fork your own copy and then clone that. See the next section.

Fork and clone
--------------

Go to `the repository in GitHub <https://github.com/apytypes/apytypes>`_ and
then click the Fork button. This will create a copy of APyTypes in your own user
space. Now, click the Code button and clone using either ssh (preferred) or https.
For ssh to work you need to set up ssh keys.

Installation
------------

To install APyTypes from a cloned git-version, make sure that you stand in the
directory containing the cloned git-version, then do:

.. code-block:: bash

    python -m pip install .

Any change in the C++-code will require a reinstallation, so this is something done frequently
during development.

If you want to run tests, instead doing

.. code-block:: bash

    python -m pip install .[test]

will also install the test dependencies required. (This only needs to be done once, or when the
dependencies are updated.)

Run test suite
--------------

To run all test cases do:

.. code-block:: bash

    python -m pytest lib

Update stub files
-----------------

There is a script in the root directory ``stubgen.sh`` that must be executed to update
the stub-files in case any additional Python-level methods or classes are added or if
argument names or Python-level docstrings are changed.

Run floating-point tests vs Berkeley TestFloat
----------------------------------------------

Randomly generated test cases from `Berkeley TestFloat <http://www.jhauser.us/arithmetic/TestFloat.html>`_ can be run for the floating-point datatypes.
These are not executed by using pytest but instead with a dedicated Python script.
To use it, start by installing Berkeley TestFloat, and make sure to add the binary testfloat_gen to your PATH.
You can then see example usage of the script by running

.. code-block:: bash

    cd lib/test/apyfloat/
    python run_berkeley_cases.py -h

These are intended to more long running tests. If any bugs or deviations are found, dedicated tests should be added to the regular test suite.
