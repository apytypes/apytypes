import pytest

from apytypes import APyCFixed


def test_not():
    a = APyCFixed((0, 0), 4, 0)
    nota = APyCFixed((15, 15), 4, 0)
    assert (~a).is_identical(nota)

    a = APyCFixed((1, 13), 4, 0)
    nota = APyCFixed((14, 2), 4, 0)
    assert (~a).is_identical(nota)


@pytest.mark.parametrize("int_bits", range(13, 100, 7))
def test_not_varying_wordlength(int_bits):
    a = APyCFixed((317, 123), int_bits, 0)
    assert (~a + 1 + 1j) == -a
