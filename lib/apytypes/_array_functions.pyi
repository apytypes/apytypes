from typing import overload, Sequence, Union
from apytypes import APyFloatArray, APyFixedArray, APyFixed, APyFloat
from apytypes.typing import APyArray, APyScalar

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
def identity(
    n: int,
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
def identity(
    n: int,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyFloatArray: ...
@overload
def eye(
    n: int,
    m: int = None,
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
def eye(
    n: int,
    m: int = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyFloatArray: ...
@overload
def zeros(
    shape: tuple[int, ...],
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
def zeros(
    shape: tuple[int, ...],
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyFloatArray: ...
@overload
def ones(
    shape: tuple[int, ...],
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
def ones(
    shape: tuple[int, ...],
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyFloatArray: ...
@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyScalar = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyArray: ...
@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyFixed = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
@overload
def full(
    shape: tuple[int, ...],
    fill_value: APyFloat = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFloatArray: ...
@overload
def full(
    shape: tuple[int, ...],
    fill_value: Union[int, float] = None,
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
@overload
def full(
    shape: tuple[int, ...],
    fill_value: Union[int, float] = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyFloatArray: ...
@overload
def zeros_like(
    a: APyArray,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyArray: ...
@overload
def zeros_like(
    a: APyFixedArray,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
def zeros_like(
    a: APyFloatArray,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFloatArray: ...
@overload
def ones_like(
    a: APyArray,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyArray: ...
@overload
def ones_like(
    a: APyFixedArray,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
def ones_like(
    a: APyFloatArray,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFloatArray: ...
@overload
def full_like(
    a: APyArray,
    fill_value: APyScalar = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyArray: ...
@overload
def full_like(
    a: APyArray,
    fill_value: APyFixed = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
@overload
def full_like(
    a: APyArray,
    fill_value: APyFloat = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFloatArray: ...
@overload
def full_like(
    a: APyArray,
    fill_value: Union[int, float] = None,
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
def full_like(
    a: APyArray,
    fill_value: Union[int, float] = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyFloatArray: ...
@overload
def arange(
    start: Union[int, float],
    stop: Union[int, float] = None,
    step: Union[int, float, None] = None,
    int_bits: int = None,
    frac_bits: int = None,
    exp_bits: None = None,
    mantissa_bits: None = None,
) -> APyFixedArray: ...
@overload
def arange(
    start: Union[int, float],
    stop: Union[int, float] = None,
    step: Union[int, float, None] = None,
    int_bits: None = None,
    frac_bits: None = None,
    exp_bits: int = None,
    mantissa_bits: int = None,
) -> APyFloatArray: ...
