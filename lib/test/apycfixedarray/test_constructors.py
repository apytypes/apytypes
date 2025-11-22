import pytest

from apytypes import APyCFixedArray


@pytest.mark.parametrize(
    ("lst", "ref"),
    [
        ([1], APyCFixedArray.from_complex([1], int_bits=10, frac_bits=0)),
        ([1, 2], APyCFixedArray.from_complex([1 + 2j], int_bits=10, frac_bits=0)),
        ([1, 3, 4], APyCFixedArray.from_complex([1, 3, 4], int_bits=10, frac_bits=0)),
        ([[1], [2]], APyCFixedArray.from_complex([[1], [2]], int_bits=10, frac_bits=0)),
        (
            [[1, 2], [3, 4]],
            APyCFixedArray.from_complex([1 + 2j, 3 + 4j], int_bits=10, frac_bits=0),
        ),
        (
            [[[1, 2]], [[3, 4]]],
            APyCFixedArray.from_complex([[1 + 2j], [3 + 4j]], int_bits=10, frac_bits=0),
        ),
    ],
)
def test_constructor_complex_collapse_from_list(
    lst: list[list[int] | int], ref: APyCFixedArray
):
    assert APyCFixedArray(lst, int_bits=10, frac_bits=0).is_identical(ref)


@pytest.mark.parametrize(
    "dt",
    ["int64", "int32", "int16", "int8", "uint64", "uint32", "uint16", "uint8"],
)
@pytest.mark.parametrize(
    ("lst", "ref"),
    [
        ([1], APyCFixedArray.from_complex([1], int_bits=10, frac_bits=0)),
        ([1, 2], APyCFixedArray.from_complex([1 + 2j], int_bits=10, frac_bits=0)),
        ([1, 3, 4], APyCFixedArray.from_complex([1, 3, 4], int_bits=10, frac_bits=0)),
        ([[1], [2]], APyCFixedArray.from_complex([[1], [2]], int_bits=10, frac_bits=0)),
        (
            [[1, 2], [3, 4]],
            APyCFixedArray.from_complex([1 + 2j, 3 + 4j], int_bits=10, frac_bits=0),
        ),
        (
            [[[1, 2]], [[3, 4]]],
            APyCFixedArray.from_complex([[1 + 2j], [3 + 4j]], int_bits=10, frac_bits=0),
        ),
    ],
)
def test_constructor_complex_collapse_from_numpy(
    dt: str, lst: list[list[int] | int], ref: APyCFixedArray
):
    np = pytest.importorskip("numpy")
    np_lst = np.array(lst, dtype=dt)
    assert APyCFixedArray(np_lst, int_bits=10, frac_bits=0).is_identical(ref)
