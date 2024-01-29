from apytypes import APyFixedArray

import pytest


def test_homogeneous_shape():
    """
    Test that array shapes work as intended (must be homogeneous)
    """
    APyFixedArray([], bits=10, int_bits=10)
    APyFixedArray([1], bits=10, int_bits=10)
    APyFixedArray([range(3), (3, 4, 5), [6, 7, 8]], bits=10, int_bits=10)
    APyFixedArray([[range(2)], [range(2)], [range(2)]], bits=10, int_bits=10)

    with pytest.raises(RuntimeError, match="Inhomogeneous sequence shape"):
        APyFixedArray([[1, 2], [3]], bits=10, int_bits=10)
    with pytest.raises(RuntimeError, match="Inhomogeneous sequence shape"):
        APyFixedArray([range(3), [3, 4], (6, 7, 8)], bits=10, int_bits=10)
    with pytest.raises(RuntimeError, match="Inhomogeneous sequence shape"):
        APyFixedArray([range(3), [3, 4, 5], (7, 8)], bits=10, int_bits=10)
    with pytest.raises(RuntimeError, match="Inhomogeneous sequence shape"):
        APyFixedArray([range(4), [3, 4, 5], (6, 7, 8)], bits=10, int_bits=10)


def test_array_floating_point_construction():
    """
    Test the `APyFixedArray.from_float` construction method
    """
    a = APyFixedArray.from_float([1.0, 1.25, 2.99], bits=4, frac_bits=1)
    assert a.is_identical(APyFixedArray([2, 3, 6], bits=4, frac_bits=1))

    a = APyFixedArray.from_float([-1.0, -1.25, -2.99], bits=4, frac_bits=1)
    assert a.is_identical(APyFixedArray([-2, -3, -6], bits=4, frac_bits=1))

    with pytest.raises(RuntimeError, match="Non <type>/sequence found when walking"):
        APyFixedArray.from_float([1.0, 2.0, None], int_bits=13, frac_bits=12)
