from apytypes import APyCFloat


def test_not():
    a = APyCFloat((0, 1), (0, 15), (0, 31), 4, 5)
    nota = APyCFloat((1, 0), (15, 0), (31, 0), 4, 5)
    assert (~a).is_identical(nota)

    a = APyCFloat((1, 0), (0b01010, 0b10101), (0b010101, 0b101010), 5, 6, 7)
    nota = APyCFloat((0, 1), (0b10101, 0b01010), (0b101010, 0b010101), 5, 6, 7)
    assert (~a).is_identical(nota)


def test_and():
    # Same spec
    a = APyCFloat((0, 1), (0b1100, 0b1010), (0b10101, 0b11100), 4, 5)
    b = APyCFloat((1, 1), (0b1010, 0b1100), (0b11011, 0b01110), 4, 5)

    expected = APyCFloat(
        (0, 1),
        (0b1100 & 0b1010, 0b1010 & 0b1100),
        (0b10101 & 0b11011, 0b11100 & 0b01110),
        4,
        5,
    )
    assert (a & b).is_identical(expected)

    # Mixed spec
    a = APyCFloat((1, 0), (0b01010, 0b10100), (0b0101010, 0b1111000), 5, 7, 17)
    b = APyCFloat((1, 1), (0b00101, 0b01001), (0b01111, 0b10101), 4, 5)

    expected = a.cast(5, 7, 16) & b.cast(5, 7, 16)
    assert (a & b).is_identical(expected)


def test_or():
    # Same spec
    a = APyCFloat((0, 1), (0b1100, 0b1010), (0b10101, 0b11100), 4, 5)
    b = APyCFloat((1, 1), (0b1010, 0b1100), (0b11011, 0b01110), 4, 5)

    expected = APyCFloat(
        (1, 1),
        (0b1100 | 0b1010, 0b1010 | 0b1100),
        (0b10101 | 0b11011, 0b11100 | 0b01110),
        4,
        5,
    )
    assert (a | b).is_identical(expected)

    # Mixed spec
    a = APyCFloat((1, 0), (0b01010, 0b10100), (0b0101010, 0b1111000), 5, 7, 19)
    b = APyCFloat((1, 1), (0b00101, 0b01001), (0b01111, 0b10101), 4, 5)

    expected = a.cast(5, 7, 17) | b.cast(5, 7, 17)
    assert (a | b).is_identical(expected)


def test_xor():
    # Same spec
    a = APyCFloat((0, 1), (0b1100, 0b1010), (0b10101, 0b11100), 4, 5)
    b = APyCFloat((1, 1), (0b1010, 0b1100), (0b11011, 0b01110), 4, 5)

    expected = APyCFloat(
        (1, 0),
        (0b1100 ^ 0b1010, 0b1010 ^ 0b1100),
        (0b10101 ^ 0b11011, 0b11100 ^ 0b01110),
        4,
        5,
    )
    assert (a ^ b).is_identical(expected)

    # Mixed spec
    a = APyCFloat((1, 0), (0b01010, 0b10100), (0b0101010, 0b1111000), 5, 7)
    b = APyCFloat((1, 1), (0b00101, 0b01001), (0b01111, 0b10101), 4, 5, 5)

    print(repr(a ^ b))
    expected = a.cast(5, 7, 13) ^ b.cast(5, 7, 13)
    assert (a ^ b).is_identical(expected)
