import pytest

from apytypes import (
    APyFixed,
    APyFixedAccumulatorContext,
    APyFixedArray,
    APyFixedCastContext,
    APyFloatAccumulatorContext,
    APyFloatQuantizationContext,
    OverflowMode,
    QuantizationMode,
    fp,
    fx,
    get_fixed_quantization_seed,
    get_float_quantization_mode,
    get_float_quantization_seed,
    set_fixed_quantization_seed,
    set_float_quantization_mode,
    set_float_quantization_seed,
)


class TestFxCastContext:
    """
    This test class doesn't test if cast itself works, just that the context manager
    acts correctly.
    """

    def test_raises(self):
        with pytest.raises(ValueError, match=r".*must be specified"):
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

    def test_with_cast_context(self):
        a = APyFixed.from_float(1.25, int_bits=3, frac_bits=2)
        # TRN by default
        assert (_ := a.cast(int_bits=3, frac_bits=1)).is_identical(
            APyFixed(2, bits=4, int_bits=3)
        )

        with APyFixedCastContext(quantization=QuantizationMode.TRN_INF):
            assert (_ := a.cast(int_bits=3, frac_bits=1)).is_identical(
                APyFixed(3, bits=4, int_bits=3)
            )

        # TRN by default again
        assert (_ := a.cast(int_bits=3, frac_bits=1)).is_identical(
            APyFixed(2, bits=4, int_bits=3)
        )

    def test_cast_context_override(self):
        a = APyFixed.from_float(1.25, int_bits=3, frac_bits=2)

        with APyFixedCastContext(quantization=QuantizationMode.TRN_INF):
            assert (_ := a.cast(int_bits=3, frac_bits=1)).is_identical(
                APyFixed(3, bits=4, int_bits=3)
            )

            # Use TRN explicitly inside context
            assert (
                _ := a.cast(int_bits=3, frac_bits=1, quantization=QuantizationMode.TRN)
            ).is_identical(APyFixed(2, bits=4, int_bits=3))

    def test_cast_context_nested(self):
        a = APyFixed.from_float(1.25, int_bits=3, frac_bits=2)
        # TRN by default
        assert (_ := a.cast(int_bits=3, frac_bits=1)).is_identical(
            APyFixed(2, bits=4, int_bits=3)
        )

        with APyFixedCastContext(quantization=QuantizationMode.TRN_INF):
            assert (_ := a.cast(int_bits=3, frac_bits=1)).is_identical(
                APyFixed(3, bits=4, int_bits=3)
            )

            with APyFixedCastContext(quantization=QuantizationMode.TRN):
                assert (_ := a.cast(int_bits=3, frac_bits=1)).is_identical(
                    APyFixed(2, bits=4, int_bits=3)
                )

            assert (_ := a.cast(int_bits=3, frac_bits=1)).is_identical(
                APyFixed(3, bits=4, int_bits=3)
            )

        assert (_ := a.cast(int_bits=3, frac_bits=1)).is_identical(
            APyFixed(2, bits=4, int_bits=3)
        )

    def test_cast_context_overflow(self):
        """
        If the previous tests pass for the quantization mode, then overflow should also
        work. These are therefore more like sanity tests.
        """
        a = APyFixed.from_float(4, int_bits=4, frac_bits=0)
        # WRAP by default
        assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
            APyFixed(4, bits=3, int_bits=3)
        )

        # Set only overflow mode
        with APyFixedCastContext(overflow=OverflowMode.SAT):
            assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                APyFixed(3, bits=3, int_bits=3)
            )

        # Override overflow mode
        with APyFixedCastContext(overflow=OverflowMode.SAT):
            assert (
                _ := a.cast(int_bits=3, frac_bits=0, overflow=OverflowMode.WRAP)
            ).is_identical(APyFixed(4, bits=3, int_bits=3))

        # Nested contexts
        with APyFixedCastContext(overflow=OverflowMode.SAT):
            assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                APyFixed(3, bits=3, int_bits=3)
            )

            with APyFixedCastContext(overflow=OverflowMode.WRAP):
                assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                    APyFixed(4, bits=3, int_bits=3)
                )

            assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                APyFixed(3, bits=3, int_bits=3)
            )

        # Set both quantization mode and overflow mode
        a = APyFixed.from_float(3.5, int_bits=3, frac_bits=1)
        quantization_mode = QuantizationMode.TRN_INF
        overflow_mode = OverflowMode.SAT
        assert (
            _ := a.cast(
                int_bits=3,
                frac_bits=0,
                quantization=quantization_mode,
                overflow=overflow_mode,
            )
        ).is_identical(APyFixed(3, int_bits=3, frac_bits=0))

        with APyFixedCastContext(
            quantization=quantization_mode, overflow=overflow_mode
        ):
            assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                APyFixed(3, int_bits=3, frac_bits=0)
            )

    def test_cast_context_array(self):
        """
        Basic sanity tests, if the scalar case works then it should work for arrays as
        well.
        """
        a = APyFixedArray.from_float([4], int_bits=4, frac_bits=0)
        # WRAP by default
        assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
            APyFixedArray([4], bits=3, int_bits=3)
        )

        # Set only overflow mode
        with APyFixedCastContext(overflow=OverflowMode.SAT):
            assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                APyFixedArray([3], bits=3, int_bits=3)
            )

        # Override overflow mode
        with APyFixedCastContext(overflow=OverflowMode.SAT):
            assert (
                _ := a.cast(int_bits=3, frac_bits=0, overflow=OverflowMode.WRAP)
            ).is_identical(APyFixedArray([4], bits=3, int_bits=3))

        # Nested contexts
        with APyFixedCastContext(overflow=OverflowMode.SAT):
            assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                APyFixedArray([3], bits=3, int_bits=3)
            )

            with APyFixedCastContext(overflow=OverflowMode.WRAP):
                assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                    APyFixedArray([4], bits=3, int_bits=3)
                )

            assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                APyFixedArray([3], bits=3, int_bits=3)
            )

        # Set both quantization mode and overflow mode
        a = APyFixedArray.from_float([3.5], int_bits=3, frac_bits=1)
        quantization_mode = QuantizationMode.TRN_INF
        overflow_mode = OverflowMode.SAT
        assert (
            _ := a.cast(
                int_bits=3,
                frac_bits=0,
                quantization=quantization_mode,
                overflow=overflow_mode,
            )
        ).is_identical(APyFixedArray([3], int_bits=3, frac_bits=0))

        with APyFixedCastContext(
            quantization=quantization_mode, overflow=overflow_mode
        ):
            assert (_ := a.cast(int_bits=3, frac_bits=0)).is_identical(
                APyFixedArray([3], int_bits=3, frac_bits=0)
            )

    def test_cast_context_default_stoch_seed(self):
        seed = get_fixed_quantization_seed()
        new_seed = seed + 1
        assert get_fixed_quantization_seed != new_seed
        set_fixed_quantization_seed(new_seed)
        assert get_fixed_quantization_seed() == new_seed

    def test_context_state_captured_and_restored(self):
        """
        Contexts with state must be captured on enter and then properly restored on
        exit. Test especially that this holds for multiple levels of contexts.
        """
        a = fx(1.5, int_bits=3, frac_bits=1)
        b = fx(-1.5, int_bits=3, frac_bits=1)

        # Global context where `TRN` should apply
        assert a.cast(int_bits=3, frac_bits=0).is_identical(fx(1.0, 3, 0))
        assert b.cast(int_bits=3, frac_bits=0).is_identical(fx(-2.0, 3, 0))

        # Outer context created, but not yet entered. Global context still applies
        ctx_outer = APyFixedCastContext(quantization=QuantizationMode.RND)
        assert a.cast(int_bits=3, frac_bits=0).is_identical(fx(1.0, 3, 0))
        assert b.cast(int_bits=3, frac_bits=0).is_identical(fx(-2.0, 3, 0))

        # Inner context created, but not yet entered. Global context still applies
        ctx_inner = APyFixedCastContext(quantization=QuantizationMode.TRN_ZERO)
        assert a.cast(int_bits=3, frac_bits=0).is_identical(fx(1.0, 3, 0))
        assert b.cast(int_bits=3, frac_bits=0).is_identical(fx(-2.0, 3, 0))

        with ctx_outer:
            assert a.cast(int_bits=3, frac_bits=0).is_identical(fx(2.0, 3, 0))
            assert b.cast(int_bits=3, frac_bits=0).is_identical(fx(-1.0, 3, 0))
            with ctx_inner:
                assert a.cast(int_bits=3, frac_bits=0).is_identical(fx(1.0, 3, 0))
                assert b.cast(int_bits=3, frac_bits=0).is_identical(fx(-1.0, 3, 0))
            # The context should be restored to the `ctx_outer`
            assert a.cast(int_bits=3, frac_bits=0).is_identical(fx(2.0, 3, 0))
            assert b.cast(int_bits=3, frac_bits=0).is_identical(fx(-1.0, 3, 0))

        assert a.cast(int_bits=3, frac_bits=0).is_identical(fx(1.0, 3, 0))
        assert b.cast(int_bits=3, frac_bits=0).is_identical(fx(-2.0, 3, 0))


