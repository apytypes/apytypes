import unittest
import sys
sys.path.append("../builddir/")
from apy_types import APyFixed, STRING_TYPE


class TestFixedPoint(unittest.TestCase):
    
    def test_placeholder(self):
        # Not really a test, just a placeholder
        fx_a = APyFixed(8, 2, (1<<6) | (1 << 5))
        print(fx_a.to_string(STRING_TYPE.DEC))
        print(fx_a)
        fx_a.increment_lsb()
        print(fx_a.to_string(STRING_TYPE.DEC))
        

if __name__ == '__main__':
    unittest.main()