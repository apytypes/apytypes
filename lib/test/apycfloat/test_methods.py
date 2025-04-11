from apytypes import APyCFloat


import pytest


def test_not_implemented():
    _ = APyCFloat.from_complex(1.0, exp_bits=10, man_bits=20).__str__(base=10)

    with pytest.raises(ValueError, match=r"APyCFloat::to_string_hex()"):
        _ = APyCFloat.from_complex(1.0, exp_bits=10, man_bits=20).__str__(base=16)
    with pytest.raises(ValueError, match=r"APyCFloat::to_string_oct()"):
        _ = APyCFloat.from_complex(1.0, exp_bits=10, man_bits=20).__str__(base=8)
    with pytest.raises(ValueError, match=r"APyCFloat::to_string\(base=17\): base is"):
        _ = APyCFloat.from_complex(1.0, exp_bits=10, man_bits=20).__str__(base=17)


def test_to_complex():
    assert (
        complex(APyCFloat((0, 0), exp=(0, 0), man=(0, 0), exp_bits=5, man_bits=5))
        == 0 + 0j
    )
    assert (
        complex(APyCFloat((1, 0), exp=(15, 0), man=(0, 0), exp_bits=5, man_bits=5))
        == -1 + 0j
    )
    assert (
        complex(APyCFloat((0, 0), exp=(15, 0), man=(8, 0), exp_bits=5, man_bits=5))
        == 1.25 + 0j
    )
    assert (
        complex(APyCFloat((1, 0), exp=(15, 0), man=(16, 0), exp_bits=5, man_bits=5))
        == -1.5 + 0j
    )
    assert (
        complex(APyCFloat((0, 1), exp=(0, 17), man=(0, 0), exp_bits=5, man_bits=5))
        == 0 - 4j
    )
    assert (
        complex(APyCFloat((0, 0), exp=(0, 17), man=(0, 2), exp_bits=5, man_bits=5))
        == 0 + 4.25j
    )
    assert (
        complex(APyCFloat((0, 1), exp=(14, 18), man=(8, 0), exp_bits=5, man_bits=5))
        == 0.625 - 8j
    )
    assert (
        complex(APyCFloat((0, 0), exp=(15, 18), man=(16, 0), exp_bits=5, man_bits=5))
        == 1.5 + 8j
    )


def test_repr():
    assert (
        repr(APyCFloat((0, 0), exp=(0, 0), man=(0, 0), exp_bits=5, man_bits=5))
        == "APyCFloat(sign=(0, 0), exp=(0, 0), man=(0, 0), exp_bits=5, man_bits=5)"
    )
    assert (
        repr(APyCFloat((1, 0), exp=(15, 0), man=(2, 2), exp_bits=7, man_bits=19))
        == "APyCFloat(sign=(1, 0), exp=(15, 0), man=(2, 2), exp_bits=7, man_bits=19)"
    )
    assert (
        repr(APyCFloat((1, 7), exp=(15, 3), man=(2, 2), exp_bits=7, man_bits=19))
        == "APyCFloat(sign=(1, 1), exp=(15, 3), man=(2, 2), exp_bits=7, man_bits=19)"
    )


def test_str():
    assert (
        str(APyCFloat((0, 0), exp=(0, 0), man=(0, 0), exp_bits=5, man_bits=5))
        == "(0+0j)"
    )
    assert (
        str(
            APyCFloat((1, 0), exp=(63, 64), man=(2**18, 2**18), exp_bits=7, man_bits=19)
        )
        == "(-1.5+3j)"
    )
    assert (
        str(
            APyCFloat((1, 7), exp=(60, 61), man=(2**17, 2**16), exp_bits=7, man_bits=19)
        )
        == "(-0.15625-0.28125j)"
    )
    assert str(APyCFloat.from_float(complex("nan+nanj"), 7, 7)) == "(nan+nanj)"
    assert str(APyCFloat.from_float(complex("nan-nanj"), 7, 7)) == "(nan+nanj)"
    assert str(APyCFloat.from_float(complex("-nan+nanj"), 7, 7)) == "(nan+nanj)"
    assert str(APyCFloat.from_float(complex("-nan-nanj"), 7, 7)) == "(nan+nanj)"
