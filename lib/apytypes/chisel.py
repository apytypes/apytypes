import math
from pathlib import Path
from typing import TextIO

from apytypes._apytypes import APyFixedArray, APyFloatArray


def generate_rom(
    table: APyFixedArray | APyFloatArray,
    entity: str | None = None,
    address: APyFixedArray | APyFloatArray | None = None,
    input_register: bool = False,
    output_register: bool = False,
    file: TextIO | None = None,
) -> None:
    table = table.squeeze()
    if table.ndim != 1:
        raise TypeError("Can only create ROMs for 1D-tables.")

    n = table.shape[0]
    address_bits = (
        address.bits if address is not None else max(1, math.ceil(math.log2(n)))
    )
    entity = entity or f"ROM_{address_bits}"

    if file:
        _write_rom(
            file,
            table,
            n,
            address_bits,
            address,
            entity,
            input_register,
            output_register,
        )
    else:
        with Path(entity + ".scala").open("w") as f:
            _write_rom(
                f,
                table,
                n,
                address_bits,
                address,
                entity,
                input_register,
                output_register,
            )


def _bits(width: int, value: int) -> str:
    return f"{int(value) & ((1 << width) - 1):0{width}b}"


def _write_rom(
    f: TextIO,
    table: APyFixedArray | APyFloatArray,
    n: int,
    address_bits: int,
    address: APyFixedArray | APyFloatArray | None,
    entity: str,
    input_register: bool,
    output_register: bool,
) -> None:
    table_values = table.to_bits()
    data_bits = table.bits

    if address is None:
        address_values = range(n)
    else:
        address = address.squeeze()
        if address.shape != table.shape:
            raise TypeError(
                "table and address must have the same shape (after squeeze)"
            )
        address_values = address.to_bits()

    f.write("import chisel3._\n")
    f.write("import chisel3.util._\n\n")
    f.write(f"class {entity} extends Module {{\n")
    f.write("  val io = IO(new Bundle {\n")
    f.write(f"    val address = Input(UInt({address_bits}.W))\n")
    f.write(f"    val data = Output(UInt({data_bits}.W))\n")
    f.write("  })\n\n")

    if input_register:
        f.write("  val addressTmp = RegNext(io.address)\n")
    else:
        f.write("  val addressTmp = io.address\n")

    f.write(f"  val dataComb = Wire(UInt({data_bits}.W))\n")
    f.write(f"  dataComb := 0.U({data_bits}.W)\n\n")
    f.write("  switch(addressTmp) {\n")

    for i in range(n):
        a = _bits(address_bits, address_values[i])
        d = _bits(data_bits, table_values[i])
        f.write(
            f'    is("b{a}".U({address_bits}.W)) {{ dataComb := "b{d}".U({data_bits}.W) }}\n'
        )

    f.write("  }\n\n")

    if output_register:
        f.write("  io.data := RegNext(dataComb)\n")
    else:
        f.write("  io.data := dataComb\n")

    f.write("}\n")
