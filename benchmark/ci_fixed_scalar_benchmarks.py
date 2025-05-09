import numpy as np

from apytypes import APyFixed


def test_fixed_scalar_multiplication_short(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_multiplication_long(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_multiplication_mixed(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_division_short(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_division_long(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_division_mixed(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_addition_short(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_addition_short_different_wl(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=15, int_bits=3)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_addition_long(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_addition_mixed(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_creation_short(benchmark) -> None:
    a = np.random.rand(1)[0] - 0.5

    benchmark(APyFixed.from_float, a, 1, 19)


def test_fixed_scalar_creation_long(benchmark) -> None:
    a = np.random.rand(1)[0] - 0.5

    benchmark(APyFixed.from_float, a, 1, 199)


def test_fixed_scalar_to_float_short(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)

    benchmark(float, a)


def test_fixed_scalar_negation_short(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)

    benchmark(lambda x: -x, a)


def test_fixed_scalar_negation_long(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(lambda x: -x, a)


def test_fixed_scalar_abs_short(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=20, int_bits=1)

    benchmark(abs, a)


def test_fixed_scalar_abs_long(benchmark) -> None:
    a = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(abs, a)
