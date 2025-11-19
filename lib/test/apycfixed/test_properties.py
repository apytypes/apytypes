from apytypes import APyCFixed


def test_is_zero():
    a = APyCFixed((0, 0), 2, 10)
    assert a.is_zero

    a = APyCFixed((0, 0), 100, 200)
    assert a.is_zero

    a = APyCFixed((1, 0), 2, 10)
    assert not a.is_zero

    a = APyCFixed((1, 0), 100, 200)
    assert not a.is_zero

    a = APyCFixed((0, 1), 2, 10)
    assert not a.is_zero

    a = APyCFixed((0, 1), 100, 200)
    assert not a.is_zero


def test_leading_zeros():
    a = APyCFixed.from_number(0.25 + 1j, 4, 10)
    assert a.leading_zeros == 3

    a = APyCFixed.from_number(0.25 + 1j, 400, 10)
    assert a.leading_zeros == 399

    a = APyCFixed.from_number(0.25 - 1j, 4, 10)
    assert a.leading_zeros == 0


def test_leading_ones():
    a = APyCFixed.from_number(-0.25 - 1j, 4, 10)
    assert a.leading_ones == 4

    a = APyCFixed.from_number(-0.25 - 1j, 400, 10)
    assert a.leading_ones == 400

    a = APyCFixed.from_number(-0.25 + 1j, 4, 10)
    assert a.leading_ones == 0


def test_leading_signs():
    a = APyCFixed.from_number(0.25 + 1j, 4, 10)
    assert a.leading_signs == 3

    a = APyCFixed.from_number(0.25 + 1j, 400, 10)
    assert a.leading_signs == 399

    a = APyCFixed.from_number(0.25 - 1j, 4, 10)
    assert a.leading_signs == 4

    a = APyCFixed.from_number(-0.25 - 1j, 4, 10)
    assert a.leading_signs == 4

    a = APyCFixed.from_number(-0.25 - 1j, 400, 10)
    assert a.leading_signs == 400


def test_trailing_zeros():
    a = APyCFixed.from_number(0.25 + 1j, 4, 10)
    assert a.trailing_zeros == 8

    a = APyCFixed.from_number(0.25 + 1j, 4, 100)
    assert a.trailing_zeros == 98
