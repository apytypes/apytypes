from typing import TypeAlias

from apytypes import (
    APyCFixed,
    APyCFixedArray,
    APyCFloat,
    APyCFloatArray,
    APyFixed,
    APyFixedArray,
    APyFloat,
    APyFloatArray,
)

APyArray: TypeAlias = APyFixedArray | APyCFixedArray | APyFloatArray | APyCFloatArray
APyScalar: TypeAlias = APyFixed | APyCFixed | APyFloat | APyCFloat
