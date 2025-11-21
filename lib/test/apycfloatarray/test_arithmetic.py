import pytest

from apytypes import APyCFloat, APyCFloatArray, APyFloat, APyFloatArray


def test_arithmetic_with_apyfloat():
    a = APyCFloatArray.from_complex([1 + 2j, -4, 3.5 - 2j], exp_bits=7, man_bits=4)
    b = APyFloat.from_float(3.0, exp_bits=5, man_bits=10)
    assert (a + b).is_identical(
        APyCFloatArray.from_complex([4 + 2j, -1, 6.5 - 2j], exp_bits=7, man_bits=10)
    )
    assert (a - b).is_identical(
        APyCFloatArray.from_complex([-2 + 2j, -7, 0.5 - 2j], exp_bits=7, man_bits=10)
    )
    assert (a * b).is_identical(
        APyCFloatArray.from_complex([3 + 6j, -12, 10.5 - 6j], exp_bits=7, man_bits=10)
    )
    assert (a / b).is_identical(
        APyCFloatArray(
            [(0, 0), (1, 0), (0, 1)],
            [(61, 62), (63, 0), (63, 62)],
            [(341, 341), (341, 0), (171, 341)],
            exp_bits=7,
            man_bits=10,
        )
    )
    assert (b + a).is_identical(a + b)
    assert (b * a).is_identical(a * b)
    assert (b - a).is_identical(
        (-(a - b)).cast(exp_bits=7, man_bits=10), ignore_zero_sign=True
    )
    assert (b / a).is_identical(
        APyCFloatArray(
            [(0, 1), (1, 1), (0, 0)],
            [(62, 63), (62, 0), (62, 61)],
            [(205, 205), (512, 0), (300, 489)],
            exp_bits=7,
            man_bits=10,
        )
    )

    np = pytest.importorskip("numpy")
    assert np.all(a != b)
    assert np.all(b != a)
    assert not (np.all(a == b))
    assert not (np.all(b == a))


def test_arithmetic_with_apyfloatarray():
    a = APyCFloatArray.from_complex([1 + 2j, -4, 3.5 - 2j], exp_bits=7, man_bits=7)
    b = APyFloatArray.from_float([3.0, 0, 4.0], exp_bits=5, man_bits=10)
    assert (a + b).is_identical(
        APyCFloatArray.from_complex([4 + 2j, -4, 7.5 - 2j], exp_bits=7, man_bits=10)
    )
    assert (a - b).is_identical(
        APyCFloatArray.from_complex([-2 + 2j, -4, -0.5 - 2j], exp_bits=7, man_bits=10)
    )
    assert (a * b).is_identical(
        APyCFloatArray.from_complex([3 + 6j, 0, 14 - 8j], exp_bits=7, man_bits=10),
        ignore_zero_sign=True,
    )
    assert (a / b).is_identical(
        APyCFloatArray(
            [(0, 0), (1, 0), (0, 1)],
            [(61, 62), (127, 127), (62, 62)],
            [(341, 341), (0, 1), (768, 0)],
            exp_bits=7,
            man_bits=10,
        ),
        ignore_zero_sign=True,
    )
    assert (b + a).is_identical(a + b)
    assert (b * a).is_identical(a * b)
    assert (b - a).is_identical(-(a - b), ignore_zero_sign=True)
    assert (b / a).is_identical(
        APyCFloatArray(
            [(0, 1), (1, 1), (0, 0)],
            [(62, 63), (0, 0), (62, 61)],
            [(205, 205), (0, 0), (741, 993)],
            exp_bits=7,
            man_bits=10,
        )
    )

    np = pytest.importorskip("numpy")
    assert np.all(a != b)
    assert np.all(b != a)
    assert not (np.all(a == b))
    assert not (np.all(b == a))


def test_arrays_dont_crash_on_zero_div():
    """
    In APyTypes, array division-by-zero results in an undefined values. However,
    APyTypes should not crash when such a division occurs
    """
    a = APyCFloatArray.from_complex([0.0, 1.0, 2.0], exp_bits=10, man_bits=7)
    b = APyCFloatArray.from_complex([0.0, 0.0, 1.0], exp_bits=10, man_bits=7)
    _ = a / b
    _ = b / a

    a = APyCFloatArray.from_complex([0.0, 1.0, 2.0], exp_bits=10, man_bits=7)
    b = APyFloatArray.from_float([0.0, 1.0, 0.0], exp_bits=10, man_bits=7)
    _ = a / b
    _ = b / a

    a = APyCFloatArray.from_complex([0.0, 1.0, 2.0], exp_bits=10, man_bits=7)
    b = APyCFloat.from_complex(0.0, exp_bits=10, man_bits=7)
    _ = a / b
    _ = b / a

    a = APyCFloatArray.from_complex([0.0, 1.0, 2.0], exp_bits=10, man_bits=7)
    b = APyFloat.from_float(0.0, exp_bits=10, man_bits=7)
    _ = a / b
    _ = b / a
