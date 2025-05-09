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


def test_arith_apyfixed():
    a = APyCFixed.from_complex(1 + 1j, int_bits=5, frac_bits=0)
    b = APyFixed.from_float(2, int_bits=5, frac_bits=0)
    assert a + b == 3 + 1j
    assert a - b == -1 + 1j
    assert a * b == 2 + 2j
    assert a / b == 0.5 + 0.5j

    assert b + a == 3 + 1j
    assert b - a == 1 - 1j
    assert b * a == 2 + 2j
    assert b / a == 1 - 1j


def test_arith_float():
    a = APyCFixed.from_complex(1 + 1j, int_bits=5, frac_bits=0)
    b = 2.0
    assert a + b == 3 + 1j
    assert a - b == -1 + 1j
    assert a * b == 2 + 2j
    assert a / b == 0.5 + 0.5j

    assert b + a == 3 + 1j
    assert b - a == 1 - 1j
    assert b * a == 2 + 2j
    assert b / a == 1 - 1j


def test_arith_int():
    a = APyCFixed.from_complex(1 + 1j, int_bits=5, frac_bits=0)
    b = 2
    assert a + b == 3 + 1j
    assert a - b == -1 + 1j
    assert a * b == 2 + 2j
    assert a / b == 0.5 + 0.5j

    assert b + a == 3 + 1j
    assert b - a == 1 - 1j
    assert b * a == 2 + 2j
    assert b / a == 1 - 1j


def test_arith_complex():
    a = APyCFixed.from_complex(1 + 1j, int_bits=5, frac_bits=0)
    b = 2 + 2j
    assert a + b == 3 + 3j
    assert a - b == -1 - 1j
    assert a * b == 4j
    assert a / b == 0.5

    assert b + a == 3 + 3j
    assert b - a == 1 + 1j
    assert b * a == 4j
    assert b / a == 2.0


def test_shift():
    a = APyCFixed.from_complex(1 + 1j, int_bits=5, frac_bits=0)
    assert (a << 2).is_identical(APyCFixed.from_complex(4 + 4j, 7, -2))
    assert (a >> 2).is_identical(APyCFixed.from_complex(0.25 + 0.25j, 3, 2))
    a <<= 2
    assert a.is_identical(APyCFixed.from_complex(4 + 4j, 7, -2))
    a >>= 4
    assert a.is_identical(APyCFixed.from_complex(0.25 + 0.25j, 3, 2))


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
