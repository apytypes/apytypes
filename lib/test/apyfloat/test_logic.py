from apytypes import APyFloat


def test_not():
    a = APyFloat(0, 0, 0, 4, 5)
    nota = APyFloat(1, 15, 31, 4, 5)
    assert (~a).is_identical(nota)

    a = APyFloat(1, 15, 31, 4, 5)
    nota = APyFloat(0, 0, 0, 4, 5)
    assert (~a).is_identical(nota)

    a = APyFloat(1, 0b01010, 0b010101, 5, 6, 7)
    nota = APyFloat(0, 0b10101, 0b101010, 5, 6, 7)
    assert (~a).is_identical(nota)

    a = APyFloat(0, 0b10101, 0b101010, 5, 6, 7)
    nota = APyFloat(1, 0b01010, 0b010101, 5, 6, 7)
    assert (~a).is_identical(nota)


def test_and():
    # Same spec
    a = APyFloat(0, 0b1100, 0b10101, 4, 5)
    b = APyFloat(1, 0b1010, 0b11011, 4, 5)
    expected = APyFloat(
        0 & 1,
        0b1100 & 0b1010,
        0b10101 & 0b11011,
        4,
        5,
    )
    assert (a & b).is_identical(expected)

    a = APyFloat(1, 0b1010, 0b11100, 4, 5)
    b = APyFloat(1, 0b1100, 0b01110, 4, 5)
    expected = APyFloat(
        1 & 1,
        0b1010 & 0b1100,
        0b11100 & 0b01110,
        4,
        5,
    )
    assert (a & b).is_identical(expected)

    # Mixed spec
    a = APyFloat(1, 0b01010, 0b0101010, 5, 7)
    b = APyFloat(1, 0b00101, 0b01111, 4, 5)
    expected = a.cast(5, 7) & b.cast(5, 7)
    assert (a & b).is_identical(expected)

    a = APyFloat(0, 0b10100, 0b1111000, 5, 7, 17)
    b = APyFloat(1, 0b01001, 0b10101, 4, 5)
    expected = a.cast(5, 7, 16) & b.cast(5, 7, 16)
    assert (a & b).is_identical(expected)


def test_or():
    # Same spec
    a = APyFloat(0, 0b1100, 0b10101, 4, 5)
    b = APyFloat(1, 0b1010, 0b11011, 4, 5)
    expected = APyFloat(
        0 | 1,
        0b1100 | 0b1010,
        0b10101 | 0b11011,
        4,
        5,
    )
    assert (a | b).is_identical(expected)

    a = APyFloat(1, 0b1010, 0b11100, 4, 5)
    b = APyFloat(1, 0b1100, 0b01110, 4, 5)
    expected = APyFloat(
        1 | 1,
        0b1010 | 0b1100,
        0b11100 | 0b01110,
        4,
        5,
    )
    assert (a | b).is_identical(expected)

    # Mixed spec
    a = APyFloat(1, 0b01010, 0b0101010, 5, 7)
    b = APyFloat(1, 0b00101, 0b01111, 4, 5)
    expected = a.cast(5, 7) | b.cast(5, 7)
    assert (a | b).is_identical(expected)

    a = APyFloat(0, 0b10100, 0b1111000, 5, 7, 19)
    b = APyFloat(1, 0b01001, 0b10101, 4, 5)
    expected = a.cast(5, 7, 17) | b.cast(5, 7, 17)
    assert (a | b).is_identical(expected)


def test_xor():
    # Same spec
    a = APyFloat(0, 0b1100, 0b10101, 4, 5)
    b = APyFloat(1, 0b1010, 0b11011, 4, 5)
    expected = APyFloat(
        0 ^ 1,
        0b1100 ^ 0b1010,
        0b10101 ^ 0b11011,
        4,
        5,
    )
    assert (a ^ b).is_identical(expected)

    a = APyFloat(1, 0b1010, 0b11100, 4, 5)
    b = APyFloat(1, 0b1100, 0b01110, 4, 5)
    expected = APyFloat(
        1 ^ 1,
        0b1010 ^ 0b1100,
        0b11100 ^ 0b01110,
        4,
        5,
    )
    assert (a ^ b).is_identical(expected)

    # Mixed spec
    a = APyFloat(1, 0b01010, 0b0101010, 5, 7)
    b = APyFloat(1, 0b00101, 0b01111, 4, 5)
    expected = a.cast(5, 7) ^ b.cast(5, 7)
    assert (a ^ b).is_identical(expected)

    a = APyFloat(0, 0b10100, 0b1111000, 5, 7)
    b = APyFloat(1, 0b01001, 0b10101, 4, 5, 5)
    expected = a.cast(5, 7, 13) ^ b.cast(5, 7, 13)
    assert (a ^ b).is_identical(expected)
