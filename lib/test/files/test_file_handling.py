from pathlib import Path

import pytest

from apytypes import (
    APyFixedArray,
    APyFloatArray,
    loadtxt_bits,
)


def test_loadtxt_bits():
    curr_dir = Path(__file__).parent
    # Test raises
    with pytest.raises(
        ValueError,
        match="Could not determine array type",
    ):
        loadtxt_bits(
            curr_dir / "apyfixed_bits_1d.csv", int_bits=4, frac_bits=0, exp_bits=5
        )

    # Handling of CSV files are not tested since Python's csv module is used,
    # this includes missing file raises, testing different delimiters, etc.

    arr = loadtxt_bits(curr_dir / "apyfixed_bits_1d.csv", bits=71, frac_bits=20)

    assert arr.is_identical(
        APyFixedArray(
            [
                # Large bit patterns
                0,
                1,
                2,
                3,
                551819442767522013199730846,
                2162412574424356538749,
            ],
            bits=71,
            frac_bits=20,
        )
    )

    arr = loadtxt_bits(
        curr_dir / "apyfloat_bits_1d.csv", exp_bits=21, man_bits=50, bias=262143
    )

    assert arr.is_identical(
        APyFloatArray.from_bits(
            [
                # Large bit patterns and special values
                0,
                2361183241434822606976,
                2387565328061838810259,
                295147623704376115200,
                2361182115534915764224,
                2361182115534915764227,
            ],
            exp_bits=21,
            man_bits=50,
            bias=262143,
        )
    )

    arr = loadtxt_bits(curr_dir / "apyfixed_bits_2d.csv", bits=10, frac_bits=18)

    assert arr.is_identical(
        APyFixedArray(
            [
                [786, 849, 811, 1012, 300],
                [336, 300, 142, 913, 118],
                [507, 36, 146, 284, 786],
                [852, 665, 1000, 241, 994],
                [706, 115, 922, 925, 664],
            ],
            int_bits=-8,
            frac_bits=18,
        )
    )

    arr = loadtxt_bits(curr_dir / "apyfloat_bits_2d.csv", exp_bits=8, man_bits=12)

    assert arr.is_identical(
        APyFloatArray(
            [
                [0, 0, 0, 1, 0],
                [1, 0, 0, 1, 0],
                [0, 1, 1, 1, 0],
                [0, 0, 0, 1, 0],
                [1, 0, 1, 0, 0],
            ],
            [
                [129, 129, 129, 130, 130],
                [128, 127, 130, 129, 129],
                [127, 127, 127, 130, 126],
                [127, 130, 130, 128, 128],
                [127, 129, 129, 126, 128],
            ],
            [
                [571, 2028, 2888, 58, 1020],
                [1532, 3333, 247, 2573, 1215],
                [3731, 3583, 3024, 223, 3552],
                [3587, 1019, 129, 1645, 672],
                [4029, 2176, 817, 910, 3428],
            ],
            exp_bits=8,
            man_bits=12,
        )
    )
