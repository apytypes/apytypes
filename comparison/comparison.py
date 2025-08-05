import os
import sys
import timeit

import matplotlib.pyplot as plt
import numpy as np

# Set up path for en_cl_fix
root = os.path.dirname(__file__)  # noqa: PTH120
en_cl_path = os.path.join(root, "../en_cl_fix/bittrue/models/python")  # noqa: PTH118
sys.path.append(en_cl_path)

libraries = {
    "APyTypes": "compare_apytypes",
    "en_cl_fix": "compare_en_cl_fix",
    "fixedpoint": "compare_fixedpoint",
    "fpbinary": "compare_fpbinary",
    "fxpmath": "compare_fxpmath",
    "numfi": "compare_numfi",
    "spfpm": "compare_spfpm",
}

CREATE_TWO_SCALARS = (
    "a = lib.create_scalar(0.37, 32, 16); b = lib.create_scalar(-1.54, 32, 16)"
)
CREATE_TWO_LONG_SCALARS = (
    "a = lib.create_scalar(0.37, 320, 160); b = lib.create_scalar(-1.54, 320, 160)"
)
CREATE_TWO_ARRAYS = "import numpy as np; a = lib.create_array(np.random.rand(200, 200) - 0.5, 32, 16); b = lib.create_array(np.random.rand(200, 200), 32, 16)"
CREATE_SCALAR_AND_ARRAY = "import numpy as np; a = lib.create_array(np.random.rand(200, 200) - 0.5, 32, 16); b = lib.create_scalar(-0.37854, 32, 16)"
CREATE_SHORT_VECTOR = "import math;import numpy as np; a = lib.create_array(np.random.rand(20) - 0.5, 32, 16)"
CREATE_VECTOR = (
    "import numpy as np; a = lib.create_array(np.random.rand(200) - 0.5, 32, 16)"
)
benchmarks = {
    "Scalar creation, 32-bit": ("lib.create_scalar(0.37, 32, 16)", "pass"),
    "Scalar addition, 32-bit": ("c = a + b", CREATE_TWO_SCALARS),
    "Scalar multiplication, 32-bit": ("c = a * b", CREATE_TWO_SCALARS),
    "Scalar division, 32-bit": ("c = a / b", CREATE_TWO_SCALARS),
    "Scalar abs, 32-bit": ("c = abs(a)", CREATE_TWO_SCALARS),
    "Scalar negation, 32-bit": ("c = -a", CREATE_TWO_SCALARS),
    "Scalar right-shift, 32-bit": ("c = a >> 5", CREATE_TWO_SCALARS),
    "Scalar to float, 32-bit": ("c = float(a)", CREATE_TWO_SCALARS),
    "Scalar creation, 320-bit": ("lib.create_scalar(0.37, 160, 160)", "pass"),
    "Scalar addition, 320-bit": ("c = a + b", CREATE_TWO_LONG_SCALARS),
    "Scalar multiplication, 320-bit": ("c = a * b", CREATE_TWO_LONG_SCALARS),
    "Scalar division, 320-bit": ("c = a / b", CREATE_TWO_LONG_SCALARS),
    "Scalar abs, 320-bit": ("c = abs(a)", CREATE_TWO_LONG_SCALARS),
    "Scalar negation, 320-bit": ("c = -a", CREATE_TWO_LONG_SCALARS),
    "Scalar right-shift, 320-bit": ("c = a >> 50", CREATE_TWO_LONG_SCALARS),
    "Scalar to float, 320-bit": ("c = float(a)", CREATE_TWO_LONG_SCALARS),
    "Matrix-scalar addition": ("c = a + b", CREATE_SCALAR_AND_ARRAY),
    "Matrix-scalar multiplication": ("c = a * b", CREATE_SCALAR_AND_ARRAY),
    "Matrix-scalar division": ("c = a / b", CREATE_SCALAR_AND_ARRAY),
    "20-element vector summation": ("sum(a)", CREATE_SHORT_VECTOR),
    "200-element vector summation": ("sum(a)", CREATE_VECTOR),
    "20-element vector product": ("math.prod(a)", CREATE_SHORT_VECTOR),
    "Matrix creation": (
        "lib.create_array(a, 16, 16)",
        "import numpy as np; a = np.random.rand(200, 200)",
    ),
    "Matrix addition": ("c = a + b", CREATE_TWO_ARRAYS),
    "Matrix abs": ("c = abs(a)", CREATE_TWO_ARRAYS),
    "Matrix negation": ("c = -a", CREATE_TWO_ARRAYS),
    "Elementwise matrix multiplication": ("c = a * b", CREATE_TWO_ARRAYS),
    "Matrix multiplication": ("c = a @ b", CREATE_TWO_ARRAYS),
}

results = {}
for lib_name, file in libraries.items():
    results[lib_name] = []
    for name, (func, setup) in benchmarks.items():
        setup_tmp = (
            f"import importlib; lib = importlib.import_module('{file}');" + setup
        )
        try:
            num, timing = timeit.Timer(stmt=func, setup=setup_tmp).autorange()
            results[lib_name].append(timing / num)
            print(f"{lib_name} {name} (average of {num}): {timing / num}")
        except Exception as e:
            results[lib_name].append(float("NaN"))
            print(f"{lib_name} {name}: Error ({e})")

# %%
fig, ax = plt.subplots(layout="constrained", figsize=(8, 10))

x = np.arange(len(benchmarks))  # the label locations
width = 1 / (len(libraries) + 2)  # the width of the bars

for multiplier, (library, measurement) in enumerate(results.items()):
    offset = width * multiplier
    rects = ax.bar(x + offset, measurement, width, label=library)

ax.set_yscale("log")
ax.set_ylabel("Time, s")
ax.set_xticks(
    x + (len(libraries) - 1) * width / 2,
    benchmarks.keys(),
    rotation=90,
)
ax.legend(loc="upper left")
ax.set_xbound((-0.5, len(benchmarks)))
ax.grid(True, which="major", axis="y")
fig.savefig("comparison.png")


fig, ax = plt.subplots(layout="constrained", figsize=(8, 10))

x = np.arange(len(benchmarks))  # the label locations
width = 1 / (len(libraries) + 2)  # the width of the bars
ax.axhline(1, lw=2, color="C0", zorder=-2)
ax.grid(True, which="major", axis="y")

for multiplier, (library, measurement) in enumerate(results.items()):
    offset = width * multiplier
    rects = ax.bar(
        x + offset,
        np.array(measurement) / np.array(results["APyTypes"]) - 1,
        width,
        label=library,
        bottom=1,
    )

ax.set_yscale("log")
ax.set_ylabel("Relative time")
ax.set_xticks(
    x + (len(libraries) - 1) * width / 2,
    benchmarks.keys(),
    rotation=90,
)
ax.legend(loc="upper right")
ax.set_xbound((-0.5, len(benchmarks)))
fig.savefig("comparison_relative.png")


working = []


def map_color(b):
    return (0, 255, 0) if b else (255, 0, 0)


for library, measurement in results.items():
    working.append([map_color(m > 0) for m in measurement])

working = np.array(working)
fig, ax = plt.subplots(layout="constrained", figsize=(8, 5))
ax.imshow(working)
ax.set_xticks(
    x,
    benchmarks.keys(),
    rotation=90,
)
ax.set_yticks(range(len(libraries)), libraries.keys())
fig.savefig("comparison_matrix.png")
