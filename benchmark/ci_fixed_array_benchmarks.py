import numpy as np

from apytypes import APyFixed, APyFixedArray


def test_fixed_matrix_multiplication_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x @ y, a, b)


def test_fixed_matrix_multiplication_medium_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=39, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=39, int_bits=1)

    benchmark(lambda x, y: x @ y, a, b)


# def test_fixed_matrix_multiplication_200(benchmark):
#     a = APyFixedArray.from_float(np.random.rand(200, 200), bits=19, int_bits=1)
#     b = APyFixedArray.from_float(np.random.rand(200, 200), bits=19, int_bits=1)

#     benchmark(lambda x, y: x @ y, a, b)


def test_fixed_matrix_addition_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_long_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=200, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_different_wl_long_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=200, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=167, int_bits=13)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_20_different_wl(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=15, int_bits=3)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_200_different_wl(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=15, int_bits=3)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_elementwise_multiplication_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_elementwise_multiplication_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_elementwise_division_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_matrix_elementwise_division_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_matrix_elementwise_division_medium_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=40, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_matrix_elementwise_division_medium_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=40, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_matrix_elementwise_division_long_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=200, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_matrix_elementwise_multiplication_medium_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=40, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_elementwise_multiplication_medium_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=40, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_negation_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x: -x, a)


def test_fixed_matrix_negation_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x: -x, a)


def test_fixed_matrix_negation_20_long(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=200, int_bits=1)

    benchmark(lambda x: -x, a)


def test_fixed_matrix_negation_200_long(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=200, int_bits=1)

    benchmark(lambda x: -x, a)


def test_fixed_matrix_abs_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)

    benchmark(abs, a)


def test_fixed_matrix_abs_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)

    benchmark(abs, a)


def test_fixed_scalar_matrix_addition_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0], bits=19, int_bits=1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_matrix_addition_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0], bits=19, int_bits=1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_matrix_multiplication_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_matrix_multiplication_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_matrix_multiplication_medium_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=40, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_matrix_multiplication_medium_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=40, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_creation_20(benchmark) -> None:
    a = np.random.rand(20, 20) - 0.5

    benchmark(APyFixedArray.from_float, a, 1, 19)


def test_fixed_matrix_creation_200(benchmark) -> None:
    a = np.random.rand(200, 200) - 0.5

    benchmark(APyFixedArray.from_float, a, 1, 19)


def test_fixed_matrix_to_numpy_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x: x.to_numpy(), a)


def test_fixed_matrix_to_numpy_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x: x.to_numpy(), a)


def test_fixed_vector_sum_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20) - 0.5, bits=19, int_bits=1)

    benchmark(sum, a)


def test_fixed_vector_max_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20) - 0.5, bits=19, int_bits=1)

    benchmark(max, a)


def test_fixed_convolve_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x.convolve(y), a, b)


def test_fixed_convolve_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200) - 0.5, bits=19, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(200) - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x.convolve(y), a, b)


def test_fixed_convolve_medium_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20) - 0.5, bits=39, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20) - 0.5, bits=39, int_bits=1)

    benchmark(lambda x, y: x.convolve(y), a, b)


def test_fixed_convolve_long_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20) - 0.5, bits=119, int_bits=1)
    b = APyFixedArray.from_float(np.random.rand(20) - 0.5, bits=119, int_bits=1)

    benchmark(lambda x, y: x.convolve(y), a, b)


def test_fixed_scalar_matrix_division_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_matrix_division_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_matrix_division_medium_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=40, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_matrix_division_medium_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=40, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_matrix_division_long_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=200, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: x / y, a, b)


def test_fixed_scalar_matrix_reversed_division_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=19, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: y / x, a, b)


def test_fixed_scalar_matrix_reversed_division_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=19, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=19, int_bits=1)

    benchmark(lambda x, y: y / x, a, b)


def test_fixed_scalar_matrix_reversed_division_medium_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=40, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: y / x, a, b)


def test_fixed_scalar_matrix_reversed_division_medium_200(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, bits=40, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=40, int_bits=1)

    benchmark(lambda x, y: y / x, a, b)


def test_fixed_scalar_matrix_reversed_division_long_20(benchmark) -> None:
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, bits=200, int_bits=1)
    b = APyFixed.from_float(np.random.rand(1)[0] - 0.5, bits=200, int_bits=1)

    benchmark(lambda x, y: y / x, a, b)
