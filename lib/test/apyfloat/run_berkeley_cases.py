import argparse
from apytypes import APyFloat, QuantizationMode, APyFloatQuantizationContext
import os
import random
import subprocess
import sys


TEST_DIR = "./berkeley_tests/"


def read_test_cases(testfile: str):
    """
    Get a generator of test cases [operand, operand, reference]. The items only contains bit patterns,
    so the floating-point format and operation must be known by the caller.
    """
    with open(testfile) as file:
        # Interpret each line as a set of hexadecimal integers. Last column with exception flags is ignored.
        for line in file:
            yield tuple(map(lambda x: int(x, 16), line.split()[:-1]))


def generate_berkeley_test(
    operation: str,
    quantization: QuantizationMode,
    level: int = 1,
    seed: int = None,
    verbose: bool = False,
    filename: str = None,
) -> None:
    """
    operation: Can be an arithmetic operation such 'f16_add' or f64_mul, or a conversation like 'ui32_to_f16' or 'f64_to_f16'.
    quantization: Can be any of the quantization modes listed in the IEEE-754 standard, and jamming.
    level: Can be either 1 or 2. With level 2 more test cases are generated, with better coverage.
    seed:
    For more information visit: http://www.jhauser.us/arithmetic/TestFloat-3/doc/testfloat_gen.html
    """
    quant_arg = translate_quant_mode_berkeley_arg(quantization)

    command = [
        "testfloat_gen",
        operation,
        quant_arg,
        "-level",
        str(level),
        "-seed",
        str(seed),
    ]

    if verbose:
        print(f"command to execute: {' '.join(command)} > {filename}")

    with open(filename, "w+") as f:
        subprocess.run(command, stdout=f)


def parse_format(format: str) -> tuple:
    """Take a format as a string and return a tuple of the corresponding exponent and mantissa bits."""
    match format:
        case "f16":
            return (5, 10)
        case "f32":
            return (8, 23)
        case "f64":
            return (11, 52)
        case _:
            raise ValueError(f"Format '{format}' not recognized")


def function_under_test(operation: str):
    """Based on the operation, return a lambda of the operation to test."""
    match operation:
        case "to":
            return lambda x, exp_bits, man_bits: x.cast(exp_bits, man_bits)
        case "add":
            return lambda x, y: x + y
        case "sub":
            return lambda x, y: x - y
        case "mul":
            return lambda x, y: x * y
        case "div":
            return lambda x, y: x / y
        case "fma":
            raise ValueError(f"Operation '{operation}' not implemented")
        case _:
            raise ValueError(f"Operation '{operation}' not recognized")


def run_berkeley_test(
    operation: str,
    quantization: QuantizationMode,
    test_file: str,
    output_file: str,
    verbose: bool = False,
) -> None:
    test_cases = read_test_cases(test_file)

    args = operation.split("_")
    exp_bits, man_bits = parse_format(args[0])

    func_under_test = function_under_test(args[1])

    tests_total = 0
    tests_failed = 0

    if "to" in operation:
        to_exp_bits, to_man_bits = parse_format(args[2])
        with open(output_file, "a") as f:
            for test in test_cases:
                tests_total += 1
                lhs = APyFloat.from_bits(test[0], exp_bits, man_bits)
                ref = APyFloat.from_bits(test[1], to_exp_bits, to_man_bits)
                try:
                    res = func_under_test(lhs, to_exp_bits, to_man_bits)
                except Exception as e:
                    f.write(f"lhs: {lhs!r}, ref: {ref!r}, res: {res!r}\n")
                    f.write(f"Exception: {e}")
                else:
                    if (ref.is_nan and not res.is_nan) or (
                        not ref.is_nan and not res.is_identical(ref)
                    ):
                        f.write(f"lhs: {lhs!r}, ref: {ref!r}, res: {res!r}\n")
                        tests_failed += 1
    else:
        with APyFloatQuantizationContext(quantization):
            with open(output_file, "a") as f:
                for test in test_cases:
                    tests_total += 1
                    lhs = APyFloat.from_bits(test[0], exp_bits, man_bits)
                    rhs = APyFloat.from_bits(test[1], exp_bits, man_bits)
                    ref = APyFloat.from_bits(test[2], exp_bits, man_bits)
                    try:
                        res = func_under_test(lhs, rhs)
                    except Exception as e:
                        f.write(
                            f"lhs: {lhs!r}, rhs: {rhs!r}, ref: {ref!r}, res: {res!r}\n"
                        )
                        f.write(f"Exception: {e}")
                    else:
                        if (ref.is_nan and not res.is_nan) or (
                            not ref.is_nan and not res.is_identical(ref)
                        ):
                            f.write(
                                f"lhs: {lhs!r}, rhs: {rhs!r}, ref: {ref!r}, res: {res!r}\n"
                            )
                            tests_failed += 1

    if verbose:
        if not tests_failed:
            print(f"ALL {tests_total} TESTS PASSED")
        else:
            print(f"{tests_failed}/{tests_total} FAILED")

    return (tests_failed, tests_total)


