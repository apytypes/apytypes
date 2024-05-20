# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Methods `next_up()` and `next_down()` for floating-point,
  based on the IEEE-754 standard.
- Broadcasting support [as NumPy](https://numpy.org/doc/stable/user/basics.broadcasting.html)
  - `APyFixedArray.broadcast_to()`
  - `APyFloatArray.broadcast_to()`
  - Automatic broadcasting on array arithmetic

### Changed

### Fixed

- Smaller fixes in quantization for floating-point scalar multiplication.
- Fix in floating-point `cast`-method when result became subnormal.

### Removed

## [0.1.0] - 2024-05-15

[unreleased]: https://github.com/apytypes/apytypes/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/apytypes/apytypes/releases/tag/v0.1.0
