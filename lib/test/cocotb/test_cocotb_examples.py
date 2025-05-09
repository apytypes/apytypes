import sys
from pathlib import Path

import pytest


def cocotb_test(func):
    """
    Run a cocotb testbench example, but mark it as skipped if the simulator is not found.
    """

    def wrapper(*args, **kwargs):
        pytest.importorskip(
            "cocotb", "1.9.1", "cocotb>=1.9.1 is needed to run the example"
        )
        try:
            func(*args, **kwargs)
        except BaseException as e:
            if isinstance(e, SystemExit) and "executable not found" in str(e):
                pytest.skip(reason=str(e))
            else:
                pytest.fail(str(e))

    return wrapper


class TestCocotbExamples:
    """
    Run the cocotb examples.
    """

    def setup_class(self):
        """Add directory containing examples in path."""
        example_dir = str(Path(__file__).parent) + "/../../../examples"
        sys.path.append(example_dir)

    @cocotb_test
    def test_fp8_mul_example(self):
        from test_fp8_mul import test_fp8_mul

        test_fp8_mul()
