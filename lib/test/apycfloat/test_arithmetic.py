import platform
from itertools import product

import pytest

from apytypes import APyCFloat, APyFloat


def is_identical_or_nan(x: APyCFloat, y: APyCFloat):
    """
    Test if two :class:`APyCFloat` are identical or if they are NaN in the same fields.
    This can be useful in some tests since `NaN != NaN` always.

    Parameters
    ----------
    x: :class:`APyCFloat`
        The first apytypes complex floating-point value.
    y: :class:`APyCFloat`
        The second apytypes complex floating-point value.
    """
    if x.real.is_nan or x.imag.is_nan or y.imag.is_nan or x.imag.is_nan:
        if x.real.is_nan != y.real.is_nan or x.imag.is_nan != y.imag.is_nan:
            return False
        else:
            real_eq = True if x.real.is_nan else x.real.is_identical(y.real)
            imag_eq = True if x.imag.is_nan else x.imag.is_identical(y.imag)
            return real_eq and imag_eq
    else:
        return x.is_identical(y)


def complex_division_double_precision(z: complex, w: complex) -> complex:
    """
    Perform a complex-valued division of `complex` using the same method as in APyTypes.

    On x86 platforms, this is identical to `z / w`, however, on ARM64-based systems, the
    division algorithm used varies slightly. By comparing `APyCFloat` division results
    to this complex-valued division function, we ensure consistent behaviour during
    tests on various platforms.

    The complex-valued algorithm used in APyTypes (and on x86-based Python systems) is
    the following: Robert L. Smith, ``ALGORITHM 116 COMPLEX DIVISION''
    https://dl.acm.org/doi/pdf/10.1145/368637.368661

    Parameters
    ----------
    z: :class:`complex`
        Complex-valued numerator.
    w: :class:`complex`
        Complex-valued denominator.
    """
    a, b, c, d = z.real, z.imag, w.real, w.imag
    e, f = 0.0, 0.0
    if abs(c) >= abs(d):
        r = d / c
        den = c + r * d
        e = (a + b * r) / den
        f = (b - a * r) / den
    else:
        r = c / d
        den = d + r * c
        e = (a * r + b) / den
        f = (b * r - a) / den
    return complex(e, f)


@pytest.mark.float_div
@pytest.mark.parametrize(
    ("num_val", "den_val"),
    list(
        product(
            [
                complex(re, im)
                for (re, im) in product(
                    [0.0, -0.0, 1.0, -1.0, float("inf"), float("-inf"), float("nan")],
                    repeat=2,
                )
            ],
            repeat=2,
        )
    ),
)
def test_div_recover_infs_and_nans(num_val: complex, den_val: complex):
    # Division with zero
    num = APyCFloat.from_complex(num_val, exp_bits=11, man_bits=52)
    den = APyCFloat.from_complex(den_val, exp_bits=11, man_bits=52)
    if not (den.is_zero):
        assert is_identical_or_nan(
            num / den,
            APyCFloat.from_complex(num_val / den_val, exp_bits=11, man_bits=52),
        )


@pytest.mark.parametrize(
    "num_real", [0.0, 1.0, -2.0, float("inf"), float("-inf"), float("nan")]
)
@pytest.mark.parametrize(
    "num_imag", [0.0, 1.0, -2.0, float("inf"), float("-inf"), float("nan")]
)
@pytest.mark.parametrize("den_real", [0.0, -0.0])
def test_div_with_zero(num_real: float, num_imag: float, den_real: float):
    num = APyCFloat.from_complex(num_real + 1j * num_imag, exp_bits=11, man_bits=52)
    den = APyCFloat.from_complex(den_real, exp_bits=11, man_bits=52)
    res = num / den

    if num.real.is_zero or num.real.is_nan:
        assert res.real.is_nan
    else:
        assert res.real.is_inf
        assert res.real.sign == (num.real.sign ^ den.real.sign)

    if num.imag.is_zero or num.imag.is_nan:
        assert res.imag.is_nan
    else:
        assert res.imag.is_inf
        assert res.imag.sign == (num.imag.sign ^ den.real.sign)


@pytest.mark.float_div
@pytest.mark.parametrize("apyfloat", [APyCFloat])
def test_div_mixed_bias_overflow(apyfloat: type[APyCFloat]):
    """Test that a result can overflow to infinity due to a change in bias."""
    x = apyfloat(sign=0, exp=21, man=0, exp_bits=5, man_bits=2, bias=5)
    y = apyfloat(sign=0, exp=25, man=0, exp_bits=5, man_bits=2, bias=25)

    # Divide with one
    assert (_ := x / y).is_identical(
        apyfloat(sign=(0, 0), exp=(31, 31), man=(0, 1), exp_bits=5, man_bits=2, bias=15)
    )

    # Divide with one but with larger bias difference
    y = apyfloat(sign=0, exp=27, man=0, exp_bits=5, man_bits=2, bias=27)
    assert (_ := x / y).is_identical(
        apyfloat(sign=(0, 0), exp=(31, 31), man=(0, 1), exp_bits=5, man_bits=2, bias=16)
    )


