import unittest
from itertools import permutations
import sys
sys.path.append('../builddir/')
from apy_types import APyFloat


class TestFloatingPoint(unittest.TestCase):
    
    def test_negation(self):
        with self.subTest('negate positive'):
            self.assertEqual(-APyFloat(5, 5, 2.75), APyFloat(5, 5, -2.75)) 

        with self.subTest('negate negative'):
            self.assertEqual(-APyFloat(5, 5, -13.125), APyFloat(5, 5, 13.125))


    def test_addition(self):
        with self.subTest('same exponents'):
            for lhs, rhs in permutations([APyFloat(5, 5, 2.75), APyFloat(5, 5, 2.5)]):
                self.assertEqual(lhs + rhs, APyFloat(5, 5, 5.25), f"lhs={lhs}, rhs={rhs}")

        with self.subTest('different exponents'):
            for lhs, rhs in permutations([APyFloat(5, 5, 4), APyFloat(5, 5, 16)]):
                self.assertEqual(lhs + rhs, APyFloat(5, 5, 20), f"lhs={lhs}, rhs={rhs}")

        with self.subTest('same exponents, different signs'):
            for lhs, rhs in permutations([APyFloat(5, 4, 12), APyFloat(5, 4, -4)]):
                self.skipTest('Not implemented yet')
                self.assertEqual(lhs + rhs, APyFloat(5, 4, 8), f"lhs={lhs}, rhs={rhs}")


if __name__ == '__main__':
    unittest.main()