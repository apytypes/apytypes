# APyTypes

[![PyPI](https://img.shields.io/pypi/v/apytypes)](https://pypi.org/project/apytypes/)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/apytypes/apytypes/main.svg)](https://results.pre-commit.ci/latest/github/apytypes/apytypes/main)
[![License](https://img.shields.io/github/license/apytypes/apytypes)](https://github.com/apytypes/apytypes/blob/main/LICENSE.txt)
![Workflow Status](https://img.shields.io/github/actions/workflow/status/apytypes/apytypes/tests.yml)
[![codecov](https://codecov.io/gh/apytypes/apytypes/graph/badge.svg?token=734MDWN7SU)](https://codecov.io/gh/apytypes/apytypes)

## Documentation

<https://apytypes.github.io/apytypes/>

## Installation

APyTypes is available in the [Python Package Index](https://pypi.org/p/apytypes/) and
can be installed with the [pip](https://pypi.org/p/pip/) package installer:

```bash
pip install apytypes
```

Uninstall APyTypes with:

```bash
pip uninstall apytypes
```

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
