from apytypes import APyFloat
import numpy as np


def test_float_scalar_multiplication_short(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)

    benchmark(lambda x, y: x * y, a, b)


def test_float_scalar_multiplication_long(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x * y, a, b)


def test_float_scalar_multiplication_mixed(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x * y, a, b)


def test_float_scalar_division_short(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)

    benchmark(lambda x, y: x / y, a, b)


def test_float_scalar_division_long(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x / y, a, b)


def test_float_scalar_division_mixed(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x / y, a, b)


def test_float_scalar_addition_short(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)

    benchmark(lambda x, y: x + y, a, b)


def test_float_scalar_addition_long(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x + y, a, b)


def test_float_scalar_addition_mixed(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 16, 60)

    benchmark(lambda x, y: x + y, a, b)


def test_float_scalar_creation_short(benchmark):
    a = np.random.rand(1)[0] - 0.5

    benchmark(APyFloat.from_float, a, 4, 7)


def test_float_scalar_creation_long(benchmark):
    a = np.random.rand(1)[0] - 0.5

    benchmark(APyFloat.from_float, a, 16, 60)


def test_float_scalar_to_float_short(benchmark):
    a = APyFloat.from_float(np.random.rand(1)[0] - 0.5, 4, 7)

    benchmark(float, a)
