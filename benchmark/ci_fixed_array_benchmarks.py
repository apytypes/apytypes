#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Mar 14 08:46:11 2024

@author: oscgu95
"""

from apytypes import APyFixedArray, APyFixed
import numpy as np
import pytest


def test_fixed_matrix_multiplication_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(lambda x, y: x @ y, a, b)


# def test_fixed_matrix_multiplication_200(benchmark):
#     a = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)
#     b = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)

#     benchmark(lambda x, y: x @ y, a, b)


def test_fixed_matrix_addition_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_addition_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_matrix_elementwise_multiplication_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_elementwise_multiplication_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)
    b = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_negation_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(lambda x: -x, a)


def test_fixed_matrix_negation_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200) - 0.5, 19, 1)

    benchmark(lambda x: -x, a)


def test_fixed_matrix_abs_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20) - 0.5, 19, 1)

    benchmark(abs, a)


def test_fixed_matrix_abs_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)

    benchmark(abs, a)


def test_fixed_scalar_matrix_addition_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20), 19, 1)
    b = APyFixed.from_float(np.random.rand(1), 19, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_matrix_addition_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)
    b = APyFixed.from_float(np.random.rand(1), 19, 1)

    benchmark(lambda x, y: x + y, a, b)


def test_fixed_scalar_matrix_multiplication_20(benchmark):
    a = APyFixedArray.from_float(np.random.rand(20, 20), 19, 1)
    b = APyFixed.from_float(np.random.rand(1), 19, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_scalar_matrix_multiplication_200(benchmark):
    a = APyFixedArray.from_float(np.random.rand(200, 200), 19, 1)
    b = APyFixed.from_float(np.random.rand(1), 19, 1)

    benchmark(lambda x, y: x * y, a, b)


def test_fixed_matrix_creation_20(benchmark):
    a = np.random.rand(20, 20)

    benchmark(APyFixedArray.from_float, a, 19, 1)


def test_fixed_matrix_creation_200(benchmark):
    a = np.random.rand(200, 200)

    benchmark(APyFixedArray.from_float, a, 19, 1)