def set_up_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-qm",
        "--quant-modes",
        help="quantization modes to test",
        nargs="*",
        type=str,
        choices=["all", "ties_even", "ties_away", "to_zero", "to_neg", "to_pos", "jam"],
        default=["ties_even"],
    )

    parser.add_argument(
        "-op",
        "--operations",
        help="operations to test, see section 6 at http://www.jhauser.us/arithmetic/TestFloat-3/doc/TestFloat-general.html",
        nargs="*",
        type=str,
        default=None,
    )

    parser.add_argument(
        "-s",
        "--seed",
        help="seed to generate the random number engine with",
        type=int,
        default=None,
    )

    parser.add_argument(
        "-l",
        "--level",
        help="level of testing to perform, can be either 1 or 2 where level 2 produces more test cases with better coverage",
        type=int,
        default=2,
    )

    parser.add_argument(
        "-o",
        "--output-file",
        help=f"name of file to write failed test cases to, will be created inside {TEST_DIR}",
        type=str,
        default=None,
    )

    parser.add_argument(
        "-v",
        "--verbose",
        help="increase output verbosity",
        action="store_true",
        default=False,
    )

    return parser


def translate_quant_arg(quant_mode: str) -> QuantizationMode:
    match quant_mode:
        case "ties_even":
            return QuantizationMode.TIES_EVEN
        case "ties_away":
            return QuantizationMode.TIES_AWAY
        case "to_zero":
            return QuantizationMode.TO_ZERO
        case "to_neg":
            return QuantizationMode.TO_NEG
        case "to_pos":
            return QuantizationMode.TO_POS
        case "jam":
            return QuantizationMode.JAM
        case _:
            raise ValueError("Quantization mode {quant_mode} not supported")


def translate_quant_mode_berkeley_arg(quant_mode: QuantizationMode) -> str:
    match quant_mode:
        case QuantizationMode.TIES_EVEN:
            return "-rnear_even"
        case QuantizationMode.TIES_AWAY:
            return "-rnear_maxMag"
        case QuantizationMode.TO_ZERO:
            return "-rminMag"
        case QuantizationMode.TO_NEG:
            return "-rmin"
        case QuantizationMode.TO_POS:
            return "-rmax"
        case QuantizationMode.JAM:
            return "-rodd"
        case _:
            raise ValueError("Quantization mode {quant_mode} not supported")


def print_summary(summary, log_file: str, seed: int) -> None:
    """
    summary: { 'operation' : { quantization_mode: (tests_failed, tests_total) }, ... }
    """
    print("================ Summary ================")
    print(f"Log file: {log_file}")
    print(f"Seed: {seed}")
    failed_tests = 0
    total_tests = 0
    for op in summary:
        print(f"{op}:")
        for quant in summary[op]:
            total_tests += summary[op][quant][1]
            if summary[op][quant][0] == 0:
                print(f"\t{quant}: ALL {summary[op][quant][1]} TESTS PASSED")
            else:
                print(
                    f"\t{quant}: {summary[op][quant][0]} / {summary[op][quant][1]} TESTS FAILED"
                )
                failed_tests += summary[op][quant][0]

    success_rate = (1 - float(failed_tests) / total_tests) * 100
    print(
        f"Ran {total_tests} tests in total whereof {failed_tests} failed, giving a {success_rate:.2f}% overall success rate."
    )


if __name__ == "__main__":
    parser = set_up_argument_parser()
    args = parser.parse_args()

    if not os.path.exists(TEST_DIR):
        if args.verbose:
            print(f"Creating directory {TEST_DIR}")
        os.makedirs(TEST_DIR)
    else:
        if args.verbose:
            print(f"Directory {TEST_DIR} already exists, don't need to create it")

    seed = args.seed if args.seed else random.randint(1, 0xFFFFFFFFFFFFFFFF)
    output_file = TEST_DIR + (
        args.output_file if args.output_file else "failed_tests.txt"
    )
    summary = {}
    any_test_failed = False

    open(output_file, "w").close()  # Clear output file

    if "all" in args.quant_modes:
        args.quant_modes = [
            "ties_even",
            "ties_away",
            "to_zero",
            "to_neg",
            "to_pos",
            "jam",
        ]

    for op in args.operations:
        summary[op] = {}

        for quant_arg in args.quant_modes:
            if args.verbose:
                print((f"==== Operation: {op}, {quant_arg} ===="))
            with open(output_file, "a") as f:
                f.write(f"==== Operation: {op}, {quant_arg} ====\n")

            test_file = f"{TEST_DIR}{op}_{quant_arg}.txt"

            quant_mode = translate_quant_arg(quant_arg)
            generate_berkeley_test(
                op, quant_mode, args.level, seed, args.verbose, test_file
            )

            if args.verbose:
                print(f"Running test cases from {test_file}")
            summary[op][quant_arg] = run_berkeley_test(
                op, quant_mode, test_file, output_file, args.verbose
            )

            if summary[op][quant_arg][0] != 0:
                any_test_failed = True

    print_summary(summary, output_file, seed)
    sys.exit(-1 if any_test_failed else 0)
