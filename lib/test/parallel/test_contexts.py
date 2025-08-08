##
# Testing of concurrency support for the different APyTypes contexts
#

import threading

from apytypes import (
    APyFixed,
    APyFixedAccumulatorContext,
    APyFixedArray,
    APyFixedCastContext,
    APyFloat,
    APyFloatAccumulatorContext,
    APyFloatArray,
    APyFloatQuantizationContext,
    QuantizationMode,
)

# #################################################################################### #
# #                          Quantization context for APyFloat                       # #
# #################################################################################### #


def _test_apyfloat_quantization_context(success: list[bool] | None = None):
    a = APyFloat.from_float(0.25, exp_bits=2, man_bits=2)
    b = APyFloat.from_float(2.50, exp_bits=2, man_bits=2)
    assert (a + b).is_identical(APyFloat.from_float(3.00, exp_bits=2, man_bits=2))

    with APyFloatQuantizationContext(QuantizationMode.RND_ZERO):
        assert (a + b).is_identical(APyFloat.from_float(2.50, exp_bits=2, man_bits=2))
    with APyFloatQuantizationContext(QuantizationMode.RND_CONV_ODD):
        assert (a + b).is_identical(APyFloat.from_float(2.50, exp_bits=2, man_bits=2))
    with APyFloatQuantizationContext(QuantizationMode.RND_INF):
        assert (a + b).is_identical(APyFloat.from_float(3.00, exp_bits=2, man_bits=2))

    if success is not None:
        success[0] = True


def test_apyfloat_quantization_context():
    # Run test on main Python thread first
    _test_apyfloat_quantization_context()

    # Run the test on a new thread after entering a new accumulator context
    with APyFloatQuantizationContext(QuantizationMode.RND_ZERO):
        success = [False]
        t = threading.Thread(
            target=_test_apyfloat_quantization_context, args=(success,)
        )
        t.start()
        t.join()
        assert success[0]


# #################################################################################### #
# #                         Cast context for APyFixed                                # #
# #################################################################################### #


def _test_apyfixed_cast_context(success: list[bool] | None = None):
    a = APyFixed.from_float(1.5, int_bits=3, frac_bits=1)

    assert a.cast(int_bits=3, frac_bits=0).is_identical(
        APyFixed.from_float(1.0, int_bits=3, frac_bits=0)
    )
    assert a.cast(
        int_bits=3, frac_bits=0, quantization=QuantizationMode.RND
    ).is_identical(APyFixed.from_float(2.0, int_bits=3, frac_bits=0))

    with APyFixedCastContext(QuantizationMode.RND):
        assert a.cast(int_bits=3, frac_bits=0).is_identical(
            APyFixed.from_float(2.0, int_bits=3, frac_bits=0)
        )
    with APyFixedCastContext(QuantizationMode.TRN):
        assert a.cast(int_bits=3, frac_bits=0).is_identical(
            APyFixed.from_float(1.0, int_bits=3, frac_bits=0)
        )

    if success is not None:
        success[0] = True


def test_apyfixed_cast_context():
    # Run test on main Python thread first
    _test_apyfixed_cast_context()

    # Run the test on a new thread after entering a new accumulator context
    with APyFixedCastContext(QuantizationMode.RND):
        success = [False]
        t = threading.Thread(target=_test_apyfixed_cast_context, args=(success,))
        t.start()
        t.join()
        assert success[0]


# #################################################################################### #
# #                      Accumulator context for APyFixedArray                       # #
# #################################################################################### #


def _test_apyfixedarray_accumulator_context(success: list[bool] | None = None):
    a = APyFixedArray.from_float([0.25, 0.50], int_bits=1, frac_bits=2)
    b = APyFixedArray.from_float([0.75, 0.25], int_bits=1, frac_bits=2)

    assert (a @ b).is_identical(APyFixed.from_float(0.3125, int_bits=3, frac_bits=4))

    with APyFixedAccumulatorContext(int_bits=1, frac_bits=4):
        assert (a @ b).is_identical(
            APyFixed.from_float(0.3125, int_bits=1, frac_bits=4)
        )
    with APyFixedAccumulatorContext(int_bits=1, frac_bits=3):
        assert (a @ b).is_identical(APyFixed.from_float(0.25, int_bits=1, frac_bits=3))
    with APyFixedAccumulatorContext(int_bits=1, frac_bits=2):
        assert (a @ b).is_identical(APyFixed.from_float(0.0, int_bits=1, frac_bits=2))

    if success is not None:
        success[0] = True


def test_apyfixedarray_accumulator_context():
    # Run test on main Python thread first
    _test_apyfixedarray_accumulator_context()

    # Run the test on a new thread after entering a new accumulator context
    with APyFixedAccumulatorContext(int_bits=1, frac_bits=3):
        success = [False]
        t = threading.Thread(
            target=_test_apyfixedarray_accumulator_context, args=(success,)
        )
        t.start()
        t.join()
        assert success[0]


# #################################################################################### #
# #                      Accumulator context for APyFloatArray                       # #
# #################################################################################### #


def _test_apyfloatarray_accumulator_context(success: list[bool] | None = None):
    a = APyFloatArray.from_float([0.25, 0.50], exp_bits=2, man_bits=3)
    b = APyFloatArray.from_float([0.75, 0.25], exp_bits=2, man_bits=3)

    assert (a @ b).is_identical(APyFloat.from_float(0.375, exp_bits=2, man_bits=3))
    with APyFloatAccumulatorContext(exp_bits=10, man_bits=10):
        assert (a @ b).is_identical(
            APyFloat.from_float(0.3125, exp_bits=10, man_bits=10)
        )

    if success is not None:
        success[0] = True


def test_apyfloatarray_accumulator_context():
    # Run test on main Python thread first
    _test_apyfloatarray_accumulator_context()

    with APyFloatAccumulatorContext(exp_bits=10, man_bits=10):
        success = [False]
        t = threading.Thread(
            target=_test_apyfloatarray_accumulator_context, args=(success,)
        )
        t.start()
        t.join()
        assert success[0]
