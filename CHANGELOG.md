# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

### Fixed

### Changed

### Removed

## [0.4.0] - 2025-09-09

### Added

- Complex-valued floating-point types:
  - `APyCFloat`, a complex scalar floating-point type.
  - `APyCFloatArray`, a complex array floating-point type.
- `APyFloat.zero`, to create floating-point objects initialized to zero.
- ARM-Linux wheels.
- Specialized overload typing signatures for array functions and utility
  functions.
- Array creation methods: `fullrange`, `meshgrid`.
- CSV import/export of arrays: `import_csv` and `export_csv`.
- `to_numpy` and `__array__` now accepts `dtype` and `copy` arguments.

### Fixed

- Rounding error with `QuantizationMode.JAM` using APyTypes floating-points if an
  arithmetic result equals zero.
- Bug in `APyFloat.__lt__` for comparison of floating-points of different
  formats.
- Bug in `APyCFixedArray.__init__` where all elements become real.
- Proper string formatting for APyTypes arrays:
  - Array `__repr__` methods.
  - Array `__str__` methods.
- Improved type hints for APyArray methods.
- Bug in `APyFixedArray.from_float` when converting from `APyFixed` (issue #683).
- Bug in multi-limb `APyFixedArray.max` and `APyFixedArray.min` (issue #721).
- Wrong name in fixed-point division exception message (`__div__` -> `__truediv__`)
- Inconsistency in formatting of complex-valued numbers (e.g., `(1.0+2.5j)` for
  scalars and `[1.0+2.5j]` for arrays)
- Bug in floating-point matrix multiplications when inside accumulator contexts
  using quantization modes `TRN_INF` and `TRN_AWAY`.
- Bug in floating-point `.to_bits()` and `from_bits` for large formats
  on 32-bit systems.

### Changed

- Updated [fmt](https://github.com/fmtlib/fmt) from v11.1.1 to v11.2.0
- Updated [nanobind](https://github.com/wjakob/nanobind) from v2.4.0 to v2.8.0.
- Updated [highway](https://github.com/google/highway) from v1.2.0 to v1.3.0.
- `APyFixedArray` inner products now returns `APyFixed`, instead of returning
  an `APyFixedArray` with a single element.
- `APyFloatQuantizationContext` now accepts the seed parameter for all quantization
  modes, although it is only used for the stochastic ones.
- Initialize tensors with [Iterable](https://docs.python.org/3/library/collections.abc.html#collections.abc.Iterable)
  instead of [Sequence](https://docs.python.org/3/library/collections.abc.html#collections.abc.Sequence).
- BREAKING: arguments related to word length/data type are now keyword only
  in array utility functions.

### Removed

## [0.3.1] - 2025-04-01

### Added

- `inf` and `nan` functions to create floating-point objects initialized
  to infinity or NaN.

### Fixed

- Bug in `APyFloat` addition/subtraction when the exponent difference was larger
  than 64, introduced in v0.3.0.
- Bug in `from_float` when creating an `APyFloat` from an `APyFixed`.
- `ones_like` and `zeros_like` now support `APyCFixedArray` input.
- Bug in `APyCFixedArray.rdiv` (scalar divided by array) word-length evaluation.
- Bug in `APyFloat` addition when operands are of different formats and both
  are exactly equal to zero.

## [0.3.0] - 2025-03-23

### Added

- In `arange`, the array type can be deduced from its arguments if no
  bit-specifiers are given.
- Added optional parameter for `bias` in array creation functions that were
  missing it.
- Added `APyFloatArray.from_bits` to create floating-point arrays from bit patterns.
- It is now possible to assign parts of arrays as `APyFixedArray.__setitem__` and
  `APyFloatArray.__setitem__` has been added.
- `fx` and `fp` functions to create fixed-point and floating-point objects of
  correct type (scalar or array) depending on argument.
- `fn` utility function to easily evaluate functions with APy-classes and get a
  result with the same format.
- New complex-valued data types:
  - `APyCFixed`, a scalar fixed-point complex type.
  - `APyCFixedArray`, an array fixed-point complex type.
- `APyFloatArray.convolve` now supports accumulator contexts.
- VHDL code generation for ROMs.
- It is possible to perform arithmetic with Numpy arrays, automatically converting
  the Numpy array to an APyArray of the same size and type as the APyArray argument.
- `from_float` now accepts APyTypes scalars.
- Wheels for Python 3.13.
- `copy`-method added for the array and scalar classes. This also includes support
  for the Python `copy.copy` and `copy.deepcopy` functions.
- Single limb abs and negation now use SIMD instructions.
- Integer powers of `APyFixed`.
- Fixed-point multiplication, including matrix multiplication, is faster for
  single limb arguments resulting in a two-limb result.
- Support for unary plus: `+X`, `__pos__`.
- Support for logic not: `~X`, `__invert__`.

### Fixed

- APyTypes contexts now supports multi-threading.
- Incorrect fixed-point quantization in `RND_INF` and `RND_MIN_INF` when source
  has multiple of 64 bits.
- Incorrect fixed-point rounding in `APyFixedArray.from_array` on Windows.
- Incorrect LaTeX-representation of negative fixed-point scalars.
- Incorrect casting of large integers in `APyFixedArray.from_*`.
- Incorrect accumulation in `APyFixedArray.cumsum()` and
  `APyCFixedArray.cumsum()` when result has more limbs than source.
- Crash in `sum`, `cumsum`, `prod`, and `cumprod` for empty arrays.
- Fixed-point to floating-point conversion properly rounds to `+-inf`.

### Changed

- Updated [nanobind](https://github.com/wjakob/nanobind) from v2.1.0 to v2.4.0.
- Updated [fmt](https://github.com/fmtlib/fmt) from v10.2.0 to v11.1.1
- Updated [highway](https://github.com/google/highway) from v1.1.0 to v1.2.0.

### Removed

- Mini-GMP is no longer used. Instead, a local multi-precision integer library
  written in C++ is used.
- Support for Python 3.9.

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

Initial release.

[unreleased]: https://github.com/apytypes/apytypes/compare/v0.4.0...HEAD
[0.4.0]: https://github.com/apytypes/apytypes/releases/tag/v0.4.0
[0.3.1]: https://github.com/apytypes/apytypes/releases/tag/v0.3.1
[0.3.0]: https://github.com/apytypes/apytypes/releases/tag/v0.3.0
[0.2.2]: https://github.com/apytypes/apytypes/releases/tag/v0.2.2
[0.2.1]: https://github.com/apytypes/apytypes/releases/tag/v0.2.1
[0.2.0]: https://github.com/apytypes/apytypes/releases/tag/v0.2.0
[0.1.0]: https://github.com/apytypes/apytypes/releases/tag/v0.1.0
