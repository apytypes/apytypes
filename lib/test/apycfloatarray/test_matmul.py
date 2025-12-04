import pytest

from apytypes import fp, outer


@pytest.mark.parametrize("exp_bits", [10, 20, 30])
@pytest.mark.parametrize("man_bits", [10, 40, 59])
def test_outer_product(exp_bits: int, man_bits: int):
    a = fp(
        [1j, 1.25j, 0, -4, -3.25 + 2j, 0 + 1j, 99 + 2j],
        exp_bits=exp_bits,
        man_bits=man_bits,
        force_complex=True,
    )
    b = fp(
        [-1 + 2j, 1.25 - 1j, 0 - 0.25j, -4j, -3.25j],
        exp_bits=exp_bits,
        man_bits=man_bits,
        force_complex=True,
    )

    assert outer(a, b).is_identical(
        fp(
            [
                [-2 - 1j, 1 + 1.25j, 0.25 + 0j, 4 + 0j, 3.25 + 0j],
                [-2.5 - 1.25j, 1.25 + 1.5625j, 0.3125 + 0j, 5 + 0j, 4.0625 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [4 - 8j, -5 + 4j, 0 + 1j, 0 + 16j, 0 + 13j],
                [-0.75 - 8.5j, -2.0625 + 5.75j, 0.5 + 0.8125j, 8 + 13j, 6.5 + 10.5625j],
                [-2 - 1j, 1 + 1.25j, 0.25 + 0j, 4 + 0j, 3.25 + 0j],
                [-103 + 196j, 125.75 - 96.5j, 0.5 - 24.75j, 8 - 396j, 6.5 - 321.75j],
            ],
            exp_bits=exp_bits,
            man_bits=man_bits,
            force_complex=True,
        ),
        ignore_zero_sign=True,
    )

    assert outer(b, a).is_identical(
        fp(
            [
                [-2 - 1j, -2.5 - 1.25j, 0, 4 - 8j, -0.75 - 8.5j, -2 - 1j, -103 + 196j],
                [
                    1 + 1.25j,
                    1.25 + 1.5625j,
                    0,
                    -5 + 4j,
                    -2.0625 + 5.75j,
                    1 + 1.25j,
                    125.75 - 96.5j,
                ],
                [
                    0.25 + 0j,
                    0.3125 + 0j,
                    0,
                    0 + 1j,
                    0.5 + 0.8125j,
                    0.25 + 0j,
                    0.5 - 24.75j,
                ],
                [4 + 0j, 5 + 0j, 0, 0 + 16j, 8 + 13j, 4 + 0j, 8 - 396j],
                [
                    3.25 + 0j,
                    4.0625 + 0j,
                    0,
                    0 + 13j,
                    6.5 + 10.5625j,
                    3.25 + 0j,
                    6.5 - 321.75j,
                ],
            ],
            exp_bits=exp_bits,
            man_bits=man_bits,
            force_complex=True,
        ),
        ignore_zero_sign=True,
    )


def test_outer_product_raises():
    a = fp([[1, 1.25, 0, 4, -3.25, 0]], exp_bits=10, man_bits=10, force_complex=True)
    b = fp([-1, 1.25, 0, -4, -3.25], exp_bits=10, man_bits=10, force_complex=True)

    with pytest.raises(
        ValueError, match=r"APyCFloatArray\.outer: both `self` and `rhs`"
    ):
        _ = outer(a, b)

    with pytest.raises(
        ValueError, match=r"APyCFloatArray\.outer: both `self` and `rhs`"
    ):
        _ = outer(b, a)
