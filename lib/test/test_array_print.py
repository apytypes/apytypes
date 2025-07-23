from typing import cast

import pytest

from apytypes import APyCFixedArray, APyCFloatArray, APyFixedArray, APyFloatArray


@pytest.mark.parametrize("array", [APyFixedArray, APyFloatArray])
@pytest.mark.parametrize("ndim", [1, 2, 3, 4])
@pytest.mark.parametrize("n", [5, 10])
@pytest.mark.parametrize("factor", [0, 1, -1 / 2 - 1 / 4])
def test_array_print_real(array: type[APyFixedArray], ndim: int, n: int, factor: float):
    vals = [k * factor for k in range(1, n**ndim + 1)]
    a = array.from_float(vals, 11, 52).reshape(ndim * (n,))
    b = cast(APyFixedArray, eval(f"array.from_float({a!s}, 11, 52)"))
    assert a.cast(11, 52).is_identical(b)


@pytest.mark.parametrize("array", [APyCFixedArray])
@pytest.mark.parametrize("ndim", [1, 2, 3, 4])
@pytest.mark.parametrize("n", [5, 10])
@pytest.mark.parametrize("factor", [0, 1j, -1 / 2 - 1 / 4j])
def test_array_print_complex(
    array: type[APyCFixedArray], ndim: int, n: int, factor: float
):
    vals = [k * factor for k in range(1, n**ndim + 1)]
    a = array.from_float(vals, 11, 52).reshape(ndim * (n,))
    b = cast(APyCFixedArray, eval(f"array.from_float({a!s}, 11, 52)"))
    assert a.cast(11, 52).is_identical(b)


@pytest.mark.parametrize(
    "array", [APyCFixedArray, APyCFloatArray, APyFixedArray, APyFloatArray]
)
@pytest.mark.parametrize("base", [7, 8, 16, 19])
def test_array_print_raises(array: type[APyCFixedArray], base: int):
    arr = array.from_float([1.0, 2.0], 10, 10)
    with pytest.raises(
        ValueError, match=rf"APy[a-zA-Z]+\.__str__: base={base} is not supported"
    ):
        _ = arr.__str__(base)
