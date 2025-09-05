from itertools import permutations

import pytest

from apytypes import APyCFixed, APyCFixedArray, APyFixed, APyFixedArray


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_shape(fixed_array: type[APyCFixedArray]):
    assert fixed_array([], bits=1, int_bits=0).shape == (0,)
    assert fixed_array([1], bits=1, int_bits=0).shape == (1,)
    assert fixed_array([[range(3), range(3)]], bits=1, int_bits=0).shape == (1, 2, 3)
    assert fixed_array([range(3), range(3)], bits=1, int_bits=0).shape == (2, 3)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_ndim(fixed_array: type[APyCFixedArray]):
    assert fixed_array([], bits=1, int_bits=0).ndim == 1
    assert fixed_array([1], bits=1, int_bits=0).ndim == 1
    assert fixed_array([[range(3), range(3)]], bits=1, int_bits=0).ndim == 3
    assert fixed_array([range(3), range(3)], bits=1, int_bits=0).ndim == 2


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_squeeze(fixed_array: type[APyCFixedArray]):
    a = fixed_array([[1], [2], [3]], bits=2, int_bits=2)
    b = a.squeeze()
    assert (b).is_identical(fixed_array([1, 2, 3], bits=2, int_bits=2))
    c = fixed_array([[[1, 2], [3, 4], [5, 6], [7, 8]]], bits=4, int_bits=4)
    d = c.squeeze()
    assert (d).is_identical(
        fixed_array([[1, 2], [3, 4], [5, 6], [7, 8]], bits=4, int_bits=4)
    )
    e = fixed_array([1, 2, 3], bits=2, int_bits=2)
    f = e.squeeze()
    assert f.is_identical(e)
    g = fixed_array([[[[[[[[2]]]]]]]], bits=2, int_bits=2)
    h = g.squeeze()
    assert h.is_identical(fixed_array([2], bits=2, int_bits=2))
    i = fixed_array([], bits=4, int_bits=4)
    j = fixed_array([[]], bits=4, int_bits=4)
    k = i.squeeze()
    z = j.squeeze()
    assert k.is_identical(i)
    assert z.is_identical(i)
    m = fixed_array([[1], [2], [3]], bits=2, int_bits=2)
    with pytest.raises(
        ValueError, match=r"APyC?FixedArray\.squeeze: cannot squeeze non-one dimensions"
    ):
        _ = m.squeeze(axis=0)
    m1 = m.squeeze(axis=1)
    assert m1.is_identical(fixed_array([1, 2, 3], bits=2, int_bits=2))
    with pytest.raises(
        IndexError,
        match=r"APyC?FixedArray\.squeeze: specified axis larger than number of array dim",
    ):
        _ = m.squeeze(axis=2)

    n = fixed_array([[[[[[[[2]]]]]]]], bits=2, int_bits=2)
    o = n.squeeze((0, 1, 2, 3))
    assert o.is_identical(fixed_array([[[[2]]]], bits=2, int_bits=2))
    p = n.squeeze((0, 1, 3))
    assert p.is_identical(fixed_array([[[[[2]]]]], bits=2, int_bits=2))
    q = fixed_array([[[1]], [[2]], [[3]], [[4]]], bits=2, int_bits=2)
    with pytest.raises(
        ValueError, match=r"APyC?FixedArray\.squeeze: cannot squeeze non-one dimensions"
    ):
        _ = q.squeeze((0, 1, 2))
    with pytest.raises(
        IndexError,
        match=r"APyC?FixedArray.squeeze: specified axis larger than number of array dim",
    ):
        _ = m.squeeze((1, 4))


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)],
)
@pytest.mark.parametrize("sum_func", ["sum", "nansum"])
def test_sum(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed], sum_func: str
):
    a = fixed_array([[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]], bits=5, int_bits=5)
    b = getattr(a, sum_func)()
    assert b.is_identical(fixed_scalar(78, bits=9, int_bits=9))
    d = getattr(a, sum_func)((0, 1))
    e = getattr(a, sum_func)(0)
    f = getattr(a, sum_func)(1)
    assert d.is_identical(fixed_scalar(78, bits=9, int_bits=9))
    assert e.is_identical(fixed_array([22, 26, 30], bits=7, int_bits=7))
    assert f.is_identical(fixed_array([6, 15, 24, 33], bits=7, int_bits=7))

    # test for size larger than 32 and 64 when number over multiple limbs
    g = fixed_array([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=33)
    h = getattr(g, sum_func)(0)
    assert h.is_identical(fixed_array([3, 5, 7], frac_bits=0, int_bits=34))
    j = fixed_array([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=65)
    k = getattr(j, sum_func)(0)
    assert k.is_identical(fixed_array([3, 5, 7], frac_bits=0, int_bits=66))

    # test some float and negative summation
    j = fixed_array.from_float([0.25, 1.375, 3.25], frac_bits=3, int_bits=5)
    k = getattr(j, sum_func)()
    assert k.is_identical(fixed_scalar.from_float(4.875, bits=10, int_bits=7))
    m = fixed_array.from_float([0.25, 1.25, 3.25], frac_bits=3, int_bits=5)
    n = getattr(m, sum_func)()
    assert n.is_identical(fixed_scalar.from_float(4.75, frac_bits=3, int_bits=7))

    o = fixed_array([[-1, -2, 3], [-3, -4, 5], [3, 3, 3]], frac_bits=0, int_bits=5)
    p = getattr(o, sum_func)(1)
    assert p.is_identical(fixed_array([-0, -2, 9], frac_bits=0, int_bits=7))

    q = fixed_array([[-1, -2, 3], [1, 2, 3]], frac_bits=0, int_bits=5)
    r = getattr(q, sum_func)(0)
    assert r.is_identical(fixed_array([0, 0, 6], frac_bits=0, int_bits=6))

    m = fixed_array([1, 2, 3], bits=2, int_bits=2)
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple: dimension 1 out of range \(ndim = 1\)",
    ):
        _ = getattr(m, sum_func)(1)

    x = fixed_array(
        [
            [[[0, 1, 1], [2, 3, -1]], [[4, 5, 1], [6, 7, -1]]],
            [[[8, 9, -1], [10, 11, 1]], [[12, 13, -1], [14, 15, 1]]],
        ],
        int_bits=10,
        frac_bits=0,
    )
    y = getattr(x, sum_func)(1)
    z = getattr(x, sum_func)(2)
    w = getattr(x, sum_func)((1, 2))
    assert y.is_identical(
        fixed_array(
            [[[4, 6, 2], [8, 10, -2]], [[20, 22, -2], [24, 26, 2]]],
            int_bits=11,
            frac_bits=0,
        )
    )
    assert z.is_identical(
        fixed_array(
            [[[2, 4, 0], [10, 12, 0]], [[18, 20, 0], [26, 28, 0]]],
            int_bits=11,
            frac_bits=0,
        )
    )
    assert w.is_identical(
        fixed_array([[12, 16, 0], [44, 48, 0]], int_bits=12, frac_bits=0)
    )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
@pytest.mark.parametrize("sum_func", ["cumsum", "nancumsum"])
def test_cumsum(fixed_array: type[APyCFixedArray], sum_func: str):
    a = fixed_array([[1, 2, 3], [4, 5, 6]], frac_bits=0, int_bits=5)
    b = getattr(a, sum_func)()
    assert b.is_identical(fixed_array([1, 3, 6, 10, 15, 21], frac_bits=0, int_bits=8))
    c = getattr(a, sum_func)(0)
    assert c.is_identical(fixed_array([[1, 2, 3], [5, 7, 9]], frac_bits=0, int_bits=6))
    d = getattr(a, sum_func)(1)
    assert d.is_identical(fixed_array([[1, 3, 6], [4, 9, 15]], frac_bits=0, int_bits=7))
    e = fixed_array(
        [[[1, 2, 3], [3, 4, 5]], [[5, 6, 7], [7, 8, 0]]], frac_bits=0, int_bits=8
    )
    f = getattr(e, sum_func)()
    g = getattr(e, sum_func)(0)
    h = getattr(e, sum_func)(1)
    i = getattr(e, sum_func)(2)
    assert f.is_identical(
        fixed_array(
            [1, 3, 6, 9, 13, 18, 23, 29, 36, 43, 51, 51], int_bits=12, frac_bits=0
        )
    )
    assert g.is_identical(
        fixed_array(
            [[[1, 2, 3], [3, 4, 5]], [[6, 8, 10], [10, 12, 5]]], frac_bits=0, int_bits=9
        )
    )
    assert h.is_identical(
        fixed_array(
            [[[1, 2, 3], [4, 6, 8]], [[5, 6, 7], [12, 14, 7]]], frac_bits=0, int_bits=9
        )
    )
    assert i.is_identical(
        fixed_array(
            [[[1, 3, 6], [3, 7, 12]], [[5, 11, 18], [7, 15, 15]]],
            frac_bits=0,
            int_bits=10,
        )
    )
    with pytest.raises(
        IndexError,
        match=r"APyC?FixedArray\.cumsum: axis 4 out of range \(ndim = 3\)",
    ):
        _ = getattr(e, sum_func)(4)

    k = fixed_array.from_float([[0.25, 0.25], [0.25, 0.25]], frac_bits=10, int_bits=10)
    m = getattr(k, sum_func)()
    assert m.is_identical(
        fixed_array.from_float([0.25, 0.5, 0.75, 1], frac_bits=10, int_bits=12)
    )

    # test for size larger than 32 and 64 when number over multiple limbs
    g = fixed_array([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=33)
    h = getattr(g, sum_func)(0)
    assert h.is_identical(fixed_array([[0, 1, 2], [3, 5, 7]], frac_bits=0, int_bits=34))
    j = fixed_array([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=65)
    k = getattr(j, sum_func)(0)
    assert k.is_identical(fixed_array([[0, 1, 2], [3, 5, 7]], frac_bits=0, int_bits=66))

    n = fixed_array(
        [[[1, 2, 3], [3, 4, 5]], [[5, 6, 6], [7, 8, 9]]], frac_bits=0, int_bits=5
    )
    o = n.sum((0, 1))
    assert o.is_identical(fixed_array([16, 20, 23], frac_bits=0, int_bits=7))

    x = fixed_array(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        int_bits=10,
        frac_bits=0,
    )
    y = getattr(x, sum_func)(1)
    z = getattr(x, sum_func)(2)
    assert y.is_identical(
        fixed_array(
            [
                [[[0, 1], [2, 3]], [[4, 6], [8, 10]]],
                [[[8, 9], [10, 11]], [[20, 22], [24, 26]]],
            ],
            int_bits=11,
            frac_bits=0,
        )
    )
    assert z.is_identical(
        fixed_array(
            [
                [[[0, 1], [2, 4]], [[4, 5], [10, 12]]],
                [[[8, 9], [18, 20]], [[12, 13], [26, 28]]],
            ],
            int_bits=11,
            frac_bits=0,
        )
    )


def test_special_case():  # Trigger the addition with different number of limbs
    p = APyFixedArray(
        [170141183460469231713240559642174554112] * 16, bits=128, frac_bits=79
    )
    psum = APyFixed(2722258935367507707411848954274792865792, bits=132, int_bits=53)
    assert psum.is_identical(p.sum())


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)],
)
@pytest.mark.parametrize("prod_func", ["prod", "nanprod"])
def test_prod(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed], prod_func: str
):
    # Complex mult results in one additional bit compared to real mult
    cb = fixed_array == APyCFixedArray

    a = fixed_array([[1, 2, 3], [3, 4, 6], [5, 6, 1], [7, 8, 8]], bits=5, int_bits=5)
    b = getattr(a, prod_func)()
    assert b.is_identical(fixed_scalar(5806080, int_bits=60 + 11 * cb, frac_bits=0))
    c = fixed_array([[0, 1, 2], [3, 4, 5], [1, 1, 2]], frac_bits=0, int_bits=5)
    e = getattr(c, prod_func)(0)
    f = getattr(c, prod_func)(1)
    assert e.is_identical(fixed_array([0, 4, 20], int_bits=15 + 2 * cb, frac_bits=0))
    assert f.is_identical(fixed_array([0, 60, 2], int_bits=15 + 2 * cb, frac_bits=0))
    # test for size larger than 32 and 64 when number over multiple limbs
    g = fixed_array([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=33)
    h = getattr(g, prod_func)(0)
    assert h.is_identical(fixed_array([0, 4, 10], frac_bits=0, int_bits=66 + cb))
    j = fixed_array([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=65)
    k = getattr(j, prod_func)(0)
    assert k.is_identical(fixed_array([0, 4, 10], frac_bits=0, int_bits=130 + cb))

    m = fixed_array([1, 2, 3], bits=2, int_bits=2)
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple: dimension 1 out of range \(ndim = 1\)",
    ):
        _ = getattr(m, prod_func)(1)

    n = fixed_array(
        [[[1, 2, 3], [3, 4, 5]], [[5, 6, 7], [7, 8, 9]]], frac_bits=0, int_bits=5
    )
    o = getattr(n, prod_func)((0, 1))
    assert o.is_identical(
        fixed_array([105, 384, 945], frac_bits=0, int_bits=20 + 3 * cb)
    )

    x = fixed_array(
        [
            [[[0, 1, 2], [2, 3, 4]], [[4, 5, 10], [6, 7, 9]]],
            [[[8, 9, 3], [10, 11, 5]], [[12, 13, 10], [14, 15, 1]]],
        ],
        int_bits=10,
        frac_bits=0,
    )
    y = getattr(x, prod_func)(1)
    z = getattr(x, prod_func)(2)

    assert y.is_identical(
        fixed_array(
            [[[0, 5, 20], [12, 21, 36]], [[96, 117, 30], [140, 165, 5]]],
            int_bits=20 + cb,
            frac_bits=0,
        )
    )
    assert z.is_identical(
        fixed_array(
            [[[0, 3, 8], [24, 35, 90]], [[80, 99, 15], [168, 195, 10]]],
            int_bits=20 + cb,
            frac_bits=0,
        )
    )


@pytest.mark.parametrize("prod_func", ["prod", "nanprod"])
def test_prod_float(prod_func: str):
    # test some float and negative multiplication
    j = APyFixedArray.from_float([0.25, 8], frac_bits=3, int_bits=5)
    k = getattr(j, prod_func)()
    assert k.is_identical(APyFixed.from_float(2, frac_bits=6, int_bits=10))

    n = APyFixedArray.from_float([[0.25, 0.5]], frac_bits=10, int_bits=10)
    o = getattr(n, prod_func)(1)
    assert o.is_identical(APyFixedArray.from_float([0.125], frac_bits=20, int_bits=20))


@pytest.mark.parametrize("prod_func", ["prod", "nanprod"])
def test_prod_negative(prod_func: str):
    o = APyFixedArray([[-1, -2], [-3, -4]], frac_bits=0, int_bits=5)
    p = getattr(o, prod_func)(1)
    assert p.is_identical(APyFixedArray([2, 12], frac_bits=0, int_bits=10))

    q = APyFixedArray([[-1, -2], [1, 2]], frac_bits=0, int_bits=5)
    r = getattr(q, prod_func)(0)
    assert r.is_identical(APyFixedArray([-1, -4], frac_bits=0, int_bits=10))


@pytest.mark.parametrize("prod_func", ["prod", "nanprod"])
def test_prod_multiple_axes(prod_func: str):
    c = APyFixedArray([[0, 1, 2], [3, 4, 5]], frac_bits=0, int_bits=5)
    d = getattr(c, prod_func)((0, 1))
    assert d.is_identical(APyFixed(0, bits=30, int_bits=30))

    x = APyFixedArray(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        int_bits=10,
        frac_bits=0,
    )
    w = getattr(x, prod_func)((1, 3))
    assert w.is_identical(
        APyFixedArray([[0, 252], [11232, 23100]], int_bits=40, frac_bits=0)
    )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
@pytest.mark.parametrize("prod_func", ["cumprod", "nancumprod"])
def test_cumprod(fixed_array: type[APyCFixedArray], prod_func: str):
    # Complex mult results in one additional bit compared to real mult
    cb = fixed_array == APyCFixedArray

    a = fixed_array.from_float([[1, 2, 3], [-4, 5, 6]], int_bits=5, frac_bits=5)
    b = getattr(a, prod_func)()
    assert b.is_identical(
        fixed_array.from_float(
            [1, 2, 6, -24, -120, -720], int_bits=30 + 5 * cb, frac_bits=30
        )
    )
    c = getattr(a, prod_func)(0)
    assert c.is_identical(
        fixed_array.from_float(
            [[1, 2, 3], [-4, 10, 18]], int_bits=10 + cb, frac_bits=10
        )
    )
    d = getattr(a, prod_func)(1)
    assert d.is_identical(
        fixed_array.from_float(
            [[1, 2, 6], [-4, -20, -120]], int_bits=15 + 2 * cb, frac_bits=15
        )
    )
    e = fixed_array.from_float(
        [[[1, 2], [-3, 4]], [[5, 6], [7, -8]]], int_bits=8, frac_bits=8
    )
    f = getattr(e, prod_func)()
    g = getattr(e, prod_func)(0)
    h = getattr(e, prod_func)(1)
    i = getattr(e, prod_func)(2)
    assert f.is_identical(
        fixed_array.from_float(
            [1, 2, -6, -24, -120, -720, -5040, 40320],
            int_bits=64 + 7 * cb,
            frac_bits=64,
        )
    )
    assert g.is_identical(
        fixed_array.from_float(
            [[[1, 2], [-3, 4]], [[5, 12], [-21, -32]]], int_bits=16 + cb, frac_bits=16
        )
    )
    assert h.is_identical(
        fixed_array.from_float(
            [[[1, 2], [-3, 8]], [[5, 6], [35, -48]]], int_bits=16 + cb, frac_bits=16
        )
    )
    assert i.is_identical(
        fixed_array.from_float(
            [[[1, 2], [-3, -12]], [[5, 30], [7, -56]]], int_bits=16 + cb, frac_bits=16
        )
    )
    with pytest.raises(
        IndexError,
        match=r"APyC?FixedArray\.cumprod: axis 4 out of range \(ndim = 3\)",
    ):
        _ = getattr(e, prod_func)(4)

    k = fixed_array.from_float([[0.25, 0.5], [1, 2]], frac_bits=10, int_bits=10)
    m = getattr(k, prod_func)()
    assert m.is_identical(
        fixed_array.from_float(
            [0.25, 0.125, 0.125, 0.25], int_bits=40 + 3 * cb, frac_bits=40
        )
    )

    x = fixed_array.from_float(
        [
            [[[0, 1], [2, -3]], [[4, 5], [6, -7]]],
            [[[8, 9], [10, 11]], [[12, 13], [-14, 15]]],
        ],
        int_bits=10,
        frac_bits=10,
    )
    y = getattr(x, prod_func)(1)
    z = getattr(x, prod_func)(2)
    assert y.is_identical(
        fixed_array.from_float(
            [
                [[[0, 1], [2, -3]], [[0, 5], [12, 21]]],
                [[[8, 9], [10, 11]], [[96, 117], [-140, 165]]],
            ],
            int_bits=20 + cb,
            frac_bits=20,
        )
    )
    assert z.is_identical(
        fixed_array.from_float(
            [
                [[[0, 1], [0, -3]], [[4, 5], [24, -35]]],
                [[[8, 9], [80, 99]], [[12, 13], [-168, 195]]],
            ],
            int_bits=20 + cb,
            frac_bits=20,
        )
    )


@pytest.mark.parametrize("prod_func", ["cumprod", "nancumprod"])
def test_cumprod_itemsize(prod_func: str):
    # test for size larger than 32 and 64 when number over multiple limbs
    g = APyFixedArray.from_float([[0, 1, 2], [3, 4, 5]], frac_bits=33, int_bits=33)
    h = getattr(g, prod_func)(0)
    assert h.is_identical(
        APyFixedArray.from_float([[0, 1, 2], [0, 4, 10]], frac_bits=66, int_bits=66)
    )
    j = APyFixedArray.from_float([[0, 1, 2], [3, 4, 5]], frac_bits=65, int_bits=65)
    k = getattr(j, prod_func)(0)
    assert k.is_identical(
        APyFixedArray.from_float([[0, 1, 2], [0, 4, 10]], frac_bits=130, int_bits=130)
    )


@pytest.mark.parametrize("max_func", ["max", "nanmax"])
def test_max(max_func: str):
    a = APyFixedArray([[0, 1], [2, 3]], int_bits=5, frac_bits=0)
    b = getattr(a, max_func)()
    c = getattr(a, max_func)(0)
    d = getattr(a, max_func)(1)
    assert b.is_identical(APyFixed(3, int_bits=5, frac_bits=0))
    assert c.is_identical(APyFixedArray([2, 3], int_bits=5, frac_bits=0))
    assert d.is_identical(APyFixedArray([1, 3], int_bits=5, frac_bits=0))
    e = APyFixedArray(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        int_bits=10,
        frac_bits=0,
    )
    f = getattr(e, max_func)(1)
    g = getattr(e, max_func)((0, 1))
    h = getattr(e, max_func)((1, 3))
    i = getattr(e, max_func)()
    k = getattr(e, max_func)((0, 1, 2, 3))
    assert f.is_identical(
        APyFixedArray(
            [[[4, 5], [6, 7]], [[12, 13], [14, 15]]], int_bits=10, frac_bits=0
        )
    )
    assert g.is_identical(APyFixedArray([[12, 13], [14, 15]], int_bits=10, frac_bits=0))
    assert h.is_identical(APyFixedArray([[5, 7], [13, 15]], int_bits=10, frac_bits=0))
    assert i.is_identical(APyFixed(15, int_bits=10, frac_bits=0))
    assert k.is_identical(i)


@pytest.mark.parametrize("max_func", ["max", "nanmax"])
def test_max_negative(max_func: str):
    # test negative numbers
    z = APyFixedArray([[0, -1], [-2, -3]], int_bits=5, frac_bits=0)
    m = getattr(z, max_func)()
    n = getattr(z, max_func)(0)
    o = getattr(z, max_func)(1)
    assert m.is_identical(APyFixed(0, int_bits=5, frac_bits=0))
    assert n.is_identical(APyFixedArray([0, -1], int_bits=5, frac_bits=0))
    assert o.is_identical(APyFixedArray([0, -2], int_bits=5, frac_bits=0))


@pytest.mark.parametrize("max_func", ["max", "nanmax"])
def test_max_itemsize(max_func: str):
    # test with larger than 64 bit size
    p = APyFixedArray([[-4, -1], [-2, -3]], int_bits=85, frac_bits=0)
    q = getattr(p, max_func)()
    assert q.is_identical(APyFixed(-1, int_bits=85, frac_bits=0))


@pytest.mark.parametrize("max_func", ["max", "nanmax"])
def test_max_float(max_func: str):
    # test float numbers
    r = APyFixedArray.from_float([1.0, 1.25, 2.875], int_bits=10, frac_bits=10)
    s = getattr(r, max_func)()
    assert s.is_identical(APyFixed.from_float(2.875, int_bits=10, frac_bits=10))
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple\: dimension 4 out of range \(ndim = 1\)",
    ):
        _ = getattr(r, max_func)(4)
    e = APyFixedArray(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        int_bits=10,
        frac_bits=0,
    )
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple\: dimension 4 out of range \(ndim = 4\)",
    ):
        _ = getattr(e, max_func)(4)


