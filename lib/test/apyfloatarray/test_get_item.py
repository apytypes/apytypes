from apytypes import APyFloatArray
from apytypes import APyFloat

import pytest


def test_raise_invalid_item():
    fp_array = APyFloatArray.from_float([[1, 2, 3], [4, 5, 6]], exp_bits=8, man_bits=23)
    with pytest.raises(ValueError, match=r"APyFloatArray\.__getitem__: supported keys"):
        fp_array[1, 2.5]


def test_get_item_integer():
    # ndim == 1
    fp_array = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=8, man_bits=23)
    assert fp_array[0].is_identical(APyFloat.from_float(1, exp_bits=8, man_bits=23))
    print(fp_array[1])
    print(APyFloat.from_float(2, exp_bits=8, man_bits=23))
    assert fp_array[1].is_identical(APyFloat.from_float(2, exp_bits=8, man_bits=23))
    assert fp_array[5].is_identical(APyFloat.from_float(6, exp_bits=8, man_bits=23))
    with pytest.raises(IndexError, match="APyFloatArray.__getitem__: index 6 is out"):
        fp_array[6]

    # ndim == 2
    fp_array = APyFloatArray.from_float([[1, 2], [3, 4]], exp_bits=8, man_bits=23)
    assert fp_array[0].is_identical(
        APyFloatArray.from_float([1, 2], exp_bits=8, man_bits=23)
    )
    assert fp_array[1].is_identical(
        APyFloatArray.from_float([3, 4], exp_bits=8, man_bits=23)
    )
    with pytest.raises(IndexError, match="APyFloatArray.__getitem__: index 2 is out"):
        fp_array[2]

    # ndim == 3
    fp_array = APyFloatArray.from_float(
        [[[1, 2], [3, 4]], [[5, 6], [7, 8]]], exp_bits=8, man_bits=23
    )
    assert fp_array[0].is_identical(
        APyFloatArray.from_float([[1, 2], [3, 4]], exp_bits=8, man_bits=23)
    )
    assert fp_array[1].is_identical(
        APyFloatArray.from_float([[5, 6], [7, 8]], exp_bits=8, man_bits=23)
    )
    with pytest.raises(IndexError, match="APyFloatArray.__getitem__: index 2 is out"):
        fp_array[2]


