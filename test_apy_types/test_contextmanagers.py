import pytest
import sys
sys.path.append('../builddir/')
import apy_types
from apy_types import RoundingContext, RoundingMode

class TestRoundingContext:
    """
    This test class doesn't test if the rounding itself works,
    just that the context manager acts correctly.
    """
    default_mode = None

    def setup_class(self):
        """ Save the current rounding mode so that it can be restored later for other tests. """
        self.default_mode = apy_types.get_rounding_mode()

    def teardown_class(self):
        """ Restore rounding mode. """
        apy_types.set_rounding_mode(self.default_mode)

    def test_default_mode(self):
        assert apy_types.get_rounding_mode() == RoundingMode.TIES_TO_EVEN

    def test_prerequisite(self):
        """ Make sure get/set works for the rounding mode to begin with. """
        apy_types.set_rounding_mode(RoundingMode.TO_POSITIVE)
        assert apy_types.get_rounding_mode() == RoundingMode.TO_POSITIVE
        apy_types.set_rounding_mode(RoundingMode.TO_NEGATIVE)
        assert apy_types.get_rounding_mode() == RoundingMode.TO_NEGATIVE

    def test_not_nested(self):
        """ Single layer of context. """
        apy_types.set_rounding_mode(RoundingMode.TO_POSITIVE)
        with RoundingContext(RoundingMode.TO_NEGATIVE):
            assert apy_types.get_rounding_mode() == RoundingMode.TO_NEGATIVE
        assert apy_types.get_rounding_mode() == RoundingMode.TO_POSITIVE
        with RoundingContext(RoundingMode.JAMMING):
            assert apy_types.get_rounding_mode() == RoundingMode.JAMMING
        assert apy_types.get_rounding_mode() == RoundingMode.TO_POSITIVE

    def test_nested(self):
        """ Nested context layers. """
        apy_types.set_rounding_mode(RoundingMode.TO_POSITIVE)
        with RoundingContext(RoundingMode.TO_NEGATIVE):
            assert apy_types.get_rounding_mode() == RoundingMode.TO_NEGATIVE
            with RoundingContext(RoundingMode.TO_ZERO):
                assert apy_types.get_rounding_mode() == RoundingMode.TO_ZERO
            assert apy_types.get_rounding_mode() == RoundingMode.TO_NEGATIVE
        assert apy_types.get_rounding_mode() == RoundingMode.TO_POSITIVE
