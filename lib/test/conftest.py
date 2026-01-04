import sys

import apytypes


def pytest_collection_finish(session):
    print()
    print("This is the APyTypes test suite:")
    print(f"  * Platform: {sys.platform}")
    print(f"  * APyTypes version: {apytypes.__version__}")
    print(f"  * APyTypes threads: {apytypes.n_threads()}")
    print(f"  * {apytypes._get_simd_version_str()}")

    try:
        from numpy import __version__

        print(f"  * NumPy version: {__version__}")
    except ImportError:
        print("  * NumPy not available")

    try:
        from torch import __version__

        print(f"  * PyTorch version: {__version__}")
    except ImportError:
        print("  * PyTorch not available")

    try:
        from tensorflow import __version__

        print(f"  * TensorFlow version: {__version__}")
    except ImportError:
        print("  * TensorFlow not available")

    try:
        from jax import __version__

        print(f"  * JAX version: {__version__}")
    except ImportError:
        print("  * JAX not available")

    try:
        import cupy

        print(f"  * CuPy version: {cupy.__version__}")
    except ImportError:
        print("  * CuPy not available")
