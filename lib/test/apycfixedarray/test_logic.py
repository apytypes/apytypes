import pytest

from apytypes import APyCFixedArray


def test_not():
    a = APyCFixedArray.from_complex([0, 7 + 5j], 4, 0)
    nota = APyCFixedArray.from_complex([15 + 15j, 8 + 10j], 4, 0)
    assert (~a).is_identical(nota)


@pytest.mark.parametrize("int_bits", range(13, 100, 7))
def test_not_varying_wordlength(int_bits):
    a = APyCFixedArray.from_float([317 + 1j, -3 - 1j], int_bits, 0)
    nota = APyCFixedArray.from_float([-318 - 2j, 2], int_bits, 0)
    assert (~a).is_identical(nota)
