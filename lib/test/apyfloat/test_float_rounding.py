import apytypes
from apytypes import APyFloat, RoundingMode, RoundingContext
import pytest


class TestAPyFloatRounding:
    """
    Test class for the different rounding modes for APyFloat.
    """

    default_mode = None

    def setup_class(self):
        """Save the current rounding mode so that it can be restored later for other tests."""
        self.default_mode = apytypes.get_rounding_mode()

    def teardown_class(self):
        """Restore rounding mode."""
        apytypes.set_rounding_mode(self.default_mode)

    def test_rounding_mantissa_overflow(self):
        apytypes.set_rounding_mode(RoundingMode.TO_POSITIVE)
        assert APyFloat(0, 5, 0b11111, 5, 5).cast_to(5, 3) == APyFloat(
            0, 6, 0b000, 5, 3
        )
        assert APyFloat(0, 0b11110, 0b11111, 5, 5).cast_to(5, 3) == APyFloat(
            0, 0b11111, 0b000, 5, 3
        )  # Rounding becomes inf

        apytypes.set_rounding_mode(RoundingMode.TO_NEGATIVE)
        assert APyFloat(1, 5, 0b11111, 5, 5).cast_to(5, 3) == APyFloat(
            1, 6, 0b000, 5, 3
        )
        assert APyFloat(1, 0b11110, 0b11111, 5, 5).cast_to(5, 3) == APyFloat(
            1, 0b11111, 0b000, 5, 3
        )  # Rounding becomes -inf

    def test_rounding_to_positive(self):
        apytypes.set_rounding_mode(RoundingMode.TO_POSITIVE)
        # Rounding from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round up
        assert APyFloat(0, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round up

        # Rounding from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Round up
        assert APyFloat(0, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Round up

        # Rounding from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down

        # Rounding from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down

    def test_rounding_to_negative(self):
        apytypes.set_rounding_mode(RoundingMode.TO_NEGATIVE)
        # Rounding from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down

        # Rounding from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down

        # Rounding from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down

        # Rounding from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Round up
        assert APyFloat(1, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(1, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Round ip

    def test_rounding_to_zero(self):
        apytypes.set_rounding_mode(RoundingMode.TO_ZERO)
        # Rounding from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down

        # Rounding from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down

        # Rounding from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down

        # Rounding from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down

    def test_rounding_ties_to_even(self):
        apytypes.set_rounding_mode(RoundingMode.TIES_TO_EVEN)
        # Rounding from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round up

        # Rounding from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Round up

        # Rounding from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round up

        # Rounding from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(1, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Round up

    def test_rounding_ties_to_away(self):
        apytypes.set_rounding_mode(RoundingMode.TIES_TO_AWAY)
        # Rounding from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round up

        # Rounding from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Round up

        # Rounding from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Tie break, round up
        assert APyFloat(1, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round up

        # Rounding from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No rounding needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(1, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Round up


    def test_rounding_ties_to_zero(self):
            apytypes.set_rounding_mode(RoundingMode.TIES_TO_ZERO)
            # Rounding from 0.xx
            assert APyFloat(0, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b100, 5, 3
            )  # No rounding needed
            assert APyFloat(0, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b100, 5, 3
            )  # Round down
            assert APyFloat(0, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b100, 5, 3
            )  # Tie break, round down
            assert APyFloat(0, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            )  # Round up

            # Rounding from 1.xx
            assert APyFloat(0, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            )  # No rounding needed
            assert APyFloat(0, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            )  # Round down
            assert APyFloat(0, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            )  # Tie break, round down
            assert APyFloat(0, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b110, 5, 3
            )  # Round up

            # Rounding from 0.xx, negative sign
            assert APyFloat(1, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b100, 5, 3
            )  # No rounding needed
            assert APyFloat(1, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b100, 5, 3
            )  # Round down
            assert APyFloat(1, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b100, 5, 3
            )  # Tie break, round down
            assert APyFloat(1, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            )  # Round up

            # Rounding from 1.xx, negative sign
            assert APyFloat(1, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            )  # No rounding needed
            assert APyFloat(1, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            )  # Round down
            assert APyFloat(1, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            )  # Tie break, round down
            assert APyFloat(1, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b110, 5, 3
            )  # Round up

    def test_rounding_jamming(self):
            apytypes.set_rounding_mode(RoundingMode.JAMMING)
            # Rounding from 0.xx
            assert APyFloat(0, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            )
            assert APyFloat(0, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            )
            assert APyFloat(0, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            ) 
            assert APyFloat(0, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            )

            # Rounding from 1.xx
            assert APyFloat(0, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            ) 
            assert APyFloat(0, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            )
            assert APyFloat(0, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            ) 
            assert APyFloat(0, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
                0, 5, 0b101, 5, 3
            ) 

            # Rounding from 0.xx, negative sign
            assert APyFloat(1, 5, 0b10000, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            ) 
            assert APyFloat(1, 5, 0b10001, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            )
            assert APyFloat(1, 5, 0b10010, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            )
            assert APyFloat(1, 5, 0b10011, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            ) 

            # Rounding from 1.xx, negative sign
            assert APyFloat(1, 5, 0b10100, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            ) 
            assert APyFloat(1, 5, 0b10101, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            )
            assert APyFloat(1, 5, 0b10110, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            )
            assert APyFloat(1, 5, 0b10111, 5, 5).cast_to(5, 3) == APyFloat(
                1, 5, 0b101, 5, 3
            )

    def test_rounding_stochastic_weighted(self):
            """
            A bit naive, but test that a value can be rounded both up and down in 1000 tries.
            An exact value should however not be rounded.
            """
            apytypes.set_rounding_mode(RoundingMode.STOCHASTIC_WEIGHTED)
            larger_format = APyFloat(0, 5, 0b10000, 5, 5)
            assert larger_format.cast_to(5, 3) == APyFloat(
                0, 5, 0b100, 5, 3
            )

            larger_format = APyFloat(0, 5, 0b10001, 5, 5)
            rounded_down = APyFloat(0, 5, 0b100, 5, 3)
            rounded_up = APyFloat(0, 5, 0b101, 5, 3)

            done_up = False
            done_down = False
            for i in range(1000):
                smaller_format = larger_format.cast_to(5, 3)
                if smaller_format == rounded_down:
                    done_down = True
                elif smaller_format == rounded_up:
                    done_up = True
                else:
                    self.fail(f'{larger_format} was rounded to {smaller_format}')
                if done_down and done_up:
                     break

    def test_rounding_stochastic_equal(self):
            """
            A bit naive, but test that a value can be rounded both up and down in 1000 tries.
            An exact value should however not be rounded.
            """
            apytypes.set_rounding_mode(RoundingMode.STOCHASTIC_EQUAL)
            larger_format = APyFloat(0, 5, 0b10000, 5, 5)
            assert larger_format.cast_to(5, 3) == APyFloat(
                0, 5, 0b100, 5, 3
            )

            larger_format = APyFloat(0, 5, 0b10001, 5, 5)
            rounded_down = APyFloat(0, 5, 0b100, 5, 3)
            rounded_up = APyFloat(0, 5, 0b101, 5, 3)

            done_up = False
            done_down = False
            for i in range(1000):
                smaller_format = larger_format.cast_to(5, 3)
                if smaller_format == rounded_down:
                    done_down = True
                elif smaller_format == rounded_up:
                    done_up = True
                else:
                    pytest.fail(f'{larger_format} was rounded to {smaller_format}')
                if done_down and done_up:
                     break
                

# Floating-point divison is implemented quite differently and should therefore be tested seperately
@pytest.mark.float_div
@pytest.mark.parametrize("a", [14, 20])
@pytest.mark.parametrize("b", [14, 20])
@pytest.mark.parametrize("sign", [1, -1])
class TestAPyFloatRoundingDiv:
    def test_to_positive(self, a, b, sign):
        with RoundingContext(RoundingMode.TO_POSITIVE):
            assert APyFloat.from_float(sign * a, 5, 5) / APyFloat.from_float(
                b, 5, 5
            ) == APyFloat.from_float(sign * a / b, 5, 5)

    def test_to_negative(self, a, b, sign):
        with RoundingContext(RoundingMode.TO_NEGATIVE):
            assert APyFloat.from_float(sign * a, 5, 5) / APyFloat.from_float(
                b, 5, 5
            ) == APyFloat.from_float(sign * a / b, 5, 5)

    def test_to_zero(self, a, b, sign):
        with RoundingContext(RoundingMode.TO_ZERO):
            assert APyFloat.from_float(sign * a, 5, 5) / APyFloat.from_float(
                b, 5, 5
            ) == APyFloat.from_float(sign * a / b, 5, 5)

    def test_to_ties_to_even(self, a, b, sign):
        with RoundingContext(RoundingMode.TIES_TO_EVEN):
            assert APyFloat.from_float(sign * a, 5, 5) / APyFloat.from_float(
                b, 5, 5
            ) == APyFloat.from_float(sign * a / b, 5, 5)

    def test_to_ties_to_away(self, a, b, sign):
        with RoundingContext(RoundingMode.TIES_TO_AWAY):
            assert APyFloat.from_float(sign * a, 5, 5) / APyFloat.from_float(
                b, 5, 5
            ) == APyFloat.from_float(sign * a / b, 5, 5)