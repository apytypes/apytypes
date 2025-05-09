from typing import TypeAlias

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyCFloat,
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
)

APyArray: TypeAlias = APyFixedArray | APyCFixedArray | APyFloatArray
APyScalar: TypeAlias = APyFixed | APyCFixed | APyFloat | APyCFloat
