# %%
import timeit

import matplotlib.pyplot as plt

CREATE_TWO_FIXED_15_MATRICES = "import numpy as np; a = APyFixedArray.from_float(np.random.rand({l}, {l}) - 0.5, 1, 14); b = APyFixedArray.from_float(np.random.rand({l}, {l}) - 0.5, 1, 14)"
CREATE_TWO_FIXED_31_MATRICES = "import numpy as np; a = APyFixedArray.from_float(np.random.rand({l}, {l}) - 0.5, 1, 30); b = APyFixedArray.from_float(np.random.rand({l}, {l}) - 0.5, 1, 30)"
CREATE_TWO_FIXED_63_MATRICES = "import numpy as np; a = APyFixedArray.from_float(np.random.rand({l}, {l}) - 0.5, 1, 62); b = APyFixedArray.from_float(np.random.rand({l}, {l}) - 0.5, 1, 62)"
CREATE_TWO_FLOAT_BFLOAT16_MATRICES = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 8, 7); b = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 8, 7)"
CREATE_TWO_FLOAT_HALF_MATRICES = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 5, 10); b = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 5, 10)"
CREATE_TWO_FLOAT_E5M2_MATRICES = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 5, 2); b = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 5, 2)"
CREATE_TWO_FLOAT_SINGLE_MATRICES = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 8, 23); b = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 8, 23)"
CREATE_TWO_FLOAT_DOUBLE_MATRICES = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 11, 52); b = APyFloatArray.from_float(np.random.rand({l}, {l}) - 0.5, 11, 52)"
CREATE_TWO_NUMPY_FLOAT_MATRICES = "import numpy as np; a = np.random.rand({l}, {l})-0.5; b = np.random.rand({l}, {l})-0.5"
CREATE_TWO_NUMPY_SINGLE_MATRICES = "import numpy as np; a = np.array(np.random.rand({l}, {l})-0.5, dtype='float32'); b = np.array(np.random.rand({l}, {l})-0.5, dtype='float32')"
CREATE_TWO_NUMPY_HALF_MATRICES = "import numpy as np; a = np.array(np.random.rand({l}, {l})-0.5, dtype='float16'); b = np.array(np.random.rand({l}, {l})-0.5, dtype='float16')"
CREATE_TWO_NUMPY_BFLOAT16_MATRICES = "import numpy as np; from ml_dtypes import bfloat16; a = np.array(np.random.rand({l}, {l})-0.5, dtype='bfloat16'); b = np.array(np.random.rand({l}, {l})-0.5, dtype='bfloat16')"
CREATE_TWO_NUMPY_E5M2_MATRICES = "import numpy as np; from ml_dtypes import float8_e5m2; a = np.array(np.random.rand({l}, {l})-0.5, dtype='float8_e5m2'); b = np.array(np.random.rand({l}, {l})-0.5, dtype='float8_e5m2')"
CREATE_TWO_NUMPY_INT64_MATRICES = "import numpy as np; a = np.array(np.round((np.random.rand({l}, {l})-0.5)*(2**31)), dtype='int64'); b = np.array(np.round((np.random.rand({l}, {l})-0.5)*(2**31)), dtype='int64')"
CREATE_TWO_NUMPY_INT32_MATRICES = "import numpy as np; a = np.array(np.round((np.random.rand({l}, {l})-0.5)*(2**15)), dtype='int32'); b = np.array(np.round((np.random.rand({l}, {l})-0.5)*(2**15)), dtype='int32')"
CREATE_TWO_NUMPY_INT16_MATRICES = "import numpy as np; a = np.array(np.round((np.random.rand({l}, {l})-0.5)*(2**15)), dtype='int16'); b = np.array(np.round((np.random.rand({l}, {l})-0.5)*(2**15)), dtype='int16')"
benchmarks = {
    "APyTypes fixed-point, 15-bit": ("c = a @ b", CREATE_TWO_FIXED_15_MATRICES),
    "APyTypes transposed fixed-point, 15-bit": (
        "c = a @ b.T",
        CREATE_TWO_FIXED_15_MATRICES,
    ),
    "APyTypes half floating-point": ("c = a @ b", CREATE_TWO_FLOAT_HALF_MATRICES),
    "APyTypes transposed half floating-point": (
        "c = a @ b.T",
        CREATE_TWO_FLOAT_HALF_MATRICES,
    ),
    "Numpy half floating-point": (
        "c = a @ b",
        CREATE_TWO_NUMPY_HALF_MATRICES,
    ),
    "Numpy transposed half floating-point": (
        "c = a @ b.T",
        CREATE_TWO_NUMPY_HALF_MATRICES,
    ),
    "Numpy 16-bit integer": (
        "c = a @ b",
        CREATE_TWO_NUMPY_INT16_MATRICES,
    ),
    "Numpy transposed 16-bit integer": (
        "c = a @ b.T",
        CREATE_TWO_NUMPY_INT16_MATRICES,
    ),
}

counts = [1, 2, 4, 7, 10, 20, 40, 70, 100, 200, 400]
results = {}
for name, (func, setup) in benchmarks.items():
    results[name] = []
    for count in counts:
        setup_tmp = "from apytypes import APyFixedArray, APyFloatArray;" + setup.format(
            l=count
        )
        num, timing = timeit.Timer(stmt=func, setup=setup_tmp).autorange()
        results[name].append(num * (count**3) / timing)

# %%
fig, ax = plt.subplots(layout="constrained", figsize=(8, 8))


for name, measurement in results.items():
    marker = "x" if "Numpy" in name else None
    ax.plot(counts, measurement, label=name, marker=marker)
ax.set_title("Matrix multiplication performance")
ax.set_xscale("log")
ax.set_yscale("log")
ax.set_ylabel("Operations/s")
ax.set_xlabel("Square matrix size")
ax.legend(loc="upper left")
ax.grid(True)
fig.savefig("transposescale.png")
