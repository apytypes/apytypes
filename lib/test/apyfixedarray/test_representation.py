from apytypes import APyFixedArray


def test_array_representation():
    assert (
        APyFixedArray([], bits=1, int_bits=-5).__repr__()
        == "APyFixedArray([], shape=(0,), bits=1, int_bits=-5)"
    )
    assert (
        APyFixedArray([1, 2, 3], bits=2, int_bits=2).__repr__()
        == "APyFixedArray([1, 2, 3], shape=(3,), bits=2, int_bits=2)"
    )
    assert (
        APyFixedArray([range(5, 8), range(3)], bits=10, int_bits=20).__repr__()
        == "APyFixedArray([5, 6, 7, 0, 1, 2], shape=(2, 3), bits=10, int_bits=20)"
    )
    assert (
        APyFixedArray([[range(5, 8), range(3)]], bits=64, int_bits=32).__repr__()
        == "APyFixedArray([5, 6, 7, 0, 1, 2], shape=(1, 2, 3), bits=64, int_bits=32)"
    )
    assert (
        APyFixedArray([[0, -1, -2, 1, 2]], bits=10, int_bits=0).__repr__()
        == "APyFixedArray([0, 1023, 1022, 1, 2], shape=(1, 5), bits=10, int_bits=0)"
    )
    assert (
        APyFixedArray([[0, -1, -2, 1, 2]], bits=11, int_bits=-5).__repr__()
        == "APyFixedArray([0, 2047, 2046, 1, 2], shape=(1, 5), bits=11, int_bits=-5)"
    )
    assert (
        APyFixedArray([[range(5, 8), range(3)]], bits=1000, int_bits=500).__repr__()
        == "APyFixedArray([5, 6, 7, 0, 1, 2], shape=(1, 2, 3), bits=1000, int_bits=500)"
    )
