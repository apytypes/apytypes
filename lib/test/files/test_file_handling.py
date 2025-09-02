import difflib
import shutil
import subprocess
from pathlib import Path

import pytest

from apytypes import (
    APyCFixedArray,
    APyFixed,
    APyFixedArray,
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

    with pytest.raises(
        ValueError,
        match="Unknown layout",
    ):
        import_csv(
            curr_dir / "apyfixed_bits_1d.csv",
            int_bits=4,
            frac_bits=0,
            layout="abc123",
        )

    with pytest.raises(
        ValueError,
        match="vunit_3d_shape provided",
    ):
        import_csv(
            curr_dir / "apyfixed_bits_2d.csv",
            int_bits=4,
            frac_bits=0,
            vunit_3d_shape=(1, 2, 3),
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


def test_export_csv(tmp_path):
    """Test exporting to CSV files, should match the existing files."""
    curr_dir = Path(__file__).parent

    def compare_files(file1, file2):
        """Check if two files are identical, ignoring whitespace."""
        with Path.open(file1, newline="") as f, Path.open(file2, newline="") as g:
            flines = [line.replace(" ", "").rstrip() for line in f]
            glines = [line.replace(" ", "").rstrip() for line in g]
            assert "".join(difflib.context_diff(flines, glines, n=0)) == ""

    def test_export(arr, ref_file, layout="default"):
        export_csv(arr, tmp_path / ref_file, layout=layout)
        compare_files(tmp_path / ref_file, curr_dir / ref_file)

    # Test raises
    with pytest.raises(
        ValueError,
        match="Complex data types are not supported",
    ):
        export_csv(APyCFixedArray([0, 1, 2, 3], 4, 3), tmp_path / "test.csv")

    with pytest.raises(
        ValueError,
        match="Unknown layout",
    ):
        export_csv(
            APyFixedArray([0, 1, 2, 3], 4, 3),
            curr_dir / "test.csv",
            layout="abc123",
        )

    with pytest.raises(
        ValueError,
        match="Unsupported array dimension",
    ):
        export_csv(
            APyFixedArray(range(3**3), 4, 3).reshape((3, 3, 3)),
            curr_dir / "test.csv",
        )

    with pytest.raises(
        ValueError,
        match="Unsupported array dimension",
    ):
        export_csv(
            APyFixedArray(range(2**4), 4, 3).reshape((2, 2, 2, 2)),
            curr_dir / "test.csv",
            layout="vunit",
        )

    # Handling of CSV files are not tested since Python's csv module is used,
    # this includes missing file raises, testing different delimiters, etc.

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
    test_export(arr, "apyfixed_bits_1d.csv")

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
    test_export(arr, "apyfloat_bits_1d.csv")

    arr = APyFixedArray(range(2 * 3 * 4), bits=4, frac_bits=1).reshape((6, 4))
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


@pytest.mark.skipif(
    not shutil.which("nvc") and not shutil.which("vsim"),
    reason="neither nvc or vunit not found",
)
def test_vunit_csv_1d_layout(tmp_path):
    """Test that the index order is the same in VUnit as in APyTypes. Also check that negative values are handled correctly."""
    pytest.importorskip("vunit")

    arr = APyFixedArray(range(8), 2, 1)

    # Check index order
    assert arr[0].is_identical(APyFixed(0, 2, 1))
    assert arr[1].is_identical(APyFixed(1, 2, 1))
    assert arr[4].is_identical(APyFixed(4, 2, 1))
    assert arr[7].is_identical(APyFixed(7, 2, 1))

    fname = tmp_path / "apyfixed_bits_1d_vunit.csv"
    export_csv(arr, fname, layout="vunit")

    # Try reading back the CSV
    arr_csv = import_csv(fname, int_bits=2, frac_bits=1)
    assert arr_csv.is_identical(arr)

    # Check that the index order is the same in VUnit
    subprocess.run(
        ["python", "run.py", "lib.tb_same_indexing_csv.Case 1D", "--csv-file", fname],
        cwd=Path(__file__).parent,
        check=True,
    )


@pytest.mark.skipif(
    not shutil.which("nvc") and not shutil.which("vsim"),
    reason="neither nvc or vunit not found",
)
def test_vunit_csv_2d_layout(tmp_path):
    """Test that one gets the same 2D index order in VUnit as in APyTypes."""
    pytest.importorskip("vunit")

    arr = APyFixedArray(range(2 * 3 * 4), 6, 0).reshape((6, 4))

    # Check index order
    assert arr[0, 1].is_identical(APyFixed(1, 6, 0))
    assert arr[1, 0].is_identical(APyFixed(4, 6, 0))
    assert arr[1, 1].is_identical(APyFixed(5, 6, 0))
    assert arr[5, 3].is_identical(APyFixed(23, 6, 0))

    fname = tmp_path / "apyfixed_bits_2d_vunit.csv"
    export_csv(arr, fname, layout="vunit")

    # Try reading back the CSV
    arr_csv = import_csv(fname, bits=6, frac_bits=0, layout="vunit")
    assert arr_csv.is_identical(arr)

    # Check that the index order is the same in VUnit
    subprocess.run(
        ["python", "run.py", "lib.tb_same_indexing_csv.Case 2D", "--csv-file", fname],
        cwd=Path(__file__).parent,
        check=True,
    )


@pytest.mark.skipif(
    not shutil.which("nvc") and not shutil.which("vsim"),
    reason="neither nvc or vunit not found",
)
def test_vunit_csv_3d_layout(tmp_path):
    """Test that one gets the same 3D index order in VUnit as in APyTypes."""
    pytest.importorskip("vunit")

    arr = APyFixedArray(range(2 * 3 * 4), 6, 0).reshape((2, 3, 4))

    # Check index order
    assert arr[0, 0, 1].is_identical(APyFixed(1, 6, 0))
    assert arr[0, 1, 0].is_identical(APyFixed(4, 6, 0))
    assert arr[0, 1, 1].is_identical(APyFixed(5, 6, 0))
    assert arr[1, 0, 0].is_identical(APyFixed(12, 6, 0))
    assert arr[1, 0, 1].is_identical(APyFixed(13, 6, 0))
    assert arr[1, 1, 0].is_identical(APyFixed(16, 6, 0))
    assert arr[1, 1, 1].is_identical(APyFixed(17, 6, 0))
    assert arr[1, 2, 3].is_identical(APyFixed(23, 6, 0))

    fname = tmp_path / "apyfixed_bits_3d_vunit.csv"
    export_csv(arr, fname, layout="vunit")

    # Try reading back the CSV
    arr_csv = import_csv(
        fname, bits=6, frac_bits=0, layout="vunit", vunit_3d_shape=arr.shape
    )
    assert arr_csv.is_identical(arr)

    # Check that the index order is the same in VUnit
    subprocess.run(
        ["python", "run.py", "lib.tb_same_indexing_csv.Case 3D", "--csv-file", fname],
        cwd=Path(__file__).parent,
        check=True,
    )