def test_get_item_slice():
    # ndim == 1
    fp = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=8, man_bits=23)
    assert fp[:].is_identical(fp)
    assert fp[4:].is_identical(
        APyFloatArray.from_float([5, 6], exp_bits=8, man_bits=23)
    )
    assert fp[:5].is_identical(
        APyFloatArray.from_float([1, 2, 3, 4, 5], exp_bits=8, man_bits=23)
    )
    assert fp[0:1].is_identical(APyFloatArray.from_float([1], exp_bits=8, man_bits=23))
    assert fp[0:2].is_identical(
        APyFloatArray.from_float([1, 2], exp_bits=8, man_bits=23)
    )
    assert fp[0:3].is_identical(
        APyFloatArray.from_float([1, 2, 3], exp_bits=8, man_bits=23)
    )
    assert fp[2:6].is_identical(
        APyFloatArray.from_float([3, 4, 5, 6], exp_bits=8, man_bits=23)
    )
    assert fp[2:100].is_identical(
        APyFloatArray.from_float([3, 4, 5, 6], exp_bits=8, man_bits=23)
    )
    assert fp[0:6:2].is_identical(
        APyFloatArray.from_float([1, 3, 5], exp_bits=8, man_bits=23)
    )
    assert fp[1:6:2].is_identical(
        APyFloatArray.from_float([2, 4, 6], exp_bits=8, man_bits=23)
    )
    assert fp[1:6:3].is_identical(
        APyFloatArray.from_float([2, 5], exp_bits=8, man_bits=23)
    )
    assert fp[0:6:3].is_identical(
        APyFloatArray.from_float([1, 4], exp_bits=8, man_bits=23)
    )
    assert fp[::-1].is_identical(
        APyFloatArray.from_float([6, 5, 4, 3, 2, 1], exp_bits=8, man_bits=23)
    )
    assert fp[::-2].is_identical(
        APyFloatArray.from_float([6, 4, 2], exp_bits=8, man_bits=23)
    )
    assert fp[6::-1].is_identical(
        APyFloatArray.from_float([6, 5, 4, 3, 2, 1], exp_bits=8, man_bits=23)
    )
    assert fp[6:0:-1].is_identical(
        APyFloatArray.from_float([6, 5, 4, 3, 2], exp_bits=8, man_bits=23)
    )

    # ndim == 2
    fp = APyFloatArray.from_float(
        [[1, 2], [3, 4], [5, 6], [7, 8]], exp_bits=8, man_bits=23
    )
    assert fp[:].is_identical(fp)
    assert fp[2:].is_identical(
        APyFloatArray.from_float([[5, 6], [7, 8]], exp_bits=8, man_bits=23)
    )
    assert fp[3:].is_identical(
        APyFloatArray.from_float([[7, 8]], exp_bits=8, man_bits=23)
    )
    assert fp[:1].is_identical(
        APyFloatArray.from_float([[1, 2]], exp_bits=8, man_bits=23)
    )
    assert fp[:2].is_identical(
        APyFloatArray.from_float([[1, 2], [3, 4]], exp_bits=8, man_bits=23)
    )
    assert fp[:3].is_identical(
        APyFloatArray.from_float([[1, 2], [3, 4], [5, 6]], exp_bits=8, man_bits=23)
    )
    assert fp[1:3].is_identical(
        APyFloatArray.from_float([[3, 4], [5, 6]], exp_bits=8, man_bits=23)
    )
    assert fp[1:100].is_identical(
        APyFloatArray.from_float([[3, 4], [5, 6], [7, 8]], exp_bits=8, man_bits=23)
    )
    assert fp[0:4:2].is_identical(
        APyFloatArray.from_float([[1, 2], [5, 6]], exp_bits=8, man_bits=23)
    )
    assert fp[1:4:2].is_identical(
        APyFloatArray.from_float([[3, 4], [7, 8]], exp_bits=8, man_bits=23)
    )
    assert fp[0:4:3].is_identical(
        APyFloatArray.from_float([[1, 2], [7, 8]], exp_bits=8, man_bits=23)
    )
    assert fp[::-1].is_identical(
        APyFloatArray.from_float(
            [[7, 8], [5, 6], [3, 4], [1, 2]], exp_bits=8, man_bits=23
        )
    )
    assert fp[::-2].is_identical(
        APyFloatArray.from_float([[7, 8], [3, 4]], exp_bits=8, man_bits=23)
    )

    # ndim == 3
    fp = APyFloatArray.from_float(range(5 * 5 * 5), exp_bits=8, man_bits=23).reshape(
        (5, 5, 5)
    )
    assert fp[:].is_identical(fp)
    assert fp[2:].is_identical(
        APyFloatArray.from_float(range(50, 125), exp_bits=8, man_bits=23).reshape(
            (3, 5, 5)
        )
    )
    assert fp[3:].is_identical(
        APyFloatArray.from_float(range(75, 125), exp_bits=8, man_bits=23).reshape(
            (2, 5, 5)
        )
    )
    assert fp[3:5].is_identical(
        APyFloatArray.from_float(range(75, 125), exp_bits=8, man_bits=23).reshape(
            (2, 5, 5)
        )
    )
    assert fp[3:4].is_identical(
        APyFloatArray.from_float(range(75, 100), exp_bits=8, man_bits=23).reshape(
            (1, 5, 5)
        )
    )
    assert fp[:4].is_identical(
        APyFloatArray.from_float(range(0, 100), exp_bits=8, man_bits=23).reshape(
            (4, 5, 5)
        )
    )
    assert fp[4:].is_identical(
        APyFloatArray.from_float(range(100, 125), exp_bits=8, man_bits=23).reshape(
            (1, 5, 5)
        )
    )
    assert fp[:100].is_identical(fp)


def test_get_item_tuple():
    np = pytest.importorskip("numpy")
    np_array = np.array(range(3 * 4 * 5 * 6 * 7)).reshape((3, 4, 5, 6, 7))
    fp_array = APyFloatArray.from_float(np_array, exp_bits=20, man_bits=50)
    assert np_array.shape == fp_array.shape

    slice_tuples = [
        (2, 1, 4),
        (2, 3),
        (1,),
        (1, slice(0, 5)),
        (0, slice(1, 3)),
        (2, slice(1, 4), 4),
        (slice(0, 1), slice(0, 3), 4),
        (slice(0, 1), slice(0, 100), 4),
        (slice(1, 3), 0, slice(1, 3)),
        (slice(0, 3), 1, slice(2, 5)),
        (slice(0, 3), 2, slice(2, 5), 5, 6),
        (slice(1, 3), 2, slice(2, 5), slice(1, 6, 3), 6),
        (slice(0, 3, 2), 3, slice(2, 5), slice(2, 5, 2), slice(2, 9, 3)),
    ]
    for slice_tuple in slice_tuples:
        np_slice = np_array[slice_tuple]
        fp_slice = fp_array[slice_tuple]
        print(slice_tuple)
        assert fp_slice.shape == np_slice.shape
        assert np.all(fp_slice.to_numpy() == np_slice)

    with pytest.raises(ValueError, match=r"APyFloatArray\.__getitem__: key tuple size"):
        fp_array[1, 2, 3, 4, 5, 6]


def test_get_item_ellipsis():
    np = pytest.importorskip("numpy")
    np_array = np.array(range(3 * 4 * 5 * 6 * 7)).reshape((3, 4, 5, 6, 7))
    fp_array = APyFloatArray.from_float(np_array, exp_bits=20, man_bits=50)
    assert fp_array.shape == np_array.shape
    assert fp_array[...].is_identical(fp_array)

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
        np_slice = np_array[slice_tuple]
        fp_slice = fp_array[slice_tuple]
        assert fp_slice.shape == np_slice.shape
        assert np.all(fp_slice.to_numpy() == np_slice)

    with pytest.raises(
        ValueError, match=r"APyFloatArray\.__getitem__: only one single ellipsis"
    ):
        fp_array[..., ...]
