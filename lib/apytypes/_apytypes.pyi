from collections.abc import Sequence
import enum
import types
from typing import Annotated, overload

from numpy.typing import ArrayLike

class APyFixed:
    r"""
    Class for configurable scalar fixed-point formats.

    :class:`APyFixed` is an arbitrary precision two's complement fixed-point scalar type. In
    many ways it behaves like the built-in Python types :class:`int` and :class:`float`, in
    that it can be used within ordinary arithmetic expressions. Every fixed-point instance
    has an associated word length, determined by its :attr:`bits`, :attr:`int_bits`, and
    :attr:`frac_bits` bit specifiers. These specifiers determine the location of the binary
    fix-point and the total word length. Only two of three bit specifers need to be set to
    uniquely determine the complete fixed-point format.

    In general, the fixed-point representation is described by:

    .. math::
        \underbrace{
            \underbrace{
                x_{n-1} \; x_{n-2} \; \ldots \; x_{k+1} \; x_{k}
            }_{\mathrm{int\_bits}}
            \; . \;
            \underbrace{
                x_{k-1} \; x_{k-2} \; \ldots \; x_{1} \; x_{0}
            }_{\mathrm{frac\_bits}}
        }_{\mathrm{bits}}

    The following is an example of a fixed-point number with :code:`bits=8`,
    :code:`int_bits=5`, and :code:`frac_bits=3`, that has a stored value of -6.625:

    .. math::
        \begin{align*}
            1 \, 1 \, 0 \, 0 \, 1 \,.\, 0 \, 1 \, 1_{2} = \frac{-53_{10}}{2^3} = -6.625_{10}
        \end{align*}

    APyFixed uses static word-length inference to determine word lengths of results to
    arithmetic operations. This ensures that overflow or quantization **never** occurs
    unless explicitly instructed to by a user through the :func:`cast` method. The following
    table shows word lengths of elementary arithmetic operations.

    .. list-table:: Word-length of fixed-point arithmetic operations
       :widths: 12 44 44
       :header-rows: 1

       * - Operation
         - Result :attr:`int_bits`
         - Result :attr:`frac_bits`
       * - :code:`a + b`
         - :code:`max(a.int_bits, b.int_bits) + 1`
         - :code:`max(a.frac_bits, b.frac_bits)`
       * - :code:`a - b`
         - :code:`max(a.int_bits, b.int_bits) + 1`
         - :code:`max(a.frac_bits, b.frac_bits)`
       * - :code:`a * b`
         - :code:`a.int_bits + b.int_bits`
         - :code:`a.frac_bits + b.frac_bits`
       * - :code:`a / b`
         - :code:`a.int_bits + b.frac_bits`
         - :code:`a.frac_bits + b.int_bits`
       * - :code:`-a`
         - :code:`a.int_bits + 1`
         - :code:`a.frac_bits`
    """

    def __init__(
        self,
        bit_pattern: int,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> None: ...
    @overload
    def __eq__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __eq__(self, arg: int, /) -> bool: ...
    @overload
    def __eq__(self, arg: float, /) -> bool: ...
    @overload
    def __ne__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __ne__(self, arg: int, /) -> bool: ...
    @overload
    def __ne__(self, arg: float, /) -> bool: ...
    @overload
    def __lt__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __lt__(self, arg: int, /) -> bool: ...
    @overload
    def __lt__(self, arg: float, /) -> bool: ...
    @overload
    def __le__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __le__(self, arg: int, /) -> bool: ...
    @overload
    def __le__(self, arg: float, /) -> bool: ...
    @overload
    def __gt__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __gt__(self, arg: int, /) -> bool: ...
    @overload
    def __gt__(self, arg: float, /) -> bool: ...
    @overload
    def __ge__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __ge__(self, arg: int, /) -> bool: ...
    @overload
    def __ge__(self, arg: float, /) -> bool: ...
    @overload
    def __add__(self, arg: APyFixed, /) -> APyFixed: ...
    @overload
    def __add__(self, arg: int, /) -> APyFixed: ...
    @overload
    def __add__(self, arg: float, /) -> APyFixed: ...
    @overload
    def __sub__(self, arg: APyFixed, /) -> APyFixed: ...
    @overload
    def __sub__(self, arg: int, /) -> APyFixed: ...
    @overload
    def __sub__(self, arg: float, /) -> APyFixed: ...
    @overload
    def __mul__(self, arg: APyFixed, /) -> APyFixed: ...
    @overload
    def __mul__(self, arg: int, /) -> APyFixed: ...
    @overload
    def __mul__(self, arg: float, /) -> APyFixed: ...
    @overload
    def __truediv__(self, arg: APyFixed, /) -> APyFixed: ...
    @overload
    def __truediv__(self, arg: int, /) -> APyFixed: ...
    @overload
    def __truediv__(self, arg: float, /) -> APyFixed: ...
    def __neg__(self) -> APyFixed: ...
    def __ilshift__(self, arg: int, /) -> APyFixed: ...
    def __irshift__(self, arg: int, /) -> APyFixed: ...
    @overload
    def __radd__(self, arg: int, /) -> APyFixed: ...
    @overload
    def __radd__(self, arg: float, /) -> APyFixed: ...
    @overload
    def __radd__(self, arg: float, /) -> APyFixed: ...
    @overload
    def __rsub__(self, arg: int, /) -> APyFixed: ...
    @overload
    def __rsub__(self, arg: float, /) -> APyFixed: ...
    @overload
    def __rmul__(self, arg: int, /) -> APyFixed: ...
    @overload
    def __rmul__(self, arg: float, /) -> APyFixed: ...
    @overload
    def __rtruediv__(self, arg: int, /) -> APyFixed: ...
    @overload
    def __rtruediv__(self, arg: float, /) -> APyFixed: ...
    def to_bits(self) -> int:
        """
        Retrieve underlying bit-pattern in an :class:`int`.

        Examples
        --------
        >>> from apytypes import APyFixed

        Create fixed-point number `fx_a` of value -5.75

        >>> fx_a = APyFixed.from_float(-5.75, int_bits=4, frac_bits=4)

        Returns: 164 == 0xA4 == 0b10100100

        >>> fx_a.to_bits()
        164

        Returns
        -------
        :class:`int`
        """

    @property
    def bits(self) -> int:
        """
        Total number of bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def frac_bits(self) -> int:
        """
        Number of fractional bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def int_bits(self) -> int:
        """
        Number of integer bits.

        Returns
        -------
        :class:`int`
        """

    def is_identical(self, other: APyFixed) -> bool:
        """
        Test if two fixed-point objects are exactly identical.

        Two `APyFixed` objects are considered exactly identical if, and only if,
        they represent the same fixed-point value, and have the exact same
        bit-specification (`bits`, `int_bits`, and `frac_bits`). This is a more
        restrictive test than ``==``,  that only tests equality of the numerical
        fixed-point value.

        Parameters
        ----------
        other : :class:`APyFixed`
            The fixed-point number to test identicality against

        Examples
        --------
        >>> from apytypes import APyFixed
        >>> fx_a = APyFixed.from_float(2.0, int_bits=3, frac_bits=3)
        >>> fx_b = APyFixed.from_float(2.0, int_bits=4, frac_bits=3)

        `fx_a` and `fx_b` store the same fixed-point value

        >>> fx_a == fx_b
        True

        `fx_a` and `fx_b` differ in the `int_bits` specifier

        >>> fx_a.is_identical(fx_b)
        False

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_zero(self) -> bool:
        """True if the value equals zero, false otherwise."""

    def cast(
        self,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        quantization: QuantizationMode | None = None,
        overflow: OverflowMode | None = None,
        bits: int | None = None,
    ) -> APyFixed:
        """
        Change format of the fixed-point number.

        This is the primary method for performing quantization and
        overflowing/saturation when dealing with APyTypes fixed-point numbers.

        Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) needs
        to be set.

        Parameters
        ----------
        int_bits : int, optional
            Number of integer bits in the result.
        frac_bits : int, optional
            Number of fractional bits in the result.
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use in this cast.
        overflow : :class:`OverflowMode`, optional
            Overflowing mode to use in this cast.
        bits : int, optional
            Total number of bits in the result.

        Examples
        --------
        >>> from apytypes import APyFixed
        >>> from apytypes import QuantizationMode
        >>> from apytypes import OverflowMode
        >>> fx = APyFixed.from_float(2.125, int_bits=3, frac_bits=3)

        Truncation (2.0)

        >>> fx.cast(int_bits=3, frac_bits=2, quantization=QuantizationMode.TRN)
        APyFixed(8, bits=5, int_bits=3)

        Rounding (2.25)

        >>> fx.cast(
        ...     int_bits=3, frac_bits=2, quantization=QuantizationMode.RND
        ... )  # doctest: +SKIP
        APyFixed(8, bits=5, int_bits=3)

        Two's complement overflowing (-1.875)

        >>> fx.cast(int_bits=2, frac_bits=3, overflow=OverflowMode.WRAP)
        APyFixed(17, bits=5, int_bits=2)

        Returns
        -------
        :class:`APyFixed`
        """

    @property
    def leading_ones(self) -> int:
        """
        Number of leading ones.

        Returns
        -------
        :class:`int`
        """

    @property
    def leading_zeros(self) -> int:
        """
        Number of leading zeros.

        Returns
        -------
        :class:`int`
        """

    @property
    def leading_fractional_zeros(self) -> int:
        """
        Number of leading zeros after the binary fixed-point.

        Returns
        -------
        :class:`int`
        """

    @property
    def leading_signs(self) -> int:
        """
        Number of leading signs.

        Returns
        -------
        :class:`int`
        """

    def __abs__(self) -> APyFixed: ...
    def __float__(self) -> float: ...
    def __repr__(self) -> str: ...
    def __str__(self, base: int = 10) -> str: ...
    def __lshift__(self, shift_amnt: int) -> APyFixed: ...
    def __rshift__(self, shift_amnt: int) -> APyFixed: ...
    @staticmethod
    def from_float(
        value: object,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixed:
        """
        Create an :class:`APyFixed` object from an :class:`int` or :class:`float`.

        The initialized fixed-point value is the one closest to the input
        floating-point value, rounded away from zero on ties. Exactly two of the
        three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

        Parameters
        ----------
        value : int, float
            Floating point value to initialize from
        int_bits : int, optional
            Number of integer bits in the created fixed-point object
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point object
        bits : int, optional
            Total number of bits in the created fixed-point object

        Examples
        --------
        >>> from apytypes import APyFixed
        >>> fx_a = APyFixed.from_float(1.234, int_bits=2, frac_bits=2)

        Fixed-point `fx_a`, initialized from the floating-point value 1.234,
        rounded to 1.25 as it is the closest representable number

        >>> fx_a
        APyFixed(5, bits=4, int_bits=2)
        >>> str(fx_a)
        '1.25'

        Returns
        -------
        :class:`APyFixed`
        """

    @staticmethod
    def from_str(
        string_value: str,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        base: int = 10,
        bits: int | None = None,
    ) -> APyFixed:
        """
        Create an :class:`APyFixed` object from :class:`str`.

        Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must
        be set.

        Parameters
        ----------
        string_value : str
            String to initialize the value from
        int_bits : int, optional
            Number of integer bits in the created fixed-point object
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point object
        base : int, default: 10
            Numeric base used in `string_value`
        bits : int, optional
            Total number of bits in the created fixed-point object

        Examples
        --------
        >>> from apytypes import APyFixed

        Larger fixed-point value initialization from a string (base-10)

        >>> fx_a = APyFixed.from_str(
        ...     "-1376018206341311063223476816643087998331620501540496640."
        ...     "021222579872958058370179355618716816066859017361262100333952697594702"
        ...     "314679773970519809467311447652539955943903993200932791396783892142688"
        ...     "708904952458654442554723081083186210082207584128592922850820472478833"
        ...     "257136662269306798708182072507551281664490003441493733349403017982015"
        ...     "56238154807942919433116912841796875",
        ...     bits=511,
        ...     int_bits=199,
        ...     base=10,
        ... )

        Returns
        -------
        :class:`APyFixed`
        """

class APyFixedAccumulatorContext(ContextManager):
    def __init__(
        self,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        quantization: QuantizationMode | None = None,
        overflow: OverflowMode | None = None,
        bits: int | None = None,
    ) -> None: ...
    def __enter__(self) -> None: ...
    def __exit__(
        self,
        exc_type: object | None = None,
        exc_value: object | None = None,
        traceback: object | None = None,
    ) -> None: ...

class APyFixedArray:
    def __init__(
        self,
        bit_pattern_sequence: Sequence,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> None: ...
    @overload
    def __add__(self, arg: APyFixedArray, /) -> APyFixedArray: ...
    @overload
    def __add__(self, arg: int, /) -> APyFixedArray: ...
    @overload
    def __add__(self, arg: float, /) -> APyFixedArray: ...
    @overload
    def __add__(self, arg: APyFixed, /) -> APyFixedArray: ...
    @overload
    def __radd__(self, arg: int, /) -> APyFixedArray: ...
    @overload
    def __radd__(self, arg: float, /) -> APyFixedArray: ...
    @overload
    def __radd__(self, arg: APyFixed, /) -> APyFixedArray: ...
    @overload
    def __sub__(self, arg: APyFixedArray, /) -> APyFixedArray: ...
    @overload
    def __sub__(self, arg: int, /) -> APyFixedArray: ...
    @overload
    def __sub__(self, arg: float, /) -> APyFixedArray: ...
    @overload
    def __sub__(self, arg: APyFixed, /) -> APyFixedArray: ...
    @overload
    def __rsub__(self, arg: int, /) -> APyFixedArray: ...
    @overload
    def __rsub__(self, arg: float, /) -> APyFixedArray: ...
    @overload
    def __rsub__(self, arg: APyFixed, /) -> APyFixedArray: ...
    @overload
    def __mul__(self, arg: APyFixedArray, /) -> APyFixedArray: ...
    @overload
    def __mul__(self, arg: APyFixed, /) -> APyFixedArray: ...
    @overload
    def __mul__(self, arg: int, /) -> APyFixedArray: ...
    @overload
    def __mul__(self, arg: float, /) -> APyFixedArray: ...
    @overload
    def __rmul__(self, arg: APyFixed, /) -> APyFixedArray: ...
    @overload
    def __rmul__(self, arg: int, /) -> APyFixedArray: ...
    @overload
    def __rmul__(self, arg: float, /) -> APyFixedArray: ...
    @overload
    def __truediv__(self, arg: APyFixedArray, /) -> APyFixedArray: ...
    @overload
    def __truediv__(self, arg: APyFixed, /) -> APyFixedArray: ...
    @overload
    def __truediv__(self, arg: int, /) -> APyFixedArray: ...
    @overload
    def __truediv__(self, arg: float, /) -> APyFixedArray: ...
    @overload
    def __rtruediv__(self, arg: APyFixed, /) -> APyFixedArray: ...
    @overload
    def __rtruediv__(self, arg: int, /) -> APyFixedArray: ...
    @overload
    def __rtruediv__(self, arg: float, /) -> APyFixedArray: ...
    def __neg__(self) -> APyFixedArray: ...
    def __ilshift__(self, arg: int, /) -> APyFixedArray: ...
    def __irshift__(self, arg: int, /) -> APyFixedArray: ...
    @property
    def bits(self) -> int:
        """
        Total number of bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def int_bits(self) -> int:
        """
        Number of integer bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def frac_bits(self) -> int:
        """
        Number of fractional bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def shape(self) -> tuple:
        """
        The shape of the array.

        Returns
        -------
        :class:`tuple` of :class:`int`
        """

    @property
    def ndim(self) -> int:
        """
        Number of dimensions in the array.

        Returns
        -------
        :class:`int`
        """

    @property
    def T(self) -> APyFixedArray:
        """
        The transposition of the array.

        Equivalent to calling :func:`APyFixedArray.transpose`.

        Returns
        -------
        :class:`APyFixedArray`
        """

    def to_numpy(self) -> Annotated[ArrayLike, dict(dtype="float64")]:
        """
        Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

        The returned array has the same `shape` and values as `self`. This
        method rounds away from infinity on ties.

        Returns
        -------
        :class:`numpy.ndarray`
        """

    def to_bits(
        self, numpy: bool = False
    ) -> (
        list
        | Annotated[ArrayLike, dict(dtype="uint64")]
        | Annotated[ArrayLike, dict(dtype="uint32")]
        | Annotated[ArrayLike, dict(dtype="uint16")]
        | Annotated[ArrayLike, dict(dtype="uint8")]
    ):
        """
        Return the underlying bit representations.

        When `numpy` is true, the bit representations are returned in a
        :class:`numpy.ndarray`. Otherwise, they are returned in a :class:`list`.

        Returns
        -------
        :class:`list` or :class:`numpy.ndarray`
        """

    def reshape(self, number_sequence: tuple) -> APyFixedArray:
        """
        Reshape the APyFixedArray to the specified shape without changing its data.

        Parameters
        ----------
        new_shape : `tuple`
            The new shape should be compatible with the original shape. If a dimension is -1, its value will be inferred from the length of the array and remaining dimensions. Only one dimension can be -1.

        Raises
        ------
        ValueError
            If negative dimensions less than -1 are provided, if the total size of the new array is not unchanged and divisible by the known dimensions, or if the total number of elements does not match the original array.

        Examples
        --------
        >>> from apytypes import APyFixedArray
        >>> a = APyFixedArray([2, 3, 4, 5], int_bits=2, frac_bits=1)
        >>> a.to_numpy()
        array([ 1. ,  1.5, -2. , -1.5])

        >>> a.reshape((2, 2)).to_numpy()
        array([[ 1. ,  1.5],
               [-2. , -1.5]])

        >>> a.reshape((4,)).to_numpy()
        array([ 1. ,  1.5, -2. , -1.5])

        >>> a.reshape((2, -1)).to_numpy()
        array([[ 1. ,  1.5],
               [-2. , -1.5]])

        Returns
        -------
        :class:`APyFixedArray`
        """

    def flatten(self) -> APyFixedArray:
        """
        Return a copy of the array collapsed into one dimension.


        Examples
        --------
        >>> from apytypes import APyFixedArray
        >>> arr = APyFixedArray([[2, 3], [4, 5]], int_bits=2, frac_bits=1)
        >>> arr.to_numpy()
        array([[ 1. ,  1.5],
               [-2. , -1.5]])

        >>> arr.flatten().to_numpy()
        array([ 1. ,  1.5, -2. , -1.5])

        Returns
        -------
        :class:`APyFixedArray`
        """

    def ravel(self) -> APyFixedArray:
        """
        Return a copy of the array collapsed into one dimension. Same as flatten with
        current memory-copy model.


        Examples
        --------
        >>> from apytypes import APyFixedArray
        >>> arr = APyFixedArray([[2, 3], [4, 5]], int_bits=2, frac_bits=1)
        >>> arr.to_numpy()
        array([[ 1. ,  1.5],
               [-2. , -1.5]])

        >>> arr.ravel().to_numpy()
        array([ 1. ,  1.5, -2. , -1.5])

        Returns
        -------
        :class:`APyFixedArray`
        """

    def is_identical(self, other: APyFixedArray) -> bool:
        """
        Test if two :class:`APyFixedArray` objects are identical.

        Two :class:`APyFixedArray` objects are considered identical if, and only if:
          * They represent exactly the same tensor shape
          * They store the exact same fixed-point values in all tensor elements
          * They have the exact same bit specification (`bits`, `int_bits`, and
            `frac_bits` are all equal)

        Returns
        -------
        :class:`bool`
        """

    def swapaxes(self, axis1: int, axis2: int) -> APyFixedArray:
        """
        Interchange two axes of an array.

        Parameters
        ----------
        axis1 : int
            First axis.
        axis2 : int
            Second axis.

        Examples
        --------
        >>> from apytypes import APyFixedArray
        >>> x = APyFixedArray.from_float([[1, 2, 3]], bits=5, frac_bits=0)
        >>> x.swapaxes(0, 1).to_numpy()
        array([[1.],
               [2.],
               [3.]])

        >>> x = APyFixedArray.from_float(
        ...     [[[0, 1], [2, 3]], [[4, 5], [6, 7]]], bits=5, frac_bits=0
        ... )
        >>> x.to_numpy()
        array([[[0., 1.],
                [2., 3.]],
        <BLANKLINE>
               [[4., 5.],
                [6., 7.]]])

        >>> x.swapaxes(0, 2).to_numpy()
        array([[[0., 4.],
                [2., 6.]],
        <BLANKLINE>
               [[1., 5.],
                [3., 7.]]])

        Returns
        -------
        a_swapped : :class:`APyFixedArray`
            Copy of `a` with axes swapped
        """

    def transpose(self, axes: tuple | None = None) -> APyFixedArray:
        """
        Return copy of array with axes transposed.

        For a 1-D array, this return the same array.
        For a 2-D array, this is the standard matrix transpose.
        For an n-D array, if axes are given, their order indicates how the
        axes are permuted (see Examples). If axes are not provided, then
        ``a.transpose(a).shape == a.shape[::-1]``.

        Parameters
        ----------
        axes : tuple of ints, optional
            If specified, it must be a tuple or list which contains a permutation
            of [0,1,...,N-1] where N is the number of axes of `a`. The `i`'th axis
            of the returned array will correspond to the axis numbered ``axes[i]``
            of the input. If not specified, defaults to ``range(a.ndim)[::-1]``,
            which reverses the order of the axes.

        Examples
        --------
        >>> from apytypes import APyFixedArray
        >>> a = APyFixedArray.from_float(
        ...     [[1.0, 2.0, 3.0], [-4.0, -5.0, -6.0]], bits=5, frac_bits=0
        ... )
        >>> a.to_numpy()
        array([[ 1.,  2.,  3.],
               [-4., -5., -6.]])
        >>> a = a.transpose()
        >>> a.to_numpy()
        array([[ 1., -4.],
               [ 2., -5.],
               [ 3., -6.]])

        >>> a = APyFixedArray.from_float([1.0] * 6, bits=5, frac_bits=0).reshape(
        ...     (1, 2, 3)
        ... )
        >>> a.transpose((1, 0, 2)).shape
        (2, 1, 3)

        >>> a.transpose((-2, -3, -1)).shape
        (2, 1, 3)

        Returns
        -------
        :class:`APyFixedArray`
            `a` with its axes permuted.
        """

    def cast(
        self,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        quantization: QuantizationMode | None = None,
        overflow: OverflowMode | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Change format of the fixed-point array.

        This is the primary method for performing quantization and overflowing/saturation
        when dealing with APyTypes fixed-point arrays.

        Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must
        be set.

        Parameters
        ----------
        int_bits : int, optional
            Number of integer bits in the result.
        frac_bits : int, optional
            Number of fractional bits in the result.
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use in this cast.
        overflow : :class:`OverflowMode`, optional
            Overflowing mode to use in this cast.
        bits : int, optional
            Total number of bits in the result.

        Returns
        -------
        :class:`APyFixedArray`
        """

    def broadcast_to(self, shape: tuple | int) -> APyFixedArray:
        """
        Broadcast array to new shape.

        .. versionadded:: 0.2

        Parameters
        ----------
        shape : tuple or int
            The shape to broadcast to. A single integer ``i`` is interpreted as
            ``(i,)``.

        Returns
        -------
        :class:`APyFixedArray`
        """

    def convolve(self, other: APyFixedArray, mode: str = "full") -> APyFixedArray:
        """
        Return the discrete linear convolution with another one-dimensional array.

        Requires that ``ndim = 1`` for both `self` and `other`.

        Parameters
        ----------
        other : :class:`APyFixedArray`
            The one-dimensional array of length :code:`N` to convolve with.

        mode : {'full', 'same', 'valid'}, default: 'full'
            'full':
                Return the full convolution for each point of overlap. The
                resulting single-dimensional shape will have length :code:`N + M -
                1`. Boundary effects occurs for points where the `a` and `v` do not
                overlap completely.
            'same':
                Return a convolution of length :code:`max(M, N)`. Boundary effects
                still occur around the edges of the result.
            'valid':
                Return the convolution for each point of full overlap. The
                resulting single-dimensional shape will have length :code:`max(M, N)
                - min(M, N) + 1`

        Returns
        -------
        convolved : :class:`APyFixedArray`
            The convolved array.
        """

    def squeeze(self, axis: int | tuple | None = None) -> APyFixedArray:
        """
        Removes axes of size one at the specified axis/axes, if no axis is given removes all dimensions with size one.

        Parameters
        ----------
        axis : tuple or int, optional
            The axes to squeeze, a given axis with a size other than one will result in an error. No given axes  will be remove all dimensions of size one.

        Returns
        -------
        :class:`APyFixedArray`

        Raises
        ------
        :class:`ValueError`
            If given an axis of a size other than one a ValueError will be thrown.
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def sum(self, axis: tuple | int | None = None) -> APyFixedArray | APyFixed:
        """
        Return the sum of the elements along specified axis/axes treating Not a Number as 0.

        Parameters
        ----------
        axis : tuple or int, optional
            The axis/axes to summate across. Will summate the whole array if no int or tuple is specified.

        Returns
        -------
        :class:`APyFixedArray` or :class:`APyFixed`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------

        >>> from apytypes import APyFixedArray

        >>> a = APyFixedArray([1, 2, 3, 4, 5, 6], int_bits=10, frac_bits=0)

        >>> a.sum()
        APyFixed(21, bits=13, int_bits=13)

        -------
        """

    def cumsum(self, axis: int | None = None) -> APyFixedArray:
        """
        Return the cumulative sum of the elements along a given axes.

        Parameters
        ----------
        axis : int, optional
            The axes to summate across. If not given an axis it will return the cumulative sum of the flattened array.

        Returns
        -------
        :class:`APyFixedArray`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------

        >>> from apytypes import APyFixedArray

        Array `a`, array to summate across.

        >>> a = APyFixedArray([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)

        >>> a.cumsum()
        APyFixedArray([1, 3, 6, 10, 15, 21], shape=(6,), bits=13, int_bits=13)

        >>> a.cumsum(0)
        APyFixedArray([1, 2, 3, 5, 7, 9], shape=(2, 3), bits=11, int_bits=11)

        >>> a.cumsum(1)
        APyFixedArray([1, 3, 6, 4, 9, 15], shape=(2, 3), bits=12, int_bits=12)

        -------
        """

    def nansum(self, axis: tuple | int | None = None) -> APyFixedArray | APyFixed:
        """
        Return the sum of the elements along specified axis/axes treating Not a Number as 0.

        Parameters
        ----------
        axis : tuple or int, optional
            The axis/axes to summate across. Will summate the whole array if no int or tuple is specified.

        Returns
        -------
        :class:`APyFixedArray` or :class:`APyFixed`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def nancumsum(self, axis: int | None = None) -> APyFixedArray:
        """
        Return the cumulative sum of the elements along a given axis treating Not a Number as 0.

        Parameters
        ----------
        axis : int, optional
            The axis to summate across. If not given an axis it will return the cumulative sum of the flattened array.

        Returns
        -------
        :class:`APyFixedArray`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def max(self, axis: tuple | int | None = None) -> APyFixedArray | APyFixed:
        """
        Returns the maximum value from an array or the maximum along an axis

        Parameters
        ----------
        axis : tuple or int, optional
            The axis to get the maximum along.

        Returns
        -------
        :class:`APyFixedArray` or :class:`APyFixed`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        -------

        >>> from apytypes import APyFixedArray

        Array `a`, array to get the maximum along.

        >>> a = APyFixedArray([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)

        >>> a.max()
        APyFixed(6, bits=10, int_bits=10)

        >>> a.max(0)
        APyFixedArray([4, 5, 6], shape=(3,), bits=10, int_bits=10)

        >>> a.max(1)
        APyFixedArray([3, 6], shape=(2,), bits=10, int_bits=10)

        -------
        """

    def min(self, axis: tuple | int | None = None) -> APyFixedArray | APyFixed:
        """
        Returns the minimum value from an array or the minimum along an axis

        Parameters
        ----------
        axis : tuple or int, optional
            The axis to get the minimum along.

        Returns
        -------
        :class:`APyFixedArray` or :class:`APyFixed`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        -------

        >>> from apytypes import APyFixedArray

        Array `a`, array to get the minimum along.

        >>> a = APyFixedArray([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)

        >>> a.min()
        APyFixed(1, bits=10, int_bits=10)

        >>> a.min(0)
        APyFixedArray([1, 2, 3], shape=(3,), bits=10, int_bits=10)

        >>> a.min(1)
        APyFixedArray([1, 4], shape=(2,), bits=10, int_bits=10)

        -------
        """

    def nanmax(self, axis: tuple | int | None = None) -> APyFixedArray | APyFixed:
        """
        Returns the maximum value from an array or the maximum along an axis. Ignoring nan.
        Issues a warning when encountering an all-nan slice or axis.

        Parameters
        ----------
        axis : tuple or int, optional
            The axis to get the maximum along.

        Returns
        -------
        :class:`APyFixedArray` or :class:`APyFixed`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def nanmin(self, axis: tuple | int | None = None) -> APyFixedArray | APyFixed:
        """
        Returns the minimum value from an array or the minimum along an axis. Ignoring nan.
        Issues a warning when encountering an all-nan slice or axis.

        Parameters
        ----------
        axis : tuple or int, optional
            The axis to get the minimum along.

        Returns
        -------
        :class:`APyFixedArray` or :class:`APyFixed`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def prod(self, axis: tuple | int | None = None) -> APyFixedArray | APyFixed:
        """
        Returns the product of the elements along specified axis/axes.

        Parameters
        ----------
        axis : tuple, int, optional
            The axis/axes to calculate the product across. If not given an axis it will return the product of the flattened array.

        Returns
        -------
        :class:`APyFixedArray` or :class:`APyFixed`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------

        >>> from apytypes import APyFixedArray

        >>> a = APyFixedArray([1, 2, 3, 4, 5, 6], int_bits=10, frac_bits=0)

        >>> a.prod()
        APyFixed(720, bits=60, int_bits=60)

        -------
        """

    def cumprod(self, axis: int | None = None) -> APyFixedArray:
        """
        Return the cumulative product of the elements along a given axes.

        Parameters
        ----------
        axis : int, optional
            The axes to calculate the product across. If not given an axis it will return the cumulative product of the flattened array.

        Returns
        -------
        :class:`APyFixedArray`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------

        >>> from apytypes import APyFixedArray


        >>> a = APyFixedArray([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)

        >>> a.cumprod()
        APyFixedArray([1, 2, 6, 24, 120, 720], shape=(6,), bits=60, int_bits=60)

        >>> a.cumprod(0)
        APyFixedArray([1, 2, 3, 4, 10, 18], shape=(2, 3), bits=20, int_bits=20)

        >>> a.cumprod(1)
        APyFixedArray([1, 2, 6, 4, 20, 120], shape=(2, 3), bits=30, int_bits=30)

        -------
        """

    def nanprod(self, axis: tuple | int | None = None) -> APyFixedArray | APyFixed:
        """
        Return the product of the elements along a given axis treating Not a Number as 0.

        Parameters
        ----------
        axis : int, optional
            The axis to calculate the product across. If not given an axis it will return the product of the flattened array.

        Returns
        -------
        :class:`APyFixedArray` or :class:`APyFixed`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def nancumprod(self, axis: int | None = None) -> APyFixedArray:
        """
        Return the cumulative product of the elements along a given axis treating Not a Number as 0.

        Parameters
        ----------
        axis : int, optional
            The axis to calculate the product across. If not given an axis it will return the cumulative product of the flattened array.

        Returns
        -------
        :class:`APyFixedArray`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    @staticmethod
    def from_float(
        float_sequence: Sequence,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Create an :class:`APyFixedArray` object from a sequence of :class:`float`.

        The initialized fixed-point values are the one closest to the
        input floating-point values, rounded away from zero on ties. Exactly two of
        the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

        Using NumPy arrays as input is in general faster than e.g. lists.

        Parameters
        ----------
        float_sequence : sequence of float
            Floating point values to initialize from. The tensor shape will be taken
            from the sequence shape.
        int_bits : int, optional
            Number of integer bits in the created fixed-point tensor.
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point tensor.
        bits : int, optional
            Total number of bits in the created fixed-point tensor.

        Examples
        --------

        >>> from apytypes import APyFixedArray

        Array `a`, initialized from floating-point values.

        >>> a = APyFixedArray.from_float([1.0, 1.25, 1.49], int_bits=2, frac_bits=2)

        Array `b` (2 x 3 matrix), initialized from floating-point values.

        >>> b = APyFixedArray.from_float(
        ...     [
        ...         [1.0, 2.0, 3.0],
        ...         [4.0, 5.0, 6.0],
        ...     ],
        ...     bits=5,
        ...     frac_bits=0,
        ... )

        Returns
        -------
        :class:`APyFixedArray`
        """

    @staticmethod
    def from_array(
        ndarray: Annotated[ArrayLike, dict(order="C")],
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Create an :class:`APyFixedArray` object from an ndarray.

        The initialized fixed-point values are the one closest to the
        input floating-point value, rounded away from zero on ties. Exactly two of
        the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

        Using NumPy arrays as input is in general faster than e.g. lists.

        Parameters
        ----------
        ndarray : ndarray
            Values to initialize from. The tensor shape will be taken from the ndarray shape.
        int_bits : int, optional
            Number of integer bits in the created fixed-point tensor.
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point tensor.
        bits : int, optional
            Total number of bits in the created fixed-point tensor.

        Examples
        --------

        >>> from apytypes import APyFixedArray
        >>> import numpy as np

        Array `a`, initialized from NumPy ndarray

        >>> a = APyFixedArray.from_array(
        ...     np.array(
        ...         [
        ...             [1.0, 2.0, 3.0],
        ...             [4.0, 5.0, 6.0],
        ...         ]
        ...     ),
        ...     int_bits=10,
        ...     frac_bits=0,
        ... )

        Returns
        -------
        :class:`APyFixedArray`
        """

    @staticmethod
    def zeros(
        shape: tuple,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Initializes an array with zeros.

        Parameters
        ----------
        shape : tuple
            Shape of the array.
        int_bits : int, optional
            Number of integer bits.
        frac_bits : int, optional
            Number of fractional bits.
        bits : int, optional
            Total number of bits.

        Returns
        -------
        :class:`APyFixedArray`
            An array initialized with zeros.
        """

    @staticmethod
    def ones(
        shape: tuple,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Initializes an array with ones.

        Parameters
        ----------
        shape : tuple
            Shape of the array.
        int_bits : int, optional
            Number of integer bits.
        frac_bits : int, optional
            Number of fractional bits.
        bits : int, optional
            Total number of bits.

        Returns
        -------
        :class:`APyFixedArray`
            An array initialized with ones.
        """

    @staticmethod
    def eye(
        n: int,
        m: int | None = None,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Initializes an array with ones on the diagonal.

        Parameters
        ----------
        n : int
            Number of rows.
        m : int, optional
            Number of columns.
        int_bits : int, optional
            Number of integer bits.
        frac_bits : int, optional
            Number of fractional bits.
        bits : int, optional
            Total number of bits.

        Returns
        -------
        :class:`APyFixedArray`
            An array with ones on the diagonal.
        """

    @staticmethod
    def identity(
        n: int,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Initializes an identity matrix with ones on the diagonal.

        Parameters
        ----------
        n : int
            Number of rows (and columns) in *n* x *n* output.
        int_bits : int, optional
            Number of integer bits.
        frac_bits : int, optional
            Number of fractional bits.
        bits : int, optional
            Total number of bits.

        Returns
        -------
        :class:`APyFixedArray`
            An identity matrix with ones on the diagonal.
        """

    @staticmethod
    def full(shape: tuple, fill_value: APyFixed) -> APyFixedArray:
        """
        Initializes an array with the specified value.

        Parameters
        ----------
        shape : tuple
            Shape of the array.
        fill_value : :class:`APyFixed`
            Value to fill the array.

        Returns
        -------
        :class:`APyFixedArray`
            An array filled with the specified value.
        """

    def __lshift__(self, shift_amnt: int) -> APyFixedArray: ...
    def __matmul__(self, rhs: APyFixedArray) -> APyFixedArray: ...
    def __repr__(self) -> str: ...
    def __rshift__(self, shift_amnt: int) -> APyFixedArray: ...
    def __abs__(self) -> APyFixedArray: ...
    def __getitem__(
        self, idx: int | slice | types.EllipsisType | tuple
    ) -> APyFixedArray | APyFixed: ...
    def __len__(self) -> int: ...
    def __iter__(self) -> APyFixedArrayIterator: ...
    def __array__(self) -> Annotated[ArrayLike, dict(dtype="float64")]: ...

class APyFixedArrayIterator:
    def __iter__(self) -> APyFixedArrayIterator: ...
    def __next__(self) -> APyFixedArray | APyFixed: ...

class APyFixedCastContext(ContextManager):
    def __init__(
        self,
        quantization: QuantizationMode | None = None,
        overflow: OverflowMode | None = None,
    ) -> None: ...
    def __enter__(self) -> None: ...
    def __exit__(
        self,
        exc_type: object | None = None,
        exc_value: object | None = None,
        traceback: object | None = None,
    ) -> None: ...

class APyFloat:
    r"""
    Floating-point scalar with configurable format.

    The implementation is a generalization of the IEEE 754 standard, meaning that features like
    subnormals, infinities, and NaN, are still supported. The format is defined
    by the number of exponent and mantissa bits, and a non-negative bias. These fields are
    named :attr:`exp_bits`, :attr:`man_bits`, and :attr:`bias` respectively. Similar to the hardware
    representation of a floating-point number, the value is stored using three fields;
    a sign bit :attr:`sign`, a biased exponent :attr:`exp`, and an integral mantissa with a hidden one :attr:`man`.
    The value of a *normal* number would thus be

    .. math::
        (-1)^{\texttt{sign}} \times 2^{\texttt{exp} - \texttt{bias}} \times (1 + \texttt{man} \times 2^{\texttt{-man_bits}}).

    In general, if the bias is not explicitly given for a format :class:`APyFloat` will default to an IEEE-like bias
    using the formula

    .. math::
        \texttt{bias} = 2^{\texttt{exp_bits - 1}} - 1.

    Arithmetic can be performed similarly to the operations of the built-in type
    :class:`float` in Python. The resulting word length from operations will be
    the same as the input operands' by quantizing to nearest number with ties to even (:class:`QuantizationMode.TIES_EVEN`).
    If the operands do not share the same format, the resulting
    bit widths of the exponent and mantissa field will be the maximum of its inputs:

    Examples
    --------

    >>> from apytypes import APyFloat
    >>> a = APyFloat.from_float(1.25, exp_bits=5, man_bits=2)
    >>> b = APyFloat.from_float(1.75, exp_bits=5, man_bits=2)

    Operands with same format, result will have exp_bits=5, man_bits=2

    >>> a + b
    APyFloat(sign=0, exp=16, man=2, exp_bits=5, man_bits=2)

    >>> d = APyFloat.from_float(1.75, exp_bits=4, man_bits=4)

    Operands with different formats, result will have exp_bits=5, man_bits=4

    >>> a + d
    APyFloat(sign=0, exp=16, man=8, exp_bits=5, man_bits=4)

    If the operands of an arithmetic operation have IEEE-like biases, then the result will
    also have an IEEE-like bias -- based on the resulting number of exponent bits.
    To support operations with biases deviating from the standard, the bias of the resulting format
    is calculated as the "average" of the inputs' biases:

    .. math::
        \texttt{bias}_3 = \frac{\left ( \left (\texttt{bias}_1 + 1 \right ) / 2^{\texttt{exp_bits}_1} + \left (\texttt{bias}_2 + 1 \right ) / 2^{\texttt{exp_bits}_2} \right ) \times 2^{\texttt{exp_bits}_3}}{2} - 1,

    where :math:`\texttt{exp_bits}_1` and :math:`\texttt{exp_bits}_2` are the bit widths of the operands, :math:`\texttt{bias}_1` and :math:`\texttt{bias}_2` are the
    input biases, and :math:`\texttt{exp_bits}_3` is the target bit width.
    Note that this formula still results in an IEEE-like bias when the inputs use IEEE-like biases.
    """

    def __init__(
        self,
        sign: int,
        exp: int,
        man: int,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
    ) -> None:
        """
        Create an :class:`APyFloat` object.

        Parameters
        ----------
        sign : bool or int
            The sign of the float. False/0 means positive. True/non-zero means negative.
        exp : int
            Exponent of the float as stored, i.e., actual value + bias.
        man : int
            Mantissa of the float as stored, i.e., without a hidden one.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : int, optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Returns
        -------
        :class:`APyFloat`
        """

    @staticmethod
    def from_float(
        value: object, exp_bits: int, man_bits: int, bias: int | None = None
    ) -> APyFloat:
        """
        Create an :class:`APyFloat` object from an :class:`int` or :class:`float`.

        The quantization mode used is :class:`QuantizationMode.TIES_EVEN`.

        Parameters
        ----------
        value : int, float
            Floating-point value to initialize from.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : int, optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Examples
        --------

        >>> from apytypes import APyFloat

        `a`, initialized from floating-point values.

        >>> a = APyFloat.from_float(1.35, exp_bits=10, man_bits=15)

        Returns
        -------
        :class:`APyFloat`

        See also
        --------
        from_bits
        """

    def __float__(self) -> float: ...
    @staticmethod
    def from_bits(
        bits: int, exp_bits: int, man_bits: int, bias: int | None = None
    ) -> APyFloat:
        """
        Create an :class:`APyFloat` object from a bit-representation.

        Parameters
        ----------
        bits : int
            The bit-representation for the float.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : int, optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Examples
        --------

        >>> from apytypes import APyFloat

        `a`, initialized to -1.5 from a bit pattern.

        >>> a = APyFloat.from_bits(0b1_01111_10, exp_bits=5, man_bits=2)

        Returns
        -------
        :class:`APyFloat`

        See also
        --------
        to_bits
        from_float
        """

    def to_bits(self) -> int:
        """
        Get the bit-representation of an :class:`APyFloat`.

        Examples
        --------

        >>> from apytypes import APyFloat

        `a`, initialized to -1.5 from a bit pattern.

        >>> a = APyFloat.from_bits(0b1_01111_10, exp_bits=5, man_bits=2)
        >>> a
        APyFloat(sign=1, exp=15, man=2, exp_bits=5, man_bits=2)
        >>> a.to_bits() == 0b1_01111_10
        True

        Returns
        -------
        :class:`int`

        See also
        --------
        from_bits
        """

    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def cast(
        self,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        quantization: QuantizationMode | None = None,
    ) -> APyFloat:
        """
        Change format of the floating-point number.

        This is the primary method for performing quantization when dealing with
        APyTypes floating-point numbers.

        Parameters
        ----------
        exp_bits : int, optional
            Number of exponent bits in the result.
        man_bits : int, optional
            Number of mantissa bits in the result.
        bias : int, optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.
        quantization : :class:`QuantizationMode`, optional.
            Quantization mode to use in this cast. If None, use the global
            quantization mode.

        Returns
        -------
        :class:`APyFloat`
        """

    @overload
    def __add__(self, arg: APyFloat, /) -> APyFloat: ...
    @overload
    def __add__(self, arg: int, /) -> APyFloat: ...
    @overload
    def __add__(self, arg: float, /) -> APyFloat: ...
    def __neg__(self) -> APyFloat: ...
    @overload
    def __sub__(self, arg: APyFloat, /) -> APyFloat: ...
    @overload
    def __sub__(self, arg: int, /) -> APyFloat: ...
    @overload
    def __sub__(self, arg: float, /) -> APyFloat: ...
    @overload
    def __mul__(self, arg: APyFloat, /) -> APyFloat: ...
    @overload
    def __mul__(self, arg: int, /) -> APyFloat: ...
    @overload
    def __mul__(self, arg: float, /) -> APyFloat: ...
    @overload
    def __truediv__(self, arg: APyFloat, /) -> APyFloat: ...
    @overload
    def __truediv__(self, arg: int, /) -> APyFloat: ...
    @overload
    def __truediv__(self, arg: float, /) -> APyFloat: ...
    @overload
    def __radd__(self, arg: int, /) -> APyFloat: ...
    @overload
    def __radd__(self, arg: float, /) -> APyFloat: ...
    @overload
    def __rsub__(self, arg: int, /) -> APyFloat: ...
    @overload
    def __rsub__(self, arg: float, /) -> APyFloat: ...
    @overload
    def __rmul__(self, arg: int, /) -> APyFloat: ...
    @overload
    def __rmul__(self, arg: float, /) -> APyFloat: ...
    @overload
    def __rtruediv__(self, arg: int, /) -> APyFloat: ...
    @overload
    def __rtruediv__(self, arg: float, /) -> APyFloat: ...
    def __abs__(self) -> APyFloat: ...
    @overload
    def __pow__(self, arg: APyFloat, /) -> APyFloat: ...
    @overload
    def __pow__(self, arg: int, /) -> APyFloat: ...
    @overload
    def __eq__(self, arg: APyFloat, /) -> bool: ...
    @overload
    def __eq__(self, arg: float, /) -> bool: ...
    @overload
    def __eq__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __ne__(self, arg: APyFloat, /) -> bool: ...
    @overload
    def __ne__(self, arg: float, /) -> bool: ...
    @overload
    def __ne__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __lt__(self, arg: APyFloat, /) -> bool: ...
    @overload
    def __lt__(self, arg: float, /) -> bool: ...
    @overload
    def __lt__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __gt__(self, arg: APyFloat, /) -> bool: ...
    @overload
    def __gt__(self, arg: float, /) -> bool: ...
    @overload
    def __gt__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __le__(self, arg: APyFloat, /) -> bool: ...
    @overload
    def __le__(self, arg: float, /) -> bool: ...
    @overload
    def __le__(self, arg: APyFixed, /) -> bool: ...
    @overload
    def __ge__(self, arg: APyFloat, /) -> bool: ...
    @overload
    def __ge__(self, arg: float, /) -> bool: ...
    @overload
    def __ge__(self, arg: APyFixed, /) -> bool: ...
    def __and__(self, arg: APyFloat, /) -> APyFloat: ...
    def __or__(self, arg: APyFloat, /) -> APyFloat: ...
    def __xor__(self, arg: APyFloat, /) -> APyFloat: ...
    def __invert__(self) -> APyFloat: ...
    @property
    def is_zero(self) -> bool:
        """
        True if and only if value is zero.

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_normal(self) -> bool:
        """
        True if and only if value is normal (not zero, subnormal, infinite, or NaN).

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_subnormal(self) -> bool:
        """
        True if and only if value is subnormal.

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_finite(self) -> bool:
        """
        True if and only if value is zero, subnormal, or normal.

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_nan(self) -> bool:
        """
        True if and only if value is NaN.

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_inf(self) -> bool:
        """
        True if and only if value is infinite.

        Returns
        -------
        :class:`bool`
        """

    def is_identical(self, other: APyFloat) -> bool:
        """
        Test if two :py:class:`APyFloat` objects are identical.

        Two :py:class:`APyFloat` objects are considered identical if, and only if,  they have
        the same sign, exponent, mantissa, and format.

        Returns
        -------
        :class:`bool`
        """

    @property
    def sign(self) -> bool:
        """
        Sign bit.

        See also
        --------
        true_sign

        Returns
        -------
        :class:`bool`
        """

    @property
    def true_sign(self) -> int:
        """
        Sign value.

        See also
        --------
        sign

        Returns
        -------
        :class:`int`
        """

    @property
    def man(self) -> int:
        """
        Mantissa bits.

        These are without a possible hidden one.

        See also
        --------
        true_man

        Returns
        -------
        :class:`int`
        """

    @property
    def true_man(self) -> int:
        """
        Mantissa value.

        These are with a possible hidden one.

        See also
        --------
        man

        Returns
        -------
        :class:`int`
        """

    @property
    def exp(self) -> int:
        """
        Exponent bits with bias.

        See also
        --------
        true_exp

        Returns
        -------
        :class:`int`
        """

    @property
    def true_exp(self) -> int:
        """
        Exponent value.

        The bias value is subtracted and exponent adjusted in case of
        a subnormal number.

        See also
        --------
        exp

        Returns
        -------
        :class:`int`
        """

    @property
    def bias(self) -> int:
        """
        Exponent bias.

        Returns
        -------
        :class:`int`
        """

    @property
    def man_bits(self) -> int:
        """
        Number of mantissa bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def exp_bits(self) -> int:
        """
        Number of exponent bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def bits(self) -> int:
        """
        Total number of bits.

        Returns
        -------
        :class:`int`
        """

    def cast_to_double(self, quantization: QuantizationMode | None = None) -> APyFloat:
        """
        Cast to IEEE 754 binary64 (double-precision) format.

        Convenience method corresponding to

        .. code-block:: python

           f.cast(exp_bits=11, man_bits=52)

        Parameters
        ----------
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.
        """

    def cast_to_single(self, quantization: QuantizationMode | None = None) -> APyFloat:
        """
        Cast to IEEE 754 binary32 (single-precision) format.

        Convenience method corresponding to

        .. code-block:: python

           f.cast(exp_bits=8, man_bits=23)

        Parameters
        ----------
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.
        """

    def cast_to_half(self, quantization: QuantizationMode | None = None) -> APyFloat:
        """
        Cast to IEEE 754 binary16 (half-precision) format.

        Convenience method corresponding to

        .. code-block:: python

           f.cast(exp_bits=5, man_bits=10)

        Parameters
        ----------
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.
        """

    def cast_to_bfloat16(
        self, quantization: QuantizationMode | None = None
    ) -> APyFloat:
        """
        Cast to bfloat16 format.

        Convenience method corresponding to

        .. code-block:: python

           f.cast(exp_bits=8, man_bits=7)

        Parameters
        ----------
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.
        """

    def next_up(self) -> APyFloat:
        """
        Get the smallest floating-point number in the same format that compares greater.

        See also
        --------
        next_down

        Returns
        -------
        :class:`APyFloat`
        """

    def next_down(self) -> APyFloat:
        """
        Get the largest floating-point number in the same format that compares less.

        See also
        --------
        next_up

        Returns
        -------
        :class:`APyFloat`
        """

class APyFloatAccumulatorContext(ContextManager):
    """
    Context for using custom accumulators when performing inner products and matrix multiplications.

    Inner products and matrix multiplications will by default perform the summation in the resulting format
    of the operands, but with :class:`APyFloatAccumulatorContext` a custom accumulator can be simulated
    as seen in the example below.

    Examples
    --------

    >>> import numpy as np
    >>> from apytypes import APyFloatArray, QuantizationMode
    >>> from apytypes import APyFloatAccumulatorContext

    >>> An = np.random.normal(1, 2, size=(100, 100))
    >>> A = APyFloatArray.from_float(An, exp_bits=5, man_bits=10)

    >>> bn = (np.random.uniform(0, 1, size=100),)
    >>> b = APyFloatArray.from_float(bn, exp_bits=5, man_bits=10)

    Normal matrix multiplication

    >>> c = A @ b.T

    Matrix multiplication using stochastic quantization and a wider accumulator

    >>> m = QuantizationMode.STOCH_WEIGHTED
    >>> with APyFloatAccumulatorContext(exp_bits=6, man_bits=15, quantization=m):
    ...     d = A @ b.T


    If no quantization mode is specified to the accumulator context it will fallback to the mode set globally,
    see :class:`APyFloatQuantizationContext`.
    """

    def __init__(
        self,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        quantization: QuantizationMode | None = None,
    ) -> None: ...
    def __enter__(self) -> None: ...
    def __exit__(
        self,
        exc_type: object | None = None,
        exc_value: object | None = None,
        traceback: object | None = None,
    ) -> None: ...

class APyFloatArray:
    """
    Class for multidimensional arrays with configurable floating-point formats.

    :class:`APyFloatArray` is a class for multidimensional arrays with configurable
    floating-point formats. The interface is much like the one of NumPy,
    and direct plotting is supported by most functions in Matplotlib.
    :class:`APyFloatArray` should always be preferred, if possible, when working with
    arrays as it allows for better performance, and integration with other features of APyTypes.

    For information about the workings of floating-point numbers, see its scalar
    equivalent :class:`APyFloat`.
    """

    def __init__(
        self,
        signs: Sequence,
        exps: Sequence,
        mans: Sequence,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
    ) -> None:
        """
        Create an :class:`APyFloat` object.

        Parameters
        ----------
        signs : sequence of bools or ints
            The sign of the float. False/0 means positive. True/non-zero means negative.
        exps : sequence of ints
            Exponents of the floats as stored, i.e., actual value + bias.
        mans : sequence of ints
            Mantissas of the floats as stored, i.e., without a hidden one.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : int, optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Returns
        -------
        :class:`APyFloatArray`
        """

    @overload
    def __add__(self, arg: APyFloatArray, /) -> APyFloatArray: ...
    @overload
    def __add__(self, arg: int, /) -> APyFloatArray: ...
    @overload
    def __add__(self, arg: float, /) -> APyFloatArray: ...
    @overload
    def __add__(self, arg: APyFloat, /) -> APyFloatArray: ...
    @overload
    def __radd__(self, arg: int, /) -> APyFloatArray: ...
    @overload
    def __radd__(self, arg: float, /) -> APyFloatArray: ...
    @overload
    def __radd__(self, arg: APyFloat, /) -> APyFloatArray: ...
    @overload
    def __sub__(self, arg: APyFloatArray, /) -> APyFloatArray: ...
    @overload
    def __sub__(self, arg: int, /) -> APyFloatArray: ...
    @overload
    def __sub__(self, arg: float, /) -> APyFloatArray: ...
    @overload
    def __sub__(self, arg: APyFloat, /) -> APyFloatArray: ...
    def __neg__(self) -> APyFloatArray: ...
    @overload
    def __rsub__(self, arg: int, /) -> APyFloatArray: ...
    @overload
    def __rsub__(self, arg: float, /) -> APyFloatArray: ...
    @overload
    def __rsub__(self, arg: APyFloat, /) -> APyFloatArray: ...
    @overload
    def __mul__(self, arg: APyFloatArray, /) -> APyFloatArray: ...
    @overload
    def __mul__(self, arg: int, /) -> APyFloatArray: ...
    @overload
    def __mul__(self, arg: float, /) -> APyFloatArray: ...
    @overload
    def __mul__(self, arg: APyFloat, /) -> APyFloatArray: ...
    @overload
    def __rmul__(self, arg: int, /) -> APyFloatArray: ...
    @overload
    def __rmul__(self, arg: float, /) -> APyFloatArray: ...
    @overload
    def __rmul__(self, arg: APyFloat, /) -> APyFloatArray: ...
    @overload
    def __truediv__(self, arg: APyFloatArray, /) -> APyFloatArray: ...
    @overload
    def __truediv__(self, arg: int, /) -> APyFloatArray: ...
    @overload
    def __truediv__(self, arg: float, /) -> APyFloatArray: ...
    @overload
    def __truediv__(self, arg: APyFloat, /) -> APyFloatArray: ...
    @overload
    def __rtruediv__(self, arg: int, /) -> APyFloatArray: ...
    @overload
    def __rtruediv__(self, arg: float, /) -> APyFloatArray: ...
    @overload
    def __rtruediv__(self, arg: APyFloat, /) -> APyFloatArray: ...
    def __abs__(self) -> APyFloatArray: ...
    @property
    def exp_bits(self) -> int:
        """
        Number of exponent bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def man_bits(self) -> int:
        """
        Number of mantissa bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def bits(self) -> int:
        """
        Total number of bits.

        Returns
        -------
        :class:`int`
        """

    @property
    def bias(self) -> int:
        """
        Bias.

        Returns
        -------
        :class:`int`
        """

    @property
    def shape(self) -> tuple:
        """
        The shape of the array.

        Returns
        -------
        :class:`tuple` of :class:`int`
        """

    @property
    def ndim(self) -> int:
        """
        Number of dimensions in the array.

        Returns
        -------
        :class:`int`
        """

    @property
    def T(self) -> APyFloatArray:
        """
        The transposition of the array.

        Equivalent to calling :func:`APyFloatArray.transpose`.

        Returns
        -------
        :class:`APyFloatArray`
        """

    def to_numpy(self) -> Annotated[ArrayLike, dict(dtype="float64")]:
        """
        Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

        The returned array has the same `shape` and values as `self`. This
        method rounds away from infinity on ties.

        Returns
        -------
        :class:`numpy.ndarray`
        """

    def to_bits(
        self, numpy: bool = False
    ) -> (
        list
        | Annotated[ArrayLike, dict(dtype="uint64")]
        | Annotated[ArrayLike, dict(dtype="uint32")]
        | Annotated[ArrayLike, dict(dtype="uint16")]
        | Annotated[ArrayLike, dict(dtype="uint8")]
    ):
        """
        Return the underlying bit representations.

        When `numpy` is true, the bit representations are returned in a
        :class:`numpy.ndarray`. Otherwise, they are returned in a :class:`list`.

        Returns
        -------
        :class:`list` or :class:`numpy.ndarray`
        """

    def reshape(self, number_sequence: tuple) -> APyFloatArray:
        """
        Reshape the APyFloatArray to the specified shape without changing its data.

        Parameters
        ----------
        new_shape : `tuple`
            The new shape should be compatible with the original shape. If a dimension is -1, its value will be inferred from the length of the array and remaining dimensions. Only one dimension can be -1.

        Raises
        ------
        :class:`ValueError`
            If negative dimensions less than -1 are provided, if the total size of the new array is not unchanged and divisible by the known dimensions, or if the total number of elements does not match the original array.

        Examples
        --------
        >>> from apytypes import APyFloatArray
        >>> signs = [0, 0, 1, 1]
        >>> exps = [127, 128, 128, 129]
        >>> mans = [0, 0, 4194304, 0]
        >>> arr = APyFloatArray(
        ...     signs=signs, exps=exps, mans=mans, exp_bits=8, man_bits=23
        ... )
        >>> arr.to_numpy()
        array([ 1.,  2., -3., -4.])

        >>> arr.reshape((2, 2)).to_numpy()
        array([[ 1.,  2.],
               [-3., -4.]])

        >>> arr.reshape((4,)).to_numpy()
        array([ 1.,  2., -3., -4.])

        >>> arr.reshape((2, -1)).to_numpy()
        array([[ 1.,  2.],
               [-3., -4.]])

        Returns
        -------
        :class:`APyFloatArray`
        """

    def flatten(self) -> APyFloatArray:
        """
        Return a copy of the array collapsed into one dimension.

        Examples
        --------
        >>> from apytypes import APyFloatArray
        >>> signs = [[0, 0], [1, 1]]
        >>> exps = [[127, 128], [128, 129]]
        >>> mans = [[0, 0], [4194304, 0]]
        >>> arr = APyFloatArray(
        ...     signs=signs, exps=exps, mans=mans, exp_bits=8, man_bits=23
        ... )
        >>> arr.to_numpy()
        array([[ 1.,  2.],
               [-3., -4.]])

        >>> arr.flatten().to_numpy()
        array([ 1.,  2., -3., -4.])

        Returns
        -------
        :class:`APyFloatArray`
        """

    def ravel(self) -> APyFloatArray:
        """
        Return a copy of the array collapsed into one dimension. Same as flatten with current memory-copy model.

        Examples
        --------
        >>> from apytypes import APyFloatArray
        >>> signs = [[0, 0], [1, 1]]
        >>> exps = [[127, 128], [128, 129]]
        >>> mans = [[0, 0], [4194304, 0]]
        >>> arr = APyFloatArray(
        ...     signs=signs, exps=exps, mans=mans, exp_bits=8, man_bits=23
        ... )
        >>> arr.to_numpy()
        array([[ 1.,  2.],
               [-3., -4.]])

        >>> arr.ravel().to_numpy()
        array([ 1.,  2., -3., -4.])

        Returns
        -------
        :class:`APyFloatArray`
        """

    @staticmethod
    def from_float(
        number_sequence: Sequence, exp_bits: int, man_bits: int, bias: int | None = None
    ) -> APyFloatArray:
        """
        Create an :class:`APyFloatArray` object from a sequence of :class:`int` or :class:`float`.

        Parameters
        ----------
        number_sequence : sequence of numbers
            Floating point values to initialize from. The tensor shape will be taken
            from the sequence shape.
        exp_bits : int
            Number of exponent bits in the created floating-point tensor
        man_bits : int
            Number of mantissa bits in the created floating-point tensor
        bias : int, optional
            Bias in the created floating-point tensor

        Examples
        --------

        >>> from apytypes import APyFloatArray

        Array `a`, initialized from floating-point values.

        >>> a = APyFloatArray.from_float([1.0, 1.25, 1.49], exp_bits=10, man_bits=15)

        Array `lhs` (2 x 3 matrix), initialized from floating-point values.

        >>> lhs = APyFloatArray.from_float(
        ...     [
        ...         [1.0, 2.0, 3.0],
        ...         [4.0, 5.0, 6.0],
        ...     ],
        ...     exp_bits=5,
        ...     man_bits=2,
        ... )

        Returns
        -------
        :class:`APyFloatArray`
        """

    @staticmethod
    def from_array(
        ndarray: Annotated[ArrayLike, dict(order="C")],
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
    ) -> APyFloatArray:
        """
        Create an :class:`APyFloatArray` object from an ndarray.

        Parameters
        ----------
        ndarray : ndarray
            Values to initialize from. The tensor shape will be taken from the ndarray shape.
        exp_bits : int
            Number of exponent bits in the created floating-point tensor
        man_bits : int
            Number of mantissa bits in the created floating-point tensor
        bias : int, optional
            Bias in the created floating-point tensor

        Examples
        --------

        >>> from apytypes import APyFloatArray
        >>> import numpy as np

        Array `a`, initialized from NumPy ndarray

        >>> a = APyFloatArray.from_array(
        ...     np.array(
        ...         [
        ...             [1.0, 2.0, 3.0],
        ...             [4.0, 5.0, 6.0],
        ...         ]
        ...     ),
        ...     man_bits=10,
        ...     exp_bits=10,
        ... )

        Returns
        -------
        :class:`APyFloatArray`
        """

    @staticmethod
    def zeros(
        shape: tuple, exp_bits: int, man_bits: int, bias: int | None = None
    ) -> APyFloatArray:
        """
        Initializes an array with zeros.

        Parameters
        ----------
        shape : tuple
            Shape of the array.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Returns
        -------
        APyFloatArray
            An array filled with zeros.
        """

    @staticmethod
    def ones(
        shape: tuple, exp_bits: int, man_bits: int, bias: int | None = None
    ) -> APyFloatArray:
        """
        Initializes an array with ones.

        Parameters
        ----------
        shape : tuple
            Shape of the array.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Returns
        -------
        APyFloatArray
            An array filled with ones.
        """

    @staticmethod
    def eye(
        n: int,
        exp_bits: int,
        man_bits: int,
        m: int | None = None,
        bias: int | None = None,
    ) -> APyFloatArray:
        """
        Initializes an array with ones on the diagonal.

        Parameters
        ----------
        n : int
            Number of rows (and columns) in the n x n output.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        m : int, optional
            Number of columns. Default is None, which results in an n x n output.
        bias : optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Returns
        -------
        APyFloatArray
            An array with the specified value on the diagonal.
        """

    @staticmethod
    def identity(
        n: int, exp_bits: int, man_bits: int, bias: int | None = None
    ) -> APyFloatArray:
        """
        Initializes an identity matrix with ones on the diagonal.

        Parameters
        ----------
        n : int
            Number of rows (and columns) in the n x n output.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Returns
        -------
        APyFloatArray
            An identity matrix with ones on the diagonal.
        """

    @staticmethod
    def full(shape: tuple, fill_value: APyFloat) -> APyFloatArray:
        """
        Initializes an array filled with the specified value.

        Parameters
        ----------
        shape : tuple
            Shape of the array.
        fill_value : APyFloat
            Value to fill the array.

        Returns
        -------
        APyFloatArray
            An array filled with the specified value.
        """

    def __matmul__(self, rhs: APyFloatArray) -> APyFloatArray | APyFloat: ...
    def __repr__(self) -> str: ...
    def __len__(self) -> int: ...
    def is_identical(self, other: APyFloatArray) -> bool:
        """
        Test if two :class:`APyFloatArray` objects are identical.

        Two :class:`APyFloatArray` objects are considered identical if, and only if:
          * They represent exactly the same tensor shape
          * They store the exact same floating-ppint values in all tensor elements
          * They have the exact same bit format (`exp_bits`, `man_bits`, and `bias`)

        Returns
        -------
        :class:`bool`
        """

    def swapaxes(self, axis1: int, axis2: int) -> APyFloatArray:
        """
        Interchange two axes of an array.

        Parameters
        ----------
        axis1 : int
            First axis.
        axis2 : int
            Second axis.

        Examples
        --------
        >>> from apytypes import APyFloatArray
        >>> x = APyFloatArray.from_float([[1, 2, 3]], exp_bits=5, man_bits=2)
        >>> x.swapaxes(0, 1).to_numpy()
        array([[1.],
               [2.],
               [3.]])

        >>> x = APyFloatArray.from_float(
        ...     [[[0, 1], [2, 3]], [[4, 5], [6, 7]]], exp_bits=5, man_bits=5
        ... )
        >>> x.to_numpy()
        array([[[0., 1.],
                [2., 3.]],
        <BLANKLINE>
               [[4., 5.],
                [6., 7.]]])

        >>> x.swapaxes(0, 2).to_numpy()
        array([[[0., 4.],
                [2., 6.]],
        <BLANKLINE>
               [[1., 5.],
                [3., 7.]]])

        Returns
        -------
        a_swapped : :class:`APyFloatArray`
            Copy of `a` with axes swapped
        """

    def transpose(self, axes: tuple | None = None) -> APyFloatArray:
        """
        Return copy of array with axes transposed.

        For a 1-D array, this return the same array.
        For a 2-D array, this is the standard matrix transpose.
        For an n-D array, if axes are given, their order indicates how the
        axes are permuted (see Examples). If axes are not provided, then
        ``a.transpose(a).shape == a.shape[::-1]``.

        Parameters
        ----------
        axes : tuple of ints, optional
            If specified, it must be a tuple or list which contains a permutation
            of [0,1,...,N-1] where N is the number of axes of `a`. The `i`'th axis
            of the returned array will correspond to the axis numbered ``axes[i]``
            of the input. If not specified, defaults to ``range(a.ndim)[::-1]``,
            which reverses the order of the axes.

        Examples
        --------
        >>> from apytypes import APyFloatArray
        >>> a = APyFloatArray.from_float(
        ...     [[1.0, 2.0, 3.0], [-4.0, -5.0, -6.0]], exp_bits=5, man_bits=2
        ... )
        >>> a.to_numpy()
        array([[ 1.,  2.,  3.],
               [-4., -5., -6.]])
        >>> a = a.transpose()
        >>> a.to_numpy()
        array([[ 1., -4.],
               [ 2., -5.],
               [ 3., -6.]])

        >>> a = APyFloatArray.from_float([1.0] * 6, exp_bits=5, man_bits=2).reshape(
        ...     (1, 2, 3)
        ... )
        >>> a.transpose((1, 0, 2)).shape
        (2, 1, 3)

        >>> a.transpose((-2, -3, -1)).shape
        (2, 1, 3)

        Returns
        -------
        :class:`APyFloatArray`
            `a` with its axes permuted.
        """

    def broadcast_to(self, shape: tuple | int) -> APyFloatArray:
        """
        Broadcast array to new shape.

        .. versionadded:: 0.2

        Parameters
        ----------
        shape : tuple or int
            The shape to broadcast to. A single integer ``i`` is interpreted as ``(i,)``.

        Returns
        -------
        :class:`APyFloatArray`
        """

    def convolve(self, other: APyFloatArray, mode: str = "full") -> APyFloatArray:
        """
        Return the discrete linear convolution with another one-dimensional array.

        Requires that ``ndim = 1`` for both `self` and `other`.

        Parameters
        ----------
        other : :class:`APyFloatArray`
            The one-dimensional array of length :code:`N` to convolve with.

        mode : {'full', 'same', 'valid'}, default: 'full'
            'full':
                Return the full convolution for each point of overlap. The
                resulting single-dimensional shape will have length :code:`N + M -
                1`. Boundary effects occurs for points where the `a` and `v` do not
                overlap completely.
            'same':
                Return a convolution of length :code:`max(M, N)`. Boundary effects
                still occur around the edges of the result.
            'valid':
                Return the convolution for each point of full overlap. The
                resulting single-dimensional shape will have length :code:`max(M, N)
                - min(M, N) + 1`

        Returns
        -------
        convolved : :class:`APyFloatArray`
            The convolved array.
        """

    def squeeze(self, axis: int | tuple | None = None) -> APyFloatArray:
        """
        Remove axes of size one at the specified axis/axes, if no axís is given removes all dimensions with size one.

        Parameters
        ----------
        axis : tuple or int, optional
            The axis/axes to squeeze, a given axis with a size other than one will result in an error. No given axes  will be remove all dimensions of size 1.

        Returns
        -------
        :class:`APyFloatArray`

        Raises
        ------
        :class:`ValueError`
            If given an axis of a size other than one, a ValueError will be thrown.
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def sum(self, axis: tuple | int | None = None) -> APyFloatArray | APyFloat:
        """
        Return the sum of the elements along specified axis/axes.

        Parameters
        ----------
        axis : tuple or int, optional
            The axis/axes to summate across. Will summate the whole array if no int or tuple is specified.

        Returns
        -------
        :class:`APyFloatArray` or :class:`APyFloat`

        Raises
        -------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------

        >>> from apytypes import APyFloatArray

        >>> a = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)

        >>> a.sum()
        APyFloat(sign=0, exp=515, man=320, exp_bits=10, man_bits=10)

        -------
        """

    def cumsum(self, axis: int | None = None) -> APyFloatArray:
        """
        Return the cumulative sum of the elements along a given axis.

        Parameters
        ----------
        axis : int, optional
            The axis to summate across. If not given an axis it will return the cumulative sum of the flattened array.

        Returns
        -------
        :class:`APyFloatArray`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------

        >>> from apytypes import APyFloatArray

        >>> a = APyFloatArray.from_float(
        ...     [[1, 2, 3], [4, 5, 6]], exp_bits=10, man_bits=10
        ... )

        >>> a.cumsum()
        APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 514, 514, 515], [0, 512, 512, 256, 896, 320], shape=(6,), exp_bits=10, man_bits=10, bias=511)

        >>> a.cumsum(0)
        APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 512, 513, 513, 514], [0, 0, 512, 256, 768, 128], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

        >>> a.cumsum(1)
        APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 513, 514, 514], [0, 512, 512, 0, 128, 896], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

        -------
        """

    def nansum(self, axis: tuple | int | None = None) -> APyFloatArray | APyFloat:
        """
        Return the sum of the elements along specified axis/axes treating Not a Number as 0.

        Parameters
        ----------
        axis : tuple or int, optional
            The axis/axes to summate across. Will summate the whole array if no int or tuple is specified.

        Returns
        -------
        :class:`APyFloatArray` or :class:`APyFloat`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------

        >>> from apytypes import APyFloatArray

        >>> nan = float("nan")

        >>> a = APyFloatArray.from_float([1, 2, 3, 4, 5, nan], exp_bits=10, man_bits=10)

        >>> a.nansum()
        APyFloat(sign=0, exp=514, man=896, exp_bits=10, man_bits=10)



        -------
        """

    def nancumsum(self, axis: int | None = None) -> APyFloatArray:
        """
        Return the cumulative sum of the elements along a given axis treating Not a Number as 0.

        Parameters
        ----------
        axis : int, optional
            The axis to summate across. If not given an axis it will return the cumulative sum of the flattened array.

        Returns
        -------
        :class:`APyFloatArray`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------


        >>> from apytypes import APyFloatArray

        >>> nan = float("nan")

        >>> a = APyFloatArray.from_float(
        ...     [[1, 2, 3], [4, 5, 6]], exp_bits=10, man_bits=10
        ... )

        >>> a.nancumsum()
        APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 514, 514, 515], [0, 512, 512, 256, 896, 320], shape=(6,), exp_bits=10, man_bits=10, bias=511)

        >>> a.nancumsum(0)
        APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 512, 513, 513, 514], [0, 0, 512, 256, 768, 128], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

        >>> a.nancumsum(1)
        APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 513, 514, 514], [0, 512, 512, 0, 128, 896], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)


        -------
        """

    def prod(self, axis: tuple | int | None = None) -> APyFloatArray | APyFloat:
        """
        Return the product of the elements along specified axis/axes.

        Parameters
        ----------
        axis : tuple, int, optional
            The axis/axes to calculate the product across. If not given an axis it will return the product of the flattened array.

        Returns
        -------
        :class:`APyFloatArray` or :class:`APyFloat`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------

        >>> from apytypes import APyFloatArray

        >>> a = APyFloatArray.from_float([1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10)

        >>> a.prod()
        APyFloat(sign=0, exp=520, man=416, exp_bits=10, man_bits=10)

        -------
        """

    def cumprod(self, axis: int | None = None) -> APyFloatArray:
        """
        Return the cumulative product of the elements along a given axes.

        Parameters
        ----------
        axis : int, optional
            The axes to calculate the product across. If not given an axis it will return the cumulative product of the flattened array.

        Returns
        -------
        :class:`APyFloatArray`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        --------

        >>> from apytypes import APyFloatArray


        >>> a = APyFloatArray.from_float(
        ...     [[1, 2, 3], [4, 5, 6]], exp_bits=10, man_bits=10
        ... )

        >>> a.cumprod()
        APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 515, 517, 520], [0, 0, 512, 512, 896, 416], shape=(6,), exp_bits=10, man_bits=10, bias=511)

        >>> a.cumprod(0)
        APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 512, 513, 514, 515], [0, 0, 512, 0, 256, 128], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

        >>> a.cumprod(1)
        APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 513, 515, 517], [0, 0, 512, 0, 256, 896], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

        -------
        """

    def nanprod(self, axis: tuple | int | None = None) -> APyFloatArray | APyFloat:
        """
        Return the product of the elements along a given axis treating Not a Number as 0.

        Parameters
        ----------
        axis : int, optional
            The axis to calculate the product across. If not given an axis it will return the product of the flattened array.

        Returns
        -------
        :class:`APyFloatArray` or :class:`APyFloat`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def nancumprod(self, axis: int | None = None) -> APyFloatArray:
        """
        Return the cumulative product of the elements along a given axis treating Not a Number as 0.

        Parameters
        ----------
        axis : int, optional
            The axis to calculate the product across. If not given an axis it will return the cumulative product of the flattened array.

        Returns
        -------
        :class:`APyFloatArray`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def max(self, axis: tuple | int | None = None) -> APyFloatArray | APyFloat:
        """
        Returns the maximum value from an array or the maximum along an axis

        Parameters
        ----------
        axis : tuple or int, optional
            The axis to get the maximum along.

        Returns
        -------
        :class:`APyFloatArray` or :class:`APyFloat`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        -------

        >>> from apytypes import APyFloatArray

        Array `a`, array to get the maximum along.

        >>> a = APyFloatArray.from_float(
        ...     [[1, 2, 3], [4, 5, 6]], exp_bits=10, man_bits=10
        ... )

        >>> a.max()
        APyFloat(sign=0, exp=513, man=512, exp_bits=10, man_bits=10)

        >>> a.max(0)
        APyFloatArray([0, 0, 0], [513, 513, 513], [0, 256, 512], shape=(3,), exp_bits=10, man_bits=10, bias=511)

        >>> a.max(1)
        APyFloatArray([0, 0], [512, 513], [512, 512], shape=(2,), exp_bits=10, man_bits=10, bias=511)

        -------
        """

    def min(self, axis: tuple | int | None = None) -> APyFloatArray | APyFloat:
        """
        Returns the minimum value from an array or the minimum along an axis

        Parameters
        ----------
        axis : tuple or int, optional
            The axis to get the minimum along.

        Returns
        -------
        :class:`APyFloatArray` or :class:`APyFloat`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.

        Examples
        -------

        >>> from apytypes import APyFloatArray

        Array `a`, array to get the minimum along.

        >>> a = APyFloatArray.from_float(
        ...     [[1, 2, 3], [4, 5, 6]], exp_bits=10, man_bits=10
        ... )

        >>> a.min()
        APyFloat(sign=0, exp=511, man=0, exp_bits=10, man_bits=10)

        >>> a.min(0)
        APyFloatArray([0, 0, 0], [511, 512, 512], [0, 0, 512], shape=(3,), exp_bits=10, man_bits=10, bias=511)

        >>> a.min(1)
        APyFloatArray([0, 0], [511, 513], [0, 0], shape=(2,), exp_bits=10, man_bits=10, bias=511)

        -------
        """

    def nanmax(self, axis: tuple | int | None = None) -> APyFloatArray | APyFloat:
        """
        Returns the maximum value from an array or the maximum along an axis. Ignoring nan.
        Issues a warning when encountering an all-nan slice or axis.

        Parameters
        ----------
        axis : tuple or int, optional
            The axis to get the maximum along.

        Returns
        -------
        :class:`APyFloatArray` or :class:`APyFloat`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def nanmin(self, axis: tuple | int | None = None) -> APyFloatArray | APyFloat:
        """
        Returns the minimum value from an array or the minimum along an axis. Ignoring nan.
        Issues a warning when encountering an all-nan slice or axis.

        Parameters
        ----------
        axis : tuple or int, optional
            The axis to get the minimum along.

        Returns
        -------
        :class:`APyFloatArray` or :class:`APyFloat`

        Raises
        ------
        :class:`IndexError`
            If a specified axis is outside of the existing number of dimensions for the array.
        """

    def __getitem__(
        self, idx: int | slice | types.EllipsisType | tuple
    ) -> APyFloatArray | APyFloat: ...
    def __iter__(self) -> APyFloatArrayIterator: ...
    def __array__(self) -> Annotated[ArrayLike, dict(dtype="float64")]: ...
    def cast(
        self,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        quantization: QuantizationMode | None = None,
    ) -> APyFloatArray:
        """
        Change format of the floating-point number.

        This is the primary method for performing quantization when dealing with
        APyTypes floating-point numbers.

        Parameters
        ----------
        exp_bits : int, optional
            Number of exponent bits in the result.
        man_bits : int, optional
            Number of mantissa bits in the result.
        bias : int, optional
            Bias used in the result.
        quantization : :class:`QuantizationMode`, optional.
            Quantization mode to use in this cast. If None, use the global
            quantization mode.

        Returns
        -------
        :class:`APyFloat`
        """

    def cast_to_double(
        self, quantization: QuantizationMode | None = None
    ) -> APyFloatArray:
        """
        Cast to IEEE 754 binary64 (double-precision) format.

        Convenience method corresponding to

        .. code-block:: python

           f.cast(exp_bits=11, man_bits=52)

        Parameters
        ----------

        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.
        """

    def cast_to_single(
        self, quantization: QuantizationMode | None = None
    ) -> APyFloatArray:
        """
        Cast to IEEE 754 binary32 (single-precision) format.

        Convenience method corresponding to

        .. code-block:: python

           f.cast(exp_bits=8, man_bits=23)

        Parameters
        ----------

        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.
        """

    def cast_to_half(
        self, quantization: QuantizationMode | None = None
    ) -> APyFloatArray:
        """
        Cast to IEEE 754 binary16 (half-precision) format.

        Convenience method corresponding to

        .. code-block:: python

           f.cast(exp_bits=5, man_bits=10)

        Parameters
        ----------

        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.
        """

    def cast_to_bfloat16(
        self, quantization: QuantizationMode | None = None
    ) -> APyFloatArray:
        """
        Cast to bfloat16 format.

        Convenience method corresponding to

        .. code-block:: python

           f.cast(exp_bits=8, man_bits=7)

        Parameters
        ----------

        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.
        """

class APyFloatArrayIterator:
    def __iter__(self) -> APyFloatArrayIterator: ...
    def __next__(self) -> APyFloatArray | APyFloat: ...

class APyFloatQuantizationContext(ContextManager):
    """
    Context for changing the quantization mode used for floating-point operations.

    If not specified explicitly, floating-point operations will use the quantization mode that is set globally,
    which is :class:`QuantizationMode.TIES_EVEN` by default. The mode however can be changed using the static method
    :func:`apytypes.set_float_quantization_mode`, or, preferably, by using a so-called quantization context.
    With a quantization context one can change the quantization mode used by all operations within a specific section
    in the code.

    Examples
    --------

    >>> from apytypes import APyFloat, QuantizationMode
    >>> from apytypes import APyFloatQuantizationContext

    Addition using the default round to nearest, ties even

    >>> a = APyFloat.from_float(0.123, exp_bits=5, man_bits=10)
    >>> b = APyFloat.from_float(3.21, exp_bits=5, man_bits=10)
    >>> a + b
    APyFloat(sign=0, exp=16, man=683, exp_bits=5, man_bits=10)

    Addition using round towards zero

    >>> m = QuantizationMode.TO_ZERO
    >>> with APyFloatQuantizationContext(quantization=m):
    ...     a + b
    APyFloat(sign=0, exp=16, man=682, exp_bits=5, man_bits=10)

    Stochastic rounding with an optional seed

    >>> m = QuantizationMode.STOCH_WEIGHTED
    >>> s = 0x1234
    >>> with APyFloatQuantizationContext(quantization=m, quantization_seed=s):
    ...     a + b
    APyFloat(sign=0, exp=16, man=683, exp_bits=5, man_bits=10)

    The quantization mode is reverted automatically upon exiting the context.
    Nesting the contexts is also possible.
    """

    def __init__(
        self, quantization: QuantizationMode, quantization_seed: int | None = None
    ) -> None: ...
    def __enter__(self) -> None: ...
    def __exit__(
        self,
        exc_type: object | None = None,
        exc_value: object | None = None,
        traceback: object | None = None,
    ) -> None: ...

class ContextManager:
    pass

class OverflowMode(enum.Enum):
    WRAP = 0
    """Two's complement overflow. Remove MSBs."""

    SAT = 1
    """Saturate to the closest of most positive and most negative value."""

    NUMERIC_STD = 2
    """
    Remove MSBs, but keep the most significant bit. As ieee.numeric_std resize for signed.
    """

class QuantizationMode(enum.Enum):
    TO_NEG = 0
    """Alias. Round towards negative infinity."""

    TO_ZERO = 2
    """Alias. Round towards zero."""

    TO_POS = 1
    """Alias. Round towards positive infinity."""

    TO_AWAY = 3
    """Alias. Truncate away from zero."""

    TIES_ZERO = 6
    """Alias. Round to nearest, ties toward zero."""

    TIES_AWAY = 7
    """Alias. Round to nearest, ties away from zero."""

    TIES_EVEN = 9
    """Alias. Round to nearest, ties to even."""

    TIES_ODD = 10
    """Alias. Round to nearest, ties to odd."""

    TIES_NEG = 8
    """Alias. Round to nearest, ties towards negative infinity."""

    TIES_POS = 5
    """Alias. Round to nearest, ties towards positive infinity."""

    TRN = 0
    """Truncation (remove bits, round towards negative infinity"""

    TRN_ZERO = 2
    """Unbiased magnitude truncation (round towards zero)."""

    TRN_INF = 1
    """Round towards positive infinity."""

    TRN_AWAY = 3
    """Truncate away from zero."""

    TRN_MAG = 4
    """
    Magnitude truncation (round towards zero), fixed-point friendly variant (add sign-bit).
    """

    RND = 5
    """
    Round to nearest, ties towards positive infinity (standard 'round' for fixed-point).
    """

    RND_ZERO = 6
    """Round to nearest, ties toward zero."""

    RND_INF = 7
    """Round to nearest, ties away from zero."""

    RND_MIN_INF = 8
    """Round to nearest, ties toward negative infinity."""

    RND_CONV = 9
    """Round to nearest, ties to even."""

    RND_CONV_ODD = 10
    """Round to nearest, ties to odd."""

    JAM = 11
    """Jamming/von Neumann rounding. Set LSB to 1."""

    JAM_UNBIASED = 12
    """
    Unbiased jamming/von Neumann rounding. Set LSB to 1 unless the previous LSB and all the removed bits are 0.
    """

    STOCH_WEIGHTED = 13
    """Stochastic rounding. Probability depends on the bits to remove."""

    STOCH_EQUAL = 14
    """Stochastic rounding with equal probability."""

def get_float_quantization_mode() -> QuantizationMode:
    """
    Get current quantization context.

    Returns
    -------
    :class:`QuantizationMode`

    See also
    --------
    set_float_quantization_mode
    """

def get_float_quantization_seed() -> int:
    """
    Set current quantization seed.

    The quantization seed is used for stochastic quantization.

    Returns
    -------
    :class:`int`

    See also
    --------
    set_float_quantization_seed
    """

def set_float_quantization_mode(mode: QuantizationMode) -> None:
    """
    Set current quantization context.

    Parameters
    ----------
    mode : :class:`QuantizationMode`
        The quantization mode to use.

    See also
    --------
    get_float_quantization_mode
    """

def set_float_quantization_seed(seed: int) -> None:
    """
    Set current quantization seed.

    The quantization seed is used for stochastic quantization.

    Parameters
    ----------
    seed : int
        The quantization seed to use.

    See also
    --------
    get_float_quantization_seed
    """
