# APyTypes

[![PyPI](https://img.shields.io/pypi/v/apytypes)](https://pypi.org/project/apytypes/)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/apytypes/apytypes/main.svg)](https://results.pre-commit.ci/latest/github/apytypes/apytypes/main)
[![License](https://img.shields.io/github/license/apytypes/apytypes)](https://github.com/apytypes/apytypes/blob/main/LICENSE.txt)
![Workflow Status](https://img.shields.io/github/actions/workflow/status/apytypes/apytypes/tests.yml)
[![codecov](https://codecov.io/gh/apytypes/apytypes/graph/badge.svg?token=734MDWN7SU)](https://codecov.io/gh/apytypes/apytypes)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.11197885.svg)](https://doi.org/10.5281/zenodo.11197885)

APyTypes is a Python library providing algorithmic scalar and array data types,
both fixed- and floating-point in Python. The main purpose is to simplify finite
word length design and simulation. Written in C++, the focus is on performance and
flexibility.

## Documentation

<https://apytypes.github.io/apytypes/>

## Installation

APyTypes is available in the
[Python Package Index](https://pypi.org/p/apytypes/) and can be installed with
the [pip](https://pypi.org/p/pip/) package installer:

```bash
pip install apytypes
```

### Running

The package can now be used as:

```python
$ python
>>> import apytypes as apy
>>> fx_a = apy.fx(3.5, int_bits=4, frac_bits=4)
>>> print(fx_a)
3.5
>>> fx_a
APyFixed(56, bits=8, int_bits=4)
>>> fx_b = apy.fx(1.25, int_bits=3, frac_bits=6)
>>> fx_a + fx_b
APyFixed(304, bits=11, int_bits=5)
>>> print(fx_a + fx_b)
4.75
```

### Tests

Test dependencies can be installed by running `pip install [.test]` and then
executed by `pytest`.
