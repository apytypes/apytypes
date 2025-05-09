import pytest

from apytypes import APyCFloat, APyFloatQuantizationContext, QuantizationMode


@pytest.mark.parametrize("apyfloat", [APyCFloat])
def test_issue_245(apyfloat: type[APyCFloat]):
    # Test that jamming bit *is* applied even when operands are zero
    # https://github.com/apytypes/apytypes/issues/245
    with APyFloatQuantizationContext(QuantizationMode.JAM):
        res = apyfloat(0, 15, 0, 5, 2) + apyfloat(0, 0, 0, 5, 2)
        assert res.is_identical(apyfloat((0, 0), (15, 0), (1, 1), 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.JAM):
        res = apyfloat(0, 15, 0, 5, 2) - apyfloat(0, 0, 0, 5, 2)
        assert res.is_identical(apyfloat((0, 0), (15, 0), (1, 1), 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.JAM):
        res = apyfloat(0, 15, 0, 5, 2) * apyfloat(0, 0, 0, 5, 2)
        assert res.is_identical(apyfloat((0, 0), (0, 0), (1, 1), 5, 2))

    with APyFloatQuantizationContext(QuantizationMode.JAM):
        res = apyfloat(0, 0, 0, 5, 2) / apyfloat(0, 15, 0, 5, 2)
        assert res.is_identical(apyfloat((0, 0), (0, 0), (1, 1), 5, 2))
