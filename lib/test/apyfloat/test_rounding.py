import apytypes
from apytypes import APyFloat, QuantizationMode, APyFloatQuantizationContext
import pytest


def test_issue_245():
    # Test that jamming bit is not applied when result is zero
    # https://github.com/apytypes/apytypes/issues/245
    with APyFloatQuantizationContext(QuantizationMode.JAM):
        res = APyFloat(0, 15, 0, 5, 2) + APyFloat(0, 0, 0, 5, 2)
        assert res.is_identical(APyFloat(0, 15, 0, 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.JAM):
        res = APyFloat(0, 15, 0, 5, 2) - APyFloat(0, 0, 0, 5, 2)
        assert res.is_identical(APyFloat(0, 15, 0, 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.JAM):
        res = APyFloat(0, 15, 0, 5, 2) * APyFloat(0, 0, 0, 5, 2)
        assert res.is_identical(APyFloat(0, 0, 0, 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.JAM):
        res = APyFloat(0, 0, 0, 5, 2) / APyFloat(0, 15, 0, 5, 2)
        assert res.is_identical(APyFloat(0, 0, 0, 5, 2))


@pytest.mark.float_add
@pytest.mark.float_sub
@pytest.mark.parametrize("man", [10, 60])
def test_add_sub_zero_sign(man):
    pos_zero = APyFloat(0, 0, 0, 5, 10)
    neg_zero = APyFloat(1, 0, 0, 5, man)
    non_zero = APyFloat.from_float(1.0, 5, man)
    modes = (
        QuantizationMode.TIES_EVEN,
        QuantizationMode.TO_ZERO,
        QuantizationMode.TO_POS,
    )

    for mode in modes:
        with APyFloatQuantizationContext(mode):
            assert (_ := pos_zero + pos_zero).sign == False
            assert (_ := pos_zero + neg_zero).sign == False
            assert (_ := neg_zero + pos_zero).sign == False
            assert (_ := neg_zero + neg_zero).sign == True

            assert (_ := pos_zero - pos_zero).sign == False
            assert (_ := pos_zero - neg_zero).sign == False
            assert (_ := neg_zero - pos_zero).sign == True
            assert (_ := neg_zero - neg_zero).sign == False

            assert (_ := non_zero + (-non_zero)).sign == False

    with APyFloatQuantizationContext(QuantizationMode.TO_NEG):
        assert (_ := pos_zero + pos_zero).sign == False
        assert (_ := pos_zero + neg_zero).sign == True
        assert (_ := neg_zero + pos_zero).sign == True
        assert (_ := neg_zero + neg_zero).sign == True

        assert (_ := pos_zero - pos_zero).sign == True
        assert (_ := pos_zero - neg_zero).sign == False
        assert (_ := neg_zero - pos_zero).sign == True
        assert (_ := neg_zero - neg_zero).sign == True

        assert (_ := non_zero - non_zero).sign == True


@pytest.mark.float_add
class TestAPyFloatQuantizationAddSub:
    """
    Test class for the different quantization modes for addition in APyFloat.
    If subtraction is implemented as 'a + (-b)' then this also tests the quantization modes for subtraction.
    """

    def test_to_pos(self):
        with APyFloatQuantizationContext(QuantizationMode.TO_POS):
            # 1.5 + very small number should quantize to 1.75
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 3, 5, 2))

            # -1.5 + very small number should quantize to 1.25
            res = APyFloat(1, 15, 2, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 1, 5, 2))

            # Two big positive numbers should become infinity
            res = APyFloat(0, 30, 3, 5, 2) + APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 31, 0, 5, 2))

            # Two big negative numbers should saturate
            res = APyFloat(1, 30, 3, 5, 2) + APyFloat(1, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 30, 3, 5, 2))

    def test_to_neg(self):
        with APyFloatQuantizationContext(QuantizationMode.TO_NEG):
            # 1.5 + very small number should quantize to 1.5
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2))

            # -1.5 + very small number should quantize to 1.5
            res = APyFloat(1, 15, 2, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 2, 5, 2))

            # Two big positive numbers should saturate
            res = APyFloat(0, 30, 3, 5, 2) + APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 30, 3, 5, 2))

            # Two big negative numbers should become infinity
            res = APyFloat(1, 30, 3, 5, 2) + APyFloat(1, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 31, 0, 5, 2))

    def test_to_zero(self):
        with APyFloatQuantizationContext(QuantizationMode.TO_ZERO):
            # 1.5 + relatively big number (0.21875) should still quantize to 1.5
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(
                sign=0, exp=12, man=3, exp_bits=5, man_bits=2
            )
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2))

            # -1.5 + relatively big negative number (0.21875) should still quantize to -1.5
            res = APyFloat(1, 15, 2, 5, 2) + APyFloat(
                sign=1, exp=12, man=3, exp_bits=5, man_bits=2
            )
            assert res.is_identical(APyFloat(1, 15, 2, 5, 2))

            # Two big positive numbers should saturate
            res = APyFloat(0, 30, 3, 5, 2) + APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 30, 3, 5, 2))

            # Two big negative numbers should saturate
            res = APyFloat(1, 30, 3, 5, 2) + APyFloat(1, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 30, 3, 5, 2))

    def test_to_ties_even(self):
        with APyFloatQuantizationContext(QuantizationMode.TIES_EVEN):
            # 1.5 + relatively big number (0.21875) should quantize to 1.75
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(
                sign=0, exp=12, man=3, exp_bits=5, man_bits=2
            )
            assert res.is_identical(APyFloat(0, 15, 3, 5, 2))

            # -1.5 + relatively big negative number (0.21875) should quantize to -1.75
            res = APyFloat(1, 15, 2, 5, 2) + APyFloat(
                sign=1, exp=12, man=3, exp_bits=5, man_bits=2
            )
            assert res.is_identical(APyFloat(1, 15, 3, 5, 2))

            # 1.5 + very small number should quantize to 1.5
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2))

            # -1.5 + very small negative number should quantize to 1.5
            res = APyFloat(1, 15, 2, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 2, 5, 2))

            # 1.5 + tie should quantize to 1.5
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(0, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2))

            # -1.5 + negative tie should quantize to -1.5
            res = APyFloat(1, 15, 2, 5, 2) + APyFloat(1, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 2, 5, 2))

            # 1.75 + tie should quantize to 2.0
            res = APyFloat(0, 15, 3, 5, 2) + APyFloat(0, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 16, 0, 5, 2))

            # -1.75 + negative tie should quantize to -2.0
            res = APyFloat(1, 15, 3, 5, 2) + APyFloat(1, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 16, 0, 5, 2))

            # Two big positive numbers should become infinity
            res = APyFloat(0, 30, 3, 5, 2) + APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 31, 0, 5, 2))

            # Two big negative numbers should become infinity
            res = APyFloat(1, 30, 3, 5, 2) + APyFloat(1, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 31, 0, 5, 2))

    def test_to_ties_away(self):
        with APyFloatQuantizationContext(QuantizationMode.TIES_EVEN):
            # 1.5 + relatively big number (0.21875) should quantize to 1.75
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(
                sign=0, exp=12, man=3, exp_bits=5, man_bits=2
            )
            assert res.is_identical(APyFloat(0, 15, 3, 5, 2))

            # -1.5 + relatively big negative number (0.21875) should quantize to -1.75
            res = APyFloat(1, 15, 2, 5, 2) + APyFloat(
                sign=1, exp=12, man=3, exp_bits=5, man_bits=2
            )
            assert res.is_identical(APyFloat(1, 15, 3, 5, 2))

            # 1.5 + very small number should quantize to 1.5
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2))

            # -1.5 + very small negative number should quantize to 1.5
            res = APyFloat(1, 15, 2, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 2, 5, 2))

            # 1.5 + tie should quantize to 1.75
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(0, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2))

            # -1.5 + negative tie should quantize to -1.75
            res = APyFloat(1, 15, 2, 5, 2) + APyFloat(1, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 2, 5, 2))

            # 1.75 + tie should quantize to 2.0
            res = APyFloat(0, 15, 3, 5, 2) + APyFloat(0, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 16, 0, 5, 2))

            # -1.75 + negative tie should quantize to -2.0
            res = APyFloat(1, 15, 3, 5, 2) + APyFloat(1, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 16, 0, 5, 2))

            # Two big positive numbers should become infinity
            res = APyFloat(0, 30, 3, 5, 2) + APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 31, 0, 5, 2))

            # Two big negative numbers should become infinity
            res = APyFloat(1, 30, 3, 5, 2) + APyFloat(1, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 31, 0, 5, 2))

    def test_jam(self):
        with APyFloatQuantizationContext(QuantizationMode.JAM):
            # 1.5 + very small number should quantize to 1.75
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 3, 5, 2))

            # -1.25 + very small number should quantize to -1.25
            res = APyFloat(1, 15, 1, 5, 2) + APyFloat(0, 1, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 1, 5, 2))

            # 1.0 + 1.0 should become 2.5
            res = APyFloat(0, 15, 0, 5, 2) + APyFloat(0, 15, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 16, 1, 5, 2))

            # Two big positive numbers should saturate
            res = APyFloat(0, 30, 3, 5, 2) + APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 30, 3, 5, 2))

            # Two big negative numbers should saturate
            res = APyFloat(1, 30, 3, 5, 2) + APyFloat(1, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 30, 3, 5, 2))

    def test_stoch_weighted(self):
        with APyFloatQuantizationContext(QuantizationMode.STOCH_WEIGHTED):
            # 1.5 + 0.125 should quantize to 1.5 or 1.75
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(0, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2)) or (
                res.is_identical(APyFloat(0, 15, 3, 5, 2))
            )

            res = APyFloat(0, 1023, 0, 11, 52) + APyFloat(
                0, 1022, (1 << 52) - 1, 11, 52
            )
            assert res.is_identical(APyFloat(0, 1024, 0, 11, 52)) or (
                res.is_identical(APyFloat(0, 1023, 4503599627370495, 11, 52))
            )

    def test_stoch_equal(self):
        with APyFloatQuantizationContext(QuantizationMode.STOCH_EQUAL):
            # 1.5 + 0.125 should quantize to 1.5 or 1.75
            res = APyFloat(0, 15, 2, 5, 2) + APyFloat(0, 12, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2)) or (
                res.is_identical(APyFloat(0, 15, 3, 5, 2))
            )

            res = APyFloat(0, 1023, 0, 11, 52) + APyFloat(
                0, 1022, (1 << 52) - 1, 11, 52
            )
            assert res.is_identical(APyFloat(0, 1024, 0, 11, 52)) or (
                res.is_identical(APyFloat(0, 1023, 4503599627370495, 11, 52))
            )