@pytest.mark.parametrize("max_func", ["max", "nanmax"])
def test_max_mixed_signs(max_func: str):
    # test a mix of negative and positive numbers
    p = APyFixedArray([[4, -1], [-2, -3]], int_bits=6, frac_bits=0)
    q = getattr(p, max_func)()
    assert q.is_identical(APyFixed(4, int_bits=6, frac_bits=0))


@pytest.mark.parametrize("min_func", ["min", "nanmin"])
def test_min(min_func: str):
    a = APyFixedArray([[0, 1], [2, 3]], int_bits=5, frac_bits=0)
    b = getattr(a, min_func)()
    c = getattr(a, min_func)(0)
    d = getattr(a, min_func)(1)
    assert b.is_identical(APyFixed(0, int_bits=5, frac_bits=0))
    assert c.is_identical(APyFixedArray([0, 1], int_bits=5, frac_bits=0))
    assert d.is_identical(APyFixedArray([0, 2], int_bits=5, frac_bits=0))
    e = APyFixedArray(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        int_bits=10,
        frac_bits=0,
    )
    f = getattr(e, min_func)(1)
    g = getattr(e, min_func)((0, 1))
    h = getattr(e, min_func)((1, 3))
    i = getattr(e, min_func)()
    k = getattr(e, min_func)((0, 1, 2, 3))
    assert f.is_identical(
        APyFixedArray([[[0, 1], [2, 3]], [[8, 9], [10, 11]]], int_bits=10, frac_bits=0)
    )
    assert g.is_identical(APyFixedArray([[0, 1], [2, 3]], int_bits=10, frac_bits=0))
    assert h.is_identical(APyFixedArray([[0, 2], [8, 10]], int_bits=10, frac_bits=0))
    assert i.is_identical(APyFixed(0, int_bits=10, frac_bits=0))
    assert k.is_identical(i)

    e = APyFixedArray(
        [
            [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            [[[8, 9], [10, 11]], [[12, 13], [14, 15]]],
        ],
        int_bits=10,
        frac_bits=0,
    )
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple\: dimension 4 out of range \(ndim = 4\)",
    ):
        _ = getattr(e, min_func)(4)


