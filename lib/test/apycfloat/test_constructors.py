from apytypes import APyCFloat

import pytest


def test_constructor_raises():
    # Does not throw
    _ = APyCFloat(sign=(0, 1), exp=(7, 7), man=(9, 8), man_bits=7, exp_bits=6)
    _ = APyCFloat(sign=(0,), exp=(7,), man=(9,), man_bits=7, exp_bits=6)

    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: different sized"):
        _ = APyCFloat(sign=(1,), exp=(7, 7), man=(9, 8), man_bits=7, exp_bits=6)
    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: different sized"):
        _ = APyCFloat(sign=(0, 1), exp=(7,), man=(9, 8), man_bits=7, exp_bits=6)
    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: different sized"):
        _ = APyCFloat(sign=(0, 1), exp=(7, 7), man=(9,), man_bits=7, exp_bits=6)

    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: less than one element"):
        _ = APyCFloat(sign=tuple(), exp=tuple(), man=tuple(), man_bits=7, exp_bits=6)

    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: more than two"):
        _ = APyCFloat(
            sign=(0, 1, 3), exp=(4, 4, 3), man=(3, 5, 7), man_bits=7, exp_bits=6
        )

    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: sign is non-bool"):
        _ = APyCFloat(sign=(0.0, 1), exp=(7, 7), man=(9, 8), man_bits=7, exp_bits=6)
    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: exponent is non-int"):
        _ = APyCFloat(sign=(0, 1), exp=(7.0, 7), man=(9, 8), man_bits=7, exp_bits=6)
    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: mantissa is non-int"):
        _ = APyCFloat(sign=(0, 1), exp=(7, 7), man=(9.0, 8), man_bits=7, exp_bits=6)
    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: sign is non-bool"):
        _ = APyCFloat(sign=(0, 1.0), exp=(7, 7), man=(9, 8), man_bits=7, exp_bits=6)
    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: exponent is non-int"):
        _ = APyCFloat(sign=(0, 1), exp=(7, 7.0), man=(9, 8), man_bits=7, exp_bits=6)
    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: mantissa is non-int"):
        _ = APyCFloat(sign=(0, 1), exp=(7, 7), man=(9, 8.0), man_bits=7, exp_bits=6)

    with pytest.raises(ValueError, match=r"Non supported type: "):
        _ = APyCFloat.from_complex("hello", man_bits=10, exp_bits=8)

    # These should not throw exceptions
    _ = APyCFloat(sign=(0, 0), exp=(5, 5), man=(3, 3), exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=(1, 1), exp=(5, 5), man=(3, 3), exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=(False, False), exp=(5, 5), man=(3, 3), exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=(True, True), exp=(5, 5), man=(3, 3), exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=(0, False), exp=(5, 5), man=(3, 3), exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=(1, True), exp=(5, 5), man=(3, 3), exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=(False, 0), exp=(5, 5), man=(3, 3), exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=(True, 1), exp=(5, 5), man=(3, 3), exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=(True, 1), exp=(5, 5), man=(3, 3), exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=1, exp=5, man=3, exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=0, exp=5, man=3, exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=True, exp=5, man=3, exp_bits=8, man_bits=9)
    _ = APyCFloat(sign=False, exp=5, man=3, exp_bits=8, man_bits=9)


def test_from_complex():
    assert APyCFloat.from_complex(0.0 + 0.0j, exp_bits=10, man_bits=10).is_identical(
        APyCFloat(0, 0, 0, exp_bits=10, man_bits=10)
    )
    assert APyCFloat.from_complex(1.0 + 0.0j, exp_bits=10, man_bits=10).is_identical(
        APyCFloat(0, 2**9 - 1, 0, exp_bits=10, man_bits=10)
    )
    assert APyCFloat.from_complex(1.0 - 2.0j, exp_bits=10, man_bits=10).is_identical(
        APyCFloat((0, 1), (2**9 - 1, 2**9), (0, 0), exp_bits=10, man_bits=10)
    )
    assert APyCFloat.from_complex(3.0, exp_bits=10, man_bits=8).is_identical(
        APyCFloat((0, 0), (2**9, 0), (2**7, 0), exp_bits=10, man_bits=8)
    )
    assert APyCFloat.from_complex(-4, exp_bits=10, man_bits=8).is_identical(
        APyCFloat((True, 0), (2**9 + 1, 0), (0, 0), exp_bits=10, man_bits=8)
    )