@pytest.mark.float_mul
class TestAPyFloatQuantizationMul:
    """
    Test class for the different quantization modes for multiplication in APyFloat.
    """

    def test_to_pos(self):
        with APyFloatQuantizationContext(QuantizationMode.TO_POS):
            # Should round up
            res = APyFloat(0, 7, 6, 4, 3) * APyFloat(0, 7, 1, 4, 3)
            assert res.is_identical(APyFloat(0, 8, 0, 4, 3))

            # Should round down
            res = APyFloat(1, 7, 6, 4, 3) * APyFloat(0, 7, 1, 4, 3)
            assert res.is_identical(APyFloat(1, 7, 7, 4, 3))

            # Close to +0 should quantize to subnormal
            res = APyFloat(0, 1, 0, 4, 3) * APyFloat(0, 1, 1, 4, 3)
            assert res.is_identical(APyFloat(0, 0, 1, 4, 3))

            # Close to -0 should quantize to 0
            res = APyFloat(1, 1, 0, 4, 3) * APyFloat(0, 1, 1, 4, 3)
            assert res.is_identical(APyFloat(1, 0, 0, 4, 3))

            # Same two tests but with subnormal operands
            res = APyFloat(0, 0, 1, 4, 3) * APyFloat(0, 0, 1, 4, 3)
            assert res.is_identical(APyFloat(0, 0, 1, 4, 3))

            res = APyFloat(1, 0, 1, 4, 3) * APyFloat(0, 0, 1, 4, 3)
            assert res.is_identical(APyFloat(1, 0, 0, 4, 3))

            # Big positive number should become infinity
            res = APyFloat(0, 30, 3, 5, 2) * APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 31, 0, 5, 2))

            # Big negative number should saturate
            res = APyFloat(1, 30, 3, 5, 2) * APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 30, 3, 5, 2))

            # Tests from https://github.com/apytypes/apytypes/issues/406
            res = APyFloat(1, 1, 1023, 5, 10) * APyFloat(1, 26, 80, 5, 10)
            assert res.is_identical(
                APyFloat(sign=0, exp=13, man=80, exp_bits=5, man_bits=10)
            )

            res = APyFloat(sign=0, exp=2, man=72, exp_bits=5, man_bits=10) * APyFloat(
                sign=0, exp=0, man=58, exp_bits=5, man_bits=10
            )
            assert res.is_identical(
                APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=10)
            )

    def test_to_neg(self):
        with APyFloatQuantizationContext(QuantizationMode.TO_NEG):
            # Should round down
            res = APyFloat(0, 7, 6, 4, 3) * APyFloat(0, 7, 1, 4, 3)
            assert res.is_identical(APyFloat(0, 7, 7, 4, 3))

            # Should round down
            res = APyFloat(1, 7, 6, 4, 3) * APyFloat(0, 7, 1, 4, 3)
            assert res.is_identical(APyFloat(1, 8, 0, 4, 3))

            # Close to +0 should quantize to zero
            res = APyFloat(0, 1, 0, 4, 3) * APyFloat(0, 1, 1, 4, 3)
            assert res.is_identical(APyFloat(0, 0, 0, 4, 3))

            # Close to -0 should quantize to subnormal
            res = APyFloat(1, 1, 0, 4, 3) * APyFloat(0, 1, 1, 4, 3)
            assert res.is_identical(APyFloat(1, 0, 1, 4, 3))

            # Same two tests but with subnormal operands
            res = APyFloat(0, 0, 1, 4, 3) * APyFloat(0, 0, 1, 4, 3)
            assert res.is_identical(APyFloat(0, 0, 0, 4, 3))

            res = APyFloat(1, 0, 1, 4, 3) * APyFloat(0, 0, 1, 4, 3)
            assert res.is_identical(APyFloat(1, 0, 1, 4, 3))

            # Big positive number should saturate
            res = APyFloat(0, 30, 3, 5, 2) * APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 30, 3, 5, 2))

            # Big negative number should become infinity
            res = APyFloat(1, 30, 3, 5, 2) * APyFloat(0, 30, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 31, 0, 5, 2))

    def test_jam(self):
        # TODO: more tests
        with APyFloatQuantizationContext(QuantizationMode.JAM):
            # Test jamming for subnormal result
            res = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=10) * APyFloat(
                sign=1, exp=0, man=1, exp_bits=5, man_bits=10
            )
            assert res.is_identical(
                APyFloat(sign=1, exp=0, man=1, exp_bits=5, man_bits=10)
            )

    def test_jam_unbiased(self):
        # TODO: more tests
        with APyFloatQuantizationContext(QuantizationMode.JAM_UNBIASED):
            # Test jamming for subnormal result
            res = APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=10) * APyFloat(
                sign=1, exp=0, man=1, exp_bits=5, man_bits=10
            )
            assert res.is_identical(
                APyFloat(sign=1, exp=0, man=1, exp_bits=5, man_bits=10)
            )

    def test_stoch_weighted(self):
        with APyFloatQuantizationContext(QuantizationMode.STOCH_WEIGHTED):
            # 1.25 * 1.25 should quantize to 1.5 or 1.75
            res = APyFloat(0, 15, 1, 5, 2) * APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2)) or (
                res.is_identical(APyFloat(0, 15, 3, 5, 2))
            )

            res = APyFloat(0, 15, 1, 5, 52) * APyFloat(0, 15, 1, 5, 52)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 52)) or (
                res.is_identical(APyFloat(0, 15, 3, 5, 52))
            )

            # Should quantize to zero or smallest subnormal
            res = APyFloat(0, 1, 0, 4, 2) * APyFloat(0, 1, 0, 4, 2)
            assert res.is_identical(APyFloat(0, 0, 0, 4, 2)) or (
                res.is_identical(APyFloat(0, 0, 1, 4, 2))
            )

            res = APyFloat(0, 0, (1 << 27) - 1, 4, 52) * APyFloat(
                0, 0, (1 << 30) - 1, 4, 52
            )
            assert res.is_identical(APyFloat(0, 0, 0, 4, 52)) or (
                res.is_identical(APyFloat(0, 0, 1, 4, 52))
            )

    def test_stoch_equal(self):
        with APyFloatQuantizationContext(QuantizationMode.STOCH_EQUAL):
            # 1.25 * 1.25 should quantize to 1.5 or 1.75
            res = APyFloat(0, 15, 1, 5, 2) * APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 2)) or (
                res.is_identical(APyFloat(0, 15, 3, 5, 2))
            )

            res = APyFloat(0, 15, 1, 5, 52) * APyFloat(0, 15, 1, 5, 52)
            assert res.is_identical(APyFloat(0, 15, 2, 5, 52)) or (
                res.is_identical(APyFloat(0, 15, 3, 5, 52))
            )

            # Should quantize to zero or smallest subnormal
            res = APyFloat(0, 1, 0, 4, 2) * APyFloat(0, 1, 0, 4, 2)
            assert res.is_identical(APyFloat(0, 0, 0, 4, 2)) or (
                res.is_identical(APyFloat(0, 0, 1, 4, 2))
            )

            res = APyFloat(0, 0, (1 << 27) - 1, 4, 52) * APyFloat(
                0, 0, (1 << 30) - 1, 4, 52
            )
            assert res.is_identical(APyFloat(0, 0, 0, 4, 52)) or (
                res.is_identical(APyFloat(0, 0, 1, 4, 52))
            )