class TestAccumulatorContext:
    """
    This test class doesn't test if the accumulation itself works,
    just that the context manager acts correctly.
    """

    def test_raises(self):
        with pytest.raises(ValueError, match=r"Both.*must be specified"):
            with APyFloatAccumulatorContext():
                pass

        with pytest.raises(ValueError, match=r".*specification needs"):
            with APyFixedAccumulatorContext():
                pass

        with APyFixedAccumulatorContext(bits=5, int_bits=2):  # should not raise
            pass

        with APyFloatAccumulatorContext(exp_bits=5, man_bits=2):  # should not raise
            pass

        with pytest.raises(
            ValueError,
            match=r"APyFloatAccumulatorContext: exponent bits must be a non-negative integer less or equal to .. but 300 was given",
        ):
            _ = APyFloatAccumulatorContext(exp_bits=300, man_bits=5)
        with pytest.raises(
            ValueError,
            match=r"APyFloatAccumulatorContext: exponent bits must be a non-negative integer less or equal to .. but -300 was given",
        ):
            _ = APyFloatAccumulatorContext(exp_bits=-300, man_bits=5)
        # Too many mantissa bits
        with pytest.raises(
            ValueError,
            match=r"APyFloatAccumulatorContext: mantissa bits must be a non-negative integer less or equal to .. but 300 was given",
        ):
            _ = APyFloatAccumulatorContext(exp_bits=5, man_bits=300)
        with pytest.raises(
            ValueError,
            match=r"APyFloatAccumulatorContext: mantissa bits must be a non-negative integer less or equal to .. but -300 was given",
        ):
            _ = APyFloatAccumulatorContext(exp_bits=5, man_bits=-300)

    def test_with_quantization_context(self):
        """Test that the APyFloatAccumulatorContext interacts correctly with the APyFloatQuanizationContext."""
        with APyFloatQuantizationContext(QuantizationMode.TO_POS):
            assert get_float_quantization_mode() == QuantizationMode.TO_POS

            with APyFloatAccumulatorContext(exp_bits=5, man_bits=3):
                assert get_float_quantization_mode() == QuantizationMode.TO_POS

            with APyFloatAccumulatorContext(
                exp_bits=5, man_bits=3, quantization=QuantizationMode.TO_NEG
            ):
                assert get_float_quantization_mode() == QuantizationMode.TO_POS

    def test_with_seed(self):
        # Test setting a stochastic quantization mode without changing the seed
        set_float_quantization_seed(123)
        with APyFloatAccumulatorContext(
            exp_bits=5, man_bits=4, quantization=QuantizationMode.STOCH_EQUAL
        ):
            pass
        assert get_float_quantization_seed() == 123

        # Test setting a stochastic quantization mode and changing the seed
        with APyFloatAccumulatorContext(
            exp_bits=5,
            man_bits=4,
            quantization=QuantizationMode.STOCH_WEIGHTED,
            seed=77,
        ):
            assert get_float_quantization_seed() == 77
        assert get_float_quantization_seed() == 123

    def test_nested_seed(self):
        set_float_quantization_seed(123)
        with APyFloatAccumulatorContext(
            exp_bits=5,
            man_bits=4,
            quantization=QuantizationMode.STOCH_WEIGHTED,
            seed=456,
        ):
            assert get_float_quantization_seed() == 456
            with APyFloatAccumulatorContext(
                exp_bits=5,
                man_bits=4,
                quantization=QuantizationMode.STOCH_EQUAL,
                seed=789,
            ):
                assert get_float_quantization_seed() == 789
            assert get_float_quantization_seed() == 456
        assert get_float_quantization_seed() == 123

    def test_seed_with_quantization_context(self):
        set_float_quantization_seed(123)
        with APyFloatAccumulatorContext(
            exp_bits=5,
            man_bits=4,
            quantization=QuantizationMode.STOCH_WEIGHTED,
            seed=456,
        ):
            assert get_float_quantization_seed() == 456
            with APyFloatQuantizationContext(
                quantization=QuantizationMode.STOCH_EQUAL, seed=789
            ):
                assert get_float_quantization_seed() == 789
            assert get_float_quantization_seed() == 456
        assert get_float_quantization_mode() == QuantizationMode.TIES_EVEN
        assert get_float_quantization_seed() == 123

    def test_fx_context_state_captured_and_restored(self):
        """
        Contexts with state must be captured on enter and then properly restored on
        exit. Test especially that this holds for multiple levels of contexts.
        """
        a = fx([0.75], int_bits=2, frac_bits=2)

        # Global context where no accumulator option is applied
        assert (a @ a).is_identical(fx(0.5625, int_bits=4, frac_bits=4))

        # Outer context created, but not yet entered. Global context still applies
        ctx_outer = APyFixedAccumulatorContext(int_bits=4, frac_bits=3)
        assert (a @ a).is_identical(fx(0.5625, int_bits=4, frac_bits=4))

        # Inner context created, but not yet entered. Global context still applies
        ctx_inner = APyFixedAccumulatorContext(int_bits=4, frac_bits=1)
        assert (a @ a).is_identical(fx(0.5625, int_bits=4, frac_bits=4))

        with ctx_outer:
            assert (a @ a).is_identical(fx(0.525, int_bits=4, frac_bits=3))
            with ctx_inner:
                assert (a @ a).is_identical(fx(0.50, int_bits=4, frac_bits=1))

            # The context is restored to `ctx_outer`
            assert (a @ a).is_identical(fx(0.525, int_bits=4, frac_bits=3))

        # The context is restored to the global state
        assert (a @ a).is_identical(fx(0.5625, int_bits=4, frac_bits=4))

    def test_fp_context_state_captured_and_restored(self):
        """
        Contexts with state must be captured on enter and then properly restored on
        exit. Test especially that this holds for multiple levels of contexts.
        """
        a = fp([2.0**10 + 2.0**-10], exp_bits=7, man_bits=22)

        # Global context where no accumulator option is applied
        assert (a @ a).is_identical(fp(2.0**20 + 2.0, exp_bits=7, man_bits=22))

        # Outer context created, but not yet entered. Global context still applies
        ctx_outer = APyFloatAccumulatorContext(exp_bits=10, man_bits=48)
        assert (a @ a).is_identical(fp(2.0**20 + 2.0, exp_bits=7, man_bits=22))

        # Inner context created, but not yet entered. Global context still applies
        ctx_inner = APyFloatAccumulatorContext(
            exp_bits=7, man_bits=22, quantization=QuantizationMode.TRN_INF
        )
        assert (a @ a).is_identical(fp(2.0**20 + 2.0, exp_bits=7, man_bits=22))

        with ctx_outer:
            assert (a @ a).is_identical(
                fp(2.0**20 + 2.0 + 2.0**-20, exp_bits=10, man_bits=48)
            )
            with ctx_inner:
                assert (a @ a).is_identical(
                    fp(2.0**20 + 2.0 + 2**-2, exp_bits=7, man_bits=22)
                )

            # The context is restored to `ctx_outer`
            assert (a @ a).is_identical(
                fp(2.0**20 + 2.0 + 2.0**-20, exp_bits=10, man_bits=48)
            )

        # The context is restored to the global state
        assert (a @ a).is_identical(fp(2.0**20 + 2.0, exp_bits=7, man_bits=22))


