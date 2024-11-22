from apytypes import APyCFixedArray


def test_array_representation():
    assert (
        APyCFixedArray([], bits=1, int_bits=-5).__repr__()
        == "APyCFixedArray([], shape=(0,), bits=1, int_bits=-5)"
    )
    assert (
        APyCFixedArray([1, 2, 3], bits=2, int_bits=2).__repr__()
        == "APyCFixedArray([(1, 0), (2, 0), (3, 0)], shape=(3,), bits=2, int_bits=2)"
    )
    assert (
        APyCFixedArray([range(5, 8), range(3)], bits=10, int_bits=20).__repr__()
        == "APyCFixedArray([(5, 0), (6, 0), (7, 0), (0, 0), (1, 0), (2, 0)], shape=(2, 3), bits=10, int_bits=20)"
    )
    assert (
        APyCFixedArray([[range(5, 8), range(3)]], bits=64, int_bits=32).__repr__()
        == "APyCFixedArray([(5, 0), (6, 0), (7, 0), (0, 0), (1, 0), (2, 0)], shape=(1, 2, 3), bits=64, int_bits=32)"
    )
    assert (
        APyCFixedArray([[0, -1, -2, 1, 2]], bits=10, int_bits=0).__repr__()
        == "APyCFixedArray([(0, 0), (1023, 0), (1022, 0), (1, 0), (2, 0)], shape=(1, 5), bits=10, int_bits=0)"
    )
    assert (
        APyCFixedArray([[0, -1, -2, 1, 2]], bits=11, int_bits=-5).__repr__()
        == "APyCFixedArray([(0, 0), (2047, 0), (2046, 0), (1, 0), (2, 0)], shape=(1, 5), bits=11, int_bits=-5)"
    )
    assert (
        APyCFixedArray([[range(5, 8), range(3)]], bits=1000, int_bits=500).__repr__()
        == "APyCFixedArray([(5, 0), (6, 0), (7, 0), (0, 0), (1, 0), (2, 0)], shape=(1, 2, 3), bits=1000, int_bits=500)"
    )
