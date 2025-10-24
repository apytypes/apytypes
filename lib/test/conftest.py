import sys

import apytypes


def pytest_collection_finish(session):
    print()
    print("This is the APyTypes test suite:")
    print(f"  * sys.platform: '{sys.platform}'")
    print(f"  * apytypes.__version__: '{apytypes.__version__}'")
    print(f"  * {apytypes._get_simd_version_str()}")

    try:
        from numpy import __version__

        print(f"  * NumPy version: {__version__}")
    except ImportError:
        print("  * No NumPy module detected")
