from apytypes import APyFixed

import pytest


def test_not_implemented_yet():
    with pytest.raises(RuntimeError, match="Not implemented"):
        APyFixed(0, 1, 0).to_string_hex()
    with pytest.raises(RuntimeError, match="Not implemented"):
        APyFixed(0, 1, 0).to_string_oct()
    with pytest.raises(RuntimeError, match="Not implemented"):
        APyFixed.from_string("0", bits=1, int_bits=0, base=8)
    with pytest.raises(RuntimeError, match="Not implemented"):
        APyFixed.from_string("0", bits=1, int_bits=0, base=16)
    with pytest.raises(RuntimeError, match="Not implemented"):
        APyFixed.from_string("0", bits=1, int_bits=0, base=-1)
