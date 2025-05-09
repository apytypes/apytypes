import pytest

from apytypes import APyFixedArray


def test_not():
    a = APyFixedArray([0, 7], 4, 0)
    nota = APyFixedArray([15, 8], 4, 0)
    assert (~a).is_identical(nota)


@pytest.mark.parametrize("int_bits", range(13, 100, 7))
def test_not_varying_wordlength(int_bits):
    a = APyFixedArray.from_float([317, -3], int_bits, 0)
    nota = APyFixedArray.from_float([-318, 2], int_bits, 0)
    assert (~a).is_identical(nota)