@pytest.mark.float_div
class TestAPyFloatQuantizationDiv:
    """
    Test class for the different quantization modes for division in APyFloat.
    """

    def test_to_pos(self):
        with APyFloatQuantizationContext(QuantizationMode.TO_POS):
            # 1.5 / 1.25 (=1.2) should quantize to 1.25
            res = APyFloat(0, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 1, 5, 2))

            # -1.5 / 1.25 (=-1.2) should quantize to -1
            res = APyFloat(1, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 0, 5, 2))

            # Should become infinity
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(0, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 31, 0, 5, 2))

            # Should saturate
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(1, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 30, 3, 5, 2))

    def test_to_neg(self):
        with APyFloatQuantizationContext(QuantizationMode.TO_NEG):
            # 1.5 / 1.25 (=1.2) should quantize to 1.0
            res = APyFloat(0, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 0, 5, 2))

            # -1.5 / 1.25 (=-1.2) should quantize to -1.25
            res = APyFloat(1, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 1, 5, 2))

            # Should saturate
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(0, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 30, 3, 5, 2))

            # Should become infinity
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(1, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 31, 0, 5, 2))

    def test_to_zero(self):
        with APyFloatQuantizationContext(QuantizationMode.TO_ZERO):
            # 1.25 / 1.5 should quantize to 0.75
            res = APyFloat(0, 15, 1, 5, 2) / APyFloat(0, 15, 2, 5, 2)
            assert res.is_identical(APyFloat(0, 14, 2, 5, 2))

            # -1.25 / 1.5 should quantize to -0.75
            res = APyFloat(1, 15, 1, 5, 2) / APyFloat(0, 15, 2, 5, 2)
            assert res.is_identical(APyFloat(1, 14, 2, 5, 2))

            # Should saturate
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(0, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 30, 3, 5, 2))

            # Should saturate
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(1, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 30, 3, 5, 2))

    def test_to_ties_even(self):
        with APyFloatQuantizationContext(QuantizationMode.TIES_EVEN):
            # 1.25 / 1.5 should quantize to closes which is 0.875
            res = APyFloat(0, 15, 1, 5, 2) / APyFloat(0, 15, 2, 5, 2)
            assert res.is_identical(APyFloat(0, 14, 3, 5, 2))

            # -1.25 / 1.5 should quantize to closes which is 0.875
            res = APyFloat(1, 15, 1, 5, 2) / APyFloat(0, 15, 2, 5, 2)
            assert res.is_identical(APyFloat(1, 14, 3, 5, 2))

            # 1.5 / 1.25 (=1.2) should quantize to closest which is 1.25
            res = APyFloat(0, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 1, 5, 2))

            # -1.5 / 1.25 (=-1.2) should quantize to closes which is -1.25
            res = APyFloat(1, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 1, 5, 2))

            # 2**-16 / 2 should tie to 0
            res = APyFloat(0, 0, 1, 5, 2) / APyFloat(0, 16, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 0, 0, 5, 2))

            # -2**-16 / 2 should tie to 0
            res = APyFloat(1, 0, 1, 5, 2) / APyFloat(0, 16, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 0, 0, 5, 2))

            # 3*2**-16 / 2 should tie to 2*2**-16
            res = APyFloat(0, 0, 3, 5, 2) / APyFloat(0, 16, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 0, 2, 5, 2))

            # -3*2**-16 / 2 should tie to -2*2**-16
            res = APyFloat(1, 0, 3, 5, 2) / APyFloat(0, 16, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 0, 2, 5, 2))

            # Should become infinity
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(0, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 31, 0, 5, 2))

            # Should become infinity
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(1, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 31, 0, 5, 2))

    def test_to_ties_away(self):
        with APyFloatQuantizationContext(QuantizationMode.TIES_AWAY):
            # 1.25 / 1.5 should quantize to closes which is 0.875
            res = APyFloat(0, 15, 1, 5, 2) / APyFloat(0, 15, 2, 5, 2)
            assert res.is_identical(APyFloat(0, 14, 3, 5, 2))

            # -1.25 / 1.5 should quantize to closes which is 0.875
            res = APyFloat(1, 15, 1, 5, 2) / APyFloat(0, 15, 2, 5, 2)
            assert res.is_identical(APyFloat(1, 14, 3, 5, 2))

            # 1.5 / 1.25 (=1.2) should quantize to closest which is 1.25
            res = APyFloat(0, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 1, 5, 2))

            # -1.5 / 1.25 (=-1.2) should quantize to closes which is -1.25
            res = APyFloat(1, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 1, 5, 2))

            # 2**-16 / 2 should tie to 2**-16
            res = APyFloat(0, 0, 1, 5, 2) / APyFloat(0, 16, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 0, 1, 5, 2))

            # -2**-16 / 2 should tie to 2**-16
            res = APyFloat(1, 0, 1, 5, 2) / APyFloat(0, 16, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 0, 1, 5, 2))

            # 3*2**-16 / 2 should tie to 2*2**-16
            res = APyFloat(0, 0, 3, 5, 2) / APyFloat(0, 16, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 0, 2, 5, 2))

            # -3*2**-16 / 2 should tie to -2*2**-16
            res = APyFloat(1, 0, 3, 5, 2) / APyFloat(0, 16, 0, 5, 2)
            assert res.is_identical(APyFloat(1, 0, 2, 5, 2))

            # Should become infinity
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(0, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 31, 0, 5, 2))

            # Should become infinity
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(1, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 31, 0, 5, 2))

    def test_jam(self):
        with APyFloatQuantizationContext(QuantizationMode.JAM):
            # 1.5 / 1.25 (=1.2) should quantize to 1.25
            res = APyFloat(0, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 1, 5, 2))

            # -1.5 / 1.25 (=-1.2) should quantize to -1.25
            res = APyFloat(1, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(1, 15, 1, 5, 2))

            # 4 / 2 (=2) should quantize to 2.25
            res = APyFloat(0, 17, 0, 5, 2) / APyFloat(0, 16, 0, 5, 2)
            assert res.is_identical(APyFloat(0, 16, 1, 5, 2))

            # Should saturate
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(0, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(0, 30, 3, 5, 2))

            # Should saturate
            res = APyFloat(0, 30, 3, 5, 2) / APyFloat(1, 0, 3, 5, 2)
            assert res.is_identical(APyFloat(1, 30, 3, 5, 2))

    def test_stoch_weighted(self):
        with APyFloatQuantizationContext(QuantizationMode.STOCH_WEIGHTED):
            # 1.5 / 1.25 (=1.2) should quantize to 1.0 or 1.25
            res = APyFloat(0, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 0, 5, 2)) or (
                res.is_identical(APyFloat(0, 15, 1, 5, 2))
            )

            # 1.something / 2 can round to either 0.5 or 0.5(something)
            res = APyFloat(0, 1023, 1, 11, 52) / APyFloat(0, 1024, 0, 11, 52)
            assert res.is_identical(APyFloat(0, 1022, 0, 11, 52)) or (
                res.is_identical(APyFloat(0, 1022, 1, 11, 52))
            )

    def test_stoch_equal(self):
        with APyFloatQuantizationContext(QuantizationMode.STOCH_EQUAL):
            # 1.5 / 1.25 (=1.2) should quantize to 1.0 or 1.25
            res = APyFloat(0, 15, 2, 5, 2) / APyFloat(0, 15, 1, 5, 2)
            assert res.is_identical(APyFloat(0, 15, 0, 5, 2)) or (
                res.is_identical(APyFloat(0, 15, 1, 5, 2))
            )

            # 1.something / 2 can quantize to either 0.5 or 0.5(something)
            res = APyFloat(0, 1023, 1, 11, 52) / APyFloat(0, 1024, 0, 11, 52)
            assert res.is_identical(APyFloat(0, 1022, 0, 11, 52)) or (
                res.is_identical(APyFloat(0, 1022, 1, 11, 52))
            )


