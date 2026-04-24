import shutil
import subprocess
from itertools import product

import pytest

from apytypes import APyFixedArray, APyFloatArray
from apytypes.vhdl import generate_rom


@pytest.mark.skipif(not shutil.which("nvc"), reason="nvc not found")
@pytest.mark.parametrize(
    ("table", "address", "ieee2008", "input_register", "output_register"),
    product(
        [
            APyFixedArray([1, 2, 3, 4], 4, 0),
            APyFloatArray.from_float([0.2, 0.3, 0.4, 0.7], 4, 6),
        ],
        [
            None,
            APyFixedArray([1, 2, 3, 4], 4, 0),
            APyFloatArray.from_float([0.2, 0.3, 0.4, 0.7], 4, 6),
        ],
        [True, False],
        [True, False],
        [True, False],
    ),
)
def test_generate_rom_nvc(
    table, address, ieee2008, input_register, output_register, tmp_path
):
    filepath = tmp_path / "testrom.vhdl"
    with filepath.open("w") as f:
        generate_rom(
            table,
            entity="testrom",
            address=address,
            ieee2008=ieee2008,
            input_register=input_register,
            output_register=output_register,
            file=f,
        )

    subprocess.run(
        ["nvc", f"--work={tmp_path / 'work'}", "-a", str(filepath), "-e", "testrom"],
        check=True,
    )


@pytest.mark.skipif(not shutil.which("ghdl"), reason="ghdl not found")
@pytest.mark.parametrize(
    ("table", "address", "ieee2008", "input_register", "output_register"),
    product(
        [
            APyFixedArray([1, 2, 3, 4], 4, 0),
            APyFloatArray.from_float([0.2, 0.3, 0.4, 0.7], 4, 6),
        ],
        [
            None,
            APyFixedArray([1, 2, 3, 4], 4, 0),
            APyFloatArray.from_float([0.2, 0.3, 0.4, 0.7], 4, 6),
        ],
        [True, False],
        [True, False],
        [True, False],
    ),
)
def test_generate_rom_ghdl(
    table, address, ieee2008, input_register, output_register, tmp_path
):
    filepath = tmp_path / "testrom.vhdl"
    with filepath.open("w") as f:
        generate_rom(
            table,
            entity="testrom",
            address=address,
            ieee2008=ieee2008,
            input_register=input_register,
            output_register=output_register,
            file=f,
        )

    subprocess.run(
        ["ghdl", "-a", f"--workdir={tmp_path}", "--std=08", str(filepath)],
        check=True,
    )

    subprocess.run(
        ["ghdl", "-e", f"--workdir={tmp_path}", "--std=08", "testrom"],
        check=True,
    )
