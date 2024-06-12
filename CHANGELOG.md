# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Added methods `next_up()` and `next_down()` for floating-point scalars.
- Broadcasting support
  [as NumPy](https://numpy.org/doc/stable/user/basics.broadcasting.html).
  - `APyFixedArray.broadcast_to()`
  - `APyFloatArray.broadcast_to()`
  - Automatic broadcasting on array arithmetic
- Support arithmetic between APyTypes classes and Python's built-in floats and ints.
- Support for automatic stub generation added.
- Docstrings are made compatible with
  [doctest](https://docs.python.org/3/library/doctest.html), which is
  run in the CI.
- Linting using [ruff](https://docs.astral.sh/ruff/) is added to pre-commit and CI.
- Array manipulation and mathematical functions added:
  - `squeeze()`
  - `reshape()`
- Added `array_utils.h` to contain shared functions between float and fixed arrays.

### Changed

- Updated [nanobind](https://github.com/wjakob/nanobind) dependency from v1.9.2 to
  v2.0.0.
- [ruff](https://docs.astral.sh/ruff/) is used instead of black to format the code.

### Fixed

- Smaller fixes in quantization for floating-point scalar multiplication.
- Fix in floating-point `cast`-method when result became subnormal.
- Fix bug in `APyFixedArray.__truediv__()` where program crashes on long-limb zero
  denominators.
- Fix bug where single-dimensional tuple representations missed out on a comma.
- Fix bug in comparison between floating-point infinity and fixed-point numbers.

### Removed

## [0.1.0] - 2024-05-15

[unreleased]: https://github.com/apytypes/apytypes/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/apytypes/apytypes/releases/tag/v0.1.0
