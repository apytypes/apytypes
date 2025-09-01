from itertools import permutations as perm

import pytest

from apytypes import APyCFloat, APyFixed, APyFloat


def real_is_nan(fp: APyFloat | APyCFloat) -> bool:
    """
    Test if an `APyFloat` is NaN, or if the real part of an `APyCFloat` is NaN.

    Parameters
    ----------
    fp : :class:`APyFloat` or :class:`APyCFloat`
        The APyTypes floating-point number.
    """
    return fp.real.is_nan if isinstance(fp, APyCFloat) else fp.is_nan


def real_is_inf(fp: APyFloat | APyCFloat) -> bool:
    """
    Test if an `APyFloat` is inf, or if the real part of an `APyCFloat` is inf.

    Parameters
    ----------
    fp : :class:`APyFloat` or :class:`APyCFloat`
        The APyTypes floating-point number.
    """
    return fp.real.is_inf if isinstance(fp, APyCFloat) else fp.is_inf


@pytest.mark.parametrize("apyfloat", [APyFloat, APyCFloat])
def test_scalar_constructor_raises(apyfloat: type[APyCFloat]):
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.__init__: exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = apyfloat(0, 0, 0, 300, 5)
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.__init__: exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = apyfloat(0, 0, 0, -300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.__init__: mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = apyfloat(0, 0, 0, 5, 300)
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.__init__: mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = apyfloat(0, 0, 0, 5, -300)


@pytest.mark.parametrize("apyfloat", [APyFloat, APyCFloat])
def test_scalar_from_float_constructor_raises(apyfloat: type[APyCFloat]):
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.from_float: exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = apyfloat.from_float(0.0, exp_bits=300, man_bits=5)
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.from_float: exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = apyfloat.from_float(0.0, exp_bits=-300, man_bits=5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.from_float: mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = apyfloat.from_float(0.0, exp_bits=5, man_bits=300)
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.from_float: mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = apyfloat.from_float(0.0, exp_bits=5, man_bits=-300)


def test_scalar_from_bits_raises():
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.from_bits: exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = APyFloat.from_bits(0, 300, 5)
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.from_bits: exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = APyFloat.from_bits(0, -300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.from_bits: mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = APyFloat.from_bits(0, 5, 300)
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.from_bits: mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = APyFloat.from_bits(0, 5, -300)


@pytest.mark.parametrize("apyfloat", [APyFloat, APyCFloat])
def test_scalar_cast_raises(apyfloat: type[APyCFloat]):
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.cast: exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = apyfloat(0, 0, 0, 5, 5).cast(300, 5)
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.cast: exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = apyfloat(0, 0, 0, 5, 5).cast(-300, 5)
    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.cast: mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        _ = apyfloat(0, 0, 0, 5, 5).cast(5, 300)
    with pytest.raises(
        ValueError,
        match=r"APyC?Float\.cast: mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        _ = apyfloat(0, 0, 0, 5, 5).cast(5, -300)


@pytest.mark.float_special
@pytest.mark.parametrize(
    ("apyfloat", "py_type"), [(APyFloat, float), (APyCFloat, complex)]
)
@pytest.mark.parametrize(
    "float_val", [float("nan"), float("inf"), float("-inf"), 0.0, -0.0]
)
def test_special_conversions(
    apyfloat: type[APyCFloat], py_type: type[complex], float_val: float
):
    assert str(py_type(apyfloat.from_float(py_type(float_val), 5, 5))) == str(
        py_type(float_val)
    )


@pytest.mark.parametrize(
    ("apyfloat", "py_type"), [(APyFloat, float), (APyCFloat, complex)]
)
@pytest.mark.parametrize("exp", list(perm([5, 6, 7, 8], 2)))
@pytest.mark.parametrize("man", list(perm([5, 6, 7, 8], 2)))
@pytest.mark.parametrize("val", [2.625, 12])
@pytest.mark.parametrize("neg", [-1.0, 1.0])
def test_normal_conversions(
    apyfloat: type[APyCFloat],
    py_type: type[complex],
    exp: tuple[int, int],
    man: tuple[int, int],
    val: float,
    neg: float,
):
    val *= neg
    assert py_type(apyfloat.from_float(val, exp[0], man[0])) == py_type(
        apyfloat.from_float(val, exp[1], man[1])
    )


@pytest.mark.parametrize("sign", ["1", "0"])
@pytest.mark.parametrize(
    ("absx", "answer"),
    [
        ("00000_00", "0.0"),  # Zero
        ("0000_001", "1*2**-9"),  # Min subnorm
        ("0000_010", "2*2**-9"),
        ("0000_011", "3*2**-9"),
        ("0000_100", "4*2**-9"),
        ("0000_101", "5*2**-9"),
        ("0000_110", "6*2**-9"),
        ("0000_111", "7*2**-9"),  # Max subnorm
        ("0001_000", "2**-6"),  # Min normal
        ("1110_111", "240.0"),  # Max normal
        ("1111_000", 'float("inf")'),  # Infinity
    ],
)
def test_bit_conversions_e4m3(absx, sign, answer):
    assert float(APyFloat.from_bits(int(f"{sign}_{absx}", 2), 4, 3)) == eval(
        f"{'-' if sign == '1' else ''}{answer}"
    )
    assert APyFloat.from_float(
        eval(f"{'-' if sign == '1' else ''}{answer}"), 4, 3
    ).to_bits() == int(f"{sign}_{absx}", 2)


@pytest.mark.parametrize("sign", ["1", "0"])
@pytest.mark.parametrize(
    ("absx", "answer"),
    [
        ("11111_01", 'float("nan")'),  # NaN
        ("11111_10", 'float("nan")'),  # NaN
        ("11111_11", 'float("nan")'),  # NaN
    ],
)
def test_bit_conversion_nan_e5m2(absx, sign, answer):
    assert str(float(APyFloat.from_bits(int(f"{sign}_{absx}", 2), 5, 2))) == str(
        eval(f"{'-' if sign == '1' else ''}{answer}")
    )
    bits = APyFloat.from_float(
        eval(f"{'-' if sign == '1' else ''}{answer}"), 5, 2
    ).to_bits()
    assert (bits & 0x3) != 0


def test_long_python_integer():
    val = 0xF000000FFFFFFFFFF
    a = APyFloat.from_bits(val, 27, 40)
    assert a.is_identical(APyFloat(1, 117440512, 1099511627775, 27, 40))
    assert a.to_bits() == val

    val = 1  # +0, read small Python integer with large format
    a = APyFloat.from_bits(val, 21, 50)
    assert a.is_identical(APyFloat(0, 0, 1, 21, 50))
    assert a.to_bits() == val

    val = 1 << 71  # -0
    a = APyFloat.from_bits(val, 21, 50)
    assert a.is_identical(APyFloat(1, 0, 0, 21, 50))
    assert a.to_bits() == val

    val = 0xFFFFFC000000000081  # -nan
    a = APyFloat.from_bits(val, 21, 50)
    assert a.is_identical(APyFloat(1, 2097151, 129, 21, 50))
    assert a.to_bits() == val


@pytest.mark.parametrize(
    ("apyfloat", "py_type"), [(APyFloat, float), (APyCFloat, complex)]
)
def test_convert_to_double(apyfloat: type[APyCFloat], py_type: type[complex]):
    a = apyfloat(0, 1, 1, 13, 60)
    assert py_type(a) == 0.0

    a = apyfloat(0, 8100, 1, 13, 60)
    assert not real_is_inf(a)
    assert py_type(a) == float("inf")
    assert py_type(-a) == float("-inf")

    a = apyfloat(0, 0, 7, 11, 54)
    assert py_type(a) == 1e-323

    a = apyfloat(0, 0, 1, 11, 52)
    assert py_type(a) == 5e-324


@pytest.mark.parametrize("apyfloat", [APyFloat, APyCFloat])
def test_from_float(apyfloat: type[APyCFloat]):
    a = apyfloat(0, 0, 1, 11, 52)
    b = apyfloat.from_float(5e-324, 11, 52)
    assert b.is_identical(a)

    # Big float should become infinity
    a = apyfloat.from_float(-(2**10), 4, 15)
    assert a.is_identical(apyfloat(1, 15, 0, 4, 15))


@pytest.mark.parametrize("apyfloat", [APyFloat])
def test_from_float_with_non_floats(apyfloat: type[APyCFloat]):
    a = apyfloat.from_float(16, exp_bits=4, man_bits=2)
    assert a.is_identical(apyfloat(sign=0, exp=11, man=0, exp_bits=4, man_bits=2))

    # Should quantize to 16.0
    a = apyfloat.from_float(17, exp_bits=4, man_bits=2)
    assert a.is_identical(apyfloat(sign=0, exp=11, man=0, exp_bits=4, man_bits=2))

    # Should quantize to -20.0
    a = apyfloat.from_float(-19, exp_bits=4, man_bits=2)
    assert a.is_identical(apyfloat(sign=1, exp=11, man=1, exp_bits=4, man_bits=2))

    # Tie break, should quantize to 16.0
    a = apyfloat.from_float(18, exp_bits=4, man_bits=2)
    assert a.is_identical(apyfloat(sign=0, exp=11, man=0, exp_bits=4, man_bits=2))

    # Tie break, should quantize to 24.0
    a = apyfloat.from_float(22, exp_bits=4, man_bits=2)
    assert a.is_identical(apyfloat(sign=0, exp=11, man=2, exp_bits=4, man_bits=2))

    # Should quantize to 28.0
    a = apyfloat.from_float(29, exp_bits=4, man_bits=2)
    assert a.is_identical(apyfloat(sign=0, exp=11, man=3, exp_bits=4, man_bits=2))

    # Should quantize to 32.0
    a = apyfloat.from_float(31, exp_bits=4, man_bits=2)
    assert a.is_identical(apyfloat(sign=0, exp=12, man=0, exp_bits=4, man_bits=2))

    # 152-bit number, should become negative infinity
    a = apyfloat.from_float(
        -0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, exp_bits=4, man_bits=2
    )
    assert a.is_identical(apyfloat(sign=1, exp=15, man=0, exp_bits=4, man_bits=2))

    # Test with bool and non IEEE-like bias
    a = apyfloat.from_float(True, exp_bits=4, man_bits=2, bias=4)
    assert a.is_identical(
        apyfloat(sign=0, exp=4, man=0, exp_bits=4, man_bits=2, bias=4)
    )

    # Test with big integer
    a = apyfloat.from_float(2**2047, exp_bits=28, man_bits=2)
    assert a.is_identical(
        apyfloat(sign=0, exp=(2047 + (2**27) - 1), man=0, exp_bits=28, man_bits=2)
    )

    # From APyFixed
    assert apyfloat.from_float(APyFixed(0, bits=3, int_bits=2), 5, 2, 8).is_identical(
        apyfloat(0, 0, 0, 5, 2, 8)
    )  # 0

    assert apyfloat.from_float(APyFixed(3, bits=3, int_bits=2), 5, 2).is_identical(
        apyfloat(0, 15, 2, 5, 2)
    )  # 1.5

    assert apyfloat.from_float(APyFixed(13, bits=4, int_bits=3), 5, 2).is_identical(
        apyfloat(1, 15, 2, 5, 2)
    )  # -1.5

    assert apyfloat.from_float(APyFixed(3, bits=3, int_bits=-1), 7, 3).is_identical(
        apyfloat(0, 60, 4, 7, 3)
    )  # 0.1875

    assert apyfloat.from_float(APyFixed(1, bits=5, int_bits=40), 5, 2).is_identical(
        apyfloat(0, 31, 0, 5, 2)
    )  # Saturate to infinity

    assert apyfloat.from_float(APyFixed(1, bits=12, int_bits=2), 4, 3).is_identical(
        apyfloat(0, 0, 0, 4, 3)
    )  # Quantize to zero

    assert apyfloat.from_float(APyFixed(1, bits=10, int_bits=2), 4, 3).is_identical(
        apyfloat(0, 0, 2, 4, 3)
    )  # Subnormal

    assert apyfloat.from_float(
        APyFixed(1, int_bits=1, frac_bits=1076), 11, 54
    ).is_identical(apyfloat(0, 0, 1, 11, 54))  # Smallest subnormal

    assert apyfloat.from_float(
        APyFixed(1, bits=2, int_bits=-1072), 11, 52
    ).is_identical(apyfloat(0, 0, 1, 11, 52))  # Smallest subnormal

    assert apyfloat.from_float(
        APyFixed(2**11 - 1, int_bits=1025, frac_bits=25),
        5,
        10,  # Should round to smallest normal
    ).is_identical(apyfloat(0, 1, 0, 5, 10))

    # From apyfloat, which is equivalent to a cast with TIES_EVEN

    # -0 to -0
    assert apyfloat.from_float(apyfloat(1, 0, 0, 5, 7), 10, 24, 8).is_identical(
        apyfloat(1, 0, 0, 10, 24, 8)
    )

    # Cast -inf to -inf
    assert apyfloat.from_float(apyfloat(1, 31, 0, 5, 7), 11, 34).is_identical(
        apyfloat(1, 2047, 0, 11, 34)
    )

    # Cast from big number becomes infinity
    assert apyfloat.from_float(apyfloat(1, 30, 4, 5, 3), 4, 3).is_identical(
        apyfloat(1, 15, 0, 4, 3)
    )

    # Cast where result becomes subnormal
    assert apyfloat.from_float(apyfloat(0, 8, 0, 5, 2), 4, 3).is_identical(
        apyfloat(0, 0, 4, 4, 3)
    )


def test_cast_special_cases():
    # Only change the number of exponent bits
    a = APyFloat(1, 15, 4, 5, 3)
    a = a.cast(4, 3)
    assert a.is_identical(APyFloat(1, 7, 4, 4, 3))

    # Only change the number of mantissa bits
    a = APyFloat(1, 15, 4, 5, 3)
    a = a.cast(4, 2)
    assert a.is_identical(APyFloat(1, 7, 2, 4, 2))

    # Cast from big number becomes infinity
    a = APyFloat(1, 30, 4, 5, 3)
    a = a.cast(4, 3)
    assert a.is_identical(APyFloat(1, 15, 0, 4, 3))

    # Cast from small number becomes zero (for TIES_EVEN)
    a = APyFloat(0, 0, 1, 5, 3)
    a = a.cast(4, 3)
    assert a.is_identical(APyFloat(0, 0, 0, 4, 3))


def test_casting_special_numbers():
    # Cast -0 to -0
    a = APyFloat(1, 0, 0, 5, 7)
    a = a.cast(10, 34)
    assert a.is_identical(APyFloat(1, 0, 0, 10, 34))

    # Cast -inf to -inf
    a = APyFloat(1, 31, 0, 5, 7)
    a = a.cast(11, 34)
    assert a.is_identical(APyFloat(1, 2047, 0, 11, 34))

    # Cast NaN to NaN
    a = APyFloat(0, 31, 123, 5, 7)
    a = a.cast(11, 34)
    assert a.is_nan
    assert a.exp_bits == 11
    assert a.man_bits == 34

    # Cast subnormal to normal
    a = APyFloat(0, 0, 1, 4, 3)
    a = a.cast(5, 3)
    assert a.is_identical(APyFloat(0, 6, 0, 5, 3))

    # Cast normal to subnormal
    a = APyFloat(0, 6, 0, 5, 3)
    a = a.cast(4, 3)
    assert a.is_identical(APyFloat(0, 0, 1, 4, 3))


def test_properties():
    a = APyFloat(0, 3, 4, 4, 6)
    assert a.man == 4
    assert a.true_man == 68
    assert a.exp == 3
    assert a.bias == 7
    assert a.true_exp == -4
    assert not a.sign
    assert a.true_sign == 1
    assert a.bits == 11

    a = APyFloat(1, 0, 4, 4, 6)
    assert a.man == 4
    assert a.true_man == 4
    assert a.exp == 0
    assert a.bias == 7
    assert a.true_exp == -6
    assert a.sign
    assert a.true_sign == -1
    assert a.bits == 11

    # Subnormal number
    a = APyFloat(0, 0, 1, 5, 2)
    assert a.is_finite
    assert not a.is_normal
    assert a.is_subnormal
    assert not a.is_nan
    assert not a.is_inf
    assert not a.is_zero

    # Normal number
    a = APyFloat(0, 1, 1, 5, 2)
    assert a.is_finite
    assert a.is_normal
    assert not a.is_subnormal
    assert not a.is_nan
    assert not a.is_inf
    assert not a.is_zero

    # Infinity
    a = APyFloat(0, 31, 0, 5, 2)
    assert not a.is_finite
    assert not a.is_normal
    assert not a.is_subnormal
    assert not a.is_nan
    assert a.is_inf
    assert not a.is_zero

    # NaN
    a = APyFloat(0, 31, 1, 5, 2)
    assert not a.is_finite
    assert not a.is_normal
    assert not a.is_subnormal
    assert a.is_nan
    assert not a.is_inf
    assert not a.is_zero

    # Zero
    a = APyFloat(0, 0, 0, 5, 2)
    assert a.is_finite
    assert not a.is_normal
    assert not a.is_subnormal
    assert not a.is_nan
    assert not a.is_inf
    assert a.is_zero


def test_round_trip_conversion():
    for exp in reversed(range(15)):  # Skip nan
        for man in reversed(range(16)):
            a = APyFloat(0, exp, man, 4, 4)
            assert (APyFloat.from_float(float(a), 4, 4)).is_identical(a)


@pytest.mark.parametrize(
    ("apyfloat", "test_exp"),
    [
        # First some general tests
        (APyFloat(0, 0, 1, 4, 3), APyFloat(0, 0, 2, 4, 3)),
        (APyFloat(0, 5, 2, 4, 3), APyFloat(0, 5, 3, 4, 3)),
        # Mantissa overflow
        (APyFloat(0, 0, 7, 4, 3), APyFloat(0, 1, 0, 4, 3)),
        (APyFloat(0, 5, 7, 4, 3), APyFloat(0, 6, 0, 4, 3)),
        # Special numbers
        (APyFloat(0, 15, 0, 4, 3), APyFloat(0, 15, 0, 4, 3)),  # inf
        (APyFloat(0, 15, 1, 4, 3), APyFloat(0, 15, 1, 4, 3)),  # nan
        (APyFloat(0, 0, 0, 4, 3), APyFloat(0, 0, 1, 4, 3)),  # +0
        # General tests for negative numbers
        (APyFloat(1, 0, 2, 4, 3), APyFloat(1, 0, 1, 4, 3)),
        (APyFloat(1, 5, 2, 4, 3), APyFloat(1, 5, 1, 4, 3)),
        # Mantissa underflow
        (APyFloat(1, 1, 0, 4, 3), APyFloat(1, 0, 7, 4, 3)),
        (APyFloat(1, 5, 0, 4, 3), APyFloat(1, 4, 7, 4, 3)),
        # Special numbers
        (APyFloat(1, 15, 0, 4, 3), APyFloat(1, 14, 7, 4, 3)),  # -inf
        (APyFloat(1, 15, 1, 4, 3), APyFloat(1, 15, 1, 4, 3)),  # -nan
        (APyFloat(1, 0, 0, 4, 3), APyFloat(0, 0, 1, 4, 3)),  # -0
        (APyFloat(1, 0, 1, 4, 3), APyFloat(1, 0, 0, 4, 3)),  # Smallest negative number
    ],
)
def test_next_up(apyfloat, test_exp):
    assert (_ := apyfloat.next_up()).is_identical(test_exp)


@pytest.mark.parametrize("bit_pattern", list(range(2**7)))
def test_next_down(bit_pattern):
    """Test next_down by making use of the property: next_down(x) == -next_up(-x)."""
    apyfloat = APyFloat.from_bits(bit_pattern, 3, 3)
    neg_apyfloat = -apyfloat
    res = apyfloat.next_down()
    ref = -(neg_apyfloat.next_up())
    assert res.is_identical(ref)


def test_copy():
    a = APyFloat(0, 2, 4, 4, 5)
    b = a
    assert a is b
    c = a.copy()
    assert a is not c


def test_python_copy():
    import copy

    a = APyFloat(0, 2, 4, 4, 5)
    b = a
    assert a is b
    c = copy.copy(a)
    assert a is not c


def test_python_deepcopy():
    import copy

    a = APyFloat(0, 2, 4, 4, 5)
    b = a
    assert a is b
    c = copy.deepcopy(a)
    assert a is not c


@pytest.mark.parametrize("func", ["zero", "inf", "nan"])
def test_zero_inf_nan_creation(func: str):
    # Too many exponent bits
    with pytest.raises(
        ValueError,
        match=f"APyC?Float.{func}: exponent bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        eval(f"APyFloat.{func}(300, 5)")

    with pytest.raises(
        ValueError,
        match=f"APyC?Float.{func}: exponent bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        eval(f"APyFloat.{func}(-300, 5)")

    # Too many mantissa bits
    with pytest.raises(
        ValueError,
        match=f"APyC?Float.{func}: mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
    ):
        eval(f"APyFloat.{func}(5, 300)")

    with pytest.raises(
        ValueError,
        match=f"APyC?Float.{func}: mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
    ):
        eval(f"APyFloat.{func}(5, -300)")

    #
    # Test actual operation
    if func == "zero":
        x = eval(f"APyFloat.{func}(5, 7, 8)")
        assert x.is_identical(APyFloat(0, 0, 0, 5, 7, 8))

        x = eval(f"APyFloat.{func}(4, 3)")
        assert x.is_identical(APyFloat(0, 0, 0, 4, 3))
    else:
        x = eval(f"APyFloat.{func}(5, 7, 8)")
        assert x.is_identical(APyFloat.from_float(float(func), 5, 7, 8))

        x = eval(f"APyFloat.{func}(4, 3)")
        assert x.is_identical(APyFloat.from_float(float(func), 4, 3, 7))
