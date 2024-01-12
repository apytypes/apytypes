import apytypes
from apytypes import RoundingContext, RoundingMode


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
        """Make sure get/set works for the rounding mode to begin with."""
        apytypes.set_rounding_mode(RoundingMode.TO_POSITIVE)
        assert apytypes.get_rounding_mode() == RoundingMode.TO_POSITIVE
        apytypes.set_rounding_mode(RoundingMode.TO_NEGATIVE)
        assert apytypes.get_rounding_mode() == RoundingMode.TO_NEGATIVE

    def test_not_nested(self):
        """Single layer of context."""
        apytypes.set_rounding_mode(RoundingMode.TO_POSITIVE)
        with RoundingContext(RoundingMode.TO_NEGATIVE):
            assert apytypes.get_rounding_mode() == RoundingMode.TO_NEGATIVE
        assert apytypes.get_rounding_mode() == RoundingMode.TO_POSITIVE
        with RoundingContext(RoundingMode.JAMMING):
            assert apytypes.get_rounding_mode() == RoundingMode.JAMMING
        assert apytypes.get_rounding_mode() == RoundingMode.TO_POSITIVE

    def test_nested(self):
        """Nested context layers."""
        apytypes.set_rounding_mode(RoundingMode.TO_POSITIVE)
        with RoundingContext(RoundingMode.TO_NEGATIVE):
            assert apytypes.get_rounding_mode() == RoundingMode.TO_NEGATIVE
            with RoundingContext(RoundingMode.TO_ZERO):
                assert apytypes.get_rounding_mode() == RoundingMode.TO_ZERO
            assert apytypes.get_rounding_mode() == RoundingMode.TO_NEGATIVE
        assert apytypes.get_rounding_mode() == RoundingMode.TO_POSITIVE
