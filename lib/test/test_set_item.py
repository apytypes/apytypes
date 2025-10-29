from itertools import product
from math import prod

import pytest

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyCFloat,
    APyCFloatArray,
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
)


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
        (APyFloatArray, APyFloat),
        (APyCFloatArray, APyCFloat),
    ],
)
def test_raise_invalid_item(APyArray: type[APyCFixedArray], APyScalar: type[APyCFixed]):
    array = APyArray.from_float([[1, 2, 3], [4, 5, 6]], 10, 10)
    with pytest.raises(
        ValueError, match=r"APyC?(Fixed|Float)Array\.__setitem__: supported keys"
    ):
        array[1, 2.5] = APyScalar.from_float(3.0, 10, 10)


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
        (APyFloatArray, APyFloat),
        (APyCFloatArray, APyCFloat),
    ],
)
def test_raises_same_type(APyArray: type[APyCFixedArray], APyScalar: type[APyCFixed]):
    array = APyArray.from_float([[1, 2, 3], [4, 5, 6]], 10, 10)
    with pytest.raises(
        ValueError, match=r"APyC?(Fixed|Float)Array\.__setitem__: `val` has different"
    ):
        array[0] = APyScalar.from_float(0, 11, 10)

    with pytest.raises(
        ValueError, match=r"APyC?(Fixed|Float)Array\.__setitem__: `val` has different"
    ):
        array[0] = APyArray.from_float([0], 11, 10)


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
        (APyFloatArray, APyFloat),
        (APyCFloatArray, APyCFloat),
    ],
)
def test_set_item_integer(APyArray: type[APyCFixedArray], APyScalar: type[APyCFixed]):
    # ndim == 1
    array = APyArray.from_float([1, 2, 3, 4, 5, 6], 10, 10)

    array[0] = APyScalar.from_float(10, 10, 10)
    assert array.is_identical(APyArray.from_float([10, 2, 3, 4, 5, 6], 10, 10))
    array[5] = APyScalar.from_float(15, 10, 10)
    assert array.is_identical(APyArray.from_float([10, 2, 3, 4, 5, 15], 10, 10))
    array[3] = APyScalar.from_float(20, 10, 10)
    assert array.is_identical(APyArray.from_float([10, 2, 3, 20, 5, 15], 10, 10))

    with pytest.raises(IndexError, match=r"APyC?(Fixed|Float)Array\.__setitem__"):
        array[6] = APyScalar.from_float(15, 10, 10)

    # ndim == 2
    array = APyArray.from_float([[1, 2], [3, 4]], 10, 10)

    array[0] = APyScalar.from_float(10, 10, 10)
    assert array.is_identical(APyArray.from_float([[10, 10], [3, 4]], 10, 10))
    array[1] = APyScalar.from_float(12, 10, 10)
    assert array.is_identical(APyArray.from_float([[10, 10], [12, 12]], 10, 10))
    array[0] = APyArray.from_float([5, 6], 10, 10)
    assert array.is_identical(APyArray.from_float([[5, 6], [12, 12]], 10, 10))


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
        (APyFloatArray, APyFloat),
        (APyCFloatArray, APyCFloat),
    ],
)
def test_set_item_slice(APyArray: type[APyCFixedArray], APyScalar: type[APyCFixed]):
    # ndim == 1
    array = APyArray.from_float([1, 2, 3, 4, 5, 6], 10, 10)

    array[0:2] = APyScalar.from_float(-3, 10, 10)
    assert array.is_identical(APyArray.from_float([-3, -3, 3, 4, 5, 6], 10, 10))
    array[1:4] = APyScalar.from_float(-1, 10, 10)
    assert array.is_identical(APyArray.from_float([-3, -1, -1, -1, 5, 6], 10, 10))
    array[::-1] = APyArray.from_float([1, 2, 3, 4, 5, 6], 10, 10)
    assert array.is_identical(APyArray.from_float([6, 5, 4, 3, 2, 1], 10, 10))

    # ndim == 2
    array = APyArray.from_float([[1, 2], [3, 4], [5, 6], [7, 8]], 10, 10)

    array[1:3] = APyScalar.from_float(0, 10, 10)
    assert array.is_identical(
        APyArray.from_float([[1, 2], [0, 0], [0, 0], [7, 8]], 10, 10)
    )

    array[::-1] = APyArray.from_float([4, 4], 10, 10)
    assert array.is_identical(
        APyArray.from_float([[4, 4], [4, 4], [4, 4], [4, 4]], 10, 10)
    )


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
        (APyFloatArray, APyFloat),
        (APyCFloatArray, APyCFloat),
    ],
)
def test_set_item_single_ellipsis(
    APyArray: type[APyCFixedArray], APyScalar: type[APyCFixed]
):
    # ndim == 1
    array = APyArray.from_float([1, 2, 3, 4, 5, 6], 10, 10)

    array[...] = APyScalar.from_float(123, 10, 10)
    assert array.is_identical(
        APyArray.from_float([123, 123, 123, 123, 123, 123], 10, 10)
    )
    array[...] = APyArray.from_float([0, 1, 2, 3, 4, 5], 10, 10)
    assert array.is_identical(APyArray.from_float([0, 1, 2, 3, 4, 5], 10, 10))


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
        (APyFloatArray, APyFloat),
        (APyCFloatArray, APyCFloat),
    ],
)
def test_set_item_tuple(APyArray: type[APyCFixedArray], APyScalar: type[APyCFixed]):
    np = pytest.importorskip("numpy")
    np_array = np.array(range(3 * 4 * 5 * 6 * 7)).reshape((3, 4, 5, 6, 7))
    ap_array = APyArray.from_float(np_array, 25, 25)
    assert ap_array.shape == np_array.shape
    assert ap_array[...].is_identical(ap_array)

    ap_array[2, ..., 4] = APyScalar.from_float(10, 25, 25)
    np_array[2, ..., 4] = 10
    assert np.all(ap_array.to_numpy() == np_array)

    np_array[..., 1, 4, 3] = np.array(range(4))
    ap_array[..., 1, 4, 3] = APyArray.from_float(range(4), 25, 25)
    assert np.all(ap_array.to_numpy() == np_array)

    np_array[::-1, ..., 2, 1] = np.array(range(3 * 4 * 5)).reshape((3, 4, 5))
    ap_array[::-1, ..., 2, 1] = APyArray.from_float(
        np.array(range(3 * 4 * 5)).reshape((3, 4, 5)),
        25,
        25,
    )
    assert np.all(ap_array.to_numpy() == np_array)

    np_array[2, 1, 6:1:-2, 1:6:2] = np.array(range(2 * 3 * 7)).reshape((2, 3, 7))
    ap_array[2, 1, 6:1:-2, 1:6:2] = APyArray.from_float(
        np.array(range(2 * 3 * 7)).reshape((2, 3, 7)),
        25,
        25,
    )
    assert np.all(ap_array.to_numpy() == np_array)


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_set_item_tuple_multi_limb(
    APyArray: type[APyCFixedArray], APyScalar: type[APyCFixed]
):
    np = pytest.importorskip("numpy")
    np_array = np.array(range(3 * 4 * 5 * 6 * 7)).reshape((3, 4, 5, 6, 7))
    ap_array = APyArray.from_float(np_array, 500, 25)
    assert ap_array.shape == np_array.shape
    assert ap_array[...].is_identical(ap_array)

    ap_array[2, ..., 4] = APyScalar.from_float(10, 500, 25)
    np_array[2, ..., 4] = 10
    assert np.all(ap_array.to_numpy() == np_array)

    np_array[..., 1, 4, 3] = np.array(range(4))
    ap_array[..., 1, 4, 3] = APyArray.from_float(range(4), 500, 25)
    assert np.all(ap_array.to_numpy() == np_array)

    np_array[::-1, ..., 2, 1] = np.array(range(3 * 4 * 5)).reshape((3, 4, 5))
    ap_array[::-1, ..., 2, 1] = APyArray.from_float(
        np.array(range(3 * 4 * 5)).reshape((3, 4, 5)),
        500,
        25,
    )
    assert np.all(ap_array.to_numpy() == np_array)

    np_array[2, 1, 6:1:-2, 1:6:2] = np.array(range(2 * 3 * 7)).reshape((2, 3, 7))
    ap_array[2, 1, 6:1:-2, 1:6:2] = APyArray.from_float(
        np.array(range(2 * 3 * 7)).reshape((2, 3, 7)),
        500,
        25,
    )
    assert np.all(ap_array.to_numpy() == np_array)


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
        (APyFloatArray, APyFloat),
        (APyCFloatArray, APyCFloat),
    ],
)
def test_set_item_not_broadcastable(
    APyArray: type[APyCFixedArray], APyScalar: type[APyCFixed]
):
    np = pytest.importorskip("numpy")
    np_array = np.array(range(3 * 4 * 5 * 6 * 7)).reshape((3, 4, 5, 6, 7))
    ap_array = APyArray.from_float(np_array, 25, 25)

    with pytest.raises(
        ValueError,
        match=r"APyC?(Fixed|Float)Array.__setitem__: `val` shape not broadcastable",
    ):
        ap_array[1, 2, 3, 4] = APyArray.from_float([1, 2], 25, 25)


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)],
)
@pytest.mark.parametrize("int_bits", [20, 200])
@pytest.mark.parametrize("shape_ndim", [1, 2, 3, 4])
@pytest.mark.parametrize("slice_ndim", [1, 2, 3, 4])
def test_set_item_ndarray_scalar(
    APyArray: type[APyCFixedArray],
    APyScalar: type[APyCFixed],
    int_bits: int,
    shape_ndim: int,
    slice_ndim: int,
):
    seed = 1337
    np = pytest.importorskip("numpy")
    rng = np.random.default_rng(seed=seed)
    MAX_DIM = 3

    for shape in product(range(1, MAX_DIM + 1), repeat=shape_ndim):
        for slice_shape in product(range(1, MAX_DIM + 1), repeat=slice_ndim):
            apy = APyArray.from_float(range(prod(shape)), int_bits, 0).reshape(shape)
            ref = np.array(range(prod(shape))).reshape(shape)
            slice = np.array(rng.binomial(1, 0.6, slice_shape), dtype=bool)
            try:
                apy[slice] = APyScalar.from_float(-1, int_bits, 0)
            except IndexError:
                with pytest.raises(IndexError):
                    ref[slice] = -1
                continue

            ref[slice] = -1
            assert apy.ndim == ref.ndim
            assert apy.shape == ref.shape
            assert np.all(apy.to_numpy() == ref)


