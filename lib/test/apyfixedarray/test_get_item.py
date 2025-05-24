import pytest

from apytypes import APyCFixed, APyCFixedArray, APyFixed, APyFixedArray


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_raise_invalid_item(fixed_array: type[APyCFixedArray]):
    fx_array = fixed_array([[1, 2, 3], [4, 5, 6]], bits=10, int_bits=10)
    with pytest.raises(
        ValueError, match=r"APyC?FixedArray\.__getitem__: supported keys"
    ):
        fx_array[1, 2.5]


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)],
)
def test_get_item_integer(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]
):
    # ndim == 1
    fx_array = fixed_array([1, 2, 3, 4, 5, 6], bits=10, int_bits=10)
    assert fx_array[0].is_identical(fixed_scalar(1, bits=10, int_bits=10))
    assert fx_array[1].is_identical(fixed_scalar(2, bits=10, int_bits=10))
    assert fx_array[5].is_identical(fixed_scalar(6, bits=10, int_bits=10))
    with pytest.raises(IndexError, match="APyC?FixedArray.__getitem__: index 6 is out"):
        fx_array[6]

    # ndim == 2
    fx_array = fixed_array([[1, 2, 3], [4, 5, 6]], bits=10, int_bits=10)
    assert fx_array[0].is_identical(fixed_array([1, 2, 3], bits=10, int_bits=10))
    assert fx_array[1].is_identical(fixed_array([4, 5, 6], bits=10, int_bits=10))
    with pytest.raises(IndexError, match="APyC?FixedArray.__getitem__: index 2 is out"):
        fx_array[2]

    # ndim == 3
    fx_array = fixed_array([[[1, 2], [3, 4]], [[5, 6], [7, 8]]], bits=10, int_bits=10)
    assert fx_array[0].is_identical(fixed_array([[1, 2], [3, 4]], bits=10, int_bits=10))
    assert fx_array[1].is_identical(fixed_array([[5, 6], [7, 8]], bits=10, int_bits=10))
    with pytest.raises(IndexError, match="APyC?FixedArray.__getitem__: index 2 is out"):
        fx_array[2]


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_get_item_slice(fixed_array: type[APyCFixedArray]):
    # ndim == 1
    fx = fixed_array([1, 2, 3, 4, 5, 6], int_bits=10, frac_bits=0)
    assert fx[:].is_identical(fx)
    assert fx[3:].is_identical(fixed_array([4, 5, 6], int_bits=10, frac_bits=0))
    assert fx[:5].is_identical(fixed_array([1, 2, 3, 4, 5], int_bits=10, frac_bits=0))
    assert fx[0:1].is_identical(fixed_array([1], int_bits=10, frac_bits=0))
    assert fx[0:3].is_identical(fixed_array([1, 2, 3], int_bits=10, frac_bits=0))
    assert fx[2:6].is_identical(fixed_array([3, 4, 5, 6], int_bits=10, frac_bits=0))
    assert fx[2:100].is_identical(fixed_array([3, 4, 5, 6], int_bits=10, frac_bits=0))
    assert fx[0:6:2].is_identical(fixed_array([1, 3, 5], int_bits=10, frac_bits=0))
    assert fx[1:6:2].is_identical(fixed_array([2, 4, 6], int_bits=10, frac_bits=0))
    assert fx[::-1].is_identical(
        fixed_array([6, 5, 4, 3, 2, 1], int_bits=10, frac_bits=0)
    )
    assert fx[::-2].is_identical(fixed_array([6, 4, 2], int_bits=10, frac_bits=0))
    assert fx[6::-1].is_identical(
        fixed_array([6, 5, 4, 3, 2, 1], int_bits=10, frac_bits=0)
    )
    assert fx[6:0:-1].is_identical(
        fixed_array([6, 5, 4, 3, 2], int_bits=10, frac_bits=0)
    )

    # ndim == 2
    fx = fixed_array(
        [[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]], bits=10, int_bits=10
    )
    assert fx[:].is_identical(fx)
    assert fx[2:].is_identical(
        fixed_array([[7, 8, 9], [10, 11, 12]], int_bits=10, frac_bits=0)
    )
    assert fx[3:].is_identical(fixed_array([[10, 11, 12]], int_bits=10, frac_bits=0))
    assert fx[:1].is_identical(fixed_array([[1, 2, 3]], int_bits=10, frac_bits=0))
    assert fx[:2].is_identical(
        fixed_array([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)
    )
    assert fx[:3].is_identical(
        fixed_array([[1, 2, 3], [4, 5, 6], [7, 8, 9]], int_bits=10, frac_bits=0)
    )
    assert fx[1:3].is_identical(
        fixed_array([[4, 5, 6], [7, 8, 9]], int_bits=10, frac_bits=0)
    )
    assert fx[1:100].is_identical(
        fixed_array([[4, 5, 6], [7, 8, 9], [10, 11, 12]], int_bits=10, frac_bits=0)
    )
    assert fx[0:4:2].is_identical(
        fixed_array([[1, 2, 3], [7, 8, 9]], int_bits=10, frac_bits=0)
    )
    assert fx[1:4:2].is_identical(
        fixed_array([[4, 5, 6], [10, 11, 12]], int_bits=10, frac_bits=0)
    )
    assert fx[0:4:3].is_identical(
        fixed_array([[1, 2, 3], [10, 11, 12]], int_bits=10, frac_bits=0)
    )
    assert fx[::-1].is_identical(
        fixed_array(
            [[10, 11, 12], [7, 8, 9], [4, 5, 6], [1, 2, 3]], int_bits=10, frac_bits=0
        )
    )
    assert fx[::-2].is_identical(
        fixed_array([[10, 11, 12], [4, 5, 6]], int_bits=10, frac_bits=0)
    )

    # ndim == 3
    fx = fixed_array(range(5 * 5 * 5), bits=10, frac_bits=0).reshape((5, 5, 5))
    assert fx[:].is_identical(fx)
    assert fx[2:].is_identical(
        fixed_array(range(50, 125), int_bits=10, frac_bits=0).reshape((3, 5, 5))
    )
    assert fx[3:].is_identical(
        fixed_array(range(75, 125), int_bits=10, frac_bits=0).reshape((2, 5, 5))
    )
    assert fx[3:5].is_identical(
        fixed_array(range(75, 125), int_bits=10, frac_bits=0).reshape((2, 5, 5))
    )
    assert fx[3:4].is_identical(
        fixed_array(range(75, 100), int_bits=10, frac_bits=0).reshape((1, 5, 5))
    )
    assert fx[:4].is_identical(
        fixed_array(range(0, 100), int_bits=10, frac_bits=0).reshape((4, 5, 5))
    )
    assert fx[4:].is_identical(
        fixed_array(range(100, 125), int_bits=10, frac_bits=0).reshape((1, 5, 5))
    )
    assert fx[:100].is_identical(fx)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_get_item_tuple(fixed_array: type[APyCFixedArray]):
    np = pytest.importorskip("numpy")
    np_array = np.array(range(3 * 4 * 5 * 6 * 7)).reshape((3, 4, 5, 6, 7))
    fx_array = fixed_array.from_float(np_array, int_bits=100, frac_bits=0)
    assert np_array.shape == fx_array.shape

    slice_tuples = [
        (),
        (2, 1, 4),
        (2, -3),
        (1,),
        (1, slice(0, 5)),
        (0, slice(1, 3)),
        (2, slice(1, 4), 4),
        (slice(0, 1), slice(0, 3), 4),
        (slice(0, 1), slice(0, 100), 4),
        (slice(1, 3), 0, slice(1, 3)),
        (slice(0, 3), 1, slice(2, 5)),
        (slice(0, 3), -2, slice(2, 5), 5, 6),
        (slice(1, 3), -2, slice(2, 5), slice(1, 6, 3), 6),
        (slice(0, 3, 2), 3, slice(2, 5), slice(2, 5, 2), slice(2, 9, 3)),
    ]
    for slice_tuple in slice_tuples:
        print(slice_tuple)
        np_slice = np_array[slice_tuple]
        fx_slice = fx_array[slice_tuple]
        assert fx_slice.shape == np_slice.shape
        assert np.all(fx_slice.to_numpy() == np_slice)

    with pytest.raises(
        ValueError, match=r"APyC?FixedArray\.__getitem__: key tuple size"
    ):
        fx_array[1, 2, 3, 4, 5, 6]


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_get_item_ellipsis(fixed_array: type[APyCFixedArray]):
    np = pytest.importorskip("numpy")
    np_array = np.array(range(3 * 4 * 5 * 6 * 7)).reshape((3, 4, 5, 6, 7))
    fx_array = fixed_array.from_float(np_array, int_bits=100, frac_bits=0)
    assert fx_array.shape == np_array.shape
    assert fx_array[...].is_identical(fx_array)

    ellipsis_tuples = [
        (2, ..., 4),
        (2, 3, ...),
        (..., 3),
        (1,),
        (..., slice(0, 5)),
        (0, slice(1, 3), ...),
        (2, slice(1, 4), 4, ...),
        (slice(0, 1), slice(0, 3), ...),
        (..., slice(0, 100), 4),
        (..., 0, slice(1, 3)),
        (slice(0, 3), ..., slice(2, 5)),
        (slice(0, 3), ..., slice(2, 5), 5, 6),
        (slice(1, 3), 2, ..., slice(1, 4), 6),
        (slice(3, 1, -2), 3, slice(2, 5), ..., slice(9, 2, -2)),
    ]
    for slice_tuple in ellipsis_tuples:
        print(slice_tuple)
        np_slice = np_array[slice_tuple]
        fx_slice = fx_array[slice_tuple]
        assert fx_slice.shape == np_slice.shape
        assert np.all(fx_slice.to_numpy() == np_slice)

    with pytest.raises(
        ValueError, match=r"APyC?FixedArray\.__getitem__: only one single ellipsis"
    ):
        fx_array[..., ...]
