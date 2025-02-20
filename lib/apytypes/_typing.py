from typing import Union
from apytypes import (
    APyFloatArray,
    APyCFixedArray,
    APyFixedArray,
    APyFloat,
    APyFixed,
    APyCFixed,
)
# The following are type aliases. Once Python 3.9 is dropped, they should be annotated
# using ``typing.TypeAlias`` and Unions should be converted to using ``|`` syntax.

APyArray = Union[APyFixedArray, APyCFixedArray, APyFloatArray]
APyScalar = Union[APyFixed, APyCFixed, APyFloat]
