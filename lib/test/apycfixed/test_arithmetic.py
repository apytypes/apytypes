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

    b = APyFixed.from_float(2, int_bits=5, frac_bits=20)
    assert a + b == 3 + 1j
    assert a - b == -1 + 1j
    assert a * b == 2 + 2j
    assert a / b == 0.5 + 0.5j

    assert b + a == 3 + 1j
    assert b - a == 1 - 1j
    assert b * a == 2 + 2j
    assert b / a == 1 - 1j

    b = APyFixed.from_float(2, int_bits=5, frac_bits=40)
    assert a + b == 3 + 1j
    assert a - b == -1 + 1j
    assert a * b == 2 + 2j
    assert a / b == 0.5 + 0.5j

    assert b + a == 3 + 1j
    assert b - a == 1 - 1j
    assert b * a == 2 + 2j
    assert b / a == 1 - 1j

    b = APyFixed.from_float(2, int_bits=5, frac_bits=100)
    assert a + b == 3 + 1j
    assert a - b == -1 + 1j
    assert a * b == 2 + 2j
    assert a / b == 0.5 + 0.5j

    assert b + a == 3 + 1j
    assert b - a == 1 - 1j
    assert b * a == 2 + 2j
    assert b / a == 1 - 1j

    b = APyFixed.from_float(2, int_bits=5, frac_bits=1000)
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


def test_division_two_limb_result_64():
    a = APyCFixed((3, 7), frac_bits=50, int_bits=7)
    b = APyCFixed((18, -1), frac_bits=40, int_bits=7)
    assert (a / b).is_identical(
        APyCFixed((20352806008308, 55861956916422), bits=105, int_bits=48)
    )
    assert (b / a).is_identical(
        APyCFixed(
            (116782997233883206, 40564819207303020315838264892593),
            bits=105,
            int_bits=58,
        )
    )


def test_division_two_limb_result_one_longer_64():
    a = APyCFixed((3, -7), frac_bits=70, int_bits=7)
    b = APyCFixed((18, -1), frac_bits=30, int_bits=7)
    assert (a / b).is_identical(
        APyCFixed(
            (25796234343, 41538374868278621028243918618402993), bits=115, int_bits=38
        )
    )

    assert (b / a).is_identical(
        APyCFixed(
            (158932058182095645812665, 320469559940946957950128), bits=115, int_bits=78
        )
    )

    a = APyCFixed(
        (362677745884388738990080, 241785163922925848363008), bits=80, int_bits=0
    )
    b = APyCFixed((307, 819), bits=10, int_bits=0)
    assert (a / b).is_identical(
        APyCFixed(
            (464202533186689500641874, 1116447971206343375750177), bits=91, int_bits=11
        )
    )


def test_division_two_limb_result_32():
    a = APyCFixed((3, 7), frac_bits=20, int_bits=7)
    b = APyCFixed((18, -1), frac_bits=10, int_bits=7)
    assert (a / b).is_identical(APyCFixed((18955, 52025), bits=45, int_bits=18))
    assert (b / a).is_identical(
        APyCFixed((108762641, 35184073570093), bits=45, int_bits=28)
    )


def test_division_two_limb_result_one_longer_32():
    a = APyCFixed((3, -7), frac_bits=35, int_bits=7)
    b = APyCFixed((18, -1), frac_bits=10, int_bits=7)
    assert (a / b).is_identical(
        APyCFixed((24601, 1152921504606797371), bits=60, int_bits=18)
    )

    assert (b / a).is_identical(
        APyCFixed((4625531675471, 9326891739065), bits=60, int_bits=43)
    )

    a = APyCFixed((3, -7), frac_bits=30, int_bits=4)
    b = APyCFixed((18, -1), frac_bits=7, int_bits=1)
    assert (a / b).is_identical(APyCFixed((48, 8796093022112), bits=43, int_bits=12))


@pytest.mark.parametrize("real_int_bits", [3, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("real_frac_bits", [0, 5, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_int_bits", [3, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_frac_bits", [0, 5, 20, 40, 100, 10000])
def test_complex_real_division_different_wordlengths_different_fracbits(
    real_int_bits: int,
    real_frac_bits: int,
    complex_int_bits: int,
    complex_frac_bits: int,
):
    a = APyCFixed.from_complex(
        1 + 1j, int_bits=complex_int_bits, frac_bits=complex_frac_bits
    )
    b = APyFixed.from_float(2, int_bits=real_int_bits, frac_bits=real_frac_bits)
    assert a / b == 0.5 + 0.5j


@pytest.mark.parametrize("real_int_bits", [6, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("real_frac_bits", [0, 5, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_int_bits", [4, 10, 20, 40, 100, 10000])
@pytest.mark.parametrize("complex_frac_bits", [0, 5, 20, 40, 100, 10000])
def test_real_complex_division_different_wordlengths_different_fracbits(
    real_int_bits: int,
    real_frac_bits: int,
    complex_int_bits: int,
    complex_frac_bits: int,
):
    a = APyFixed.from_float(25, int_bits=real_int_bits, frac_bits=real_frac_bits)
    b = APyCFixed.from_complex(
        3 + 4j, int_bits=complex_int_bits, frac_bits=complex_frac_bits
    )
    assert a / b == 3 - 4j
