from pathlib import Path

import vunit

VU = vunit.VUnit.from_argv(compile_builtins=False)

VU.add_vhdl_builtins()

curr_dir = Path(__file__).parent

VU.add_library("lib").add_source_files(curr_dir / "csv_tb.vhdl", vhdl_standard="2008")

VU.set_compile_option("nvc.a_flags", ["--relaxed"])

VU.main()
