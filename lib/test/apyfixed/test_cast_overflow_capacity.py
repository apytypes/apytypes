import pytest

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyFixed,
    APyFixedArray,
    OverflowMode,
    QuantizationMode,
)


@pytest.mark.parametrize(
    ("value", "src_int_bits", "src_frac_bits", "dst_frac_bits"),
    [(8, 10, 66, 2), (1000, 70, 0, 0), (8, 10, 54, 2)],
)
def test_wrap_cast_normalizes_stored_value(
    value, src_int_bits, src_frac_bits, dst_frac_bits
):
    x = APyFixed.from_float(value, int_bits=src_int_bits, frac_bits=src_frac_bits)
    a = APyFixedArray.from_float(
        [value, -value], int_bits=src_int_bits, frac_bits=src_frac_bits
    )
    y = x.cast(int_bits=4, frac_bits=dst_frac_bits, overflow=OverflowMode.WRAP)
    b = a.cast(int_bits=4, frac_bits=dst_frac_bits, overflow=OverflowMode.WRAP)

    assert y.to_bits() == 8 << dst_frac_bits
    assert float(y) == -8.0
    assert str(y) == "-8"
    assert y < 0
    assert y + APyFixed(1, int_bits=4, frac_bits=0) == -7
    assert b.is_identical(
        APyFixedArray.from_float([-8, -8], int_bits=4, frac_bits=dst_frac_bits)
    )
    assert b[0] + APyFixed(1, int_bits=4, frac_bits=0) == -7


@pytest.mark.parametrize(
    ("fixed_type", "array_type"),
    [(APyFixed, APyFixedArray), (APyCFixed, APyCFixedArray)],
)
@pytest.mark.parametrize("src_bits", [32, 64, 65, 128, 192])
@pytest.mark.parametrize("dst_bits", [1, 6, 32, 33, 64, 65, 96])
@pytest.mark.parametrize("shift", [-4, 0, 1, 4, 65])
@pytest.mark.parametrize(
    "overflow", [OverflowMode.WRAP, OverflowMode.SAT, OverflowMode.NUMERIC_STD]
)
def test_cast_overflow_matches_integer_arithmetic(
    fixed_type, array_type, src_bits, dst_bits, shift, overflow
):
    src_frac_bits = 4
    dst_frac_bits = src_frac_bits + shift
    src_limit = 1 << (src_bits - 1)
    dst_limit = 1 << (dst_bits - 1)
    threshold = 1 << max(0, dst_bits - 1 - shift)
    values = [
        0,
        1,
        -1,
        1000,
        -1000,
        src_limit - 1,
        -src_limit,
        src_limit // 2,
        -src_limit // 2,
    ]
    values += [
        value
        for value in [-threshold - 1, -threshold, threshold - 1, threshold]
        if -src_limit <= value < src_limit
    ]

    expected = []
    for value in values:
        scaled = value << shift if shift >= 0 else value >> -shift
        if overflow == OverflowMode.SAT:
            scaled = min(dst_limit - 1, max(-dst_limit, scaled))
        elif overflow == OverflowMode.NUMERIC_STD:
            scaled = (scaled & (dst_limit - 1)) | (dst_limit if scaled < 0 else 0)
        expected.append(scaled & ((1 << dst_bits) - 1))

    if fixed_type is APyCFixed:
        values = list(zip(values, reversed(values), strict=True))
        expected = list(zip(expected, reversed(expected), strict=True))

    for value, bits in zip(values, expected, strict=True):
        x = fixed_type(value, bits=src_bits, frac_bits=src_frac_bits)
        y = x.cast(
            bits=dst_bits,
            frac_bits=dst_frac_bits,
            quantization=QuantizationMode.TRN,
            overflow=overflow,
        )
        reference = fixed_type(bits, bits=dst_bits, frac_bits=dst_frac_bits)
        assert y.to_bits() == reference.to_bits()
        assert y.is_identical(reference)

    a = array_type(values, bits=src_bits, frac_bits=src_frac_bits)
    b = a.cast(
        bits=dst_bits,
        frac_bits=dst_frac_bits,
        quantization=QuantizationMode.TRN,
        overflow=overflow,
    )
    reference = array_type(expected, bits=dst_bits, frac_bits=dst_frac_bits)
    assert b.to_bits() == reference.to_bits()
    assert b.is_identical(reference)
