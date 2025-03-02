from apytypes import APyCFixed
import numpy as np


def test_cfixed_scalar_multiplication_short(benchmark):
    a = APyCFixed.from_float(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )
    b = APyCFixed.from_float(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfixed_scalar_multiplication_medium(benchmark):
    a = APyCFixed.from_float(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=40,
        int_bits=1,
    )
    b = APyCFixed.from_float(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=40,
        int_bits=1,
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfixed_scalar_multiplication_long(benchmark):
    a = APyCFixed.from_float(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )
    b = APyCFixed.from_float(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfixed_scalar_multiplication_mixed(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )
    b = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )

    benchmark(lambda x, y: x * y, a, b)


def test_cfixed_scalar_division_short(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )
    b = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfixed_scalar_division_medium(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=40,
        int_bits=1,
    )
    b = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=40,
        int_bits=1,
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfixed_scalar_division_long(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )
    b = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfixed_scalar_division_mixed(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )
    b = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )

    benchmark(lambda x, y: x / y, a, b)


def test_cfixed_scalar_addition_short(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )
    b = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfixed_scalar_addition_short_different_wl(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=15,
        int_bits=3,
    )
    b = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfixed_scalar_addition_long(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )
    b = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfixed_scalar_addition_mixed(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )
    b = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )

    benchmark(lambda x, y: x + y, a, b)


def test_cfixed_scalar_creation_short(benchmark):
    a = np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5)

    benchmark(APyCFixed.from_complex, a, 1, 19)


def test_cfixed_scalar_creation_long(benchmark):
    a = np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5)

    benchmark(APyCFixed.from_complex, a, 1, 199)


def test_cfixed_scalar_to_complex_short(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )

    benchmark(complex, a)


def test_cfixed_scalar_negation_short(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=20,
        int_bits=1,
    )

    benchmark(lambda x: -x, a)


def test_cfixed_scalar_negation_long(benchmark):
    a = APyCFixed.from_complex(
        np.random.rand(1)[0] - 0.5 + 1j * (np.random.rand(1)[0] - 0.5),
        bits=200,
        int_bits=1,
    )

    benchmark(lambda x: -x, a)
