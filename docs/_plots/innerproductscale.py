# %%
import timeit

import matplotlib.pyplot as plt

CREATE_TWO_FIXED_15_VECTORS = "import numpy as np; a = APyFixedArray.from_float(np.random.rand({l}) - 0.5, 1, 14); b = APyFixedArray.from_float(np.random.rand({l}) - 0.5, 1, 14)"
CREATE_TWO_FIXED_31_VECTORS = "import numpy as np; a = APyFixedArray.from_float(np.random.rand({l}) - 0.5, 1, 30); b = APyFixedArray.from_float(np.random.rand({l}) - 0.5, 1, 30)"
CREATE_TWO_FIXED_63_VECTORS = "import numpy as np; a = APyFixedArray.from_float(np.random.rand({l}) - 0.5, 1, 62); b = APyFixedArray.from_float(np.random.rand({l}) - 0.5, 1, 62)"
CREATE_TWO_FLOAT_BFLOAT16_VECTORS = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 8, 7); b = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 8, 7)"
CREATE_TWO_FLOAT_HALF_VECTORS = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 5, 10); b = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 5, 10)"
CREATE_TWO_FLOAT_E5M2_VECTORS = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 5, 2); b = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 5, 2)"
CREATE_TWO_FLOAT_SINGLE_VECTORS = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 8, 23); b = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 8, 23)"
CREATE_TWO_FLOAT_DOUBLE_VECTORS = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 11, 52); b = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 11, 52)"
CREATE_TWO_NUMPY_FLOAT_VECTORS = (
    "import numpy as np; a = np.random.rand({l})-0.5; b = np.random.rand({l})-0.5"
)
CREATE_TWO_NUMPY_SINGLE_VECTORS = "import numpy as np; a = np.array(np.random.rand({l})-0.5, dtype='float32'); b = np.array(np.random.rand({l})-0.5, dtype='float32')"
CREATE_TWO_NUMPY_HALF_VECTORS = "import numpy as np; a = np.array(np.random.rand({l})-0.5, dtype='float16'); b = np.array(np.random.rand({l})-0.5, dtype='float16')"
CREATE_TWO_NUMPY_BFLOAT16_VECTORS = "import numpy as np; from ml_dtypes import bfloat16; a = np.array(np.random.rand({l})-0.5, dtype='bfloat16'); b = np.array(np.random.rand({l})-0.5, dtype='bfloat16')"
CREATE_TWO_NUMPY_E5M2_VECTORS = "import numpy as np; from ml_dtypes import float8_e5m2; a = np.array(np.random.rand({l})-0.5, dtype='float8_e5m2'); b = np.array(np.random.rand({l})-0.5, dtype='float8_e5m2')"
CREATE_TWO_NUMPY_INT64_VECTORS = "import numpy as np; a = np.array(np.round((np.random.rand({l})-0.5)*(2**31)), dtype='int64'); b = np.array(np.round((np.random.rand({l})-0.5)*(2**31)), dtype='int64')"
CREATE_TWO_NUMPY_INT32_VECTORS = "import numpy as np; a = np.array(np.round((np.random.rand({l})-0.5)*(2**15)), dtype='int32'); b = np.array(np.round((np.random.rand({l})-0.5)*(2**15)), dtype='int32')"
CREATE_TWO_NUMPY_INT16_VECTORS = "import numpy as np; a = np.array(np.round((np.random.rand({l})-0.5)*(2**15)), dtype='int16'); b = np.array(np.round((np.random.rand({l})-0.5)*(2**15)), dtype='int16')"
benchmarks = {
    "APyTypes fixed-point, 15-bit": ("c = a @ b", CREATE_TWO_FIXED_15_VECTORS),
    "APyTypes fixed-point, 31-bit": ("c = a @ b", CREATE_TWO_FIXED_31_VECTORS),
    "APyTypes e5m2": ("c = a @ b", CREATE_TWO_FLOAT_E5M2_VECTORS),
    "APyTypes bloat16": ("c = a @ b", CREATE_TWO_FLOAT_BFLOAT16_VECTORS),
    "APyTypes half": ("c = a @ b", CREATE_TWO_FLOAT_HALF_VECTORS),
    "APyTypes single": ("c = a @ b", CREATE_TWO_FLOAT_SINGLE_VECTORS),
    "APyTypes double": ("c = a @ b", CREATE_TWO_FLOAT_DOUBLE_VECTORS),
    "Numpy e5m2 floating-point": (
        "c = a @ b",
        CREATE_TWO_NUMPY_E5M2_VECTORS,
    ),
    "Numpy bfloat floating-point": (
        "c = a @ b",
        CREATE_TWO_NUMPY_BFLOAT16_VECTORS,
    ),
    "Numpy half floating-point": (
        "c = a @ b",
        CREATE_TWO_NUMPY_HALF_VECTORS,
    ),
    "Numpy single floating-point": (
        "c = a @ b",
        CREATE_TWO_NUMPY_SINGLE_VECTORS,
    ),
    "Numpy double floating-point": (
        "c = a @ b",
        CREATE_TWO_NUMPY_FLOAT_VECTORS,
    ),
    "Numpy 16-bit integer": (
        "c = a @ b",
        CREATE_TWO_NUMPY_INT16_VECTORS,
    ),
    "Numpy 32-bit integer": (
        "c = a @ b",
        CREATE_TWO_NUMPY_INT32_VECTORS,
    ),
    "Numpy 64-bit integer": (
        "c = a @ b",
        CREATE_TWO_NUMPY_INT64_VECTORS,
    ),
}

counts = [1, 3, 10, 30, 100, 300, 1000, 3000, 10000, 30000]
results = {}
for name, (func, setup) in benchmarks.items():
    results[name] = []
    for count in counts:
        setup_tmp = "from apytypes import APyFixedArray, APyFloatArray;" + setup.format(
            l=count
        )
        num, timing = timeit.Timer(stmt=func, setup=setup_tmp).autorange()
        results[name].append(num * count / timing)

# %%

fig, ax = plt.subplots(layout="constrained", figsize=(8, 8))


for name, measurement in results.items():
    marker = "x" if "Numpy" in name else None
    ax.plot(counts, measurement, label=name, marker=marker)
ax.set_title("Inner product performance")
ax.set_xscale("log")
ax.set_yscale("log")
ax.set_ylabel("Operations/s")
ax.set_xlabel("Elements in array")
ax.legend(loc="upper left")
ax.grid(True)
fig.savefig("innerproductscale.png")