class TestFpQuantizationContext:
    """
    This test class doesn't test if the quantization itself works, just that the context
    manager acts correctly.
    """

    default_mode = None

    def setup_class(self):
        """Save the current quantization mode so that it can be restored later for other tests."""
        self.default_mode = get_float_quantization_mode()

    def teardown_class(self):
        """Restore quantization mode."""
        set_float_quantization_mode(self.default_mode)

    def test_default_mode(self):
        assert get_float_quantization_mode() == QuantizationMode.TIES_EVEN

    def test_prerequisite(self):
        """Make sure get/set works for the quantization mode to begin with."""
        set_float_quantization_mode(QuantizationMode.TO_POS)
        assert get_float_quantization_mode() == QuantizationMode.TO_POS
        set_float_quantization_mode(QuantizationMode.TO_NEG)
        assert get_float_quantization_mode() == QuantizationMode.TO_NEG

        set_float_quantization_seed(5)
        assert get_float_quantization_seed() == 5
        set_float_quantization_seed(1 << 57)
        assert get_float_quantization_seed() == (1 << 57)

    def test_not_nested(self):
        """Single layer of context."""
        # Test setting a non-stochastic quantization mode
        set_float_quantization_mode(QuantizationMode.TO_POS)
        with APyFloatQuantizationContext(QuantizationMode.TO_NEG):
            assert get_float_quantization_mode() == QuantizationMode.TO_NEG
        assert get_float_quantization_mode() == QuantizationMode.TO_POS

        # Test setting a stochastic quantization mode without changing the seed
        set_float_quantization_seed(123)
        with APyFloatQuantizationContext(QuantizationMode.STOCH_EQUAL):
            assert get_float_quantization_mode() == QuantizationMode.STOCH_EQUAL
        assert get_float_quantization_mode() == QuantizationMode.TO_POS
        assert get_float_quantization_seed() == 123

        # Test setting a stochastic quantization mode and changing the seed
        with APyFloatQuantizationContext(QuantizationMode.STOCH_WEIGHTED, 77):
            assert get_float_quantization_mode() == QuantizationMode.STOCH_WEIGHTED
            assert get_float_quantization_seed() == 77
        assert get_float_quantization_mode() == QuantizationMode.TO_POS
        assert get_float_quantization_seed() == 123

    def test_nested(self):
        """Nested context layers."""
        set_float_quantization_mode(QuantizationMode.TO_POS)
        set_float_quantization_seed(123)
        with APyFloatQuantizationContext(QuantizationMode.STOCH_WEIGHTED, 456):
            assert get_float_quantization_mode() == QuantizationMode.STOCH_WEIGHTED
            assert get_float_quantization_seed() == 456
            with APyFloatQuantizationContext(QuantizationMode.STOCH_EQUAL, 789):
                assert get_float_quantization_mode() == QuantizationMode.STOCH_EQUAL
                assert get_float_quantization_seed() == 789
            assert get_float_quantization_mode() == QuantizationMode.STOCH_WEIGHTED
            assert get_float_quantization_seed() == 456
        assert get_float_quantization_mode() == QuantizationMode.TO_POS
        assert get_float_quantization_seed() == 123

    def test_no_exception(self):
        """Make sure no exception is raised if a seed is given for APyFloatQuantizationContext with a non-stochastic quantization mode."""
        with APyFloatQuantizationContext(QuantizationMode.TO_POS, 123):
            assert get_float_quantization_mode() == QuantizationMode.TO_POS
            assert get_float_quantization_seed() == 123
