import pytest

from apytypes import APyCFixed, APyFixed


@pytest.mark.parametrize("fixed_type", [APyFixed])
def test_from_str(fixed_type):
    with pytest.raises(ValueError, match="Not implemented"):
        fixed_type.from_str("0", bits=1, int_bits=0, base=8)
    with pytest.raises(ValueError, match="Not implemented"):
        fixed_type.from_str("0", bits=1, int_bits=0, base=16)
    with pytest.raises(ValueError, match="Not implemented"):
        fixed_type.from_str("0", bits=1, int_bits=0, base=-1)


@pytest.mark.parametrize("fixed_type", [APyFixed, APyCFixed])
def test__str__(fixed_type):
    with pytest.raises(ValueError, match=r"APy[C]?Fixed::to_string_oct"):
        fixed_type(0, 1, 0).__str__(base=8)
    with pytest.raises(ValueError, match=r"APy[C]?Fixed::to_string_hex"):
        fixed_type(0, 1, 0).__str__(base=16)
    with pytest.raises(ValueError, match=r"APy[C]?Fixed::to_string"):
        fixed_type(0, 1, 0).__str__(base=37)
