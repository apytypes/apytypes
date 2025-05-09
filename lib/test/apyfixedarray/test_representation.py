from typing import cast

from apytypes import APyFixedArray


def is_repr_identical(a: APyFixedArray):
    """
    Tests that an :class:`APyFloat` is identical to its representation.
    """
    return a.is_identical(cast(APyFixedArray, eval(repr(a))))


def test_array_representation():
    a = APyFixedArray([], bits=1, int_bits=-5)
    assert repr(a) == "APyFixedArray([], int_bits=-5, frac_bits=6)"
    assert is_repr_identical(a)

    b = APyFixedArray([[[[]]]], int_bits=1010, frac_bits=123)
    assert repr(b) == "APyFixedArray([[[[]]]], int_bits=1010, frac_bits=123)"
    assert is_repr_identical(b)

    c = APyFixedArray([1, 2, 3], bits=2, int_bits=2)
    assert repr(c) == "APyFixedArray([1, 2, 3], int_bits=2, frac_bits=0)"
    assert is_repr_identical(c)

    d = APyFixedArray([range(5, 8), range(3)], bits=10, int_bits=20)

    assert (
        repr(d)
        == """\
APyFixedArray([[5, 6, 7],
               [0, 1, 2]], int_bits=20, frac_bits=-10)"""
    )
    assert is_repr_identical(d)

    e = APyFixedArray([[range(5, 8), range(3)]], bits=64, int_bits=32)
    assert (
        repr(e)
        == """\
APyFixedArray([[[5, 6, 7],
                [0, 1, 2]]], int_bits=32, frac_bits=32)"""
    )
    assert is_repr_identical(e)

    f = APyFixedArray([[0, -1, -2, 1, 2]], bits=10, int_bits=0)
    assert (
        repr(f)
        == """\
APyFixedArray([[   0, 1023, 1022,    1,    2]], int_bits=0, frac_bits=10)"""
    )
    assert is_repr_identical(f)

    g = APyFixedArray([[0, -1, -2, 1, 2]], bits=11, int_bits=-5)
    assert (
        repr(g)
        == """\
APyFixedArray([[   0, 2047, 2046,    1,    2]], int_bits=-5, frac_bits=16)"""
    )
    assert is_repr_identical(g)

    h = APyFixedArray([[range(5, 8), range(3)]], bits=1000, int_bits=500)
    assert (
        repr(h)
        == """\
APyFixedArray([[[5, 6, 7],
                [0, 1, 2]]], int_bits=500, frac_bits=500)"""
    )
    assert is_repr_identical(h)


def test_array_representation_summary():
    a = APyFixedArray((range(10000), range(10000)), int_bits=100, frac_bits=2)
    assert (
        repr(a)
        == """\
APyFixedArray([[   0,    1,    2,  ..., 9997, 9998, 9999],
               [   0,    1,    2,  ..., 9997, 9998, 9999]], int_bits=100, frac_bits=2)\
"""
    )

    b = APyFixedArray([range(4) for _ in range(10000)], int_bits=100, frac_bits=2)
    assert (
        repr(b)
        == """\
APyFixedArray([[0, 1, 2, 3],
               [0, 1, 2, 3],
               [0, 1, 2, 3],
               ...,
               [0, 1, 2, 3],
               [0, 1, 2, 3],
               [0, 1, 2, 3]], int_bits=100, frac_bits=2)"""
    )

    c = APyFixedArray([range(200) for _ in range(200)], int_bits=100, frac_bits=2)
    assert (
        repr(c)
        == """\
APyFixedArray([[  0,   1,   2, ..., 197, 198, 199],
               [  0,   1,   2, ..., 197, 198, 199],
               [  0,   1,   2, ..., 197, 198, 199],
               ...,
               [  0,   1,   2, ..., 197, 198, 199],
               [  0,   1,   2, ..., 197, 198, 199],
               [  0,   1,   2, ..., 197, 198, 199]], int_bits=100, frac_bits=2)"""
    )

    d = APyFixedArray(range(2**3 * 10000), int_bits=20, frac_bits=0).reshape(
        (10000,) + 3 * (2,)
    )
    assert (
        repr(d)
        == """\
APyFixedArray([[[[    0,     1],
                 [    2,     3]],

                [[    4,     5],
                 [    6,     7]]],


               [[[    8,     9],
                 [   10,    11]],

                [[   12,    13],
                 [   14,    15]]],


               [[[   16,    17],
                 [   18,    19]],

                [[   20,    21],
                 [   22,    23]]],


               ...,


               [[[79976, 79977],
                 [79978, 79979]],

                [[79980, 79981],
                 [79982, 79983]]],


               [[[79984, 79985],
                 [79986, 79987]],

                [[79988, 79989],
                 [79990, 79991]]],


               [[[79992, 79993],
                 [79994, 79995]],

                [[79996, 79997],
                 [79998, 79999]]]], int_bits=20, frac_bits=0)"""
    )


def test_array_representation_too_long():
    a = APyFixedArray.from_float(range(4), int_bits=100, frac_bits=1000)
    assert (
        repr(a)
        == """\
APyFixedArray(
    [                                                                                                                                                                                                                                                                                                             0,
     10715086071862673209484250490600018105614048117055336074437503883703510511249361224931983788156958581275946729175531468251871452856923140435984577574698574803934567774824230985421074605062371141877954182153046474983581941267398767559165543946077062914571196477686542167660429831652624386837205668069376,
     21430172143725346418968500981200036211228096234110672148875007767407021022498722449863967576313917162551893458351062936503742905713846280871969155149397149607869135549648461970842149210124742283755908364306092949967163882534797535118331087892154125829142392955373084335320859663305248773674411336138752,
     32145258215588019628452751471800054316842144351166008223312511651110531533748083674795951364470875743827840187526594404755614358570769421307953732724095724411803703324472692956263223815187113425633862546459139424950745823802196302677496631838231188743713589433059626502981289494957873160511617004208128],
    int_bits=100,
    frac_bits=1000
)"""
    )
