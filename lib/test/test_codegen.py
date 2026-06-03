import importlib.util
import shutil
import subprocess
import sys
from itertools import product

import pytest

import apytypes.amaranth
import apytypes.chisel
import apytypes.clash
import apytypes.spade
import apytypes.spinal_hdl
import apytypes.verilog
import apytypes.vhdl
from apytypes import APyFixedArray, APyFloatArray

TABLES = [
    APyFixedArray([1, 2, 3, 4], 4, 0),
    APyFloatArray.from_float([0.2, 0.3, 0.4, 0.7], 4, 6),
]

ADDRESSES = [
    None,
    APyFixedArray([1, 2, 3, 4], 4, 0),
    APyFloatArray.from_float([0.2, 0.3, 0.4, 0.7], 4, 6),
]


@pytest.mark.skipif(not shutil.which("nvc"), reason="nvc not found")
@pytest.mark.parametrize(
    ("table", "address", "ieee2008", "input_register", "output_register"),
    product(
        TABLES,
        ADDRESSES,
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
        apytypes.vhdl.generate_rom(
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
        TABLES,
        ADDRESSES,
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
        apytypes.vhdl.generate_rom(
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


@pytest.mark.skipif(not shutil.which("iverilog"), reason="iverilog not found")
@pytest.mark.parametrize(
    ("table", "address", "systemverilog", "input_register", "output_register"),
    product(
        TABLES,
        ADDRESSES,
        [True, False],
        [True, False],
        [True, False],
    ),
)
def test_generate_rom_iverilog(
    table, address, systemverilog, input_register, output_register, tmp_path
):
    suffix = ".sv" if systemverilog else ".v"
    filepath = tmp_path / f"testrom{suffix}"

    with filepath.open("w") as f:
        apytypes.verilog.generate_rom(
            table,
            entity="testrom",
            address=address,
            input_register=input_register,
            output_register=output_register,
            systemverilog=systemverilog,
            file=f,
        )

    cmd = ["iverilog"]
    if systemverilog:
        cmd.append("-g2012")
    cmd.extend(["-o", str(tmp_path / "testrom.out"), str(filepath)])

    subprocess.run(cmd, check=True)


@pytest.mark.skipif(not shutil.which("verilator"), reason="verilator not found")
@pytest.mark.parametrize(
    ("table", "address", "systemverilog", "input_register", "output_register"),
    product(
        TABLES,
        ADDRESSES,
        [True, False],
        [True, False],
        [True, False],
    ),
)
def test_generate_rom_verilator(
    table, address, systemverilog, input_register, output_register, tmp_path
):
    suffix = ".sv" if systemverilog else ".v"
    filepath = tmp_path / f"testrom{suffix}"

    with filepath.open("w") as f:
        apytypes.verilog.generate_rom(
            table,
            entity="testrom",
            address=address,
            input_register=input_register,
            output_register=output_register,
            systemverilog=systemverilog,
            file=f,
        )

    cmd = ["verilator", "--lint-only", "--top-module", "testrom", str(filepath)]
    if systemverilog:
        cmd.insert(1, "--sv")

    subprocess.run(cmd, check=True)


@pytest.mark.skipif(not shutil.which("scala-cli"), reason="scala-cli not found")
@pytest.mark.parametrize(
    ("table", "address", "input_register", "output_register"),
    product(TABLES, ADDRESSES, [True, False], [True, False]),
)
def test_generate_rom_chisel(table, address, input_register, output_register, tmp_path):
    filepath = tmp_path / "testrom.scala"

    with filepath.open("w") as f:
        apytypes.chisel.generate_rom(
            table,
            entity="testrom",
            address=address,
            input_register=input_register,
            output_register=output_register,
            file=f,
        )

    subprocess.run(
        [
            "scala-cli",
            "compile",
            str(filepath),
            "--server=false",
            "--scala",
            "2.13.12",
            "--dependency",
            "org.chipsalliance::chisel:6.5.0",
        ],
        check=True,
    )


@pytest.mark.skipif(
    importlib.util.find_spec("amaranth") is None,
    reason="amaranth not found",
)
@pytest.mark.parametrize(
    ("table", "address", "input_register", "output_register"),
    product(TABLES, ADDRESSES, [True, False], [True, False]),
)
def test_generate_rom_amaranth(
    table, address, input_register, output_register, tmp_path
):
    filepath = tmp_path / "testrom.py"

    with filepath.open("w") as f:
        apytypes.amaranth.generate_rom(
            table,
            entity="testrom",
            address=address,
            input_register=input_register,
            output_register=output_register,
            file=f,
        )

    subprocess.run([sys.executable, "-m", "py_compile", str(filepath)], check=True)


@pytest.mark.skipif(not shutil.which("scala-cli"), reason="scala-cli not found")
@pytest.mark.parametrize(
    ("table", "address", "input_register", "output_register"),
    product(TABLES, ADDRESSES, [True, False], [True, False]),
)
def test_generate_rom_spinalhdl(
    table, address, input_register, output_register, tmp_path
):
    filepath = tmp_path / "testrom.scala"

    with filepath.open("w") as f:
        apytypes.spinal_hdl.generate_rom(
            table,
            entity="testrom",
            address=address,
            input_register=input_register,
            output_register=output_register,
            file=f,
        )

    subprocess.run(
        [
            "scala-cli",
            "compile",
            str(filepath),
            "--server=false",
            "--scala",
            "2.13.12",
            "--dependency",
            "com.github.spinalhdl::spinalhdl-core:1.10.2",
        ],
        check=True,
    )


@pytest.mark.skipif(not shutil.which("spade"), reason="spade not found")
@pytest.mark.parametrize(
    ("table", "address", "input_register", "output_register"),
    product(
        TABLES,
        ADDRESSES,
        [True, False],
        [True, False],
    ),
)
def test_generate_rom_spade(table, address, input_register, output_register, tmp_path):
    filepath = tmp_path / "testrom.spade"

    with filepath.open("w") as f:
        apytypes.spade.generate_rom(
            table,
            entity="testrom",
            address=address,
            input_register=input_register,
            output_register=output_register,
            file=f,
        )

    subprocess.run(
        ["spade", "-o", "/dev/null", str(filepath)],
        check=True,
    )


@pytest.mark.skipif(not shutil.which("clash"), reason="clash not found")
@pytest.mark.parametrize(
    ("table", "address", "input_register", "output_register"),
    product(
        TABLES,
        ADDRESSES,
        [True, False],
        [True, False],
    ),
)
def test_generate_rom_clash(table, address, input_register, output_register, tmp_path):
    filepath = tmp_path / "Testrom.hs"

    with filepath.open("w") as f:
        apytypes.clash.generate_rom(
            table,
            entity="testrom",
            address=address,
            input_register=input_register,
            output_register=output_register,
            file=f,
        )

    subprocess.run(
        [
            "clash",
            "--verilog",
            "-fclash-no-cache",
            "-package",
            "ghc-typelits-extra",
            "-package",
            "ghc-typelits-natnormalise",
            "-package",
            "ghc-typelits-knownnat",
            "-outputdir",
            str(tmp_path / "clash"),
            str(filepath),
        ],
        check=True,
    )
