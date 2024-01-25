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
