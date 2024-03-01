import importlib
import timeit

libraries = {
    "APyTypes": "bench_apytypes",
    "fixedpoint": "bench_fixedpoint",
    "fpbinary": "bench_fpbinary",
    "fxpmath": "bench_fxpmath",
    "spfpm": "bench_spfpm",
}

CREATE_TWO_SCALARS = (
    "a = lib.create_scalar(0.37, 32, 16); b = lib.create_scalar(1.54, 32, 16)"
)
CREATE_TWO_LONG_SCALARS = (
    "a = lib.create_scalar(0.37, 320, 16); b = lib.create_scalar(1.54, 320, 16)"
)
CREATE_TWO_ARRAYS = "import numpy as np; a = lib.create_array(np.random.rand(200, 200), 32, 16); b = lib.create_array(np.random.rand(200, 200), 32, 16)"
CREATE_VECTOR = "import numpy as np; a = lib.create_array(np.random.rand(200), 32, 16)"
benchmarks = {
    "Scalar creation, 32-bit": ("lib.create_scalar(0.37, 32, 16)", "pass"),
    "Scalar addition, 32-bit": ("c = a + b", CREATE_TWO_SCALARS),
    "Scalar multiplication, 32-bit": ("c = a * b", CREATE_TWO_SCALARS),
    "Scalar division, 32-bit": ("c = a / b", CREATE_TWO_SCALARS),
    "Scalar to float, 32-bit": ("c = float(a)", CREATE_TWO_SCALARS),
    "Scalar creation, 320-bit": ("lib.create_scalar(0.37, 32, 16)", "pass"),
    "Scalar addition, 320-bit": ("c = a + b", CREATE_TWO_LONG_SCALARS),
    "Scalar multiplication, 320-bit": ("c = a * b", CREATE_TWO_LONG_SCALARS),
    "Scalar division, 320-bit": ("c = a / b", CREATE_TWO_LONG_SCALARS),
    "Scalar to float, 320-bit": ("c = float(a)", CREATE_TWO_LONG_SCALARS),
    "Vector summation": ("sum(a)", CREATE_VECTOR),
    "Array creation": (
        "lib.create_array(a, 32, 16)",
        "import numpy as np; a = np.random.rand(200, 200)",
    ),
    "Array addition": ("c = a + b", CREATE_TWO_ARRAYS),
    "Elementwise array multiplication": ("c = a * b", CREATE_TWO_ARRAYS),
    "Matrix multiplication": ("c = a @ b", CREATE_TWO_ARRAYS),
}

results = dict()
for lib_name, file in libraries.items():
    results[lib_name] = []
    for name, (func, setup) in benchmarks.items():
        setup_tmp = (
            f"import importlib; lib = importlib.import_module('{file}');" + setup
        )
        try:
            num, timing = timeit.Timer(stmt=func, setup=setup_tmp).autorange()
            results[lib_name].append(timing / num)
            print(f"{lib_name} {name} (average of {num}): {timing/num}")
        except Exception:
            results[lib_name].append(float("NaN"))
            print(f"{lib_name} {name}: Error")

# %%
import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(layout="constrained", figsize=(7, 10))

x = np.arange(len(benchmarks))  # the label locations
width = 1 / (len(libraries) + 1)  # the width of the bars
multiplier = 0

for library, measurement in results.items():
    offset = width * multiplier
    rects = ax.bar(x + offset, measurement, width, label=library)
    multiplier += 1

ax.set_yscale("log")
ax.set_ylabel("Time, s")
ax.set_xticks(
    x + width,
    benchmarks.keys(),
    rotation=90,
)
ax.legend(loc="upper left")
