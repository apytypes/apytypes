from pathlib import Path

import vunit

cli = vunit.VUnitCLI()

# This "--csv-file" parameter must always be passed
cli.parser.add_argument(
    "--csv-file",
    help="Path to CSV file to use in testbench.",
    default=None,
)

args = cli.parse_args()

VU = vunit.VUnit.from_args(args=args, compile_builtins=False)

VU.add_vhdl_builtins()

LIB = VU.add_library("lib")
LIB.add_source_files(Path(__file__).parent / "csv_tb.vhdl", vhdl_standard="2008")

TB = LIB.test_bench("tb_csv")
TB.set_generic("csv_file", args.csv_file)
TB.set_generic("tb_path", str(Path(__file__).parent))
TB.test("Case 1D")
TB.test("Case 2D")
TB.test("Case 3D")

VU.set_compile_option("nvc.a_flags", ["--relaxed"])

VU.main()
