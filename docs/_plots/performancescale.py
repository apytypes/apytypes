# %%
import timeit

import matplotlib.pyplot as plt
import numpy as np

CREATE_TWO_FIXED_VECTORS = "import numpy as np; a = APyFixedArray.from_float(np.random.rand({l}) - 0.5, int_bits=10, frac_bits=10); b = APyFixedArray.from_float(np.random.rand({l}) - 0.5, int_bits=10, frac_bits=10)"
CREATE_TWO_LONG_FIXED_VECTORS = "import numpy as np; a = APyFixedArray.from_float(np.random.rand({l}) - 0.5, int_bits=150, frac_bits=150); b = APyFixedArray.from_float(np.random.rand({l}) - 0.5, int_bits=150, frac_bits=150)"
CREATE_TWO_FIXED_VECTORS_DIFFERENT_WL = "import numpy as np; a = APyFixedArray.from_float(np.random.rand({l}) - 0.5, int_bits=16, frac_bits=16); b = APyFixedArray.from_float(np.random.rand({l}) - 0.5, int_bits=12, frac_bits=2)"
CREATE_TWO_FLOAT_VECTORS = "import numpy as np; a = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 5, 8); b = APyFloatArray.from_float(np.random.rand({l}) - 0.5, 5, 8)"
benchmarks = {
    "Fixed-point addition, 20-bit": ("c = a + b", CREATE_TWO_FIXED_VECTORS),
    "Fixed-point addition, different (short) word lengths": (
        "c = a + b",
        CREATE_TWO_FIXED_VECTORS_DIFFERENT_WL,
    ),
    "Fixed-point multiplication, 20-bit": ("c = a * b", CREATE_TWO_FIXED_VECTORS),
    "Fixed-point addition, 300-bit": ("c = a + b", CREATE_TWO_LONG_FIXED_VECTORS),
    "Fixed-point multiplication, 300-bit": ("c = a * b", CREATE_TWO_LONG_FIXED_VECTORS),
    "Floating-point addition, short word length": (
        "c = a + b",
        CREATE_TWO_FLOAT_VECTORS,
    ),
    "Floating-point multiplication, short word length": (
        "c = a * b",
        CREATE_TWO_FLOAT_VECTORS,
    ),
}

counts = [1, 3, 10, 30, 100, 300, 1000, 3000]
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

fig, ax = plt.subplots(layout="constrained", figsize=(8, 6.5))

x = np.arange(len(benchmarks))  # the label locations

for name, measurement in results.items():
    ax.plot(counts, measurement, label=name)
ax.set_xscale("log")
ax.set_yscale("log")
ax.set_ylabel("Operations/s")
ax.set_xlabel("Elements in array")
ax.legend(loc="upper left")
ax.grid(True)
fig.savefig("performancescale.png")
