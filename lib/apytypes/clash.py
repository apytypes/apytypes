import math
import re
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
        with Path(_module_name(entity) + ".hs").open("w") as f:
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


def _module_name(entity: str) -> str:
    name = re.sub(r"\W+", "_", entity)
    if not name or name[0].isdigit():
        name = "ROM_" + name
    return name[:1].upper() + name[1:]


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
    module = _module_name(entity)

    if address is None:
        address_values = range(n)
    else:
        address = address.squeeze()
        if address.shape != table.shape:
            raise TypeError(
                "table and address must have the same shape (after squeeze)"
            )
        address_values = address.to_bits()

    f.write("{-# LANGUAGE DataKinds #-}\n")
    f.write("{-# LANGUAGE NoImplicitPrelude #-}\n\n")
    f.write(f"module {module} where\n\n")
    f.write("import Clash.Prelude\n\n")

    if input_register or output_register:
        f.write(
            f"""topEntity
  :: Clock System
  -> Reset System
  -> Enable System
  -> Signal System (Unsigned {address_bits})
  -> Signal System (BitVector {data_bits})
topEntity clk rst en address =
  exposeClockResetEnable circuit clk rst en address

circuit
  :: HiddenClockResetEnable System
  => Signal System (Unsigned {address_bits})
  -> Signal System (BitVector {data_bits})
circuit address = dataOut
 where
"""
        )
        if input_register:
            f.write("  addressTmp = register 0 address\n")
        else:
            f.write("  addressTmp = address\n")

        f.write("  dataComb = fmap romf addressTmp\n")

        if output_register:
            f.write("  dataOut = register 0 dataComb\n\n")
        else:
            f.write("  dataOut = dataComb\n\n")
    else:
        f.write(
            f"""topEntity :: Unsigned {address_bits} -> BitVector {data_bits}
topEntity address = romf address

"""
        )

    f.write(f"romf :: Unsigned {address_bits} -> BitVector {data_bits}\n")
    f.write("romf address = case address of\n")

    for i in range(n):
        a = _int(address_bits, address_values[i])
        d = _int(data_bits, table_values[i])
        f.write(f"  {a} -> {d}\n")

    f.write('  _ -> errorX "undefined ROM address"\n')
