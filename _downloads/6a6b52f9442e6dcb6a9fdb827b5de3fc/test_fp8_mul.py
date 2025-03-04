"""
cocotb integration
==================

Reference models and code for verification can be written swiftly using APyTypes, and incorporated using cocotb.
Below is an exhaustive test of an eight-bit floating-point multiplier with round to nearest, ties to even.

Both source files, ``test_fp8_mul.py`` and ``fp8_multiplier.vhdl``, are available under examples_.

.. _examples: https://github.com/apytypes/apytypes/tree/main/examples
"""

import os
from pathlib import Path
import shutil
import cocotb
from cocotb.clock import Clock
from cocotb.runner import get_runner
from cocotb.triggers import FallingEdge
from apytypes import APyFloat, APyFloatArray


@cocotb.test()
async def fp8_mul_test_all(dut):
    """
    Exhaustive test of all inputs.
    """
    EXP_BITS, MAN_BITS = 4, 3
    fp8_values = APyFloatArray.from_bits(
        range(2 ** (1 + EXP_BITS + MAN_BITS)), exp_bits=EXP_BITS, man_bits=MAN_BITS
    )

    clock = Clock(dut.clk, 10, units="ns")
    cocotb.start_soon(clock.start(start_high=False))

    for x in fp8_values:
        dut.x_in.value = x.to_bits()
        for y in fp8_values:
            dut.y_in.value = y.to_bits()

            await FallingEdge(dut.clk)  # Input is set on falling clock edge
            await FallingEdge(dut.clk)  # Wait another clock cycle for the result

            z_dut = APyFloat.from_bits(
                int(dut.z_out.value), exp_bits=EXP_BITS, man_bits=MAN_BITS
            )

            z_ref = x * y

            # If the expected result is NaN, do not test for bit-exact representations
            if z_ref.is_nan:
                assert z_dut.is_nan
            else:
                assert int(dut.z_out.value) == z_ref.to_bits(), (
                    f"{z_dut!r}({z_dut}) == {z_ref!r}({z_ref})\n{x=}({x}), {y=}({y})"
                )


# %%
# The rest of the testbench is set up as one would normally do using cocotb.


def test_fp8_mul():
    """
    Simulate the floating-point multiplier using the Python runner.

    This file can be run directly or via pytest discovery.
    """
    sim = os.getenv("SIM", "nvc")

    dir_path = (
        os.path.dirname(os.path.realpath(__file__))
        if "__file__" in globals()
        else Path.cwd()
    )
    sources = [f"{dir_path}/fp8_multiplier.vhdl"]

    runner = get_runner(sim)
    runner.build(
        sources=sources,
        hdl_toplevel="fp8_multiplier",
    )

    runner.test(hdl_toplevel="fp8_multiplier", test_module="test_fp8_mul")


if __name__ == "__main__":
    if shutil.which("nvc"):
        test_fp8_mul()
    else:
        print("NVC not found")

# %%
# The contents of ``fp8_multiplier.vhdl`` is shown below.
#
# .. literalinclude:: ../../examples/fp8_multiplier.vhdl
#   :language: vhdl
