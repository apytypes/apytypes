import pytest

from apytypes import APyFixedArray


def test_trailing_zeros():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([[1, 2, 2000], [4, 24, 0]], int_bits=1, frac_bits=10)
    assert np.all(
        a.trailing_zeros == np.array([[0, 1, 4], [2, 3, 11]], dtype=np.uint64)
    )


def test_trailing_zeros_long():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([[1, 2, 2000], [4, 24, 0]], int_bits=100, frac_bits=100)
    assert np.all(
        a.trailing_zeros == np.array([[0, 1, 4], [2, 3, 200]], dtype=np.uint64)
    )


def test_leading_zeros():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([[1, 2, 2000], [4, 24, 0]], int_bits=1, frac_bits=10)
    assert np.all(
        a.leading_zeros == np.array([[10, 9, 0], [8, 6, 11]], dtype=np.uint64)
    )


def test_leading_zeros_long():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([[1, 2, 2000], [4, 24, 0]], int_bits=100, frac_bits=100)
    assert np.all(
        a.leading_zeros == np.array([[199, 198, 189], [197, 195, 200]], dtype=np.uint64)
    )


def test_leading_ones():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([[2047, 512, 2000], [4089, 24, 0]], int_bits=1, frac_bits=10)
    assert np.all(a.leading_ones == np.array([[11, 0, 5], [8, 0, 0]], dtype=np.uint64))


def test_leading_ones_long():
    np = pytest.importorskip("numpy")
    a = APyFixedArray(
        [[1, 2**200 - 8, 2**200 - 1], [4, 2**200 - 9, 0]], int_bits=100, frac_bits=100
    )
    assert np.all(
        a.leading_ones == np.array([[0, 197, 200], [0, 196, 0]], dtype=np.uint64)
    )


def test_leading_signs():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([[1, 2, 2000], [4, 2047, 0]], int_bits=1, frac_bits=10)
    assert np.all(
        a.leading_signs == np.array([[10, 9, 5], [8, 11, 11]], dtype=np.uint64)
    )


def test_leading_signs_long():
    np = pytest.importorskip("numpy")
    a = APyFixedArray(
        [[1, 2, 2**200 - 1], [4, 2**200 - 4, 0]], int_bits=100, frac_bits=100
    )
    assert np.all(
        a.leading_signs == np.array([[199, 198, 200], [197, 198, 200]], dtype=np.uint64)
    )


def test_is_zero():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([[1, 0, 2000], [4, 24, 0]], int_bits=1, frac_bits=10)
    assert np.all(
        a.is_zero == np.array([[False, True, False], [False, False, True]], dtype=bool)
    )


def test_is_zero_long():
    np = pytest.importorskip("numpy")
    a = APyFixedArray([[1, 0, 2000], [4, 24, 0]], int_bits=100, frac_bits=100)
    assert np.all(
        a.is_zero == np.array([[False, True, False], [False, False, True]], dtype=bool)
    )