@pytest.mark.parametrize("min_func", ["min", "nanmin"])
def test_min_negative(min_func: str):
    # test negative numbers
    z = APyFixedArray([[0, -1], [-2, -3]], int_bits=5, frac_bits=0)
    m = getattr(z, min_func)()
    n = getattr(z, min_func)(0)
    o = getattr(z, min_func)(1)
    assert m.is_identical(APyFixed(-3, int_bits=5, frac_bits=0))
    assert n.is_identical(APyFixedArray([-2, -3], int_bits=5, frac_bits=0))
    assert o.is_identical(APyFixedArray([-1, -3], int_bits=5, frac_bits=0))


@pytest.mark.parametrize("min_func", ["min", "nanmin"])
def test_min_mixed_signs(min_func: str):
    # test a mix of negative and positive numbers
    p = APyFixedArray([[4, -1], [-2, -3]], int_bits=6, frac_bits=0)
    q = getattr(p, min_func)()
    assert q.is_identical(APyFixed(-3, int_bits=6, frac_bits=0))


@pytest.mark.parametrize("min_func", ["min", "nanmin"])
def test_min_itemsize(min_func: str):
    # test larger than 64 bit size
    p = APyFixedArray([[-4, -1], [-2, -3]], int_bits=85, frac_bits=0)
    q = getattr(p, min_func)()
    assert q.is_identical(APyFixed(-4, int_bits=85, frac_bits=0))


