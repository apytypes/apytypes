# aPy-types

## Setting up a local workspace
1. Install [meson](https://mesonbuild.com/index.html) by
    ```
    $ pip3 install --user meson
    ```
2. Install [ninja](https://ninja-build.org/) by
    ```
    $ pip3 install --user ninja
    ```
3. Install [pybind11](https://pypi.org/project/pybind11/) by
    ```
    $ pip3 install --user pybind11
    ```

## Building and running
(This only applies to `aPyFixed` at the moment.)
1. From the top level of the project, run
    ```bash
    $ meson comile -C builddir
    ```
2. Currently, to be able to use the Python library one must enter the `builddir` directory
    ```bash
    $ cd builddir
    ```
3. The library can now be used as
    ```python
    $ python3
    >>> import apy_types
    >>> fx_a = aPyFixed(total_bits=8, integer_bits=3, value=1.5)
    >>> fx_b = aPyFixed(8, 5, 2.75)
    >>> print(fx_a + fx_b)
    fx<11, 6>(4 + 8/32)
    ```
General testing during developement can be done in `src/cpp/main.cpp` and `src/python/main.py` for C++ and Python respectively.