from apytypes import APyFixedArray, QuantizationMode
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import IndexLocator

modes = [
    "QuantizationMode.TRN",
    "QuantizationMode.TRN_INF",
    "QuantizationMode.TRN_ZERO",
    "QuantizationMode.RND",
    "QuantizationMode.RND_CONV",
    "QuantizationMode.RND_CONV_ODD",
    "QuantizationMode.RND_INF",
    "QuantizationMode.RND_MIN_INF",
    "QuantizationMode.RND_ZERO",
    "QuantizationMode.JAM",
    "QuantizationMode.JAM_UNBIASED",
]
a = np.arange(-3, 2.9, 0.125)
ai = np.arange(-3, 2.1, 1)
ah = np.arange(-2.5, 2.6, 1)
bins = np.arange(-1, 1.1, 0.125)
afx = APyFixedArray.from_float(a, 7, 4)
aifx = APyFixedArray.from_float(ai, 7, 4)
ahfx = APyFixedArray.from_float(ah, 7, 4)
fig, axs = plt.subplots(
    8, 3, layout="constrained", height_ratios=[8, 1, 8, 1, 8, 1, 8, 1], figsize=(8, 16)
)
for i, q in enumerate(modes):
    aqfx = afx.cast(4, 4, eval(q))
    aiqfx = aifx.cast(4, 4, eval(q))
    ahqfx = ahfx.cast(4, 4, eval(q))
    ax = axs[2 * (i // 3), i % 3]
    ax.set_aspect("equal")
    ax.plot((-3, 2.875), (-3, 2.875), "g")
    ax.plot(afx, aqfx, "o")
    ax.plot(aifx, aiqfx, "ro")
    ax.plot(ahfx, ahqfx, "yo")
    ax.xaxis.set_major_locator(IndexLocator(1, 0))
    ax.yaxis.set_major_locator(IndexLocator(1, 0))
    ax.set_title(q.split(".")[1])
    ax.set_xlim(-3.2, 3.2)
    ax.set_ylim(-3.2, 3.2)
    ax.set_xlabel("x")
    ax.set_ylabel("Q(x)")
    ax.yaxis.set_label_coords(-0.1, 0.5)
    ax.grid(True)
    ax = axs[2 * (i // 3) + 1, i % 3]
    err = a - aqfx
    ax.hist(
        err, bins, align="left", weights=1 / len(err) * np.ones(len(err)), rwidth=0.8
    )
    ax.axvline(float(sum(err)) / len(err), color="r")
    ax.grid(True)
    ax.set_xlim(-1.1, 1.1)
    ax.set_ylim(0, 0.14)
    ax.set_xlabel("x - Q(x)")
for ax in axs[6:8, 2]:
    ax.remove()
fig.savefig("quantization.png")