@pytest.mark.parametrize("min_func", ["min", "nanmin"])
def test_min_float(min_func: str):
    # test float numbers
    r = APyFixedArray.from_float([1.0, 1.25, 2.875], int_bits=10, frac_bits=10)
    s = getattr(r, min_func)()
    assert s.is_identical(APyFixed.from_float(1.0, int_bits=10, frac_bits=10))
    with pytest.raises(
        IndexError,
        match=r"axes_from_tuple\: dimension 4 out of range \(ndim = 1\)",
    ):
        _ = getattr(r, min_func)(4)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_to_numpy(fixed_array: type[APyCFixedArray]):
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    assert np.array_equal(fixed_array([], 1, 0).to_numpy(), np.array([]))
    assert np.array_equal(fixed_array([1], 2, 0).to_numpy(), np.array([1.0]))

    float_seq = [
        [[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]],
        [[7.0, 8.0, 9.0], [10.0, 11.0, 12.0]],
    ]
    fx_arr = fixed_array.from_float(float_seq, bits=10, int_bits=10)
    assert fx_arr.to_numpy().shape == (2, 2, 3)
    assert np.array_equal(fx_arr.to_numpy(), np.array(float_seq))


@pytest.mark.parametrize(
    "dt",
    [
        "int8",
        "int16",
        "int32",
        "int64",
        "uint8",
        "uint16",
        "uint32",
        "uint64",
        "float32",
        "float64",
    ],
)
@pytest.mark.parametrize(
    "copy",
    [
        False,
        True,
    ],
)
def test_to_numpy_args_real(dt: str, copy: bool):
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    # Test with different dtypes
    fp_arr = APyFixedArray.from_float(range(5), 10, 10)

    if copy:
        assert np.array_equal(
            fp_arr.to_numpy(dtype=dt, copy=copy), np.array(range(5), dtype=dt)
        )
    else:
        with pytest.raises(
            ValueError,
            match=r"APyFixedArray\.to_numpy: copy must be True",
        ):
            _ = fp_arr.to_numpy(dtype=dt, copy=copy)


