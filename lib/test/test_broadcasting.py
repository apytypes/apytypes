from apytypes import APyFixedArray, APyFloatArray

import pytest
from itertools import product


def all_shapes(ndim: int, dim_elements: int):
    """
    Retrieve a list of all possible shapes with `ndim` dimensions, spanning from one to
    `dim_elements` (inclusive)
    """
    return product(range(1, dim_elements + 1), repeat=ndim)


# APyTypes array convenience generators
apytypes_from_float_generators = [
    lambda f: APyFixedArray.from_float(f, int_bits=200, frac_bits=0),  # multi-limb int
    lambda f: APyFloatArray.from_float(f, man_bits=52, exp_bits=11),  # double-precision
]


@pytest.mark.parametrize("apyarray_from_float", apytypes_from_float_generators)
def test_array_broadcast_to(apyarray_from_float):
    MAX_NDIM = 3
    MAX_DIM_ELEMENTS = 3
    np = pytest.importorskip("numpy")

    # Iterate over all possible source and destination shapes and test
    for src_ndim, dst_ndim in product(range(1, MAX_NDIM + 1), range(1, MAX_NDIM + 1)):
        src_shapes = all_shapes(src_ndim, MAX_DIM_ELEMENTS)
        dst_shapes = all_shapes(dst_ndim, MAX_DIM_ELEMENTS)
        for src_shape, dst_shape in product(src_shapes, dst_shapes):
            numpy_src = np.arange(np.prod(src_shape), dtype=float).reshape(src_shape)
            apy_src = apyarray_from_float(numpy_src)
            try:
                numpy_res = np.broadcast_to(numpy_src, dst_shape)
                apy_res = apy_src.broadcast_to(dst_shape).to_numpy()
                assert np.all(numpy_res == apy_res)
            except ValueError:
                # `ValueError` raised by `np.broadcast_to()` or `apy_src.broadcast_to()`
                # for illegal broadcast. Make sure *both* arrays raises `ValueError`
                with pytest.raises(ValueError):
                    np.broadcast_to(numpy_src, dst_shape)
                with pytest.raises(ValueError, match="Operands could not be broadcast"):
                    apy_src.broadcast_to(dst_shape)


@pytest.mark.parametrize("apyarray_from_float", apytypes_from_float_generators)
def test_array_broadcast_to_integer(apyarray_from_float):
    np = pytest.importorskip("numpy")
    assert np.all(
        apyarray_from_float([1.0]).broadcast_to(9).to_numpy() == np.ones((9,))
    )


@pytest.mark.parametrize("apyarray_from_float", apytypes_from_float_generators)
def test_array_broadcast_to_raises(apyarray_from_float):
    # Can not broadcast if any of the destination dimensions are zero
    with pytest.raises(ValueError, match="Operands could not be broadcast together"):
        apyarray_from_float([1.0]).broadcast_to((3, 2, 1, 0, 1))

    # Can not broadcast if the destination number-of-dimensions is zero
    with pytest.raises(ValueError, match="Operands could not be broadcast together"):
        apyarray_from_float([1.0]).broadcast_to(tuple())


@pytest.mark.parametrize(
    "a_shape, b_shape",
    [
        ((5, 1, 3, 4), (5, 5, 1, 4)),
        ((1, 3, 4), (5, 5, 1, 4)),
        ((5, 1, 3, 4), (5, 1, 4)),
    ],
)
@pytest.mark.parametrize(
    "bin_func",
    [
        lambda a, b: a + b,
        lambda a, b: a - b,
        lambda a, b: a / b,
        lambda a, b: a * b,
    ],
)
@pytest.mark.parametrize("from_float", apytypes_from_float_generators)
def test_array_broadcast_arithmetic(bin_func, from_float, a_shape, b_shape):
    np = pytest.importorskip("numpy")
    a = np.arange(1, np.prod(a_shape) + 1).reshape(a_shape)
    b = np.ones(b_shape)
    ref = bin_func(a, b)
    acc = bin_func(from_float(a), from_float(b))
    assert np.all(ref == acc)
