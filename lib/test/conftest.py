import sys

import apytypes


def pytest_collection_finish(session):
    print()
    print("This is the APytypes test suite:")
    print(f" * sys.platform == '{sys.platform}'")
    print(f" * {apytypes._get_simd_version_str()}")

    try:
        import numpy as np

        print(f" * NumPy version: {np.__version__}")
    except ImportError:
        print(" * No NumPy support detected")