@pytest.mark.parametrize(
    "dt",
    [
        "complex64",
        "complex128",
    ],
)
@pytest.mark.parametrize(
    "copy",
    [
        False,
        True,
    ],
)
def test_to_numpy_args_complex(dt: str, copy: bool):
    # Skip this test if `NumPy` is not present on the machine
    np = pytest.importorskip("numpy")

    # Test with different dtypes
    fp_arr = APyCFixedArray.from_float(range(5), 10, 10)

    if copy:
        assert np.array_equal(
            fp_arr.to_numpy(dtype=dt, copy=copy), np.array(range(5), dtype=dt)
        )
    else:
        with pytest.raises(
            ValueError,
            match=r"APyCFixedArray\.to_numpy: copy must be True",
        ):
            _ = fp_arr.to_numpy(dtype=dt, copy=copy)


def test_iterator():
    fx_array = APyFixedArray([1, 2, 3, 4, 5, 6], bits=10, int_bits=10)
    iterator = iter(fx_array)
    assert next(iterator).is_identical(APyFixed(1, bits=10, int_bits=10))
    assert next(iterator).is_identical(APyFixed(2, bits=10, int_bits=10))


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_len(fixed_array: type[APyCFixedArray]):
    fx_array = fixed_array([1, 2, 3, 4, 5, 6], bits=10, int_bits=10)
    assert len(fx_array) == 6


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_round_trip_conversion(fixed_array: type[APyCFixedArray]):
    # Skip this test if `NumPy` is not present on the machine
    pytest.importorskip("numpy")
    for bit_pattern in range(256):
        a = fixed_array([bit_pattern], int_bits=4, frac_bits=4)
        assert (fixed_array.from_float(a.to_numpy(), 4, 4)).is_identical(a)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
