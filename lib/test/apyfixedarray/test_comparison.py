import pytest

from apytypes import APyCFixedArray, APyFixedArray


@pytest.mark.parametrize("a_int_bits", [10, 15])
@pytest.mark.parametrize("a_frac_bits", [10, 11, 13])
@pytest.mark.parametrize("b_int_bits", [10, 15])
@pytest.mark.parametrize("b_frac_bits", [10, 11, 13])
@pytest.mark.parametrize("b_add_val", [-4.25, 0.0, 0.25, 250.0])
@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_compare_eq(
    a_int_bits: int,
    a_frac_bits: int,
    b_int_bits: int,
    b_frac_bits: int,
    b_add_val: float,
    fixed_array: type[APyCFixedArray],
):
    np = pytest.importorskip("numpy")
    a = fixed_array(range(0, 100, +1), a_int_bits, a_frac_bits).reshape((10, 10))
    b = fixed_array(range(100, 0, -1), b_int_bits, b_frac_bits).reshape((10, 10))
    b += b_add_val

    # Equality operation is commutative
    assert np.all((a == b) == (b == a))
    assert np.all((a != b) == (b != a))
    assert np.all((a != b) != (b == a))
    assert np.all((a == b) != (b != a))

    assert np.all((a == b) == (a.to_numpy() == b.to_numpy()))
    assert np.all((a != b) == (a.to_numpy() != b.to_numpy()))


@pytest.mark.parametrize("a_int_bits", [10, 15])
@pytest.mark.parametrize("a_frac_bits", [10, 11, 13])
@pytest.mark.parametrize("b_int_bits", [10, 15])
@pytest.mark.parametrize("b_frac_bits", [10, 11, 13])
@pytest.mark.parametrize("b_add_val", [-4.25, 0.0, 0.25, 250.0])
def test_compare_total_order(
    a_int_bits: int,
    a_frac_bits: int,
    b_int_bits: int,
    b_frac_bits: int,
    b_add_val: float,
):
    np = pytest.importorskip("numpy")
    a = APyFixedArray(range(0, 100, +1), a_int_bits, a_frac_bits).reshape((10, 10))
    b = APyFixedArray(range(100, 0, -1), b_int_bits, b_frac_bits).reshape((10, 10))
    b += b_add_val

    # Operation symmetry
    assert np.all((a < b) == (b > a))
    assert np.all((a > b) == (b < a))
    assert np.all((a <= b) == (b >= a))
    assert np.all((a >= b) == (b <= a))

    assert np.all((a < b) == (a.to_numpy() < b.to_numpy()))
    assert np.all((b < a) == (b.to_numpy() < a.to_numpy()))
    assert np.all((a > b) == (a.to_numpy() > b.to_numpy()))
    assert np.all((b > a) == (b.to_numpy() > a.to_numpy()))
    assert np.all((a <= b) == (a.to_numpy() <= b.to_numpy()))
    assert np.all((b <= a) == (b.to_numpy() <= a.to_numpy()))
    assert np.all((a >= b) == (a.to_numpy() >= b.to_numpy()))
    assert np.all((b >= a) == (b.to_numpy() >= a.to_numpy()))