@pytest.mark.parametrize(
    ("APyArray", "APyScalar"),
    [(APyFloatArray, APyFloat), (APyCFloatArray, APyCFloat)],
)
@pytest.mark.parametrize("shape_ndim", [1, 2, 3, 4])
@pytest.mark.parametrize("slice_ndim", [1, 2, 3, 4])
def test_set_item_ndarray_scalar_float(
    APyArray: type[APyCFloatArray],
    APyScalar: type[APyCFloat],
    shape_ndim: int,
    slice_ndim: int,
):
    seed = 1337
    np = pytest.importorskip("numpy")
    rng = np.random.default_rng(seed=seed)
    MAX_DIM = 3

    for shape in product(range(1, MAX_DIM + 1), repeat=shape_ndim):
        for slice_shape in product(range(1, MAX_DIM + 1), repeat=slice_ndim):
            apy = APyArray.from_float(range(prod(shape)), 11, 52).reshape(shape)
            ref = np.array(range(prod(shape))).reshape(shape)
            slice = np.array(rng.binomial(1, 0.6, slice_shape), dtype=bool)
            try:
                apy[slice] = APyScalar.from_float(-1, 11, 52)
            except IndexError:
                with pytest.raises(IndexError):
                    ref[slice] = -1
                continue

            ref[slice] = -1
            assert apy.ndim == ref.ndim
            assert apy.shape == ref.shape
            assert np.all(apy.to_numpy() == ref)