@pytest.mark.parametrize(
    ("shape", "is_valid", "is_invalid", "test_neg_one"),
    [
        ((6,), True, False, False),  # Valid shapes
        (6, True, False, False),
        ((3, 2), True, False, False),
        ((1, 6), True, False, False),
        ((6, 1), True, False, False),
        ((2, 3), True, False, False),
        ((-1,), True, False, False),
        ((-1, 2), True, False, False),
        ((1, -1), True, False, False),
        ((6, -1), True, False, False),
        ((3, -1), True, False, False),
        ((1, 2, 50), False, True, False),  # Invalid shapes
        ((1, 2), False, True, False),
        ((1, 4, 3), False, True, False),
        ((2, 4), False, True, False),
        ((2, 2), False, True, False),
        ((-2,), False, False, True),  # Invalid negatives
        ((-1, -1), False, False, True),
        ((4, -1), False, False, True),
        ((-2, 3), False, False, True),
        ((-999, 12), False, False, True),
    ],
)
def test_reshape(
    fixed_array: type[APyCFixedArray],
    shape: tuple[int, ...] | int,
    is_valid: bool,
    is_invalid: bool,
    test_neg_one: bool,
):
    # 6 elems
    nums = [[5, 10, 6], [15, 20, 3]]
    arr = fixed_array(nums, bits=10, int_bits=10)

    if is_invalid:
        with pytest.raises(
            ValueError,
            match=r"APyC?FixedArray\.reshape: target array number of elements does not",
        ):
            _ = arr.reshape(shape)
    elif test_neg_one:
        with pytest.raises(
            ValueError,
            match=r"APyC?FixedArray\.reshape: array dimensions must be greater than|"
            r"APyC?FixedArray\.reshape: only one dimension can be -1|"
            r"APyC?FixedArray\.reshape: the size of target array must be unchanged and",
        ):
            _ = arr.reshape(shape)
    elif is_valid:
        try:
            g = arr.reshape(shape)
            if g is arr:
                pytest.fail(
                    "Reshape should return a new array, now returns a modified one"
                )
        except ValueError:
            pytest.fail(f"Unexpected ValueError for shape {shape}")


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)],
)
@pytest.mark.parametrize(
    "shape",
    [
        (1, 12),
        (2, 6),
        (3, 4),
        (4, 3),
        (6, 2),
        (12, 1),
        (-1, 12),
        (2, -1),
        (3, -1),
        (4, -1),
        (-1, 3),
        (-1, 2),
        (12, -1),
    ],
)
def test_reshape_2d(fixed_array, fixed_scalar, shape):
    # Creating 1D array to be reshaped
    nums = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]

    # Creating 1D array to be reshaped
    arr = fixed_array(nums, bits=20, int_bits=10)

    reshaped_arr = arr.reshape(shape)
    for i, row in enumerate(reshaped_arr):
        for j, fixed in enumerate(row):
            arr_index = i * reshaped_arr.shape[1] + j
            if not fixed_scalar.is_identical(arr[arr_index], fixed):
                pytest.fail(f"Mismatch at index {arr_index} during reshape")

    go_back = reshaped_arr.reshape(arr.shape)
    if not fixed_array.is_identical(go_back, arr):
        pytest.fail(f"Mismatch after reshaping back at index {arr_index}")


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [(APyFixedArray, APyFixed), (APyCFixedArray, APyCFixed)],
)
@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
        (1, 12, 1),
        (2, 1, 6),
        (2, 2, 3),
        (2, 3, 2),
        (2, 6, 1),
        (3, 1, 4),
        (3, 2, 2),
        (3, 4, 1),
        (4, 1, 3),
        (4, 3, 1),
        (6, 1, 2),
        (6, 2, 1),
        (12, 1, 1),
        (-1, 2, 2),
        (1, -1, 2),
        (6, 1, -1),
    ],
)
def test_reshape_3d(fixed_array, fixed_scalar, shape):
    # Creating 1D array to be reshaped
    nums = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    # Creating 1D array to be reshaped
    arr = fixed_array(nums, bits=20, int_bits=10)

    reshaped_arr = arr.reshape(shape)
    for i, matrix in enumerate(reshaped_arr):
        for j, row in enumerate(matrix):
            for k, fixed in enumerate(row):
                arr_index = (
                    i * reshaped_arr.shape[1] * reshaped_arr.shape[2]
                    + j * reshaped_arr.shape[2]
                    + k
                )
                if not fixed_scalar.is_identical(arr[arr_index], fixed):
                    pytest.fail(f"Mismatch at index {arr_index} during reshape")

    go_back = reshaped_arr.reshape(arr.shape)
    if not fixed_array.is_identical(go_back, arr):
        pytest.fail(f"Mismatch after reshaping back at index {arr_index}")


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
        (1, 12, 1),
        (2, 1, 6),
        (2, 2, 3),
        (2, 3, 2),
    ],
)
def test_flatten(fixed_array, shape):
    # Creating 1D array to be reshaped
    nums = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    # Creating 1D array to be reshaped
    arr = fixed_array(nums, bits=20, int_bits=10)
    reshaped = arr.reshape(shape)
    if not fixed_array.is_identical(reshaped.flatten(), arr):
        pytest.fail(f"Flatten didn't return to original 1d list after reshape {shape}")
    pass


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
@pytest.mark.parametrize(
    "shape",
    [
        (1, 1, 12),
        (1, 2, 6),
        (1, 3, 4),
        (1, 4, 3),
        (1, 6, 2),
        (1, 12, 1),
        (2, 1, 6),
        (2, 2, 3),
        (2, 3, 2),
    ],
)
def test_ravel(fixed_array, shape):
    # Creating 1D array to be reshaped
    nums = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60]
    # Creating 1D array to be reshaped
    arr = fixed_array(nums, bits=20, int_bits=10)
    reshaped = arr.reshape(shape)
    if not fixed_array.is_identical(reshaped.ravel(), arr):
        pytest.fail(f"ravel didn't return to original 1d list after reshape {shape}")
    pass


