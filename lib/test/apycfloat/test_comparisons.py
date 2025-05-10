from itertools import product

import pytest

from apytypes import APyCFixed, APyCFloat, APyFixed, APyFloat


@pytest.mark.float_comp
def test_comparisons_with_floats():
    a = APyCFloat.from_float(0.5, 3, 3)
    assert a != 0.0
    assert -a != 0.0
    assert a != 0.0
    assert -a != 0.0

    assert a == 0.5
    assert -a != 0.5
    assert a == 0.5
    assert -a != 0.5

    a = APyCFloat.from_float(0.0, 3, 3)
    assert a == 0.0
    assert -a == 0.0
    assert a == 0.0
    assert -a == 0.0


@pytest.mark.float_comp
def test_comparisons_with_apyfixed():
    a = APyCFloat.from_float(2.5, 5, 5)
    b = APyFixed.from_float(5, 5, 5)
    assert a != b
    assert a == (b >> 1)

    assert APyCFloat.from_float(float("inf"), 4, 3) != APyFixed.from_float(1000, 16, 16)
    assert APyCFloat.from_float(float("nan"), 4, 3) != APyFixed.from_float(1000, 16, 16)
    assert APyCFloat.from_float(float("nan"), 4, 3) != APyFixed.from_float(1000, 16, 16)
    assert APyCFloat.from_float(float("inf"), 4, 3) != APyFixed.from_float(1000, 16, 16)


@pytest.mark.float_comp
@pytest.mark.parametrize(
    "flp",
    [
        APyCFloat.from_complex(complex(0), exp_bits=15, man_bits=20),
        APyCFloat.from_complex(complex(-1j), exp_bits=4, man_bits=19),
        APyCFloat.from_complex(complex(-3.25), exp_bits=7, man_bits=17),
        APyCFloat.from_complex(complex(3.25 - 5.125j), exp_bits=20, man_bits=50),
        APyCFloat.from_complex(float("inf") + 0j, exp_bits=15, man_bits=51),
        APyCFloat.from_complex(0 + float("inf") * 1j, exp_bits=10, man_bits=52),
    ],
)
@pytest.mark.parametrize(
    "fxp",
    [
        APyCFixed.from_complex(complex(0), int_bits=15, frac_bits=15),
        APyCFixed.from_complex(complex(-1j), int_bits=10, frac_bits=0),
        APyCFixed.from_complex(complex(-3.25), int_bits=7, frac_bits=250),
        APyCFixed.from_complex(complex(3.25 - 5.125j), int_bits=19, frac_bits=5000),
    ],
)
def test_comparisons_with_apycfixed(flp: APyCFloat, fxp: APyCFixed):
    a = complex(flp)
    b = complex(fxp)
    assert (a == b) ^ (flp != fxp)
    assert (a != b) ^ (flp == fxp)


@pytest.mark.float_comp
@pytest.mark.parametrize(
    "cflp",
    [
        APyCFloat.from_complex(complex(0), exp_bits=15, man_bits=20),
        APyCFloat.from_complex(complex(-1j), exp_bits=4, man_bits=19),
        APyCFloat.from_complex(complex(-3.25), exp_bits=7, man_bits=17),
        APyCFloat.from_complex(complex(3.25 - 5.125j), exp_bits=20, man_bits=50),
        APyCFloat.from_complex(float("inf") + 0j, exp_bits=15, man_bits=51),
        APyCFloat.from_complex(0 + float("inf") * 1j, exp_bits=10, man_bits=52),
    ],
)
@pytest.mark.parametrize(
    "flp",
    [
        APyFloat.from_float(0, exp_bits=10, man_bits=30),
        APyFloat.from_float(-1, exp_bits=9, man_bits=10),
        APyFloat.from_float(-3.25, exp_bits=7, man_bits=10),
        APyFloat.from_float(3.25, exp_bits=20, man_bits=50),
        APyFloat.from_float(float("inf"), exp_bits=15, man_bits=51),
    ],
)
def test_comparisons_with_apyfloat(cflp: APyCFloat, flp: APyFloat):
    a = complex(cflp)
    b = float(flp)
    assert (a == b) ^ (cflp != flp)
    assert (a != b) ^ (cflp == flp)


@pytest.mark.float_comp
@pytest.mark.float_special
def test_inf_comparison():
    assert APyCFloat.from_float(float("inf"), 5, 5) == APyCFloat.from_float(
        float("inf"), 5, 5
    )
    assert APyCFloat.from_float(float("inf"), 5, 5) == APyCFloat.from_float(
        float("inf"), 3, 2
    )
    assert APyCFloat.from_float(float("inf"), 5, 5) != APyCFloat.from_float(
        float("-inf"), 5, 5
    )


@pytest.mark.float_comp
@pytest.mark.parametrize("a_sign", product([0, 1], repeat=2))
@pytest.mark.parametrize("b_sign", product([0, 1], repeat=2))
def test_signed_zero_comparison(a_sign: tuple[int, int], b_sign: tuple[int, int]):
    a = APyCFloat(a_sign, (0, 0), (0, 0), 5, 10)
    b = APyCFloat(b_sign, (0, 0), (0, 0), 5, 10)
    assert a == b
    assert a == b
    assert b == a
    assert b == a


@pytest.mark.float_comp
def test_mixed_bias_comparisons():
    """Comparisons where all fields are different."""
    # x and y are equal
    x = APyCFloat(sign=0, exp=5, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyCFloat(sign=0, exp=25, man=1, exp_bits=5, man_bits=2, bias=25)
    assert x == y
    assert x == y

    # Two normal numbers but y is smaller
    y = APyCFloat(sign=0, exp=5, man=1, exp_bits=5, man_bits=2, bias=6)
    assert x != y
    assert x != y

    # Two equal subnormal numbers
    x = APyCFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyCFloat(sign=0, exp=0, man=2, exp_bits=5, man_bits=2, bias=6)
    assert x == y
    assert x == y

    # Two subnormal numbers but y is slightly larger
    y = APyCFloat(sign=0, exp=0, man=3, exp_bits=5, man_bits=2, bias=6)
    assert x != y
    assert x != y

    # Subnormal number equal to normal number
    x = APyCFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyCFloat(sign=0, exp=1, man=0, exp_bits=5, man_bits=2, bias=7)
    assert x == y
    assert x == y


@pytest.mark.float_comp
def test_mixed_format_comparisons():
    """Comparisons with mixed formats."""
    # x and y are equal
    x = APyCFloat(sign=0, exp=5, man=2, exp_bits=4, man_bits=3, bias=5)
    y = APyCFloat(sign=0, exp=25, man=1, exp_bits=5, man_bits=2, bias=25)
    assert x == y
    assert x == y

    # Two normal numbers but y is smaller
    y = APyCFloat(sign=0, exp=7, man=3, exp_bits=5, man_bits=5, bias=8)
    assert x != y
    assert x != y

    # Two equal subnormal numbers
    x = APyCFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyCFloat(sign=0, exp=0, man=4, exp_bits=4, man_bits=3, bias=6)
    assert x == y
    assert x == y

    # Two subnormal numbers but y is slightly larger
    x = APyCFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=2, bias=5)
    y = APyCFloat(sign=0, exp=0, man=5, exp_bits=4, man_bits=3, bias=6)
    assert x != y
    assert x != y
