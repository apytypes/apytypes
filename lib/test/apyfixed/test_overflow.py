from apytypes import APyFixed
from apytypes import OverflowMode

import pytest


def test_overflow_twos_complement():
    print(float(APyFixed(0xFF, 8, 8)))
    assert float(APyFixed(0xC3, 8, 8).cast(8, 8, overflow=OverflowMode.WRAP)) == -61.0
    assert float(APyFixed(0xC3, 8, 8).cast(7, 7, overflow=OverflowMode.WRAP)) == -61.0
    assert float(APyFixed(0xC3, 8, 8).cast(6, 6, overflow=OverflowMode.WRAP)) == 3.0


def test_overflow_saturation():
    # Negative single non-full limb
    assert float(APyFixed(0xC3, 8, 8).cast(8, 8, overflow=OverflowMode.SAT)) == -61.0
    assert float(APyFixed(0xC3, 8, 8).cast(7, 7, overflow=OverflowMode.SAT)) == -61.0
    assert float(APyFixed(0xC3, 8, 8).cast(6, 6, overflow=OverflowMode.SAT)) == -32.0
    assert float(APyFixed(0xC3, 8, 8).cast(5, 5, overflow=OverflowMode.SAT)) == -16.0
    assert float(APyFixed(0xC3, 8, 8).cast(1, 1, overflow=OverflowMode.SAT)) == -1.0

    # Positive single non-full limb
    assert float(APyFixed(0x13, 8, 8).cast(8, 8, overflow=OverflowMode.SAT)) == 19.0
    assert float(APyFixed(0x13, 8, 8).cast(7, 7, overflow=OverflowMode.SAT)) == 19.0
    assert float(APyFixed(0x13, 8, 8).cast(6, 6, overflow=OverflowMode.SAT)) == 19.0
    assert float(APyFixed(0x13, 8, 8).cast(5, 5, overflow=OverflowMode.SAT)) == 15.0
    assert float(APyFixed(0x13, 8, 8).cast(1, 1, overflow=OverflowMode.SAT)) == 0.0

    # Negative multi-limb
    assert (
        APyFixed(0xC0000000000000003, 68, 68)
        .cast(68, 68, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0xC0000000000000003, 68, 68))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 68)
        .cast(67, 67, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x40000000000000003, 67, 67))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 68)
        .cast(66, 66, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x20000000000000000, 66, 66))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 68)
        .cast(65, 65, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x10000000000000000, 65, 65))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 68)
        .cast(64, 64, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x8000000000000000, 64, 64))
    )
    assert (
        APyFixed(0xC0000000000000003, 68, 68)
        .cast(63, 63, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x4000000000000000, 63, 63))
    )

    # Positive multi-limb
    assert (
        APyFixed(0x10000000000000003, 68, 68)
        .cast(68, 68, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x10000000000000003, 68, 68))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 68)
        .cast(67, 67, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x10000000000000003, 67, 67))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 68)
        .cast(66, 66, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x10000000000000003, 66, 66))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 68)
        .cast(65, 65, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0xFFFFFFFFFFFFFFFF, 65, 65))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 68)
        .cast(64, 64, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x7FFFFFFFFFFFFFFF, 64, 64))
    )
    assert (
        APyFixed(0x10000000000000003, 68, 68)
        .cast(63, 63, overflow=OverflowMode.SAT)
        .is_identical(APyFixed(0x3FFFFFFFFFFFFFFF, 63, 63))
    )
