import pytest

from apytypes import APyCFixed, APyFixed, OverflowMode


@pytest.mark.parametrize(
    ("fixed_type", "im"), [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_overflow_twos_complement(fixed_type, im):
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(8, 0, overflow=OverflowMode.WRAP)
        == -61.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(7, 0, overflow=OverflowMode.WRAP)
        == -61.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(6, 0, overflow=OverflowMode.WRAP)
        == 3.0 * im
    )


@pytest.mark.parametrize(
    ("fixed_type", "im"), [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_overflow_saturation(fixed_type, im):
    # Negative single non-full limb
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(8, 0, overflow=OverflowMode.SAT)
        == -61.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(7, 0, overflow=OverflowMode.SAT)
        == -61.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(6, 0, overflow=OverflowMode.SAT)
        == -32.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(5, 0, overflow=OverflowMode.SAT)
        == -16.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(1, 0, overflow=OverflowMode.SAT)
        == -1.0 * im
    )

    # Positive non-full limb
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(8, 0, overflow=OverflowMode.SAT)
        == 19.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(7, 0, overflow=OverflowMode.SAT)
        == 19.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(6, 0, overflow=OverflowMode.SAT)
        == 19.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(5, 0, overflow=OverflowMode.SAT)
        == 15.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(1, 0, overflow=OverflowMode.SAT)
        == 0.0 * im
    )

    # Negative multi-limb
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        68, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        67, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        66, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x20000000000000000 * im, 66, 0)
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        65, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x10000000000000000 * im, 65, 0)
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        64, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x8000000000000000 * im, 64, 0)
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        63, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x4000000000000000 * im, 63, 0)

    # Positive multi-limb
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        68, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        67, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x10000000000000000 * im, 67, 0) + 3 * im
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        66, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x10000000000000000 * im, 66, 0) + 3 * im
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        65, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x10000000000000000 * im, 66, 0) - 1 * im
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        64, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x8000000000000000 * im, 66, 0) - 1 * im
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        63, 0, overflow=OverflowMode.SAT
    ) == fixed_type.from_float(0x4000000000000000 * im, 68, 0) - 1 * im


@pytest.mark.parametrize(
    ("fixed_type", "im"), [(APyFixed, 1), (APyCFixed, 1), (APyCFixed, 1j)]
)
def test_overflow_numeric_std(fixed_type, im):
    # Negative single non-full limb
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(
            8, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == -61.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(
            7, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == -61.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(
            6, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == -29.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(
            5, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == -13.0 * im
    )
    assert (
        fixed_type.from_float(0xC3 * im, 8, 0).cast(
            1, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == -1.0 * im
    )

    # Positive single non-full limb
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(
            8, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == 19.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(
            7, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == 19.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(
            6, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == 19.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(
            5, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == 3.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(
            4, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == 3.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(
            2, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == 1.0 * im
    )
    assert (
        fixed_type.from_float(0x13 * im, 8, 0).cast(
            1, 0, overflow=OverflowMode.NUMERIC_STD
        )
        == 0.0 * im
    )

    # Negative multi-limb
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        68, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        67, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x40000000000000000 * im, 67, 0) + 3 * im
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        66, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x20000000000000000 * im, 66, 0) + 3 * im
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        65, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x10000000000000000 * im, 65, 0) + 3 * im
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        64, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x8000000000000000 * im, 64, 0) + 3 * im
    assert (fixed_type.from_float(0xC0000000000000000 * im, 68, 0) + 3 * im).cast(
        63, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x4000000000000000 * im, 63, 0) + 3 * im

    # Positive multi-limb
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        68, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        67, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x10000000000000000 * im, 67, 0) + 3 * im
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        66, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x10000000000000000 * im, 66, 0) + 3 * im
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        65, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x03 * im, 65, 0)
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        64, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x03 * im, 64, 0)
    assert (fixed_type.from_float(0x10000000000000000 * im, 68, 0) + 3 * im).cast(
        63, 0, overflow=OverflowMode.NUMERIC_STD
    ) == fixed_type.from_float(0x03 * im, 63, 0)
