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
        with Path(entity + ".py").open("w") as f:
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


def _int(width: int, value: int) -> int:
    return int(value) & ((1 << width) - 1)


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

    f.write("from amaranth import *\n\n\n")
    f.write(f"class {entity}(Elaboratable):\n")
    f.write("    def __init__(self):\n")
    f.write(f"        self.address = Signal({address_bits})\n")
    f.write(f"        self.data = Signal({data_bits})\n\n")
    f.write("    def elaborate(self, platform):\n")
    f.write("        m = Module()\n")
    f.write(f"        address_tmp = Signal({address_bits})\n")
    f.write(f"        data_comb = Signal({data_bits})\n\n")

    if input_register:
        f.write("        m.d.sync += address_tmp.eq(self.address)\n")
    else:
        f.write("        m.d.comb += address_tmp.eq(self.address)\n")

    f.write("        m.d.comb += data_comb.eq(0)\n\n")
    f.write("        with m.Switch(address_tmp):\n")

    for i in range(n):
        a = _int(address_bits, address_values[i])
        d = _int(data_bits, table_values[i])
        f.write(f"            with m.Case({a}):\n")
        f.write(f"                m.d.comb += data_comb.eq({d})\n")

    f.write("            with m.Default():\n")
    f.write("                m.d.comb += data_comb.eq(0)\n\n")

    if output_register:
        f.write("        m.d.sync += self.data.eq(data_comb)\n")
    else:
        f.write("        m.d.comb += self.data.eq(data_comb)\n")

    f.write("        return m\n")
