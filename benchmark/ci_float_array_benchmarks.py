from apytypes import APyFloatArray, APyFloat
import numpy as np


def test_float_matrix_multiplication_20(benchmark):
    a = APyFloatArray.from_float(np.random.rand(20, 20) - 0.5, 4, 7)
    b = APyFloatArray.from_float(np.random.rand(20, 20) - 0.5, 4, 7)

    benchmark(lambda x, y: x @ y, a, b)


# def test_float_matrix_multiplication_200(benchmark):
#     a = APyFloatArray.from_float(np.random.rand(200, 200), 4, 7)
#     b = APyFloatArray.from_float(np.random.rand(200, 200), 4, 7)

#     benchmark(lambda x, y: x @ y, a, b)


def test_float_matrix_addition_20(benchmark):
    a = APyFloatArray.from_float(np.random.rand(20, 20) - 0.5, 4, 7)
    b = APyFloatArray.from_float(np.random.rand(20, 20) - 0.5, 4, 7)

    benchmark(lambda x, y: x + y, a, b)


def test_float_matrix_addition_200(benchmark):
    a = APyFloatArray.from_float(np.random.rand(200, 200) - 0.5, 4, 7)
    b = APyFloatArray.from_float(np.random.rand(200, 200) - 0.5, 4, 7)

    benchmark(lambda x, y: x + y, a, b)


def test_float_matrix_elementwise_multiplication_20(benchmark):
    a = APyFloatArray.from_float(np.random.rand(20, 20) - 0.5, 4, 7)
    b = APyFloatArray.from_float(np.random.rand(20, 20) - 0.5, 4, 7)

    benchmark(lambda x, y: x * y, a, b)


def test_float_matrix_elementwise_multiplication_200(benchmark):
    a = APyFloatArray.from_float(np.random.rand(200, 200) - 0.5, 4, 7)
    b = APyFloatArray.from_float(np.random.rand(200, 200) - 0.5, 4, 7)

    benchmark(lambda x, y: x * y, a, b)


# def test_float_matrix_negation_20(benchmark):
#     a = APyFloatArray.from_float(np.random.rand(20, 20) - 0.5, 4, 7)

#     benchmark(lambda x: -x, a)


# def test_float_matrix_negation_200(benchmark):
#     a = APyFloatArray.from_float(np.random.rand(200, 200) - 0.5, 4, 7)

#     benchmark(lambda x: -x, a)


# def test_float_matrix_abs_20(benchmark):
#     a = APyFloatArray.from_float(np.random.rand(20, 20) - 0.5, 4, 7)

#     benchmark(abs, a)


# def test_float_matrix_abs_200(benchmark):
#     a = APyFloatArray.from_float(np.random.rand(200, 200), 4, 7)

#     benchmark(abs, a)


def test_float_scalar_matrix_addition_20(benchmark):
    a = APyFloatArray.from_float(np.random.rand(20, 20), 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0], 4, 7)

    benchmark(lambda x, y: x + y, a, b)


def test_float_scalar_matrix_addition_200(benchmark):
    a = APyFloatArray.from_float(np.random.rand(200, 200), 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0], 4, 7)

    benchmark(lambda x, y: x + y, a, b)


def test_float_scalar_matrix_multiplication_20(benchmark):
    a = APyFloatArray.from_float(np.random.rand(20, 20), 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0], 4, 7)

    benchmark(lambda x, y: x * y, a, b)


def test_float_scalar_matrix_multiplication_200(benchmark):
    a = APyFloatArray.from_float(np.random.rand(200, 200), 4, 7)
    b = APyFloat.from_float(np.random.rand(1)[0], 4, 7)

    benchmark(lambda x, y: x * y, a, b)


def test_float_matrix_creation_20(benchmark):
    a = np.random.rand(20, 20)

    benchmark(APyFloatArray.from_float, a, 4, 7)


def test_float_matrix_creation_200(benchmark):
    a = np.random.rand(200, 200)

    benchmark(APyFloatArray.from_float, a, 4, 7)


def test_float_matrix_to_numpy_20(benchmark):
    a = APyFloatArray.from_float(np.random.rand(20, 20), 4, 7)

    benchmark(lambda x: x.to_numpy(), a)


def test_float_matrix_to_numpy_200(benchmark):
    a = APyFloatArray.from_float(np.random.rand(200, 200), 4, 7)

    benchmark(lambda x: x.to_numpy(), a)


def test_float_matrix_cast_20(benchmark):
    a = APyFloatArray.from_float(np.random.rand(20, 20) - 0.5, 8, 20)

    benchmark(lambda x: x.cast(4, 7), a)


def test_float_matrix_cast_200(benchmark):
    a = APyFloatArray.from_float(np.random.rand(200, 200) - 0.5, 8, 20)

    benchmark(lambda x: x.cast(4, 7), a)
