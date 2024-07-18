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
@overload
def convolve(a: APyFixedArray, v: APyFixedArray, mode="full") -> APyFixedArray: ...
@overload
def convolve(a: APyFloatArray, v: APyFloatArray, mode="full") -> APyFloatArray: ...
def convolve(a: APyArray, v: APyArray, mode="full") -> APyArray: ...
@overload
def reshape(a: APyFloatArray, new_shape: tuple[int, ...]) -> APyFloatArray: ...
@overload
def reshape(a: APyFixedArray, new_shape: tuple[int, ...]) -> APyFixedArray: ...
def reshape(a: APyArray, new_shape: tuple[int, ...]) -> APyArray: ...
@overload
def shape(a: APyFloatArray) -> tuple[int, ...]: ...
@overload
def shape(a: APyFixedArray) -> tuple[int, ...]: ...
def shape(a: APyArray) -> tuple[int, ...]: ...
@overload
def transpose(
    a: APyFloatArray, axis: tuple[int, ...] | None = None
) -> APyFloatArray: ...
@overload
def transpose(
    a: APyFixedArray, axis: tuple[int, ...] | None = None
) -> APyFixedArray: ...
def transpose(a: APyArray, axis: tuple[int] | None = None) -> APyArray: ...
@overload
def ravel(a: APyFloatArray) -> APyFloatArray: ...
@overload
def ravel(a: APyFixedArray) -> APyFixedArray: ...
def ravel(a: APyArray) -> APyArray: ...
@overload
def moveaxis(
    a: APyFixedArray, source: Sequence[int], destination: Sequence[int]
) -> APyFixedArray: ...
@overload
def moveaxis(
    a: APyFloatArray, source: Sequence[int], destination: Sequence[int]
) -> APyFixedArray: ...
def moveaxis(
    a: APyArray, source: Sequence[int], destination: Sequence[int]
) -> APyArray: ...
@overload
def swapaxes(a: APyFixedArray, axis1: int, axis2: int) -> APyFixedArray: ...
@overload
def swapaxes(a: APyFloatArray, axis1: int, axis2: int) -> APyFixedArray: ...
def swapaxes(a: APyArray, axis1: int, axis2: int) -> APyArray: ...
@overload
def expand_dims(a: APyFloatArray, axis: int | Sequence[int]) -> APyFloatArray: ...
@overload
def expand_dims(a: APyFixedArray, axis: int | Sequence[int]) -> APyFixedArray: ...
def expand_dims(a: APyArray, axis: int | Sequence[int]) -> APyArray: ...
@overload
def eye(
    a_type: APyArray,
    shape: tuple[int, ...],
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyArray: ...
@overload
def eye(
    a_type: APyFloatArray,
    shape: tuple[int, ...],
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyFloatArray: ...
def eye(
    a_type: APyFixedArray,
    shape: tuple[int, ...],
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
@overload
def zeros(
    a_type: APyArray,
    shape: tuple[int, ...],
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyArray: ...
@overload
def zeros(
    a_type: APyFloatArray,
    shape: tuple[int, ...],
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyFloatArray: ...
def zeros(
    a_type: APyFixedArray,
    shape: tuple[int, ...],
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
