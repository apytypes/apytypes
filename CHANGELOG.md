# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- In `arange`, the array type can be deduced from its arguments if no bit-
  specifiers are given.
- Added optional parameter for `bias` in array creation functions that were
  missing it.
- Added `APyFloatArray.from_bits`.

### Fixed

### Changed

### Removed

## [0.2.2] - 2024-10-03

### Fixed

- Fix nanobind interaction with cocotb.

## [0.2.1] - 2024-09-19

### Fixed

- Memory-access bug in `APyFloatArray.cumsum`.
- Bug where `APyFixedArray` multiplicative methods could give wrong result for
  multi-limb products if the right-hand side have longer word-length than the
  left-hand side.
- Memory leak in nanobind wrapper library.
- Correct subnormal conversions between `APyFixed(Array)` and `float`.
- Improved `arange`. It now handles quantization accurately and does not
  allow infinity and NaN as inputs.

### Changed

- Updated [nanobind](https://github.com/wjakob/nanobind) from v2.0.0 to v2.1.0.

## [0.2.0] - 2024-09-13

### Added

- Added methods `next_up()` and `next_down()` for floating-point scalars.
- Broadcasting support
  [as NumPy](https://numpy.org/doc/stable/user/basics.broadcasting.html).
  - `APyFixedArray.broadcast_to()`
  - `APyFloatArray.broadcast_to()`
  - Automatic broadcasting on array arithmetic
- Support arithmetic between APyTypes classes and Python's built-in floats and
  ints.
- Support for automatic stub generation added.
- Docstrings are made compatible with
  [doctest](https://docs.python.org/3/library/doctest.html), which is
  run in the CI.
- Linting using [ruff](https://docs.astral.sh/ruff/) is added to pre-commit and CI.
- Array manipulation and mathematical functions added:
  - `squeeze()`
  - `reshape()`
  - `flatten() / ravel()`
  - `convolve()`
  - `sum()`
  - `cumsum()`
  - `nansum()`
  - `nancumsum()`
  - `prod()`
  - `cumprod()`
  - `nanprod()`
  - `nancumprod()`
  - `moveaxis()`
  - `swapaxis()`
  - `expand_dims()`
  - `max()`
  - `min()`
  - `nanmax()`
  - `nanmin()`
- Array creation functions added:
  - `arange()`
  - `eye()`
  - `identity()`
  - `ones()`
  - `ones_like()`
  - `zeros()`
  - `zeros_like()`
  - `full()`
  - `full_like()`
- Added `to_bits()` to array types.
- Significantly improved indexing of arrays.
- Added `array_utils.h` to contain shared functions between float and fixed arrays.
- Added support for higher dimensions (ndim > 2) in `transpose()`.

### Changed

- Updated [nanobind](https://github.com/wjakob/nanobind) dependency from v1.9.2 to
  v2.0.0.
- [ruff](https://docs.astral.sh/ruff/) is used instead of black to format the code.

### Fixed

- Smaller fixes in quantization for floating-point scalar multiplication.
- Fix in floating-point `cast`-method when result became subnormal.
- Fix bug in `APyFixedArray.__truediv__()` where program crashes on long-limb
  zero denominators.
- Fix bug where single-dimensional tuple representations missed out on a comma.
- Fix bug in comparison between floating-point infinity and fixed-point numbers.
- Start uploading source distributions of APyTypes to PyPI on releases.
- Fixed bug where `APyFixedArray.from_array()` initialized the wrong stored
  value from floating-point values that are too small for its representation.
  (#487)
- No longer consider zero a subnormal number.

### Removed

## [0.1.0] - 2024-05-15

[unreleased]: https://github.com/apytypes/apytypes/compare/v0.2.1...HEAD
[0.2.2]: https://github.com/apytypes/apytypes/releases/tag/v0.2.1
[0.2.1]: https://github.com/apytypes/apytypes/releases/tag/v0.2.1
[0.2.0]: https://github.com/apytypes/apytypes/releases/tag/v0.2.0
[0.1.0]: https://github.com/apytypes/apytypes/releases/tag/v0.1.0
