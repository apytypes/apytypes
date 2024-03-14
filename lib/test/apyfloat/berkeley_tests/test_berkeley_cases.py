import os
import pytest
from apytypes import APyFloat


def read_test_cases(testfile: str):
    test_cases = []
    with open(testfile) as file:
        for line in file:
            lhs, rhs, res, _ = line.split()
            test_cases.append((int(lhs, 16), int(rhs, 16), int(res, 16)))
    return test_cases


# Test cases generated using Berkeley TestFloat version 3e.
current_directory = os.path.dirname(os.path.realpath(__file__))
fp16_add_to_even = read_test_cases(f"{current_directory}/fp16_add_to_even.txt")
fp16_sub_to_even = read_test_cases(f"{current_directory}/fp16_sub_to_even.txt")
fp16_mul_to_even = read_test_cases(f"{current_directory}/fp16_mul_to_even.txt")
fp16_div_to_even = read_test_cases(f"{current_directory}/fp16_div_to_even.txt")


@pytest.mark.slow
@pytest.mark.parametrize(
    "lhs_bits, rhs_bits, res_bits",
    [
        (fp16_add_to_even[i][0], fp16_add_to_even[i][1], fp16_add_to_even[i][2])
        for i in range(len(fp16_add_to_even))
    ],
)
def test_add_berkeley(lhs_bits, rhs_bits, res_bits):
    lhs = APyFloat.from_bits(lhs_bits, 5, 10)
    rhs = APyFloat.from_bits(rhs_bits, 5, 10)
    ans = lhs + rhs
    ref = APyFloat.from_bits(res_bits, 5, 10)
    print(f"{lhs} + {rhs} = {ans} != {ref}")
    if ref.is_nan:
        assert ans.is_nan
    else:
        assert ans == ref


@pytest.mark.slow
@pytest.mark.parametrize(
    "lhs_bits, rhs_bits, res_bits",
    [
        (fp16_sub_to_even[i][0], fp16_sub_to_even[i][1], fp16_sub_to_even[i][2])
        for i in range(len(fp16_sub_to_even))
    ],
)
def test_sub_berkeley(lhs_bits, rhs_bits, res_bits):
    lhs = APyFloat.from_bits(lhs_bits, 5, 10)
    rhs = APyFloat.from_bits(rhs_bits, 5, 10)
    ans = lhs - rhs
    ref = APyFloat.from_bits(res_bits, 5, 10)
    print(f"{lhs} - {rhs} = {ans} != {ref}")
    if ref.is_nan:
        assert ans.is_nan
    else:
        assert ans == ref


@pytest.mark.slow
@pytest.mark.parametrize(
    "lhs_bits, rhs_bits, res_bits",
    [
        (fp16_mul_to_even[i][0], fp16_mul_to_even[i][1], fp16_mul_to_even[i][2])
        for i in range(len(fp16_mul_to_even))
    ],
)
# @pytest.mark.parametrize("lhs_bits, rhs_bits, res_bits", [(35363, 44304, 249)])
def test_mul_berkeley(lhs_bits, rhs_bits, res_bits):
    lhs = APyFloat.from_bits(lhs_bits, 5, 10)
    rhs = APyFloat.from_bits(rhs_bits, 5, 10)
    ref = APyFloat.from_bits(res_bits, 5, 10)
    ans = lhs * rhs
    print(f"{lhs} * {rhs} = {ans} != {ref}")
    if ref.is_nan:
        assert ans.is_nan
    else:
        assert ans == ref


@pytest.mark.slow
@pytest.mark.parametrize(
    "lhs_bits, rhs_bits, res_bits",
    [
        (fp16_div_to_even[i][0], fp16_div_to_even[i][1], fp16_div_to_even[i][2])
        for i in range(len(fp16_div_to_even))
    ],
)
def test_div_berkeley(lhs_bits, rhs_bits, res_bits):
    lhs = APyFloat.from_bits(lhs_bits, 5, 10)
    rhs = APyFloat.from_bits(rhs_bits, 5, 10)
    ans = lhs / rhs
    ref = APyFloat.from_bits(res_bits, 5, 10)
    print(f"{lhs} / {rhs} = {ans} != {ref}")
    if ref.is_nan:
        assert ans.is_nan
    else:
        assert ans == ref
