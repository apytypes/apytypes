# APyTypes

## Documentation

<https://daint.gitlab-pages.liu.se/apy-types/>

## Installation

The package can be install by running `pip3 install .` from the top level of
the project, and uninstalled by running `pip3 uninstall apytypes`.
This should also install the required dependencies.

### Running

The package can now be used as:

```python
$ python3
>>> from apytypes import APyFixed
>>> fx_a = APyFixed.from_float(3.5, 8, 4)
>>> print(fx_a)
3.5
>>> fx_a
APyFixed(bit_pattern=56, bits=8, int_bits=4)
>>> fx_b = APyFixed.from_float(1.25, 9, 3)
>>> fx_a + fx_b
APyFixed(bit_pattern=304, bits=11, int_bits=5)
>>> print(fx_a + fx_b)
4.75
```

### Tests

Test dependencies can be installed by running `pip3 install [.test]` and then
executed by `pytest`.

Tests for APyFixed are currently run by running `make` from the src directory,
and APyFloat tests are executed by running `pytest` from `test_apytypes`.
