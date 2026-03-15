import numpy as np

from apytypes import APyCFloat, APyCFloatArray


def test_cfloat_matrix_multiplication_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )
    b = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )

    benchmark(lambda x, y: x @ y, a, b)


# def test_cfloat_matrix_multiplication_200(benchmark):
#     a = APyCFloatArray.from_complex(np.random.rand(200, 200) - 0.5 + 1j*(np.random.rand(200, 200) - 0.5), 4, 7)
#     b = APyCFloatArray.from_complex(np.random.rand(200, 200) - 0.5 + 1j*(np.random.rand(200, 200) - 0.5), 4, 7)

#     benchmark(lambda x, y: x @ y, a, b)


def test_cfloat_matrix_addition_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )
    b = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfloat_matrix_addition_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )
    b = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfloat_matrix_elementwise_multiplication_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )
    b = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfloat_matrix_elementwise_multiplication_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )
    b = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )

    benchmark(lambda x, y: x * y, a, b)


# def test_cfloat_matrix_negation_20(benchmark):
#     a = APyCFloatArray.from_complex(np.random.rand(20, 20) - 0.5 + 1j*(np.random.rand(20, 20) - 0.5), 4, 7)

#     benchmark(lambda x: -x, a)


# def test_cfloat_matrix_negation_200(benchmark):
#     a = APyCFloatArray.from_complex(np.random.rand(200, 200) - 0.5 + 1j*(np.random.rand(200, 200) - 0.5), 4, 7)

#     benchmark(lambda x: -x, a)


# def test_cfloat_matrix_abs_20(benchmark):
#     a = APyCFloatArray.from_complex(np.random.rand(20, 20) - 0.5 + 1j*(np.random.rand(20, 20) - 0.5), 4, 7)

#     benchmark(abs, a)


# def test_cfloat_matrix_abs_200(benchmark):
#     a = APyCFloatArray.from_complex(np.random.rand(200, 200) - 0.5 + 1j*(np.random.rand(200, 200) - 0.5), 4, 7)

#     benchmark(abs, a)


def test_cfloat_scalar_matrix_addition_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfloat_scalar_matrix_addition_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfloat_scalar_matrix_multiplication_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfloat_scalar_matrix_multiplication_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfloat_matrix_creation_20(benchmark) -> None:
    a = np.random.rand(20, 20)

    benchmark(APyCFloatArray.from_complex, a, 4, 7)


def test_cfloat_matrix_creation_200(benchmark) -> None:
    a = np.random.rand(200, 200)

    benchmark(APyCFloatArray.from_complex, a, 4, 7)


def test_cfloat_matrix_to_numpy_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )

    benchmark(lambda x: x.to_numpy(), a)


def test_cfloat_matrix_to_numpy_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )

    benchmark(lambda x: x.to_numpy(), a)


def test_cfloat_matrix_cast_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 8, 20
    )

    benchmark(lambda x: x.cast(4, 7), a)


def test_cfloat_matrix_cast_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 8, 20
    )

    benchmark(lambda x: x.cast(4, 7), a)


def test_cfloat_convolve_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(np.random.rand(20) - 0.5, 8, 20)
    b = APyCFloatArray.from_complex(np.random.rand(20) - 0.5, 8, 20)

    benchmark(lambda x, y: x.convolve(y), a, b)


def test_cfloat_convolve_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(np.random.rand(200) - 0.5, 8, 20)
    b = APyCFloatArray.from_complex(np.random.rand(200) - 0.5, 8, 20)

    benchmark(lambda x, y: x.convolve(y), a, b)


def test_cfloat_convolve_long_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(np.random.rand(20) - 0.5, 10, 50)
    b = APyCFloatArray.from_complex(np.random.rand(20) - 0.5, 10, 50)

    benchmark(lambda x, y: x.convolve(y), a, b)


def test_cfloat_matrix_elementwise_division_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )
    b = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfloat_matrix_elementwise_division_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )
    b = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfloat_scalar_matrix_division_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfloat_scalar_matrix_division_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfloat_scalar_matrix_reversed_division_20(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(20, 20) - 0.5 + 1j * (np.random.rand(20, 20) - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: y / x, a, b)


def test_cfloat_scalar_matrix_reversed_division_200(benchmark) -> None:
    a = APyCFloatArray.from_complex(
        np.random.rand(200, 200) - 0.5 + 1j * (np.random.rand(200, 200) - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: y / x, a, b)