def _generate_dimensions(n):
    result = set()  # Use a set to store unique combinations

    def factor_combinations(target, factors):
        if target == 1:
            result.add(tuple(sorted(factors)))  # Add sorted tuple to set
            return
        for i in range(2, target + 1):
            if target % i == 0:
                factor_combinations(target // i, [*factors, i])

    factor_combinations(n, [])
    return list(result)  # Convert set back to list


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
@pytest.mark.float_array
def test_transpose_highdim_np(fixed_array: type[APyCFixedArray]):
    # This test assumes from_array works correctly.
    # does checks to see if you get same result as numpy
    np = pytest.importorskip("numpy")
    num_elems = 48
    elements = np.arange(num_elems)

    # Generate all possible axis permutations for a 3D array
    possible_shapes = _generate_dimensions(num_elems)

    # Test each permutation
    for shape in possible_shapes:
        if len(shape) < 3:
            axes_permutations = [None]
        else:
            axes_permutations = list(permutations(list(range(len(shape)))))

        for perm in axes_permutations:
            apy_array = fixed_array.from_array(np.reshape(elements, shape), 7474, 5)
            numpy_transposed = np.transpose(np.reshape(elements, shape), perm)

            apy_transposed = apy_array.transpose(perm)
            numpy_array = fixed_array.from_array(numpy_transposed, 7474, 5)

            assert apy_transposed.is_identical(numpy_array), (
                f"Failed for shape {shape} and permutation {perm}. "
                f"Original array = \n{np.reshape(elements, shape)}\n "
                f"ApyFloat array = \n{apy_transposed.to_numpy()}\n "
                f"Numpy created array = \n{numpy_array.to_numpy()}"
            )


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
@pytest.mark.parametrize("start_val", [2 ** (i * 10) for i in range(0, 15)])
def test_transpose_highdim(fixed_array, start_val):
    a = fixed_array.from_float(
        [[start_val + 1, start_val + 2], [start_val + 3, start_val + 4]],
        int_bits=345,
        bits=9563,
    )
    b = fixed_array.from_float(
        [[start_val + 1, start_val + 3], [start_val + 2, start_val + 4]],
        int_bits=345,
        bits=9563,
    )

    assert a.transpose().is_identical(b)

    # 1  2  3
    # 4  5  6
    # 7  8  9
    a = fixed_array.from_float(
        [
            [start_val + 1, start_val + 2, start_val + 3],
            [start_val + 4, start_val + 5, start_val + 6],
            [start_val + 7, start_val + 8, start_val + 9],
        ],
        345,
        95,
    )

    # 1  4  7
    # 2  5  8
    # 3  6  9
    b = fixed_array.from_float(
        [
            [start_val + 1, start_val + 4, start_val + 7],
            [start_val + 2, start_val + 5, start_val + 8],
            [start_val + 3, start_val + 6, start_val + 9],
        ],
        345,
        95,
    )

    assert a.transpose().is_identical(b)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_transpose_negative_dim(fixed_array: type[APyCFixedArray]):
    a = fixed_array.from_float([1.0] * 6, bits=5, frac_bits=0).reshape((1, 2, 3))
    assert a.transpose((1, 0, 2)).shape == (2, 1, 3)
    assert a.transpose((-2, -3, -1)).shape == (2, 1, 3)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
def test_swapaxes(fixed_array: type[APyCFixedArray]):
    a = fixed_array.from_float(list(range(12)), int_bits=5, frac_bits=2).reshape((6, 2))

    if not a.swapaxes(0, 1).is_identical(a.T):
        pytest.fail("swapaxes didn't correctly swap axis")

    if not a.swapaxes(1, 0).is_identical(a.T):
        pytest.fail("swapaxes didn't correctly swap axis")

    a = fixed_array.from_float([0] * 24, int_bits=5, frac_bits=2).reshape((4, 3, 2))

    if a.swapaxes(0, 1).shape != (3, 4, 2):
        pytest.fail("swapaxes didn't correctly swap axis")

    if a.swapaxes(1, 0).shape != (3, 4, 2):
        pytest.fail("swapaxes didn't correctly swap axis")

    if a.swapaxes(2, 0).shape != (2, 3, 4):
        pytest.fail("swapaxes didn't correctly swap axis")

    if a.swapaxes(0, 2).shape != (2, 3, 4):
        pytest.fail("swapaxes didn't correctly swap axis")


def test_to_bits():
    #
    # 1-D:
    #
    assert APyFixedArray.from_float(range(4), 4, 0).to_bits() == [0, 1, 2, 3]
    assert APyFixedArray.from_float(range(4), 4, 1).to_bits() == [0, 2, 4, 6]
    assert APyFixedArray.from_float(range(4), int_bits=4, frac_bits=1234).to_bits() == [
        0 * 2**1234,
        1 * 2**1234,
        2 * 2**1234,
        3 * 2**1234,
    ]
    assert APyFixedArray.from_float(
        range(-3, 3), int_bits=4, frac_bits=0
    ).to_bits() == [0b1101, 0b1110, 0b1111, 0b0000, 0b0001, 0b0010]
    assert APyFixedArray.from_float(
        range(-3, 3), int_bits=4, frac_bits=1
    ).to_bits() == [0b11010, 0b11100, 0b11110, 0b00000, 0b00010, 0b00100]

    #
    # 2-D:
    #
    assert APyFixedArray.from_float(
        [[1, 2], [4, 3]], int_bits=4, frac_bits=0
    ).to_bits() == [[1, 2], [4, 3]]
    assert APyFixedArray.from_float(
        [[1, 2], [4, 3]], int_bits=4, frac_bits=1
    ).to_bits() == [[2, 4], [8, 6]]

    #
    # 3-D:
    #
    assert APyFixedArray.from_float(
        [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]], int_bits=4, frac_bits=0
    ).to_bits() == [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]]
    assert APyFixedArray.from_float(
        [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]], int_bits=4, frac_bits=1
    ).to_bits() == [[[2, 4], [6, 8], [10, 12]], [[14, 16], [18, 16], [14, 12]]]

    #
    # Negative values represented using minus sign.
    # This method is not exposed and is already tested indirectly, but CI doesn't see it covered.
    #
    assert APyFixedArray.from_float(
        [-4, 0, 1], int_bits=4, frac_bits=1
    )._to_signed_bits() == [-8, 0, 2]


