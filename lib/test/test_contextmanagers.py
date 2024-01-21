import apytypes
from apytypes import RoundingContext, RoundingMode
import pytest


class TestRoundingContext:
    """
    This test class doesn't test if the rounding itself works,
    just that the context manager acts correctly.
    """

    default_mode = None

    def setup_class(self):
        """Save the current rounding mode so that it can be restored later for other tests."""
        self.default_mode = apytypes.get_rounding_mode()


    def teardown_class(self):
        """Restore rounding mode."""
        apytypes.set_rounding_mode(self.default_mode)


    def test_default_mode(self):
        assert apytypes.get_rounding_mode() == RoundingMode.TIES_TO_EVEN


    def test_prerequisite(self):
        """Make sure get/set works to begin with."""
        apytypes.set_rounding_mode(RoundingMode.TO_POSITIVE)
        assert apytypes.get_rounding_mode() == RoundingMode.TO_POSITIVE
        apytypes.set_rounding_mode(RoundingMode.TO_NEGATIVE)
        assert apytypes.get_rounding_mode() == RoundingMode.TO_NEGATIVE

        apytypes.set_rounding_seed(5)
        assert apytypes.get_rounding_seed() == 5
        apytypes.set_rounding_seed(1 << 57)
        assert apytypes.get_rounding_seed() == (1 << 57)


    def test_not_nested(self):
        """Single layer of context."""
        # Test setting a non-stochastic rounding mode
        apytypes.set_rounding_mode(RoundingMode.TO_POSITIVE)
        with RoundingContext(RoundingMode.TO_NEGATIVE):
            assert apytypes.get_rounding_mode() == RoundingMode.TO_NEGATIVE
        assert apytypes.get_rounding_mode() == RoundingMode.TO_POSITIVE

        # Test setting a stochastic rounding mode without changing the seed
        apytypes.set_rounding_seed(123)
        with RoundingContext(RoundingMode.STOCHASTIC_EQUAL):
            assert apytypes.get_rounding_mode() == RoundingMode.STOCHASTIC_EQUAL
            assert apytypes.get_rounding_seed() == 123
        assert apytypes.get_rounding_mode() == RoundingMode.TO_POSITIVE
        assert apytypes.get_rounding_seed() == 123

        # Test setting a stochastic rounding mode and changing the seed
        with RoundingContext(RoundingMode.STOCHASTIC_WEIGHTED, 77):
            assert apytypes.get_rounding_mode() == RoundingMode.STOCHASTIC_WEIGHTED
            assert apytypes.get_rounding_seed() == 77
        assert apytypes.get_rounding_mode() == RoundingMode.TO_POSITIVE
        assert apytypes.get_rounding_seed() == 123


    def test_nested(self):
        """Nested context layers."""
        apytypes.set_rounding_mode(RoundingMode.TO_POSITIVE)
        apytypes.set_rounding_seed(123)
        with RoundingContext(RoundingMode.STOCHASTIC_WEIGHTED, 456):
            assert apytypes.get_rounding_mode() == RoundingMode.STOCHASTIC_WEIGHTED
            assert apytypes.get_rounding_seed() == 456
            with RoundingContext(RoundingMode.STOCHASTIC_EQUAL, 789):
                assert apytypes.get_rounding_mode() == RoundingMode.STOCHASTIC_EQUAL
                assert apytypes.get_rounding_seed() == 789
            assert apytypes.get_rounding_mode() == RoundingMode.STOCHASTIC_WEIGHTED
            assert apytypes.get_rounding_seed() == 456
        assert apytypes.get_rounding_mode() == RoundingMode.TO_POSITIVE
        assert apytypes.get_rounding_seed() == 123


    def test_raised_exception(self):
        """Make sure an exception is raised if a seed is given for RoundingContext with a non-stochastic rounding mode."""
        with pytest.raises(ValueError, match='Seed'):
            with RoundingContext(RoundingMode.TO_POSITIVE, 123):
                pass