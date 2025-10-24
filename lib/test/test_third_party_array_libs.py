import re

import pytest

from apytypes import fx, get_preferred_array_lib, set_preferred_array_lib


@pytest.fixture(scope="session")
def test_at_startup():
    """
    At startup (loading the APyTypes module) the preferred array library is set to Numpy
    """
    assert get_preferred_array_lib() == "numpy"


@pytest.mark.usefixtures("test_at_startup")
@pytest.mark.parametrize("lib", ["numpy", "pytorch", "tensorflow", "jax", "cupy"])
def test_is_settable(lib: str):
    """
    All third-party array libraries are settable. Setting them, without using them, does
    not cause an exception to be raised.
    """
    set_preferred_array_lib(lib)
    assert get_preferred_array_lib() == lib

    set_preferred_array_lib("numpy")


@pytest.mark.usefixtures("test_at_startup")
@pytest.mark.parametrize("lib", ["numpy", "pytorch", "tensorflow", "jax", "cupy"])
def test_make_third_party_array(lib: str):
    """
    Coverage golf retrieving the different array types..
    """
    a = fx([0, 1, 2, 3], int_bits=5, frac_bits=5)
    b = fx([4, 3, 2, 1], int_bits=5, frac_bits=5)
    try:
        set_preferred_array_lib(lib)
        assert repr((a < b).dtype == "dtype.('bool')")
    except TypeError as e:
        regex = re.compile(r"could not export nb::ndarray:")
        assert regex.match(str(e)) is not None  # noqa: PT017
    set_preferred_array_lib("numpy")


def test_set_preferred_raises():
    """
    Setting the preferred library to something that does not exit raises an exception.
    """
    lib = "poormansarraylib"
    with pytest.raises(
        ValueError, match=rf"set_preferred_array_lib: library '{lib}' not exactly in"
    ):
        set_preferred_array_lib(lib)
