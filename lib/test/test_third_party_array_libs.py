import re

import pytest

from apytypes import (
    ThirdPartyArrayLibrary,
    fp,
    fx,
    get_array_library,
    set_array_library,
)


@pytest.fixture(scope="session")
def test_at_startup():
    """
    At startup (loading the APyTypes module) the preferred array library is set to Numpy
    """
    assert get_array_library() == ThirdPartyArrayLibrary.NUMPY


@pytest.mark.usefixtures("test_at_startup")
@pytest.mark.parametrize(
    "lib",
    [
        ThirdPartyArrayLibrary.NUMPY,
        ThirdPartyArrayLibrary.PYTORCH,
        ThirdPartyArrayLibrary.TENSORFLOW,
        ThirdPartyArrayLibrary.JAX,
        ThirdPartyArrayLibrary.CUPY,
    ],
)
def test_is_settable(lib: str):
    """
    All third-party array libraries are settable. Setting them, without using them, does
    not cause an exception to be raised.
    """
    set_array_library(lib)
    assert get_array_library() == lib

    set_array_library("numpy")


@pytest.mark.usefixtures("test_at_startup")
@pytest.mark.parametrize("lib", ["numpy", "pytorch", "tensorflow", "jax", "cupy"])
def test_make_third_party_array(lib: str):
    """
    Coverage golf retrieving the different array types..
    """
    a = fx([0, 1, 2, 3], int_bits=5, frac_bits=5)
    b = fx([4, 3, 2, 1], int_bits=5, frac_bits=5)
    try:
        set_array_library(lib)
        assert repr((a < b).dtype == "dtype.('bool')")
    except TypeError as e:
        regex = re.compile(r"could not export nb::ndarray:")
        assert regex.match(str(e)) is not None  # noqa: PT017
    set_array_library("numpy")


def test_set_preferred_raises():
    """
    Setting the preferred library to something that does not exit raises an exception.
    """
    lib = "poormansarraylib"
    with pytest.raises(
        ValueError, match=rf"set_array_library: library '{lib}' not exactly in"
    ):
        set_array_library(lib)


def test_torch():
    torch = pytest.importorskip("torch")
    values = [0.125, 0.25, 3.5]
    t = torch.tensor(values)

    assert fx(t, int_bits=5, frac_bits=5).is_identical(
        fx(values, int_bits=5, frac_bits=5)
    )
    assert fp(t, exp_bits=5, man_bits=10).is_identical(
        fp(values, exp_bits=5, man_bits=10)
    )


def test_jax():
    jax = pytest.importorskip("jax")
    values = [0.125, 0.25, 3.5]
    t = jax.numpy.array(values)

    assert fx(t, int_bits=5, frac_bits=5).is_identical(
        fx(values, int_bits=5, frac_bits=5)
    )
    assert fp(t, exp_bits=5, man_bits=10).is_identical(
        fp(values, exp_bits=5, man_bits=10)
    )


def test_tensorflow():
    tensorflow = pytest.importorskip("tensorflow")
    values = [0.125, 0.25, 3.5]
    t = tensorflow.constant(values)

    assert fx(t, int_bits=5, frac_bits=5).is_identical(
        fx(values, int_bits=5, frac_bits=5)
    )
    assert fp(t, exp_bits=5, man_bits=10).is_identical(
        fp(values, exp_bits=5, man_bits=10)
    )


def test_cupy():
    cupy = pytest.importorskip("cupy")
    values = [0.125, 0.25, 3.5]
    t = cupy.array(values)

    assert fx(t, int_bits=5, frac_bits=5).is_identical(
        fx(values, int_bits=5, frac_bits=5)
    )
    assert fp(t, exp_bits=5, man_bits=10).is_identical(
        fp(values, exp_bits=5, man_bits=10)
    )
