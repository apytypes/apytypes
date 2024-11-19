from apytypes.vhdl import generate_rom
from apytypes import APyFixedArray


def test_generate_rom():
    a = APyFixedArray([1, 2, 3, 4], 4, 0)
    generate_rom(a)
