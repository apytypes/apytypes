import apytypes
from apytypes import APyFloat, QuantizationMode, QuantizationContext
import pytest


class TestAPyFloatQuantization:
    """
    Test class for the different quantization modes for APyFloat.
    """

    default_mode = None

    def setup_class(self):
        """Save the current quantization mode so that it can be restored later for other tests."""
        self.default_mode = apytypes.get_quantization_mode()

    def teardown_class(self):
        """Restore quantization mode."""
        apytypes.set_quantization_mode(self.default_mode)

    def test_quantization_mantissa_overflow(self):
        apytypes.set_quantization_mode(QuantizationMode.TO_POS)
        assert APyFloat(0, 5, 0b11111, 5, 5).cast(5, 3) == APyFloat(0, 6, 0b000, 5, 3)
        assert APyFloat(0, 0b11110, 0b11111, 5, 5).cast(5, 3) == APyFloat(
            0, 0b11111, 0b000, 5, 3
        )  # Quantization becomes inf

        apytypes.set_quantization_mode(QuantizationMode.TO_NEG)
        assert APyFloat(1, 5, 0b11111, 5, 5).cast(5, 3) == APyFloat(1, 6, 0b000, 5, 3)
        assert APyFloat(1, 0b11110, 0b11111, 5, 5).cast(5, 3) == APyFloat(
            1, 0b11111, 0b000, 5, 3
        )  # Quantization becomes -inf

    def test_quantization_to_pos(self):
        apytypes.set_quantization_mode(QuantizationMode.TO_POS)
        # Quantization from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round up
        assert APyFloat(0, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round up

        # Quantization from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Round up
        assert APyFloat(0, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down

        # Quantization from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down

    def test_quantization_to_neg(self):
        apytypes.set_quantization_mode(QuantizationMode.TO_NEG)
        # Quantization from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down

        # Quantization from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down

        # Quantization from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down

        # Quantization from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Round up
        assert APyFloat(1, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(1, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Round up

    def test_quantization_to_zero(self):
        apytypes.set_quantization_mode(QuantizationMode.TO_ZERO)
        # Quantization from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down

        # Quantization from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down

        # Quantization from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down

        # Quantization from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down

    def test_quantization_ties_even(self):
        apytypes.set_quantization_mode(QuantizationMode.TIES_EVEN)
        # Quantization from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round up

        # Quantization from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round up

        # Quantization from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(1, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Round up

    def test_quantization_ties_away(self):
        apytypes.set_quantization_mode(QuantizationMode.TIES_AWAY)
        # Quantization from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round up

        # Quantization from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(0, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Tie break, round up
        assert APyFloat(1, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round up

        # Quantization from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Tie break, round up
        assert APyFloat(1, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Round up

    def test_quantization_ties_to_zero(self):
        apytypes.set_quantization_mode(QuantizationMode.TIES_ZERO)
        # Quantization from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round up

        # Quantization from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(0, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(0, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(0, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            0, 5, 0b110, 5, 3
        )  # Round up

        # Quantization from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b100, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round up

        # Quantization from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # No quantization needed
        assert APyFloat(1, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Round down
        assert APyFloat(1, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b101, 5, 3
        )  # Tie break, round down
        assert APyFloat(1, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(
            1, 5, 0b110, 5, 3
        )  # Round up

    def test_quantization_jamming(self):
        apytypes.set_quantization_mode(QuantizationMode.JAM)
        # Quantization from 0.xx
        assert APyFloat(0, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(0, 5, 0b101, 5, 3)
        assert APyFloat(0, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(0, 5, 0b101, 5, 3)
        assert APyFloat(0, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(0, 5, 0b101, 5, 3)
        assert APyFloat(0, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(0, 5, 0b101, 5, 3)

        # Quantization from 1.xx
        assert APyFloat(0, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(0, 5, 0b101, 5, 3)
        assert APyFloat(0, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(0, 5, 0b101, 5, 3)
        assert APyFloat(0, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(0, 5, 0b101, 5, 3)
        assert APyFloat(0, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(0, 5, 0b101, 5, 3)

        # Quantization from 0.xx, negative sign
        assert APyFloat(1, 5, 0b10000, 5, 5).cast(5, 3) == APyFloat(1, 5, 0b101, 5, 3)
        assert APyFloat(1, 5, 0b10001, 5, 5).cast(5, 3) == APyFloat(1, 5, 0b101, 5, 3)
        assert APyFloat(1, 5, 0b10010, 5, 5).cast(5, 3) == APyFloat(1, 5, 0b101, 5, 3)
        assert APyFloat(1, 5, 0b10011, 5, 5).cast(5, 3) == APyFloat(1, 5, 0b101, 5, 3)

        # Quantization from 1.xx, negative sign
        assert APyFloat(1, 5, 0b10100, 5, 5).cast(5, 3) == APyFloat(1, 5, 0b101, 5, 3)
        assert APyFloat(1, 5, 0b10101, 5, 5).cast(5, 3) == APyFloat(1, 5, 0b101, 5, 3)
        assert APyFloat(1, 5, 0b10110, 5, 5).cast(5, 3) == APyFloat(1, 5, 0b101, 5, 3)
        assert APyFloat(1, 5, 0b10111, 5, 5).cast(5, 3) == APyFloat(1, 5, 0b101, 5, 3)

    def test_quantization_stochastic_weighted(self):
        """
        A bit naive, but test that a value can be rounded both up and down in 1000 tries.
        An exact value should however not be rounded.
        """
        apytypes.set_quantization_mode(QuantizationMode.STOCH_WEIGHTED)
        larger_format = APyFloat(0, 5, 0b10000, 5, 5)
        assert larger_format.cast(5, 3) == APyFloat(0, 5, 0b100, 5, 3)

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
        apytypes.set_quantization_mode(QuantizationMode.STOCH_EQUAL)
        larger_format = APyFloat(0, 5, 0b10000, 5, 5)
        assert larger_format.cast(5, 3) == APyFloat(0, 5, 0b100, 5, 3)

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
