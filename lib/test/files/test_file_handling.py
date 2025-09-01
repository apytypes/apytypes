import difflib
import shutil
import subprocess
from pathlib import Path

import pytest

from apytypes import (
    APyCFixedArray,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
    export_csv,
    import_csv,
)


def test_import_csv():
    """Test importing from known CSV files."""
    curr_dir = Path(__file__).parent
    # Test raises
    with pytest.raises(
        ValueError,
        match="Could not determine array type",
    ):
        import_csv(
            curr_dir / "apyfixed_bits_1d.csv", int_bits=4, frac_bits=0, exp_bits=5
        )
    with pytest.raises(
        ValueError,
        match="Complex data types are not supported yet",
    ):
        import_csv(
            curr_dir / "apyfixed_bits_1d.csv",
            int_bits=4,
            frac_bits=0,
            force_complex=True,
        )

    # Handling of CSV files are not tested since Python's csv module is used,
    # this includes missing file raises, testing different delimiters, etc.

    arr = import_csv(curr_dir / "apyfixed_bits_1d.csv", bits=71, frac_bits=20)

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

    arr = import_csv(curr_dir / "apyfloat_bits_1d.csv", exp_bits=21, man_bits=50)

    assert arr.is_identical(
        APyFloatArray.from_bits(
            [
                # Large bit patterns and special values
                0,
                4722365356969738371201,
                2361182115534915764224,
                26136640437445230472,
                1180591339242434592768,
            ],
            exp_bits=21,
            man_bits=50,
        )
    )

    arr = import_csv(curr_dir / "apyfixed_bits_2d.csv", bits=4, frac_bits=1)

    assert arr.is_identical(
        APyFixedArray(range(2 * 3 * 4), bits=4, frac_bits=1).reshape((6, 4))
    )

    arr = import_csv(curr_dir / "apyfloat_bits_2d.csv", exp_bits=8, man_bits=12)

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

    arr = import_csv(curr_dir / "apyfixed_bits_3d.csv", bits=4, frac_bits=1)
    assert arr.shape == (2, 4 * 3)
    arr = arr.reshape((2, 3, 4))
    assert arr.is_identical(
        APyFixedArray(range(2 * 3 * 4), bits=4, frac_bits=1).reshape((2, 3, 4))
    )


def test_export_csv(tmp_path):
    """Test exporting to CSV files, should match the existing files."""
    curr_dir = Path(__file__).parent

    def compare_files(file1, file2):
        """Check if two files are identical, ignoring whitespace."""
        with Path.open(file1, newline="") as f, Path.open(file2, newline="") as g:
            flines = [line.replace(" ", "").rstrip() for line in f]
            glines = [line.replace(" ", "").rstrip() for line in g]
            # Potential empty last lines are fine and should not be checked
            # if flines[-1] == "":
            #     flines.pop()
            # if glines[-1] == "":
            #     glines.pop()
            print(f"after pop ({file2})")
            print(flines)
            print(glines)
            assert "".join(difflib.context_diff(flines, glines, n=0)) == ""

    def test_export(arr, ref_file, vunit=True):
        export_csv(arr, tmp_path / ref_file, vunit=vunit)
        compare_files(tmp_path / ref_file, curr_dir / ref_file)

    # Test raises
    with pytest.raises(
        ValueError,
        match="Complex data types are not supported",
    ):
        export_csv(APyCFixedArray([0, 1, 2, 3], 4, 3), tmp_path / "test.csv")

    arr = APyFixedArray(
        [
            # Large bit patterns, VUnit-like
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
    test_export(arr, "apyfixed_bits_1d.csv")

    arr = APyFixedArray(
        [
            # Large bit patterns, like how .to_bits behaves
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
    test_export(arr, "apyfixed_bits_1d_apy.csv", vunit=False)

    arr = APyFloatArray.from_bits(
        [
            # Large bit patterns and special values
            0,
            4722365356969738371201,
            2361182115534915764224,
            26136640437445230472,
            1180591339242434592768,
        ],
        exp_bits=21,
        man_bits=50,
    )
    print(f"arr: {arr}")
    print(f"arr: {arr!r}")
    print(f"arr: {arr.to_bits()}")
    a = APyFloat.from_bits(0, 21, 50)
    print(f"a: {a!r}")
    print(f"arr[0]: {arr[0]!r}")
    test_export(arr, "apyfloat_bits_1d.csv")

    arr = APyFixedArray(range(2 * 3 * 4), bits=4, frac_bits=1).reshape(
        (6, 4)
    )  # Height, width
    test_export(arr, "apyfixed_bits_2d.csv")

    arr = APyFloatArray(
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
    test_export(arr, "apyfloat_bits_2d.csv")

    arr = APyFixedArray(range(2 * 3 * 4), bits=4, frac_bits=1).reshape(
        (2, 3, 4)
    )  # Depth, height, width
    test_export(arr, "apyfixed_bits_3d.csv")


@pytest.mark.skipif(
    not shutil.which("nvc") and not shutil.which("vsim"),
    reason="neither nvc or vunit not found",
)
def test_vunit_csv_import():
    """Test importing CSV files in VUnit. We don't test exporting files from VUnit as we expect it match the read behaviour."""
    pytest.importorskip("vunit")
    subprocess.run(["python", "run.py"], cwd=Path(__file__).parent, check=True)
