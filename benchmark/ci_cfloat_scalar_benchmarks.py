import numpy as np

from apytypes import APyCFloat


def test_cfloat_scalar_multiplication_short(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfloat_scalar_multiplication_long(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 16, 60
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 16, 60
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfloat_scalar_multiplication_mixed(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 16, 60
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfloat_scalar_division_short(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfloat_scalar_division_long(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 16, 60
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 16, 60
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfloat_scalar_division_mixed(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 16, 60
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfloat_scalar_addition_short(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfloat_scalar_addition_long(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 16, 60
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 16, 60
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfloat_scalar_addition_mixed(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )
    b = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 16, 60
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfloat_scalar_creation_short(benchmark) -> None:
    a = np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5)

    benchmark(APyCFloat.from_complex, a, 4, 7)


def test_cfloat_scalar_creation_long(benchmark) -> None:
    a = np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5)

    benchmark(APyCFloat.from_complex, a, 16, 60)


def test_cfloat_scalar_to_complex_short(benchmark) -> None:
    a = APyCFloat.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5), 4, 7
    )

    benchmark(complex, a)