@pytest.mark.float_div
@pytest.mark.parametrize("apyfloat", [APyCFloat])
def test_subnormal_quant_div(apyfloat: type[APyCFloat]):
    """Subnormal becoming normal after quantization."""
    res = apyfloat(sign=0, exp=1, man=1023, exp_bits=5, man_bits=10) / apyfloat(
        sign=0, exp=16, man=0, exp_bits=5, man_bits=10
    )
    assert res.is_identical(apyfloat(sign=0, exp=1, man=0, exp_bits=5, man_bits=10))

    # -2 / -8.98847e+307
    res = apyfloat(
        sign=1, exp=1023, man=4503599627370495, exp_bits=11, man_bits=52
    ) / apyfloat(sign=1, exp=2046, man=0, exp_bits=11, man_bits=52)
    assert res.is_identical(
        apyfloat(sign=(0, 1), exp=(1, 0), man=(0, 0), exp_bits=11, man_bits=52)
    )


@pytest.mark.float_div
@pytest.mark.parametrize("x", [0.0 + 0.0j, 1.0 - 3.0j, -7.25 + 3.5j])
@pytest.mark.parametrize("y", [-2.25 + 0.0j, 0.5 - 125.25j, 72.0 - 99.875j, 0.0 + 2.5j])
def test_div_double_precision(x: complex, y: complex):
    lhs = APyCFloat.from_complex(x, 11, 52)
    rhs = APyCFloat.from_complex(y, 11, 52)
    py_res = complex_division_double_precision(x, y)
    assert is_identical_or_nan(lhs / rhs, APyCFloat.from_complex(py_res, 11, 52))

    # Also test the division reference function on suitable machines...
    if any(platform.machine() == p for p in ("AMD64", "x86_64", "x86")):
        assert py_res == x / y


@pytest.mark.float_mul
@pytest.mark.parametrize("x", [0.0 + 0.0j, 1.0 - 3.0j, -7.25 + 3.5j])
@pytest.mark.parametrize("y", [-2.25 + 0.0j, 0.5 - 125.25j, 72.0 - 99.875j, 0.0 + 2.5j])
def test_mul_double_precision(x: complex, y: complex):
    lhs = APyCFloat.from_complex(x, 11, 52)
    rhs = APyCFloat.from_complex(y, 11, 52)
    assert is_identical_or_nan(lhs * rhs, APyCFloat.from_complex(x * y, 11, 52))


@pytest.mark.float_div
@pytest.mark.parametrize("x", [0.0 + 0.0j, 1.0 - 3.0j, -7.25 + 3.5j])
@pytest.mark.parametrize("y", [-2.25 + 0.0j, 0.5 - 125.25j, 72.0 - 99.875j, 0.0 + 2.5j])
def test_add_double_precision(x: complex, y: complex):
    lhs = APyCFloat.from_complex(x, 11, 52)
    rhs = APyCFloat.from_complex(y, 11, 52)
    assert is_identical_or_nan(lhs + rhs, APyCFloat.from_complex(x + y, 11, 52))


@pytest.mark.float_div
@pytest.mark.parametrize("x", [0.0 + 0.0j, 1.0 - 3.0j, -7.25 + 3.5j])
@pytest.mark.parametrize("y", [-2.25 + 0.0j, 0.5 - 125.25j, 72.0 - 99.875j, 0.0 + 2.5j])
def test_sub_double_precision(x: complex, y: complex):
    lhs = APyCFloat.from_complex(x, 11, 52)
    rhs = APyCFloat.from_complex(y, 11, 52)
    assert is_identical_or_nan(lhs - rhs, APyCFloat.from_complex(x - y, 11, 52))


@pytest.mark.parametrize("exp_bits", [5, 11, 30])
@pytest.mark.parametrize("man_bits", [10, 35, 52])
@pytest.mark.parametrize(
    ("a_val", "b_val"),
    product(
        [0.0 + 0.0j, -1.0 + 0.0j, -0.0 - 1.0j, 3.0625 - 1.25j, -(2 ** (-4)) + 247.25j],
        repeat=2,
    ),
)
def test_arithmetic_with_python_complex(
    exp_bits: int,
    man_bits: int,
    a_val: complex,
    b_val: complex,
):
    a = APyCFloat.from_complex(a_val, exp_bits=exp_bits, man_bits=man_bits)
    b = complex(b_val)
    assert (a + b).is_identical(
        APyCFloat.from_complex(a_val + b_val, exp_bits, man_bits)
    )
    assert (a - b).is_identical(
        APyCFloat.from_complex(a_val - b_val, exp_bits, man_bits)
    )
    assert (b + a).is_identical(
        APyCFloat.from_complex(b_val + a_val, exp_bits, man_bits)
    )
    assert (b - a).is_identical(
        APyCFloat.from_complex(b_val - a_val, exp_bits, man_bits)
    )
    if exp_bits == 11 and man_bits == 52:
        assert (a * b).is_identical(
            APyCFloat.from_complex(a_val * b_val, exp_bits, man_bits)
        )
        assert (b * a).is_identical(
            APyCFloat.from_complex(b_val * a_val, exp_bits, man_bits)
        )
        if b != 0:
            assert (a / b).is_identical(
                APyCFloat.from_complex(
                    complex_division_double_precision(a_val, b_val), exp_bits, man_bits
                )
            )
        if a != 0:
            assert (b / a).is_identical(
                APyCFloat.from_complex(
                    complex_division_double_precision(b_val, a_val), exp_bits, man_bits
                )
            )


