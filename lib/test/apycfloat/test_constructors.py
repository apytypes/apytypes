import pytest

from apytypes import APyCFixed, APyCFloat, APyFixed


def test_constructor_raises():
    # Does not throw
    _ = APyCFloat(sign=(0, 1), exp=(7, 7), man=(9, 8), man_bits=7, exp_bits=6)
    _ = APyCFloat(sign=(0,), exp=(7,), man=(9,), man_bits=7, exp_bits=6)
    _ = APyCFloat((0, 1), (7, 7), (9, 8), 7, 6)
    _ = APyCFloat((0,), (7,), (9,), 7, 6)

    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: different sized"):
        _ = APyCFloat(sign=(1,), exp=(7, 7), man=(9, 8), man_bits=7, exp_bits=6)
    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: different sized"):
        _ = APyCFloat(sign=(0, 1), exp=(7,), man=(9, 8), man_bits=7, exp_bits=6)
    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: different sized"):
        _ = APyCFloat(sign=(0, 1), exp=(7, 7), man=(9,), man_bits=7, exp_bits=6)

    with pytest.raises(ValueError, match=r"APyCFloat\.__init__: less than one element"):
        _ = APyCFloat(sign=(), exp=(), man=(), man_bits=7, exp_bits=6)

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
    _ = APyCFloat((0, 0), (5, 5), (3, 3), 8, 9)
    _ = APyCFloat((1, 1), (5, 5), (3, 3), 8, 9)
    _ = APyCFloat((False, False), (5, 5), (3, 3), 8, 9)
    _ = APyCFloat((True, True), (5, 5), (3, 3), 8, 9)
    _ = APyCFloat((0, False), (5, 5), (3, 3), 8, 9)
    _ = APyCFloat((1, True), (5, 5), (3, 3), 8, 9)
    _ = APyCFloat((False, 0), (5, 5), (3, 3), 8, 9)
    _ = APyCFloat((True, 1), (5, 5), (3, 3), 8, 9)
    _ = APyCFloat((True, 1), (5, 5), (3, 3), 8, 9)
    _ = APyCFloat(1, 5, 3, 8, 9)
    _ = APyCFloat(0, 5, 3, 8, 9)
    _ = APyCFloat(True, 5, 3, 8, 9)
    _ = APyCFloat(False, 5, 3, 8, 9)


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
    assert APyCFloat.from_complex(
        APyFixed.from_float(-4, int_bits=10, frac_bits=8), exp_bits=10, man_bits=8
    ).is_identical(APyCFloat((True, 0), (2**9 + 1, 0), (0, 0), exp_bits=10, man_bits=8))
    assert APyCFloat.from_complex(
        APyCFixed.from_complex(-4, int_bits=10, frac_bits=8), exp_bits=10, man_bits=8
    ).is_identical(APyCFloat((True, 0), (2**9 + 1, 0), (0, 0), exp_bits=10, man_bits=8))

    np = pytest.importorskip("numpy")
    types = [
        np.complex64,
        np.complex128,
    ]
    for t in types:
        a = APyCFloat.from_float(t(923.75), exp_bits=11, man_bits=52)
        assert a.is_identical(APyCFloat.from_complex(923.75, exp_bits=11, man_bits=52))


def test_from_float():
    np = pytest.importorskip("numpy")
    types = [
        np.float16,
        np.float32,
        np.float64,
        np.int8,
        np.int16,
        np.int32,
        np.int64,
        np.uint8,
        np.uint16,
        np.uint32,
        np.uint64,
    ]
    for t in types:
        a = APyCFloat.from_float(t(125), exp_bits=11, man_bits=52)
        assert a.is_identical(APyCFloat.from_complex(125, exp_bits=11, man_bits=52))
