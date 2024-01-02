import sys
sys.path.append("../builddir/")
from apy_types import APyFixed


def test_placeholder():
        # Not really a test, just a placeholder
        fx_a = APyFixed(8, 2, (1<<6) | (1 << 5))
        print(fx_a.to_string())
        print(fx_a)
        fx_a.increment_lsb()
        print(fx_a.to_string())