import pytest

from apytypes import fp


@pytest.mark.parametrize("a_exp_bits", [10, 15])
@pytest.mark.parametrize("a_man_bits", [10, 11, 13])
@pytest.mark.parametrize("b_exp_bits", [10, 15])
@pytest.mark.parametrize("b_man_bits", [10, 11, 13])
@pytest.mark.parametrize("b_add_val", [-4.25, 0.0, 0.25, 250.0])
def test_compare_eq(
    a_exp_bits: int,
    a_man_bits: int,
    b_exp_bits: int,
    b_man_bits: int,
    b_add_val: float,
):
    np = pytest.importorskip("numpy")
    a = fp(range(0, 100, +1), a_exp_bits, a_man_bits).reshape((10, 10))
    b = fp(range(100, 0, -1), b_exp_bits, b_man_bits).reshape((10, 10))
    b += b_add_val

    # Equality operation is commutative
    assert np.all((a == b) == (b == a))
    assert np.all((a != b) == (b != a))
    assert np.all((a != b) != (b == a))
    assert np.all((a == b) != (b != a))

    assert np.all((a == b) == (a.to_numpy() == b.to_numpy()))
    assert np.all((a != b) == (a.to_numpy() != b.to_numpy()))


@pytest.mark.parametrize("a_exp_bits", [10, 15])
@pytest.mark.parametrize("a_man_bits", [10, 11, 13])
@pytest.mark.parametrize("b_exp_bits", [10, 15])
@pytest.mark.parametrize("b_man_bits", [10, 11, 13])
@pytest.mark.parametrize("b_add_val", [-4.25, 0.0, 0.25, 250.0])
def test_compare_total_order(
    a_exp_bits: int,
    a_man_bits: int,
    b_exp_bits: int,
    b_man_bits: int,
    b_add_val: float,
):
    np = pytest.importorskip("numpy")
    a = fp(range(0, 100, +1), a_exp_bits, a_man_bits).reshape((10, 10))
    b = fp(range(100, 0, -1), b_exp_bits, b_man_bits).reshape((10, 10))
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
