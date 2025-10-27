import pytest

from apytypes import APyCFixedArray


@pytest.mark.parametrize("a_int_bits", [10, 15])
@pytest.mark.parametrize("a_frac_bits", [10, 11, 13])
@pytest.mark.parametrize("b_int_bits", [10, 15])
@pytest.mark.parametrize("b_frac_bits", [10, 11, 13])
@pytest.mark.parametrize("b_add_val", [-4.25, 0.0, 0.25, 250.0, 4.25j, 1.0 + 1.0j])
def test_compare_eq(
    a_int_bits: int,
    a_frac_bits: int,
    b_int_bits: int,
    b_frac_bits: int,
    b_add_val: complex,
):
    np = pytest.importorskip("numpy")
    range_a = [(a, a) for a in range(0, 100, +1)]
    range_b = [(b, b) for b in range(100, 0, -1)]
    a = APyCFixedArray(range_a, a_int_bits, a_frac_bits).reshape((10, 10))
    b = APyCFixedArray(range_b, b_int_bits, b_frac_bits).reshape((10, 10))
    b += b_add_val

    # Equality operation is commutative
    assert np.all((a == b) == (b == a))
    assert np.all((a != b) == (b != a))
    assert np.all((a != b) != (b == a))
    assert np.all((a == b) != (b != a))

    assert np.all((a == b) == (a.to_numpy() == b.to_numpy()))
    assert np.all((a != b) == (a.to_numpy() != b.to_numpy()))
