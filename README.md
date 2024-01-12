# aPy-types

## Documentation

<https://daint.gitlab-pages.liu.se/apy-types/>

## Setting up a local workspace

Install [meson](https://mesonbuild.com/index.html),
[ninja](https://ninja-build.org/), and
[pybind11](https://pypi.org/project/pybind11/) by running

```bash
pip3 install --user meson ninja pybind11
```

## Building and running

### Using pip

The package can be install by running `pip3 install .` from the top level of
the project, and uninstalled by running `pip3 uninstall apytypes`.

### Without using pip

1. From the top level of the project, run

    ```bash
    meson builddir
    meson compile -C builddir
    ```

2. The Python library can be used by entering the `builddir` directory

    ```bash
    cd builddir
    ```

### Running

The package can now be used as such:

```python
$ python3
>>> from apytypes import APyFixed
>>> fx_a = APyFixed(8, 2, 1.5)
>>> print(fx_a.to_string())
1.5
>>> print(fx_a)
fx<8, 2>(1.5)
>>> fx_a.increment_lsb()
0
>>> print(fx_a.to_string())
1.515625
```

### Tests

Tests for APyFixed are currently run by running `make` from the src directory,
and APyFloat tests are executed by running `pytest` from `test_apytypes`.
