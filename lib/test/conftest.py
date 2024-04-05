import apytypes


def pytest_collection_finish(session):
    print("")
    print("APyTypes test suite:")
    print(apytypes._get_simd_version_str())
