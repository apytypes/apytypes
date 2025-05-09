import matplotlib.pyplot as plt
import numpy as np
from matplotlib import gridspec
from matplotlib.ticker import IndexLocator

from apytypes import APyFixedArray, QuantizationMode  # noqa: F401

modes = [
    "QuantizationMode.RND",
    "QuantizationMode.RND_CONV",
    "QuantizationMode.RND_CONV_ODD",
    "QuantizationMode.RND_INF",
    "QuantizationMode.RND_MIN_INF",
    "QuantizationMode.RND_ZERO",
    "QuantizationMode.TRN",
    "QuantizationMode.TRN_INF",
    "QuantizationMode.TRN_ZERO",
    "QuantizationMode.TRN_AWAY",
    "QuantizationMode.JAM",
    "QuantizationMode.JAM_UNBIASED",
    "QuantizationMode.TRN_MAG",
]
a = np.arange(-3, 2.9, 0.125)
ai = np.arange(-3, 2.1, 1)
ah = np.arange(-2.5, 2.6, 1)
bins = np.arange(-1, 1.1, 0.125)
afx = APyFixedArray.from_float(a, int_bits=4, frac_bits=3)
aifx = APyFixedArray.from_float(ai, int_bits=4, frac_bits=3)
ahfx = APyFixedArray.from_float(ah, int_bits=4, frac_bits=3)
fig, axs = plt.subplots(
    10,
    3,
    layout="constrained",
    height_ratios=[8, 1, 8, 1, 8, 1, 8, 1, 8, 1],
    figsize=(8, 20),
)
for i, q in enumerate(modes):
    aqfx = afx.cast(int_bits=4, frac_bits=0, quantization=eval(q))
    aiqfx = aifx.cast(int_bits=4, frac_bits=0, quantization=eval(q))
    ahqfx = ahfx.cast(int_bits=4, frac_bits=0, quantization=eval(q))
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

for i in range(len(modes), 15):
    ax = axs[2 * (i // 3), i % 3]
    fig.delaxes(ax)
    ax = axs[2 * (i // 3) + 1, i % 3]
    fig.delaxes(ax)

fig.savefig("quantization.png")


# %%
a = np.arange(-4, 3.9, 0.125)
ai = np.arange(-4, 3.1, 1)
ah = np.arange(-3.5, 3.6, 1)
bins = np.arange(-1, 1.1, 0.125)
afx = APyFixedArray.from_float(a, int_bits=4, frac_bits=3)
aifx = APyFixedArray.from_float(ai, int_bits=4, frac_bits=3)
ahfx = APyFixedArray.from_float(ah, int_bits=4, frac_bits=3)

half = len(a) // 2
for q in modes:
    fig = plt.figure(figsize=(8, 4), layout="constrained")
    name = q.split(".")[1]
    fig.suptitle(name)
    gs = gridspec.GridSpec(3, 2, figure=fig)
    ax = fig.add_subplot(gs[:, 0])
    aqfx = afx.cast(int_bits=4, frac_bits=0, quantization=eval(q))
    aiqfx = aifx.cast(int_bits=4, frac_bits=0, quantization=eval(q))
    ahqfx = ahfx.cast(int_bits=4, frac_bits=0, quantization=eval(q))
    ax.set_aspect("equal")
    ax.plot((-4, 3.875), (-4, 3.875), "g")
    ax.plot(afx, aqfx, "o")
    ax.plot(aifx, aiqfx, "ro")
    ax.plot(ahfx, ahqfx, "yo")
    ax.xaxis.set_major_locator(IndexLocator(1, 0))
    ax.yaxis.set_major_locator(IndexLocator(1, 0))
    ax.set_xlim(-4.2, 4.2)
    ax.set_ylim(-4.2, 4.2)
    ax.set_xlabel("x")
    ax.set_ylabel("Q(x)")
    ax.yaxis.set_label_coords(-0.1, 0.5)
    ax.grid(True)
    ax = fig.add_subplot(gs[0, 1])
    err = a - aqfx
    ax.hist(
        err, bins, align="left", weights=1 / len(err) * np.ones(len(err)), rwidth=0.8
    )
    ax.axvline(float(sum(err)) / len(err), color="r")
    ax.grid(True)
    ax.set_xlim(-1.1, 1.1)
    ax.set_ylim(0, 0.14)
    ax.set_xlabel("x - Q(x)")
    ax.set_title("All values")

    ax = fig.add_subplot(gs[1, 1])
    err = a - aqfx
    ax.hist(
        err[:half], bins, align="left", weights=1 / half * np.ones(half), rwidth=0.8
    )
    ax.axvline(float(sum(err[:half])) / half, color="r")
    ax.grid(True)
    ax.set_xlim(-1.1, 1.1)
    ax.set_ylim(0, 0.14)
    ax.set_xlabel("x - Q(x)")
    ax.set_title("Negative values")

    ax = fig.add_subplot(gs[2, 1])
    err = a - aqfx
    ax.hist(
        err[half:], bins, align="left", weights=1 / half * np.ones(half), rwidth=0.8
    )
    ax.axvline(float(sum(err[half:])) / half, color="r")
    ax.grid(True)
    ax.set_xlim(-1.1, 1.1)
    ax.set_ylim(0, 0.14)
    ax.set_xlabel("x - Q(x)")
    ax.set_title("Non-negative values")

    fig.savefig(f"{name}.png")
