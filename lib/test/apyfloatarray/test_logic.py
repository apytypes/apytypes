from apytypes import APyFloatArray


def test_logic_not():
    a = APyFloatArray([0, 1], [3, 17], [5, 8], 5, 5)
    nota = APyFloatArray([1, 0], [28, 14], [26, 23], 5, 5)
    assert (~a).is_identical(nota)
