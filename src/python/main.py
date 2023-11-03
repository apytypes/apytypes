"""
File for quick and easy testing.
"""

import sys
sys.path.append("../../builddir/")

from apy_types import aPyFixed

if __name__ == '__main__':
    fx_a = aPyFixed(total_bits=8, integer_bits=3, value=1.5)
    fx_b = aPyFixed(8, 5, 2.75)
    print(fx_a + fx_b)
