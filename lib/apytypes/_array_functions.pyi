from typing import overload, Sequence
from apytypes import APyFloatArray, APyFixedArray
from apytypes.typing import APyArray

@overload
def squeeze(
    a: APyFloatArray, axis: int | Sequence[int] | None = None
) -> APyFloatArray: ...
@overload
def squeeze(
    a: APyFixedArray, axis: int | Sequence[int] | None = None
) -> APyFixedArray: ...
def squeeze(a: APyArray, axis: int | Sequence[int] | None = None) -> APyArray: ...
