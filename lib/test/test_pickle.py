import pathlib
import pickle
from typing import Any

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


def pickle_and_writefile(obj: Any, filepath: pathlib.Path):
    """
    Serialize an object, using the Python pickle module, and store the data in a file.

    Parameters
    ----------
    obj: :class:`typing.Any`
        The object to serialize and store.
    filepath: :class:`pathlib.Path`
        Filepath where serialized object should be stored.
    """
    with filepath.open("wb") as f:
        pickle.dump(obj, f)


def unpickle_and_assert_identical(obj: Any, filepath: pathlib.Path):
    """
    Deserialize an object from file, using the Python pickle module, and assert
    identical against another object.

    Parameters
    ----------
    obj: :class:`typing.Any`
        The object to compare identical against.
    filepath: :class:`pathlib.Path`
        Filepath where serialized object is stored.
    """
    with filepath.open("rb") as f:
        ref = pickle.load(f)
        assert ref.is_identical(obj)


@pytest.mark.parametrize("int_bits", [10, 40, 100, 1000])
@pytest.mark.parametrize("frac_bits", [0, 10, 40, 100, 1000])
@pytest.mark.parametrize("apyfixed", [APyFixed, APyCFixed])
@pytest.mark.parametrize("val", [0.0, -1.0, 1.0, 1234.567, -927.456])
def test_pickle_apyfixed(
    tmp_path: pathlib.Path,
    apyfixed: type[APyCFixed],
    int_bits: int,
    frac_bits: int,
    val: float,
):
    filepath = tmp_path / f"apyfixed-{int_bits}-{frac_bits}.pickle"
    fx_ref = apyfixed.from_float(val, int_bits=int_bits, frac_bits=frac_bits)
    pickle_and_writefile(fx_ref, filepath)
    unpickle_and_assert_identical(fx_ref, filepath)


@pytest.mark.parametrize("int_bits", [10, 40, 100, 1000])
@pytest.mark.parametrize("frac_bits", [0, 10, 40, 100, 1000])
@pytest.mark.parametrize("apyfixedarray", [APyFixedArray, APyCFixedArray])
@pytest.mark.parametrize("val", [0.0, -1.0, 1.0, 1234.567, -927.456])
def test_pickle_apyfixedarray(
    tmp_path: pathlib.Path,
    apyfixedarray: type[APyCFixedArray],
    int_bits: int,
    frac_bits: int,
    val: float,
):
    filepath = tmp_path / f"apyfixedarray-{int_bits}-{frac_bits}.pickle"
    fx_ref = apyfixedarray.from_float(
        [val, -val] * 20, int_bits=int_bits, frac_bits=frac_bits
    )
    pickle_and_writefile(fx_ref, filepath)
    unpickle_and_assert_identical(fx_ref, filepath)


@pytest.mark.parametrize("exp_bits", [3, 10, 30])
@pytest.mark.parametrize("man_bits", [3, 10, 20, 60])
@pytest.mark.parametrize("bias", [None, 30])
@pytest.mark.parametrize("val", [0.0, -1.0, 1.0, 1234.567, -927.456])
@pytest.mark.parametrize("apyfloat", [APyFloat, APyCFloat])
def test_pickle_apyfloat(
    tmp_path: pathlib.Path,
    apyfloat: type[APyFloat],
    exp_bits: int,
    man_bits: int,
    bias: int,
    val: float,
):
    filepath = tmp_path / f"apyfloat-{exp_bits}-{man_bits}-{bias}.pickle"
    fp_ref = apyfloat.from_float(val, exp_bits=exp_bits, man_bits=man_bits, bias=bias)
    pickle_and_writefile(fp_ref, filepath)
    unpickle_and_assert_identical(fp_ref, filepath)


@pytest.mark.parametrize("exp_bits", [3, 10, 30])
@pytest.mark.parametrize("man_bits", [3, 10, 20, 60])
@pytest.mark.parametrize("bias", [None, 30])
@pytest.mark.parametrize("val", [0.0, -1.0, 1.0, 1234.567, -927.456])
@pytest.mark.parametrize("apyfloatarray", [APyFloatArray, APyCFloatArray])
def test_pickle_apyfloatarray(
    tmp_path: pathlib.Path,
    apyfloatarray: type[APyFloatArray],
    exp_bits: int,
    man_bits: int,
    bias: int,
    val: float,
):
    filepath = tmp_path / f"apyfloat-{exp_bits}-{man_bits}-{bias}.pickle"
    fp_ref = apyfloatarray.from_float(
        [val, -val] * 20, exp_bits=exp_bits, man_bits=man_bits, bias=bias
    )
    pickle_and_writefile(fp_ref, filepath)
    unpickle_and_assert_identical(fp_ref, filepath)
