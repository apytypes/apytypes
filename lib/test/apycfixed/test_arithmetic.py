import pytest

from apytypes import APyCFixed, APyFixed


def test_mul():
    a = APyCFixed.from_complex(-1 - 1j, int_bits=1, frac_bits=0)
    assert (a * a).is_identical(APyCFixed.from_complex(2j, int_bits=3, frac_bits=0))

    a = APyCFixed.from_complex(-1 - 1j, int_bits=100, frac_bits=0)
    assert (a * a).is_identical(APyCFixed.from_complex(2j, int_bits=201, frac_bits=0))


def test_div():
    a = APyCFixed.from_complex(-1 - 1j, int_bits=30, frac_bits=100)
    b = APyCFixed.from_complex(-1 + 1j, int_bits=30, frac_bits=100)

    assert a / b == 0.0 + 1.0j
    assert b / a == 0.0 - 1.0j


def test_unary_arith():
    a = APyCFixed.from_complex(1 + 1j, int_bits=5, frac_bits=0)
    assert -a == -1 - 1j
    assert +a == 1 + 1j


@pytest.mark.parametrize("int_bits", [*range(2, 10), 100, 10000])
@pytest.mark.parametrize("frac_bits", [*range(0, 10), 100, 10000])
def test_arith_apyfixed(int_bits: int, frac_bits: int):
    a = APyCFixed.from_complex(1 + 1j, int_bits=int_bits, frac_bits=frac_bits)
    b = APyFixed.from_float(2, int_bits=5, frac_bits=2)
    assert a + b == 3 + 1j
    assert a - b == -1 + 1j
    assert a * b == 2 + 2j
    assert a / b == 0.5 + 0.5j

    assert b + a == 3 + 1j
    assert b - a == 1 - 1j
    assert b * a == 2 + 2j
    assert b / a == 1 - 1j


@pytest.mark.parametrize("int_bits", [*range(3, 10), 100, 10000])
@pytest.mark.parametrize("frac_bits", [*range(0, 10), 100, 10000])
def test_arith_float(int_bits: int, frac_bits: int):
    a = APyCFixed.from_complex(1 + 1j, int_bits=int_bits, frac_bits=frac_bits)
    b: float = 2.0
    assert a + b == 3 + 1j
    assert a - b == -1 + 1j
    assert a * b == 2 + 2j
    assert a / b == 0.5 + 0.5j

    assert b + a == 3 + 1j
    assert b - a == 1 - 1j
    assert b * a == 2 + 2j
    assert b / a == 1 - 1j


@pytest.mark.parametrize("int_bits", [*range(3, 10), 100, 10000])
@pytest.mark.parametrize("frac_bits", [*range(0, 10), 100, 10000])
def test_arith_int(int_bits: int, frac_bits: int):
    a = APyCFixed.from_complex(1 + 1j, int_bits=int_bits, frac_bits=frac_bits)
    b: int = 2
    assert a + b == 3 + 1j
    assert a - b == -1 + 1j
    assert a * b == 2 + 2j
    assert a / b == 0.5 + 0.5j

    assert b + a == 3 + 1j
    assert b - a == 1 - 1j
    assert b * a == 2 + 2j
    assert b / a == 1 - 1j


@pytest.mark.parametrize("int_bits", [*range(3, 10), 100, 10000])
@pytest.mark.parametrize("frac_bits", [*range(0, 10), 100, 10000])
def test_arith_complex(int_bits: int, frac_bits: int):
    a = APyCFixed.from_complex(1 + 1j, int_bits=int_bits, frac_bits=frac_bits)
    b = 2 + 2j
    assert a + b == 3 + 3j
    assert a - b == -1 - 1j
    assert a * b == 4j
    assert a / b == 0.5

    assert b + a == 3 + 3j
    assert b - a == 1 + 1j
    assert b * a == 4j
    assert b / a == 2.0


@pytest.mark.parametrize("int_bits", [*range(3, 10), 100, 10000])
@pytest.mark.parametrize("frac_bits", [*range(0, 10), 100, 10000])
def test_shift(int_bits: int, frac_bits: int):
    a = APyCFixed.from_complex(1 + 1j, int_bits=int_bits, frac_bits=frac_bits)
    assert (a << 2).is_identical(
        APyCFixed.from_complex(4 + 4j, int_bits + 2, frac_bits - 2)
    )
    assert (a >> 2).is_identical(
        APyCFixed.from_complex(0.25 + 0.25j, int_bits - 2, frac_bits + 2)
    )
    a <<= 2
    assert a.is_identical(APyCFixed.from_complex(4 + 4j, int_bits + 2, frac_bits - 2))
    a >>= 4
    assert a.is_identical(
        APyCFixed.from_complex(0.25 + 0.25j, int_bits - 2, frac_bits + 2)
    )


@pytest.mark.parametrize("bits", range(13, 90))
def test_multiplication_different_wordlengths(bits):
    a = APyCFixed((3, 7), bits=bits, int_bits=7)
    b = APyCFixed((18, 1), bits=bits, int_bits=7)
    assert (a * b).is_identical(APyCFixed((47, 129), bits=2 * bits + 1, int_bits=15))


@pytest.mark.parametrize("bits", range(13, 90))
def test_multiplicatio_different_wordlengths_different_fracbits(bits):
    a = APyCFixed((3, 7), bits=bits, int_bits=8)
    b = APyCFixed((18, 1), bits=bits, int_bits=6)
    assert (a * b).is_identical(APyCFixed((47, 129), bits=2 * bits + 1, int_bits=15))
