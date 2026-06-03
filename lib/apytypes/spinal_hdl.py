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

    f.write("import spinal.core._\n\n")
    f.write(f"class {entity} extends Component {{\n")
    f.write("  val io = new Bundle {\n")
    f.write(f"    val address = in UInt({address_bits} bits)\n")
    f.write(f"    val data = out Bits({data_bits} bits)\n")
    f.write("  }\n\n")

    f.write(f"  val addressTmp = UInt({address_bits} bits)\n")
    if input_register:
        f.write("  val addressReg = RegNext(io.address)\n")
        f.write("  addressTmp := addressReg\n")
    else:
        f.write("  addressTmp := io.address\n")

    f.write(f"\n  val dataComb = Bits({data_bits} bits)\n")
    f.write(f"  dataComb := B(0, {data_bits} bits)\n\n")
    f.write("  switch(addressTmp) {\n")

    for i in range(n):
        a = _bits(address_bits, address_values[i])
        d = _bits(data_bits, table_values[i])
        f.write(
            f'    is(U(BigInt("{a}", 2), {address_bits} bits)) {{ '
            f'dataComb := B(BigInt("{d}", 2), {data_bits} bits) }}\n'
        )

    f.write("  }\n\n")

    if output_register:
        f.write("  io.data := RegNext(dataComb)\n")
    else:
        f.write("  io.data := dataComb\n")

    f.write("}\n")
