from apytypes import APyFixedArray, OverflowMode
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import IndexLocator

modes = [
    "OverflowMode.WRAP",
    "OverflowMode.SAT",
    # "OverflowMode.NUMERIC_STD",
]
a = np.arange(-3, 2.9, 0.125)
ai = np.arange(-3, 2.1, 1)
ah = np.arange(-2.5, 2.6, 1)
bins = np.arange(-1, 1.1, 0.125)
afx = APyFixedArray.from_float(a, 7, 4)
aifx = APyFixedArray.from_float(ai, 7, 4)
ahfx = APyFixedArray.from_float(ah, 7, 4)

for mode in modes:
    fig, ax = plt.subplots(layout="constrained", figsize=(8, 3.6))
    aqfx = afx.cast(4, 1, overflow=eval(mode))
    aiqfx = aifx.cast(4, 1, overflow=eval(mode))
    ax.set_aspect("equal")
    ax.plot((-3, 2.875), (-3, 2.875), "g")
    ax.plot(afx, aqfx, "o")
    ax.plot(aifx, aiqfx, "ro")
    ax.xaxis.set_major_locator(IndexLocator(1, 0))
    ax.yaxis.set_major_locator(IndexLocator(1, 0))
    name = mode.split(".")[1]
    ax.set_title(name)
    ax.set_xlim(-3.2, 3.2)
    ax.set_ylim(-1.2, 1.2)
    ax.set_xlabel("x")
    ax.set_ylabel("O(x)")
    ax.yaxis.set_label_coords(-0.1, 0.5)
    ax.grid(True)
    fig.savefig(f"{name}.png")