def test_to_bits_numpy():
    np = pytest.importorskip("numpy")

    #
    # Numpy currently does not support long fixed-point conversions
    #
    with pytest.raises(ValueError, match=r"APyFixedArray::to_bits_ndarray"):
        APyFixedArray([0], int_bits=100, frac_bits=1000).to_bits(True)

    #
    # Correct Numpy dtypes
    #
    assert APyFixedArray([0], int_bits=8, frac_bits=0).to_bits(True).dtype == np.dtype(
        "uint8"
    )
    assert APyFixedArray([0], int_bits=9, frac_bits=0).to_bits(True).dtype == np.dtype(
        "uint16"
    )
    assert APyFixedArray([0], int_bits=16, frac_bits=0).to_bits(True).dtype == np.dtype(
        "uint16"
    )
    assert APyFixedArray([0], int_bits=17, frac_bits=0).to_bits(True).dtype == np.dtype(
        "uint32"
    )
    assert APyFixedArray([0], int_bits=32, frac_bits=0).to_bits(True).dtype == np.dtype(
        "uint32"
    )
    assert APyFixedArray([0], int_bits=33, frac_bits=0).to_bits(True).dtype == np.dtype(
        "uint64"
    )

    #
    # 1-D:
    #
    assert np.all(
        APyFixedArray.from_float(range(4), 4, 0).to_bits(True) == np.array([0, 1, 2, 3])
    )
    assert np.all(
        APyFixedArray.from_float(range(4), 4, 1).to_bits(True) == np.array([0, 2, 4, 6])
    )
    assert np.all(
        APyFixedArray.from_float(range(-3, 3), int_bits=4, frac_bits=0).to_bits(True)
        == np.array([0b1101, 0b1110, 0b1111, 0b0000, 0b0001, 0b0010])
    )
    assert np.all(
        APyFixedArray.from_float(range(-3, 3), int_bits=4, frac_bits=1).to_bits(True)
        == np.array([0b11010, 0b11100, 0b11110, 0b00000, 0b00010, 0b00100])
    )

    #
    # 2-D:
    #
    assert np.all(
        APyFixedArray.from_float([[1, 2], [4, 3]], int_bits=4, frac_bits=0).to_bits(
            True
        )
        == np.array([[1, 2], [4, 3]])
    )
    assert np.all(
        APyFixedArray.from_float([[1, 2], [4, 3]], int_bits=4, frac_bits=1).to_bits(
            True
        )
        == np.array([[2, 4], [8, 6]])
    )

    #
    # 3-D:
    #
    assert np.all(
        APyFixedArray.from_float(
            [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]],
            int_bits=4,
            frac_bits=0,
        ).to_bits(True)
        == np.array([[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]])
    )
    assert np.all(
        APyFixedArray.from_float(
            [[[1, 2], [3, 4], [5, 6]], [[7, 8], [9, 8], [7, 6]]],
            int_bits=4,
            frac_bits=1,
        ).to_bits(True)
        == np.array([[[2, 4], [6, 8], [10, 12]], [[14, 16], [18, 16], [14, 12]]])
    )


def test_copy():
    a = APyFixedArray([3, 1], 4, 5)
    b = a
    assert a.is_identical(b)
    c = a.copy()
    assert a.is_identical(c)
    a[0] = APyFixed(2, 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


def test_python_copy():
    import copy

    a = APyFixedArray([3, 1], 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.copy(a)
    assert a.is_identical(c)
    a[0] = APyFixed(2, 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


def test_python_deepcopy():
    import copy

    a = APyFixedArray([3, 1], 4, 5)
    b = a
    assert a.is_identical(b)
    c = copy.deepcopy(a)
    assert a.is_identical(c)
    a[0] = APyFixed(2, 4, 5)
    assert a.is_identical(b)
    assert not a.is_identical(c)


@pytest.mark.parametrize("fixed_array", [APyFixedArray, APyCFixedArray])
@pytest.mark.parametrize("bits", [16, 32, 64, 128])
def test_issue_615(fixed_array: type[APyCFixedArray], bits: int):
    """
    Test for GitHub issue #615: `APyFixedArray.cumsum()` bug when source operand fit in
    single limb, but the result requires more than one limb
    """
    A_fx = fixed_array.from_float([1, 2, -3, 4], bits=bits, frac_bits=bits // 2)
    assert A_fx.cumsum().is_identical(
        fixed_array.from_float([1, 3, 0, 4], bits=bits + 2, frac_bits=bits // 2)
    )

    A_fx = fixed_array.from_float([1, 2, -3, 4], bits=bits, frac_bits=bits // 2)
    extra_bits_complex = 3 if fixed_array == APyCFixedArray else 0
    assert A_fx.cumprod().is_identical(
        fixed_array.from_float(
            [1, 2, -6, -24], bits=4 * bits + extra_bits_complex, frac_bits=2 * bits
        )
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
@pytest.mark.parametrize("bits", [16, 32, 64, 128])
def test_issue_623(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed], bits: int
):
    """
    Test for GitHub issue #623: `cumsum` (and `cumprod`) of empty array crashing. Should
    use same semantics as Numpy.
    """
    int_bits = bits // 2
    frac_bits = bits // 2

    empty = fixed_array.from_float([], int_bits=int_bits, frac_bits=frac_bits)
    zero_scalar = fixed_scalar(0, int_bits=int_bits, frac_bits=frac_bits)
    one_scalar = fixed_scalar.from_float(1.0, int_bits=int_bits, frac_bits=frac_bits)
    assert empty.sum().is_identical(zero_scalar)
    assert empty.sum(0).is_identical(zero_scalar)
    assert empty.prod().is_identical(one_scalar)
    assert empty.prod(0).is_identical(one_scalar)
    assert empty.cumsum().is_identical(empty)
    assert empty.cumsum(0).is_identical(empty)
    assert empty.cumprod().is_identical(empty)
    assert empty.cumprod(0).is_identical(empty)


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_array_is_not_identical_to_scalar(
    fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]
):
    assert not fixed_array.from_float([-2.5], 10, 10).is_identical(
        fixed_scalar.from_float(-2.5, 10, 10)
    )


@pytest.mark.parametrize(
    ("fixed_array", "fixed_scalar"),
    [
        (APyFixedArray, APyFixed),
        (APyCFixedArray, APyCFixed),
    ],
)
def test_issue_683(fixed_array: type[APyCFixedArray], fixed_scalar: type[APyCFixed]):
    a = fixed_scalar.from_float(3, int_bits=15, frac_bits=60)
    b = fixed_scalar.from_float(3, int_bits=15, frac_bits=62)
    c = b.cast(int_bits=7, frac_bits=30)
    d = b.cast(int_bits=7, frac_bits=31)

    arr0 = fixed_array.from_float([a], int_bits=7, frac_bits=30)
    arr1 = fixed_array.from_float([b], int_bits=7, frac_bits=31)
    arr2 = fixed_array.from_float([c], int_bits=7, frac_bits=30)
    arr3 = fixed_array.from_float([d], int_bits=7, frac_bits=31)

    assert arr0[0].is_identical(fixed_scalar.from_float(3, int_bits=7, frac_bits=30))
    assert arr1[0].is_identical(fixed_scalar.from_float(3, int_bits=7, frac_bits=31))
    assert arr2[0].is_identical(fixed_scalar.from_float(3, int_bits=7, frac_bits=30))
    assert arr3[0].is_identical(fixed_scalar.from_float(3, int_bits=7, frac_bits=31))
