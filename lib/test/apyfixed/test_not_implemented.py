from apytypes import APyFixed

import pytest


def test_not_implemented_yet():
    with pytest.raises(ValueError, match="Not implemented"):
        APyFixed.from_str("0", bits=1, int_bits=0, base=8)
    with pytest.raises(ValueError, match="Not implemented"):
        APyFixed.from_str("0", bits=1, int_bits=0, base=16)
    with pytest.raises(ValueError, match="Not implemented"):
        APyFixed.from_str("0", bits=1, int_bits=0, base=-1)

    with pytest.raises(ValueError, match=r"APyFixed::to_string_oct"):
        APyFixed(0, 1, 0).__str__(base=8)
    with pytest.raises(ValueError, match=r"APyFixed::to_string_hex"):
        APyFixed(0, 1, 0).__str__(base=16)
    with pytest.raises(ValueError, match=r"APyFixed::to_string"):
        APyFixed(0, 1, 0).__str__(base=37)
