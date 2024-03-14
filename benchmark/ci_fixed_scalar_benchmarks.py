from apytypes import APyFixed
import numpy as np
import pytest


def test_fixed_scalar_multiplication_short(benchmark):
    a = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 20, 1)
    b = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 20, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_multiplication_long(benchmark):
    a = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 200, 1)
    b = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 200, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_multiplication_mixed(benchmark):
    a = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 20, 1)
    b = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 200, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_division_short(benchmark):
    a = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 20, 1)
    b = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 20, 1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_division_long(benchmark):
    a = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 200, 1)
    b = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 200, 1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_division_mixed(benchmark):
    a = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 20, 1)
    b = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 200, 1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_addition_short(benchmark):
    a = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 20, 1)
    b = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 20, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_addition_long(benchmark):
    a = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 200, 1)
    b = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 200, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_addition_mixed(benchmark):
    a = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 20, 1)
    b = APyFixed.from_fixed(np.random.rand(1)[0] - 0.5, 200, 1)

    benchmark(lambda x, y: x + y, a, b)
