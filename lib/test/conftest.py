import apytypes


def pytest_collection_finish(session):
    print()
    print("APyTypes test suite:")
    print(f" * {apytypes._get_simd_version_str()}")

    try:
        import numpy as np

        print(f" * NumPy version: {np.__version__}")
    except ImportError:
        print(" * No NumPy support detected")