@pytest.mark.parametrize("exp_bits", [5, 11, 30])
@pytest.mark.parametrize("man_bits", [10, 35, 52])
@pytest.mark.parametrize(
    "a_val",
    [0.0 + 0.0j, -1.0 + 0.0j, -0.0 - 1.0j, 3.0625 - 1.25j, -(2 ** (-4)) + 247.25j],
)
@pytest.mark.parametrize("b_val", [0.0, -1.0, 3.0625, -(2**-7), 1234.0625])
def test_arithmetic_with_python_float(
    exp_bits: int,
    man_bits: int,
    a_val: complex,
    b_val: float,
):
    a = APyCFloat.from_complex(a_val, exp_bits=exp_bits, man_bits=man_bits)
    b = float(b_val)
    assert (a + b).is_identical(
        APyCFloat.from_complex(a_val + b_val, exp_bits, man_bits)
    )
    assert (a - b).is_identical(
        APyCFloat.from_complex(a_val - b_val, exp_bits, man_bits)
    )
    assert (b + a).is_identical(
        APyCFloat.from_complex(b_val + a_val, exp_bits, man_bits)
    )
    assert (b - a).is_identical(
        APyCFloat.from_complex(b_val - a_val, exp_bits, man_bits), ignore_zero_sign=True
    )
    if exp_bits == 11 and man_bits == 52:
        assert (a * b).is_identical(
            APyCFloat.from_complex(a_val * complex(b_val), exp_bits, man_bits)
        )
        assert (b * a).is_identical(
            APyCFloat.from_complex(b_val * complex(a_val), exp_bits, man_bits),
            ignore_zero_sign=True,
        )
        if b != 0:
            assert (a / b).is_identical(
                APyCFloat.from_complex(
                    complex_division_double_precision(a_val, b_val), exp_bits, man_bits
                )
            )
        if a != 0:
            assert (b / a).is_identical(
                APyCFloat.from_complex(
                    complex_division_double_precision(b_val, a_val), exp_bits, man_bits
                )
            )


@pytest.mark.parametrize("exp_bits", [5, 11, 30])
@pytest.mark.parametrize("man_bits", [10, 35, 52])
@pytest.mark.parametrize(
    "a_val",
    [0.0 + 0.0j, -1.0 + 0.0j, -0.0 - 1.0j, 3.0625 - 1.25j, -(2 ** (-4)) + 247.25j],
)
@pytest.mark.parametrize("b_val", [0.0, -1.0, 3.0625, -(2**-7), 1234.0625])
def test_arithmetic_with_apyfloat(
    exp_bits: int,
    man_bits: int,
    a_val: complex,
    b_val: float,
):
    a = APyCFloat.from_complex(a_val, exp_bits=exp_bits, man_bits=man_bits)
    b = APyFloat.from_float(b_val, exp_bits=exp_bits, man_bits=man_bits)
    assert (a + b).is_identical(
        APyCFloat.from_complex(a_val + b_val, exp_bits, man_bits)
    )
    assert (a - b).is_identical(
        APyCFloat.from_complex(a_val - b_val, exp_bits, man_bits)
    )
    assert (b + a).is_identical(
        APyCFloat.from_complex(b_val + a_val, exp_bits, man_bits)
    )
    assert (b - a).is_identical(
        APyCFloat.from_complex(b_val - a_val, exp_bits, man_bits), ignore_zero_sign=True
    )
    if exp_bits == 11 and man_bits == 52:
        assert (a * b).is_identical(
            APyCFloat.from_complex(a_val * complex(b_val), exp_bits, man_bits)
        )
        assert (b * a).is_identical(
            APyCFloat.from_complex(b_val * complex(a_val), exp_bits, man_bits),
            ignore_zero_sign=True,
        )
        if b != 0:
            assert (a / b).is_identical(
                APyCFloat.from_complex(
                    complex_division_double_precision(a_val, b_val), exp_bits, man_bits
                )
            )
        if a != 0:
            assert (b / a).is_identical(
                APyCFloat.from_complex(
                    complex_division_double_precision(b_val, a_val), exp_bits, man_bits
                )
            )