@pytest.mark.parametrize(("APyArray", "APyScalar"), [(APyFixedArray, APyFixed)])
@pytest.mark.parametrize("int_bits", [100])
@pytest.mark.parametrize("shape_ndim", [1, 2, 3, 4])
@pytest.mark.parametrize("slice_ndim", [1, 2, 3, 4])
@pytest.mark.parametrize("val_ndim", [1, 2, 3, 4])
def test_set_item_ndarray_array(
    APyArray: type[APyCFixedArray],
    APyScalar: type[APyCFixed],
    int_bits: int,
    shape_ndim: int,
    slice_ndim: int,
    val_ndim: int,
):
    seed = 1337
    np = pytest.importorskip("numpy")
    rng = np.random.default_rng(seed=seed)
    MAX_DIM = 3

    for shape in product(range(1, MAX_DIM + 1), repeat=shape_ndim):
        for slice_shape in product(range(1, MAX_DIM + 1), repeat=slice_ndim):
            for val_shape in product(range(1, MAX_DIM + 1), repeat=val_ndim):
                print("------------------------------------------")
                print(end="\n\n")
                ref_np = np.array(range(prod(shape))).reshape(shape)
                apy = APyArray.from_float(ref_np, int_bits, 0)
                print(f"ref_np: \n{ref_np!r}\napy: \n{apy!r}")

                val_np = -1 * np.array(range(prod(val_shape))).reshape(val_shape)
                val = APyArray.from_float(val_np, int_bits, 0).reshape(val_shape)
                print(f"val_np: \n{val_np!r}\nval: \n{val!r}")

                slice = np.array(rng.binomial(1, 0.6, slice_shape), dtype=bool)
                print(f"slice: \n{slice!r}")

                # apy[slice] = val
                try:
                    apy[slice] = val
                except IndexError:
                    # with pytest.raises(IndexError):
                    #    ref_np[slice] = val_np
                    continue
                except ValueError:
                    # with pytest.raises(ValueError):
                    #    ref_np[slice] = val_np
                    continue

                ref_np[slice] = val_np
                print(f"AFTER -- ref_np: \n{ref_np!r}\n, apy: \n{apy!r}")
                assert apy.ndim == ref_np.ndim
                assert apy.shape == ref_np.shape
                assert np.all(apy.to_numpy() == ref_np)
