import pytest

from apytypes import fp, fx


@pytest.mark.parametrize("int_bits", [10, 32, 40, 64])
@pytest.mark.parametrize("frac_bits", [0, 32, 64])
@pytest.mark.parametrize(
    ("val", "conj"),
    [
        (0.0, 0.0),
        (1j, -1j),
        (-1j, 1j),
        (123.5 + 7j, 123.5 - 7j),
    ],
)
def test_fixed_conj(int_bits: int, frac_bits: int, val: complex, conj: complex):
    dat = fx(val, int_bits=int_bits, frac_bits=frac_bits, force_complex=True)
    ref = fx(conj, int_bits=int_bits + 1, frac_bits=frac_bits, force_complex=True)
    assert dat.conj().is_identical(ref)
    assert dat.conj() == ref
    assert ref.conj() == dat


@pytest.mark.parametrize("exp_bits", [3, 10])
@pytest.mark.parametrize("man_bits", [10, 30, 60])
@pytest.mark.parametrize(
    ("val", "conj"),
    [
        (0.0, 0.0),
        (1j, -1j),
        (-1j, 1j),
        (123.5 + 7j, 123.5 - 7j),
    ],
)
def test_float_conj(exp_bits: int, man_bits: int, val: complex, conj: complex):
    dat = fp(val, exp_bits=exp_bits, man_bits=man_bits, force_complex=True)
    ref = fp(conj, exp_bits=exp_bits, man_bits=man_bits, force_complex=True)
    assert dat.conj().is_identical(ref, ignore_zero_sign=True)
    assert ref.conj().is_identical(dat, ignore_zero_sign=True)


@pytest.mark.parametrize("int_bits", [10, 32, 40, 64])
@pytest.mark.parametrize("frac_bits", [0, 32, 64])
@pytest.mark.parametrize(
    ("val", "conj"),
    [
        ([0.0, 1j, -1j, 2.0, 1.0 + 2.5j], [0.0, -1j, 1j, 2.0, 1.0 - 2.5j]),
    ],
)
def test_fixedarray_conj(
    int_bits: int, frac_bits: int, val: list[complex], conj: list[complex]
):
    np = pytest.importorskip("numpy")
    dat = fx(val, int_bits=int_bits, frac_bits=frac_bits, force_complex=True)
    ref = fx(conj, int_bits=int_bits + 1, frac_bits=frac_bits, force_complex=True)
    assert dat.conj().is_identical(ref)
    assert np.all(dat.conj() == ref)
    assert np.all(ref.conj() == dat)


@pytest.mark.parametrize("exp_bits", [3, 10])
@pytest.mark.parametrize("man_bits", [10, 30, 60])
@pytest.mark.parametrize(
    ("val", "conj"),
    [
        ([0.0, 1j, -1j, 2.0, 1.0 + 2.5j], [0.0, -1j, 1j, 2.0, 1.0 - 2.5j]),
    ],
)
def test_floatarray_conj(
    exp_bits: int, man_bits: int, val: list[complex], conj: list[complex]
):
    dat = fp(val, exp_bits=exp_bits, man_bits=man_bits, force_complex=True)
    ref = fp(conj, exp_bits=exp_bits, man_bits=man_bits, force_complex=True)
    assert dat.conj().is_identical(ref, ignore_zero_sign=True)
    assert ref.conj().is_identical(dat, ignore_zero_sign=True)


@pytest.mark.parametrize("int_bits", [10, 32, 40, 64])
@pytest.mark.parametrize("frac_bits", [0, 32, 64])
@pytest.mark.parametrize(
    ("val"),
    [
        [[1.0 - 2j, 3 + 1.5j], [2.25, 4j]],
        [[-3, -3], [1j, 2j]],
        [[0, 0], [1, 0]],
    ],
)
def test_fixedarray_hermitian_transpose(
    int_bits: int, frac_bits: int, val: list[complex]
):
    np = pytest.importorskip("numpy")
    dat = fx(val, int_bits=int_bits, frac_bits=frac_bits, force_complex=True)
    ref = fx(val, int_bits=int_bits, frac_bits=frac_bits, force_complex=True).T.conj()
    assert dat.H.is_identical(ref)
    assert np.all(ref == dat.H)
    assert np.all(dat == ref.H)
    assert np.all(dat.hermitian_transpose() == ref)
    assert np.all(ref.hermitian_transpose() == dat)


@pytest.mark.parametrize("exp_bits", [3, 10])
@pytest.mark.parametrize("man_bits", [10, 30, 60])
@pytest.mark.parametrize(
    ("val"),
    [
        [[1.0 - 2j, 3 + 1.5j], [2.25, 4j]],
        [[-3, -3], [1j, 2j]],
        [[0, 0], [1, 0]],
    ],
)
def test_floatarray_hermitian_transpose(
    exp_bits: int, man_bits: int, val: list[complex]
):
    dat = fp(val, exp_bits=exp_bits, man_bits=man_bits, force_complex=True)
    ref = fp(val, exp_bits=exp_bits, man_bits=man_bits, force_complex=True).T.conj()
    assert dat.H.is_identical(ref)
    assert ref.H.is_identical(dat)
    assert dat.hermitian_transpose().is_identical(ref, ignore_zero_sign=True)
    assert ref.hermitian_transpose().is_identical(dat, ignore_zero_sign=True)
