import apytypes
from apytypes import (
    APyFloatQuantizationContext,
    QuantizationMode,
    OverflowMode,
    APyFloatAccumulatorContext,
    APyFixedAccumulatorContext,
    APyFixedCastContext,
)
import pytest


@pytest.mark.xfail()
def test_context_kw_only():
    """
    This method should raise a `TypeError`, but since kw-only arguments are not part of
    Nanobind <= v1.9.2, we wait with this until gets a tagged release.
    """
    with pytest.raises(TypeError):  # keyword only
        with APyFixedAccumulatorContext(5, 2):
            pass


class TestCastContext:
    """
    This test class doesn't test if cast itself works,
    just that the context manager acts correctly.
    """

    def test_raises(self):
        with pytest.raises(ValueError, match=".*must be specified"):
            with APyFixedCastContext():
                pass

        with APyFixedCastContext(quantization=QuantizationMode.TRN):  # should not raise
            pass

        with APyFixedCastContext(overflow=OverflowMode.SAT):  # should not raise
            pass

        with APyFixedCastContext(
            quantization=QuantizationMode.TRN, overflow=OverflowMode.SAT
        ):  # should not raise
            pass


class TestAccumulatorContext:
    """
    This test class doesn't test if the accumulation itself works,
    just that the context manager acts correctly.
    """

    def test_raises(self):
        with pytest.raises(ValueError, match="Both.*must be specified"):
            with APyFloatAccumulatorContext():
                pass

        with pytest.raises(ValueError, match=".*specification needs"):
            with APyFixedAccumulatorContext():
                pass

        with APyFixedAccumulatorContext(bits=5, int_bits=2):  # should not raise
            pass

        with APyFloatAccumulatorContext(exp_bits=5, man_bits=2):  # should not raise
            pass

    def test_with_quantization_context(self):
        """Test that the APyFloatAccumulatorContext interacts correctly with the APyFloatQuanizationContext."""
        with APyFloatQuantizationContext(QuantizationMode.TO_POS):
            assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_POS

            with APyFloatAccumulatorContext(exp_bits=5, man_bits=3):
                assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_POS

            with APyFloatAccumulatorContext(
                exp_bits=5, man_bits=3, quantization=QuantizationMode.TO_NEG
            ):
                assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_POS


class TestQuantizationContext:
    """
    This test class doesn't test if the quantization itself works,
    just that the context manager acts correctly.
    """

    default_mode = None

    def setup_class(self):
        """Save the current quantization mode so that it can be restored later for other tests."""
        self.default_mode = apytypes.get_float_quantization_mode()

    def teardown_class(self):
        """Restore quantization mode."""
        apytypes.set_float_quantization_mode(self.default_mode)

    def test_default_mode(self):
        assert apytypes.get_float_quantization_mode() == QuantizationMode.TIES_EVEN

    def test_prerequisite(self):
        """Make sure get/set works for the quantization mode to begin with."""
        apytypes.set_float_quantization_mode(QuantizationMode.TO_POS)
        assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_POS
        apytypes.set_float_quantization_mode(QuantizationMode.TO_NEG)
        assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_NEG

        apytypes.set_float_quantization_seed(5)
        assert apytypes.get_float_quantization_seed() == 5
        apytypes.set_float_quantization_seed(1 << 57)
        assert apytypes.get_float_quantization_seed() == (1 << 57)

    def test_not_nested(self):
        """Single layer of context."""
        # Test setting a non-stochastic quantization mode
        apytypes.set_float_quantization_mode(QuantizationMode.TO_POS)
        with APyFloatQuantizationContext(QuantizationMode.TO_NEG):
            assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_NEG
        assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_POS

        # Test setting a stochastic quantization mode without changing the seed
        apytypes.set_float_quantization_seed(123)
        with APyFloatQuantizationContext(QuantizationMode.STOCH_EQUAL):
            assert (
                apytypes.get_float_quantization_mode() == QuantizationMode.STOCH_EQUAL
            )
            assert apytypes.get_float_quantization_seed() == 123
        assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_POS
        assert apytypes.get_float_quantization_seed() == 123

        # Test setting a stochastic quantization mode and changing the seed
        with APyFloatQuantizationContext(QuantizationMode.STOCH_WEIGHTED, 77):
            assert (
                apytypes.get_float_quantization_mode()
                == QuantizationMode.STOCH_WEIGHTED
            )
            assert apytypes.get_float_quantization_seed() == 77
        assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_POS
        assert apytypes.get_float_quantization_seed() == 123

    def test_nested(self):
        """Nested context layers."""
        apytypes.set_float_quantization_mode(QuantizationMode.TO_POS)
        apytypes.set_float_quantization_seed(123)
        with APyFloatQuantizationContext(QuantizationMode.STOCH_WEIGHTED, 456):
            assert (
                apytypes.get_float_quantization_mode()
                == QuantizationMode.STOCH_WEIGHTED
            )
            assert apytypes.get_float_quantization_seed() == 456
            with APyFloatQuantizationContext(QuantizationMode.STOCH_EQUAL, 789):
                assert (
                    apytypes.get_float_quantization_mode()
                    == QuantizationMode.STOCH_EQUAL
                )
                assert apytypes.get_float_quantization_seed() == 789
            assert (
                apytypes.get_float_quantization_mode()
                == QuantizationMode.STOCH_WEIGHTED
            )
            assert apytypes.get_float_quantization_seed() == 456
        assert apytypes.get_float_quantization_mode() == QuantizationMode.TO_POS
        assert apytypes.get_float_quantization_seed() == 123

    def test_raised_exception(self):
        """Make sure an exception is raised if a seed is given for APyFloatQuantizationContext with a non-stochastic quantization mode."""
        with pytest.raises(ValueError, match="Seed"):
            with APyFloatQuantizationContext(QuantizationMode.TO_POS, 123):
                pass
