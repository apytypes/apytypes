from apytypes import APyFixedArray, APyFloatArray

import pytest
from typing import List, Tuple
from itertools import product


def all_shapes(ndim: int, dim_elements: int) -> List[Tuple[int, ...]]:
    """
    Retrieve a list of all possible shapes with `ndim` dimensions, spanning from one to
    `dim_elements` (inclusive)
    """
    return list(product(*[range(1, dim_elements + 1) for _ in range(ndim)]))


# APyTypes array convenience generators
apytypes_from_float_generators = [
    lambda f: APyFixedArray.from_float(f, int_bits=100, frac_bits=0),
    lambda f: APyFloatArray.from_float(f, man_bits=20, exp_bits=15),
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
