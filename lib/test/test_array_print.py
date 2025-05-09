from typing import cast

import pytest

from apytypes import APyCFixedArray, APyFixedArray, APyFloatArray


@pytest.mark.parametrize("array", [APyFixedArray, APyFloatArray])
@pytest.mark.parametrize("ndim", [1, 2, 3, 4])
@pytest.mark.parametrize("n", [5, 10])
@pytest.mark.parametrize("factor", [0, 1, -1 / 2 - 1 / 4])
def test_array_print_real(array: type[APyFixedArray], ndim: int, n: int, factor: float):
    a = factor * array.from_float(range(1, n**ndim + 1), 11, 52).reshape(ndim * (n,))
    b = cast(APyFixedArray, eval(f"array.from_float({a!s}, 11, 52)"))
    assert a.cast(11, 52).is_identical(b)


@pytest.mark.parametrize("array", [APyCFixedArray])
@pytest.mark.parametrize("ndim", [1, 2, 3, 4])
@pytest.mark.parametrize("n", [5, 10])
@pytest.mark.parametrize("factor", [0, 1j, -1 / 2 - 1 / 4j])
def test_array_print_complex(
    array: type[APyCFixedArray], ndim: int, n: int, factor: float
):
    a = factor * array.from_float(range(1, n**ndim + 1), 11, 52).reshape(ndim * (n,))
    b = cast(APyCFixedArray, eval(f"array.from_float({a!s}, 11, 52)"))
    assert a.cast(11, 52).is_identical(b)
