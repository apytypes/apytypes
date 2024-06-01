from apytypes import APyFixed
from apytypes import OverflowMode


def test_overflow_twos_complement():
    print(float(APyFixed(0xFF, 8, 8)))
    assert float(APyFixed(0xC3, 8, 0).cast(8, 0, overflow=OverflowMode.WRAP)) == -61.0
    assert float(APyFixed(0xC3, 8, 0).cast(7, 0, overflow=OverflowMode.WRAP)) == -61.0
    assert float(APyFixed(0xC3, 8, 0).cast(6, 0, overflow=OverflowMode.WRAP)) == 3.0


def test_overflow_saturation():
    # Negative single non-full limb
    assert float(APyFixed(0xC3, 8, 0).cast(8, 0, overflow=OverflowMode.SAT)) == -61.0
    assert float(APyFixed(0xC3, 8, 0).cast(7, 0, overflow=OverflowMode.SAT)) == -61.0
    assert float(APyFixed(0xC3, 8, 0).cast(6, 0, overflow=OverflowMode.SAT)) == -32.0
    assert float(APyFixed(0xC3, 8, 0).cast(5, 0, overflow=OverflowMode.SAT)) == -16.0
    assert float(APyFixed(0xC3, 8, 0).cast(1, 0, overflow=OverflowMode.SAT)) == -1.0

    # Positive single non-full limb
    assert float(APyFixed(0x13, 8, 0).cast(8, 0, overflow=OverflowMode.SAT)) == 19.0
    assert float(APyFixed(0x13, 8, 0).cast(7, 0, overflow=OverflowMode.SAT)) == 19.0
    assert float(APyFixed(0x13, 8, 0).cast(6, 0, overflow=OverflowMode.SAT)) == 19.0
    assert float(APyFixed(0x13, 8, 0).cast(5, 0, overflow=OverflowMode.SAT)) == 15.0
    assert float(APyFixed(0x13, 8, 0).cast(1, 0, overflow=OverflowMode.SAT)) == 0.0

    # Negative multi-limb
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(68, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0xC0000000000000003, 68, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(67, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x40000000000000003, 67, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(66, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x20000000000000000, 66, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(65, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x10000000000000000, 65, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(64, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x8000000000000000, 64, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(63, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x4000000000000000, 63, 0))
    )

    # Positive multi-limb
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(68, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x10000000000000003, 68, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(67, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x10000000000000003, 67, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(66, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x10000000000000003, 66, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(65, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0xFFFFFFFFFFFFFFFF, 65, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(64, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x7FFFFFFFFFFFFFFF, 64, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(63, 0, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x3FFFFFFFFFFFFFFF, 63, 0))
    )


def test_overflow_numeric_std():
    # Negative single non-full limb
    assert (
        float(APyFixed(0xC3, 8, 0).cast(8, 0, overflow=OverflowMode.NUMERIC_STD))
        == -61.0
    )
    assert (
        float(APyFixed(0xC3, 8, 0).cast(7, 0, overflow=OverflowMode.NUMERIC_STD))
        == -61.0
    )
    assert (
        float(APyFixed(0xC3, 8, 0).cast(6, 0, overflow=OverflowMode.NUMERIC_STD))
        == -29.0
    )
    assert (
        float(APyFixed(0xC3, 8, 0).cast(5, 0, overflow=OverflowMode.NUMERIC_STD))
        == -13.0
    )
    assert (
        float(APyFixed(0xC3, 8, 0).cast(1, 0, overflow=OverflowMode.NUMERIC_STD))
        == -1.0
    )

    # Positive single non-full limb
    assert (
        float(APyFixed(0x13, 8, 0).cast(8, 0, overflow=OverflowMode.NUMERIC_STD))
        == 19.0
    )
    assert (
        float(APyFixed(0x13, 8, 0).cast(7, 0, overflow=OverflowMode.NUMERIC_STD))
        == 19.0
    )
    assert (
        float(APyFixed(0x13, 8, 0).cast(6, 0, overflow=OverflowMode.NUMERIC_STD))
        == 19.0
    )
    assert (
        float(APyFixed(0x13, 8, 0).cast(5, 0, overflow=OverflowMode.NUMERIC_STD)) == 3.0
    )
    assert (
        float(APyFixed(0x13, 8, 0).cast(4, 0, overflow=OverflowMode.NUMERIC_STD)) == 3.0
    )
    assert (
        float(APyFixed(0x13, 8, 0).cast(2, 0, overflow=OverflowMode.NUMERIC_STD)) == 1.0
    )
    assert (
        float(APyFixed(0x13, 8, 0).cast(1, 0, overflow=OverflowMode.NUMERIC_STD)) == 0.0
    )

    # Negative multi-limb
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(68, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0xC0000000000000003, 68, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(67, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x40000000000000003, 67, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(66, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x20000000000000003, 66, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(65, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x10000000000000003, 65, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(64, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x8000000000000003, 64, 0))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 0)
        .cast(63, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x4000000000000003, 63, 0))
    )

    # Positive multi-limb
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(68, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x10000000000000003, 68, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(67, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x10000000000000003, 67, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(66, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x10000000000000003, 66, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(65, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x03, 65, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(64, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x03, 64, 0))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 0)
        .cast(63, 0, overflow=OverflowMode.NUMERIC_STD)
        .is_identical(APyFixed(0x03, 63, 0))
    )
