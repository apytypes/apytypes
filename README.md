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
1. From the top level of the project, run
    ```bash
    $ meson builddir
    $ meson compile -C builddir
    ```
2. Currently, to be able to use the Python library one must enter the `builddir` directory
    ```bash
    $ cd builddir
    ```
3. The library can now be used as
    ```python
    $ python3
    >>> from apy_types import APyFixed, STRING_TYPE
    >>> fx_a = APyFixed(8, 2, (1<<6) | (1 << 5))
    >>> print(fx_a.to_string())
    1.5
    >>> print(fx_a)
    fx<8, 2>(1.5)
    >>> fx_a.increment_lsb()
    >>> print(fx_a.to_string())
    1.515625
    ```
Unit tests are currently built and run using GNU Make.