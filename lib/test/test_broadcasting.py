from collections.abc import Callable, Sequence
from itertools import product
from typing import Any

import pytest

from apytypes import APyCFixedArray, APyCFloatArray, APyFixedArray, APyFloatArray
from apytypes._typing import APyArray

# pyright: reportExplicitAny=false
# pyright: reportAny=false


def all_shapes(ndim: int, dim_elements: int):
    """
    Retrieve a list of all possible shapes with `ndim` dimensions, spanning from one to
    `dim_elements` (inclusive)
    """
    return product(range(1, dim_elements + 1), repeat=ndim)


def apyfloatarray_from_float(sfp: Sequence[Any] | Any):
    return APyFloatArray.from_float(sfp, man_bits=52, exp_bits=11)


def apyfixedarray_from_float(sfp: Sequence[Any] | Any):
    return APyFixedArray.from_float(sfp, int_bits=200, frac_bits=0)


def apycfloatarray_from_float(sfp: Sequence[Any] | Any):
    return APyCFloatArray.from_float(sfp, man_bits=52, exp_bits=11)


def apycfixedarray_from_float(sfp: Sequence[Any] | Any):
    return APyCFixedArray.from_float(sfp, int_bits=200, frac_bits=0)


# APyTypes array convenience generators
apy_from_float_generators = (
    apyfixedarray_from_float,
    apyfloatarray_from_float,
    apycfixedarray_from_float,
    apycfloatarray_from_float,
)


@pytest.mark.parametrize("apy_from_float", apy_from_float_generators)
def test_array_broadcast_to(apy_from_float: Callable[[Sequence[Any]], APyArray]):
    MAX_NDIM = 3
    MAX_DIM_ELEMENTS = 3
    np = pytest.importorskip("numpy")

    # Iterate over all possible source and destination shapes and test
    for src_ndim, dst_ndim in product(range(1, MAX_NDIM + 1), range(1, MAX_NDIM + 1)):
        src_shapes = all_shapes(src_ndim, MAX_DIM_ELEMENTS)
        dst_shapes = all_shapes(dst_ndim, MAX_DIM_ELEMENTS)
        for src_shape, dst_shape in product(src_shapes, dst_shapes):
            numpy_src = np.arange(np.prod(src_shape), dtype=float).reshape(src_shape)
            apy_src = apy_from_float(numpy_src)
            try:
                numpy_res = np.broadcast_to(numpy_src, dst_shape)
                apy_res = apy_src.broadcast_to(dst_shape).to_numpy()
                assert np.all(numpy_res == apy_res)
            except ValueError:
                # `ValueError` raised by `np.broadcast_to()` or `apy_src.broadcast_to()`
                # for illegal broadcast. Make sure *both* arrays raises `ValueError`
                with pytest.raises(ValueError):  # noqa: PT011
                    _ = np.broadcast_to(numpy_src, dst_shape)
                with pytest.raises(ValueError, match="Operands could not be broadcast"):
                    _ = apy_src.broadcast_to(dst_shape)


@pytest.mark.parametrize("apy_from_float", apy_from_float_generators)
def test_array_broadcast_to_integer(
    apy_from_float: Callable[[Sequence[Any]], APyArray],
):
    np = pytest.importorskip("numpy")
    assert np.all(apy_from_float([1.0]).broadcast_to(9).to_numpy() == np.ones((9,)))


@pytest.mark.parametrize("apy_from_float", apy_from_float_generators)
def test_array_broadcast_to_raises(apy_from_float: Callable[[Sequence[Any]], APyArray]):
    # Can not broadcast if any of the destination dimensions are zero
    # Cannot broadcast if any of the destination dimensions are zero
    with pytest.raises(ValueError, match="Operands could not be broadcast together"):
        _ = apy_from_float([1.0]).broadcast_to((3, 2, 1, 0, 1))

    # Cannot broadcast if the destination number-of-dimensions is zero
    with pytest.raises(ValueError, match="Operands could not be broadcast together"):
        _ = apy_from_float([1.0]).broadcast_to(())


@pytest.mark.parametrize(
    ("a_shape", "b_shape"),
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
@pytest.mark.parametrize("apy_from_float", apy_from_float_generators)
def test_array_broadcast_arithmetic(
    bin_func: Callable[[Any, Any], Any],
    apy_from_float: Callable[[Sequence[Any]], APyArray],
    a_shape: tuple[int, ...],
    b_shape: tuple[int, ...],
):
    np = pytest.importorskip("numpy")
    a = np.arange(1, np.prod(a_shape) + 1).reshape(a_shape)
    b = np.ones(b_shape)
    ref = bin_func(a, b)
    acc = bin_func(apy_from_float(a), apy_from_float(b))
    assert np.all(ref == acc)
