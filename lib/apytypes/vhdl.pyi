from typing import TextIO, Union

import apytypes._apytypes
from apytypes._apytypes import (
    APyFixedArray as APyFixedArray,
    APyFloatArray as APyFloatArray,
)

def generate_rom(
    table: Union[apytypes._apytypes.APyFixedArray, apytypes._apytypes.APyFloatArray],
    entity: Union[str, None] = None,
    address: Union[
        apytypes._apytypes.APyFixedArray, apytypes._apytypes.APyFloatArray, None
    ] = None,
    frac_bits: Union[int, None] = None,
    ieee2008: bool = False,
    input_register: bool = False,
    output_register: bool = False,
    file: Union[TextIO, None] = None,
):
    """
    Generate VHDL for a ROM.

    .. versionadded:: 0.3

    Parameters
    ----------
    table : APyFixedArray, APyFloatArray
        The data in the ROM.
    entity : str, optional
        VHDL entity name.
    address : APyFixedArray, APyFloatArray, optional
        Addresses for the ROM. If not provided, sequential addressing from 0 is assumed.
    frac_bits : int, optional
        If *ieee2008* is True and *address* is not provided, use this many fractional bits for address.
    ieee2008 : bool, default: False
        Use ``ieee.fixed_pkg`` and/or ``ieee.float_pkg`` for the resulting ROM.
    input_register : bool, default: False
        If ``True``, a register is added at the input of the ROM.
    output_register : bool, default: False
        If ``True``, a register is added at the output of the ROM.
    file : file handle, optional
        File handle to write the file to. If not provided, a file ``entity.vhdl`` is used.
    """