class TestAPyFloatQuantization:
    """
    Test class for the different quantization modes for APyFloat.
    """

    default_mode = None

    def setup_class(self):
        """Save the current quantization mode so that it can be restored later for other tests."""
        self.default_mode = apytypes.get_float_quantization_mode()

    def teardown_class(self):
        """Restore quantization mode."""
        apytypes.set_float_quantization_mode(self.default_mode)

    def test_quantization_mantissa_overflow(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TO_POS)
        assert (
            APyFloat(0, 5, 0b11111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 6, 0b000, 5, 3))
        )
        assert (
            APyFloat(0, 0b11110, 0b11111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 0b11111, 0b000, 5, 3))
        )  # Quantization becomes inf

        apytypes.set_float_quantization_mode(QuantizationMode.TO_NEG)
        assert (
            APyFloat(1, 5, 0b11111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 6, 0b000, 5, 3))
        )
        assert (
            APyFloat(1, 0b11110, 0b11111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 0b11111, 0b000, 5, 3))
        )  # Quantization becomes -inf

    def test_quantization_to_pos(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TO_POS)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 15, 0, 4, 3))
        )  # Should become infinity

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 14, 7, 4, 3))
        )  # Should saturate

    def test_quantization_to_neg(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TO_NEG)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round up
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round up

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 14, 7, 4, 3))
        )  # Should saturate

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 15, 0, 4, 3))
        )  # Should become infinity

    def test_quantization_to_away(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TO_AWAY)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round down

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 15, 0, 4, 3))
        )  # Should become infinity

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 15, 0, 4, 3))
        )  # Should become infinity

    def test_quantization_to_zero(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TO_ZERO)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 14, 7, 4, 3))
        )  # Should saturate

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 14, 7, 4, 3))
        )  # Should saturate

    def test_quantization_ties_even(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TIES_EVEN)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round up

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 15, 0, 4, 3))
        )  # Should become infinity

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 15, 0, 4, 3))
        )  # Should become infinity

        # From https://github.com/apytypes/apytypes/issues/406
        res = APyFloat(sign=0, exp=103, man=0, exp_bits=8, man_bits=23).cast(5, 10)
        assert res.is_identical(APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=10))

        res = APyFloat(sign=0, exp=103, man=1, exp_bits=8, man_bits=23).cast(5, 10)
        assert res.is_identical(APyFloat(sign=0, exp=0, man=1, exp_bits=5, man_bits=10))

    def test_quantization_ties_odd(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TIES_ODD)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round up

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 15, 0, 4, 3))
        )  # Should become infinity

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 15, 0, 4, 3))
        )  # Should become infinity

    def test_quantization_ties_pos(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TIES_POS)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round up

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 15, 0, 4, 3))
        )  # Should become infinity

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 15, 0, 4, 3))
        )  # Should become infinity

    def test_quantization_ties_neg(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TIES_NEG)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round up

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 15, 0, 4, 3))
        )  # Should become infinity

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 15, 0, 4, 3))
        )  # Should become infinity

    def test_quantization_ties_away(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TIES_AWAY)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Tie break, round up
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round up

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 15, 0, 4, 3))
        )  # Should become infinity

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 15, 0, 4, 3))
        )  # Should become infinity

    def test_quantization_ties_to_zero(self):
        apytypes.set_float_quantization_mode(QuantizationMode.TIES_ZERO)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b110, 5, 3))
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b100, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round up

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # No quantization needed
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Round down
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )  # Tie break, round down
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )  # Round up

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 15, 0, 4, 3))
        )  # Should become infinity

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 15, 0, 4, 3))
        )  # Should become infinity

    def test_quantization_jamming(self):
        apytypes.set_float_quantization_mode(QuantizationMode.JAM)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 14, 7, 4, 3))
        )  # Should saturate

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 14, 7, 4, 3))
        )  # Should saturate

    def test_quantization_unbiased_jamming(self):
        apytypes.set_float_quantization_mode(QuantizationMode.JAM_UNBIASED)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 14, 7, 4, 3))
        )  # Should saturate

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 14, 7, 4, 3))
        )  # Should saturate

    def test_quantization_magnitude_truncation(self):
        # Shouldn't be used for floating-point
        apytypes.set_float_quantization_mode(QuantizationMode.TRN_MAG)
        # Quantization from 0.xx
        assert (
            APyFloat(0, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b100, 5, 3))
        )

        # Quantization from 1.xx
        assert (
            APyFloat(0, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(0, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(0, 5, 0b101, 5, 3))
        )

        # Quantization from 0.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10000, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10001, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10010, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10011, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b101, 5, 3))
        )

        # Quantization from 1.xx, negative sign
        assert (
            APyFloat(1, 5, 0b10100, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10101, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10110, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )
        assert (
            APyFloat(1, 5, 0b10111, 5, 5)
            .cast(5, 3)
            .is_identical(APyFloat(1, 5, 0b110, 5, 3))
        )

        # Test handling of infinities
        assert (
            APyFloat(0, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(0, 15, 0, 4, 3))
        )  # Should become infinity

        assert (
            APyFloat(1, 30, 0, 5, 5).cast(4, 3).is_identical(APyFloat(1, 15, 0, 4, 3))
        )  # Should become infinity

    def test_quantization_stochastic_weighted(self):
        """
        A bit naive, but test that a value can be rounded both up and down in 1000 tries.
        An exact value should however not be rounded.
        """
        apytypes.set_float_quantization_mode(QuantizationMode.STOCH_WEIGHTED)
        larger_format = APyFloat(0, 5, 0b10000, 5, 5)
        assert larger_format.cast(5, 3).is_identical(APyFloat(0, 5, 0b100, 5, 3))

        larger_format = APyFloat(0, 5, 0b10001, 5, 5)
        rounded_down = APyFloat(0, 5, 0b100, 5, 3)
        rounded_up = APyFloat(0, 5, 0b101, 5, 3)

        done_up = False
        done_down = False
        for i in range(1000):
            smaller_format = larger_format.cast(5, 3)
            if smaller_format == rounded_down:
                done_down = True
            elif smaller_format == rounded_up:
                done_up = True
            else:
                self.fail(f"{larger_format} was rounded to {smaller_format}")
            if done_down and done_up:
                break

    def test_quantization_stochastic_equal(self):
        """
        A bit naive, but test that a value can be rounded both up and down in 1000 tries.
        An exact value should however not be rounded.
        """
        apytypes.set_float_quantization_mode(QuantizationMode.STOCH_EQUAL)
        larger_format = APyFloat(0, 5, 0b10000, 5, 5)
        assert larger_format.cast(5, 3).is_identical(APyFloat(0, 5, 0b100, 5, 3))

        larger_format = APyFloat(0, 5, 0b10001, 5, 5)
        rounded_down = APyFloat(0, 5, 0b100, 5, 3)
        rounded_up = APyFloat(0, 5, 0b101, 5, 3)

        done_up = False
        done_down = False
        for i in range(1000):
            smaller_format = larger_format.cast(5, 3)
            if smaller_format == rounded_down:
                done_down = True
            elif smaller_format == rounded_up:
                done_up = True
            else:
                pytest.fail(f"{larger_format} was rounded to {smaller_format}")
            if done_down and done_up:
                break

    def test_quantization_subnormal_shift(self):
        """Result will be subnormal but require a left shift."""
        assert (_ := APyFloat(0, 0, 1, 4, 4).cast(4, 10)).is_identical(
            APyFloat(sign=0, exp=0, man=64, exp_bits=4, man_bits=10)
        )


def test_convenience_cast():
    a = APyFloat.from_float(1 / 7, 20, 60)
    assert a.is_identical(
        APyFloat(sign=0, exp=524284, man=164703072086692352, exp_bits=20, man_bits=60)
    )

    double = a.cast_to_double()
    assert double.is_identical(
        APyFloat(sign=0, exp=1020, man=643371375338642, exp_bits=11, man_bits=52)
    )

    single = a.cast_to_single()
    assert single.is_identical(
        APyFloat(sign=0, exp=124, man=1198373, exp_bits=8, man_bits=23)
    )

    half = a.cast_to_half()
    assert half.is_identical(APyFloat(sign=0, exp=12, man=146, exp_bits=5, man_bits=10))

    bfloat16 = a.cast_to_bfloat16()
    assert bfloat16.is_identical(
        APyFloat(sign=0, exp=124, man=18, exp_bits=8, man_bits=7)
    )


def test_issue_188():
    # Actually tests the rounding upon construction from float
    x = APyFloat(sign=0, exp=0, man=1, exp_bits=4, man_bits=3)
    y = APyFloat(sign=0, exp=7, man=1, exp_bits=4, man_bits=3)

    res1 = x / y
    res2 = APyFloat.from_float(float(x) / float(y), exp_bits=4, man_bits=3)

    assert res1 == res2


def test_cast_only_one_size():
    x = APyFloat(0, 5, 10, 5, 5)
    assert (x.cast(4)).is_identical(APyFloat(0, 0, 3, 4, 5))
    assert (x.cast(6)).is_identical(APyFloat(0, 21, 10, 6, 5))
    assert (x.cast(man_bits=4)).is_identical(APyFloat(0, 5, 5, 5, 4))
    assert (x.cast(man_bits=6)).is_identical(APyFloat(0, 5, 20, 5, 6))
