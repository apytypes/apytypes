import numpy as np

from apytypes import APyFloat


def test_float_scalar_multiplication_short(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)

    benchmark(lambda x, y: x * y, a, b)


def test_float_scalar_multiplication_long(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x * y, a, b)


def test_float_scalar_multiplication_mixed(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x * y, a, b)


def test_float_scalar_division_short(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)

    benchmark(lambda x, y: x / y, a, b)


def test_float_scalar_division_long(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x / y, a, b)


def test_float_scalar_division_mixed(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x / y, a, b)


def test_float_scalar_addition_short(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)

    benchmark(lambda x, y: x + y, a, b)


def test_float_scalar_addition_long(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x + y, a, b)


def test_float_scalar_addition_mixed(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x + y, a, b)


def test_float_scalar_creation_short(benchmark) -> None:
    a = np.random.rand(1)[0] - 0.5

    benchmark(APyFloat.from_float, a, 4, 7)


def test_float_scalar_creation_long(benchmark) -> None:
    a = np.random.rand(1)[0] - 0.5

    benchmark(APyFloat.from_float, a, 16, 60)


def test_float_scalar_to_float_short(benchmark) -> None:
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)

    benchmark(float, a)
