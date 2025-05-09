import pytest

from apytypes import APyFixed


def test_not():
    a = APyFixed(0, 4, 0)
    nota = APyFixed(15, 4, 0)
    assert (~a).is_identical(nota)

    a = APyFixed(1, 4, 0)
    nota = APyFixed(14, 4, 0)
    assert (~a).is_identical(nota)


@pytest.mark.parametrize("int_bits", range(13, 100, 7))
def test_not_varying_wordlength(int_bits):
    a = APyFixed(317, int_bits, 0)
    assert (~a + 1) == -a
