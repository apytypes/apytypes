from apytypes import APyFixedArray, APyFixed
import numpy as np
import pytest


def test_fixed_matrix_multiplication_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(lambda x, y: x @ y, a, b)


# def test_fixed_matrix_multiplication_200(benchmark):
#     a = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)
#     b = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)

#     benchmark(lambda x, y: x @ y, a, b)


def test_fixed_matrix_addition_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_long_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 200, 1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 200, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_different_wl_long_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 200, 1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 167, 13)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_20_different_wl(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 15, 3)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_200_different_wl(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 15, 3)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_elementwise_multiplication_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_elementwise_multiplication_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_negation_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(lambda x: -x, a)


def test_fixed_matrix_negation_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)

    benchmark(lambda x: -x, a)


def test_fixed_matrix_abs_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(abs, a)


def test_fixed_matrix_abs_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)

    benchmark(abs, a)


def test_fixed_scalar_matrix_addition_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20), 19, 1)
    b = APyFixed.from_float(np.random.rand(1)[0], 19, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_matrix_addition_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)
    b = APyFixed.from_float(np.random.rand(1)[0], 19, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_matrix_multiplication_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20), 19, 1)
    b = APyFixed.from_float(np.random.rand(1)[0], 19, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_matrix_multiplication_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)
    b = APyFixed.from_float(np.random.rand(1)[0], 19, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_creation_20(benchmark):
    a = np.random.rand(20, 20)

    benchmark(APyFixedArray.from_float, a, 19, 1)


def test_fixed_matrix_creation_200(benchmark):
    a = np.random.rand(200, 200)

    benchmark(APyFixedArray.from_float, a, 19, 1)


def test_fixed_matrix_to_numpy_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20), 19, 1)

    benchmark(lambda x: x.to_numpy(), a)


def test_fixed_matrix_to_numpy_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)

    benchmark(lambda x: x.to_numpy(), a)


def test_fixed_vector_sum_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20), 19, 1)

    benchmark(sum, a)


def test_fixed_vector_max_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20), 19, 1)

    benchmark(max, a)
