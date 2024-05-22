from __future__ import annotations
import apytypes
import numpy
import typing

__all__ = [
    "APyFixed",
    "APyFixedArray",
    "APyFixedArrayIterator",
    "APyFixedAccumulatorContext",
    "APyFixedCastContext",
    "APyFloat",
    "APyFloatArray",
    "APyFloatArrayIterator",
    "APyFloatAccumulatorContext",
    "ContextManager",
    "OverflowMode",
    "APyFloatQuantizationContext",
    "QuantizationMode",
    "get_float_quantization_mode",
    "get_float_quantization_seed",
    "set_float_quantization_mode",
    "set_float_quantization_seed",
]

class APyFixed:
    """

    Class for configurable fixed-point formats.
    """

    __hash__: typing.ClassVar[None] = None
    @staticmethod
    def from_float(
        value: float,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixed:
        """
        Create an :class:`APyFixed` object from :class:`float`.

        The initialized fixed-point value is the one closest to the input floating-point
        value, rounded away from zero on ties. Exactly two of the three bit-specifiers
        (`bits`, `int_bits`, `frac_bits`) must be set.

        Parameters
        ----------
        value : float
            Floating point value to initialize from
        int_bits : int, optional
            Number of integer bits in the created fixed-point object
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point object
        bits : int, optional
            Total number of bits in the created fixed-point object

        Examples
        --------
        .. code-block:: python

            from apytypes import APyFixed

            # Fixed-point `fx_a`, initialized from the floating-point value 1.234,
            # rounded to 1.25 as it is the closest representable number
            fx_a = APyFixed.from_float(1.234, int_bits=2, frac_bits=2)
        """

    @staticmethod
    def from_int(
        value: float,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixed:
        """
        Create an :class:`APyFixed` object from :class:`int`.

        The initialized fixed-point value equals the integer value. Exactly two of the
        three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

        Parameters
        ----------
        value : int
            Integer value to initialize from
        int_bits : int, optional
            Number of integer bits in the created fixed-point object
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point object
        bits : int, optional
            Total number of bits in the created fixed-point object

        Examples
        --------
        .. code-block:: python

            from apytypes import APyFixed

            # Fixed-point `fx_a`, initialized from the integer value 2
            fx_a = APyFixed.from_int(2, int_bits=3, frac_bits=2)
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

        Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be
        set.

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
        .. code-block:: python

            from apytypes import APyFixed

            # Larger fixed-point value initialization from a string (base-10)
            fx_a = APyFixed.from_str(
                "-1376018206341311063223476816643087998331620501540496640."
                "021222579872958058370179355618716816066859017361262100333952697594702"
                "314679773970519809467311447652539955943903993200932791396783892142688"
                "708904952458654442554723081083186210082207584128592922850820472478833"
                "257136662269306798708182072507551281664490003441493733349403017982015"
                "56238154807942919433116912841796875",
                bits=511,
                int_bits=199,
                base=10
            )
        """

    def __abs__(self) -> APyFixed: ...
    @typing.overload
    def __add__(self, arg0: APyFixed) -> APyFixed: ...
    @typing.overload
    def __add__(self, arg0: int) -> APyFixed: ...
    @typing.overload
    def __eq__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __eq__(self, arg0: float) -> bool: ...
    @typing.overload
    def __eq__(self, arg0: int) -> bool: ...
    def __float__(self) -> float: ...
    @typing.overload
    def __ge__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __ge__(self, arg0: float) -> bool: ...
    @typing.overload
    def __ge__(self, arg0: int) -> bool: ...
    @typing.overload
    def __gt__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __gt__(self, arg0: float) -> bool: ...
    @typing.overload
    def __gt__(self, arg0: int) -> bool: ...
    def __init__(
        self,
        bit_pattern: int,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> None: ...
    @typing.overload
    def __le__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __le__(self, arg0: float) -> bool: ...
    @typing.overload
    def __le__(self, arg0: int) -> bool: ...
    def __lshift__(self, shift_amnt: int) -> APyFixed: ...
    @typing.overload
    def __lt__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __lt__(self, arg0: float) -> bool: ...
    @typing.overload
    def __lt__(self, arg0: int) -> bool: ...
    @typing.overload
    def __mul__(self, arg0: APyFixed) -> APyFixed: ...
    @typing.overload
    def __mul__(self, arg0: int) -> APyFixed: ...
    @typing.overload
    def __ne__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __ne__(self, arg0: float) -> bool: ...
    @typing.overload
    def __ne__(self, arg0: int) -> bool: ...
    @typing.overload
    def __neg__(self) -> APyFixed: ...
    @typing.overload
    def __neg__(self) -> APyFixed: ...
    def __radd__(self, arg0: int) -> APyFixed: ...
    def __repr__(self) -> str: ...
    def __rmul__(self, arg0: int) -> APyFixed: ...
    def __rshift__(self, shift_amnt: int) -> APyFixed: ...
    def __rsub__(self, arg0: int) -> APyFixed: ...
    def __str__(self, base: int = 10) -> str: ...
    @typing.overload
    def __sub__(self, arg0: APyFixed) -> APyFixed: ...
    @typing.overload
    def __sub__(self, arg0: int) -> APyFixed: ...
    def __truediv__(self, arg0: APyFixed) -> APyFixed: ...
    def _repr_latex_(self) -> str: ...
    def cast(
        self,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        quantization: QuantizationMode | None = None,
        overflow: OverflowMode | None = None,
        bits: int | None = None,
    ) -> APyFixed:
        """
        Create a new resized fixed-point number based on the bit pattern in this
        fixed-point number.

        This is the primary method for performing quantization, truncation,
        overflowing, and saturation when dealing with APyTypes fixed-point numbers.

        Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be
        set.

        Parameters
        ----------
        int_bits : int, optional
            Number of integer bits in the created fixed-point object
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point object
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use in this cast
        overflow : :class:`OverflowMode`, optional
            Overflowing mode to use in this cast
        bits : int, optional
            Total number of bits in the created fixed-point object

        Returns
        -------
        :class:`APyFixed`

        Examples
        --------
        .. code-block:: python

            from apytypes import APyFixed
            from apytypes import QuantizationMode
            from apytypes import OverflowMode

            fx = APyFixed.from_float(2.125, int_bits=3, frac_bits=3)

            # Truncation (fx_a == 2.0)
            fx_a = fx.cast(int_bits=3, frac_bits=2, quantization=QuantizationMode.TRN)

            # Quantization (fx_b == 2.25)
            fx_b = fx.cast(int_bits=3, frac_bits=2, quantization=QuantizationMode.RND)

            # Two's complement overflowing (fx_c == -1.875)
            fx_c = fx.cast(int_bits=2, frac_bits=3, overflow=OverflowMode.WRAP)
        """

    def is_identical(self, other: APyFixed) -> bool:
        """
        Test if two fixed-point objects are exactly identical.

        Two `APyFixed` objects are considered exactly identical if, and only if,
        they store the same fixed-point value, and have the exact same
        bit-specification (`bits`, `int_bits`, and `frac_bits`). This is a more
        restrictive test than ``==``,  that only tests equality of the stored
        fixed-point value.

        Parameters
        ----------
        other : :class:`APyFixed`
            The fixed-point number to test identicality against

        Returns
        -------
        :class:`bool`

        Examples
        --------
        .. code-block:: python

            from apytypes import APyFixed

            fx_a = APyFixed.from_float(2.0, int_bits=3, frac_bits=3)
            fx_b = APyFixed.from_float(2.0, int_bits=4, frac_bits=3)

            # `fx_a` and `fx_b` store the same fixed-point value
            assert fx_a == fx_b

            # `fx_a` and `fx_b` differ in the `int_bits` specifier
            assert not(fx_a.is_identical(fx_b))
        """

    def leading_fractional_zeros(self) -> int:
        """
        Retrieve the number of leading zeros after the binary fixed-point.

        Returns
        -------
        :class:`int`
        """

    def leading_ones(self) -> int:
        """
        Retrieve the number of leading ones.

        Returns
        -------
        :class:`int`
        """

    def leading_signs(self) -> int:
        """
        Retrieve the number of leading signs.

        Returns
        -------
        :class:`int`
        """

    def leading_zeros(self) -> int:
        """
        Retrieve the number of leading zeros.

        Returns
        -------
        :class:`int`
        """

    def to_bits(self) -> int:
        """
        Retrieve underlying bit-pattern in an :class:`int`.

        Returns
        -------
        :class:`int`

        Examples
        --------
        .. code-block:: python

            from apytypes import APyFixed

            # Create fixed-point number `fx_a` of value -5.75
            fx_a = APyFixed.from_float(-5.75, int_bits=4, frac_bits=4)

            # Returns: 164 == 0xA4 == 0b10100100
            fx_a.to_bits()
        """

    @property
    def _is_negative(self) -> bool: ...
    @property
    def _is_positive(self) -> bool: ...
    @property
    def _vector_size(self) -> int: ...
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

    @property
    def is_zero(self) -> bool:
        """
        True if the stored value equals zero, false otherwise.
        """

class APyFixedArray:
    @staticmethod
    def from_float(
        float_sequence: typing.Sequence,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Create an :class:`APyFixedArray` object from a sequence of :class:`float`.

        The initialized fixed-point values are the ones closest to the input
        floating-point values, rounded away from zero on ties. Exactly two of the three
        bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

        Parameters
        ----------
        float_sequence : sequence of float
            Floating point values to initialize from. The tensor shape will be taken
            from the sequence shape.
        int_bits : int, optional
            Number of integer bits in the created fixed-point tensor
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point tensor
        bits : int, optional
            Total number of bits in the created fixed-point tensor

        Returns
        -------
        :class:`APyFixedArray`

        Examples
        --------

        .. code-block:: python

            from apytypes import APyFixedArray

            # Array `a`, initialized from floating-point values.
            a = APyFixedArray.from_float([1.0, 1.25, 1.49], int_bits=2, frac_bits=2)

            # Array `b` (2 x 3 matrix), initialized from floating-point values.
            b = APyFixedArray.from_float(
                [
                    [1.0, 2.0, 3.0],
                    [4.0, 5.0, 6.0],
                ],
                bits=5,
                frac_bits=0
            )
        """

    @staticmethod
    def from_array(
        float_array,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Create an :class:`APyFixedArray` object from an ndarray.

        The initialized fixed-point values are the ones closest to the input
        floating-point values, rounded away from zero on ties. Exactly two of the three
        bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

        Using ndarrays as input is generally faster than e.g. lists.

        Parameters
        ----------
        float_sequence : sequence of float
            Floating point values to initialize from. The tensor shape will be taken
            from the sequence shape.
        int_bits : int, optional
            Number of integer bits in the created fixed-point tensor
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point tensor
        bits : int, optional
            Total number of bits in the created fixed-point tensor

        Returns
        -------
        :class:`APyFixedArray`

        Examples
        --------

        .. code-block:: python

            from apytypes import APyFixedArray
            import numpy as np

            # Array `a`, initialized from NumPy ndarray
            a = APyFixedArray.from_array(
                np.array([
                    [1.0, 2.0, 3.0],
                    [4.0, 5.0, 6.0],
                ]),
                int_bits=10,
                frac_bits=0
            )
        """

    def __abs__(self) -> APyFixedArray: ...
    @typing.overload
    def __add__(self, arg0: APyFixedArray) -> APyFixedArray: ...
    @typing.overload
    def __add__(self, arg0: int) -> APyFixedArray: ...
    @typing.overload
    def __add__(self, arg0: float) -> APyFixedArray: ...
    @typing.overload
    def __add__(self, arg0: APyFixed) -> APyFixedArray: ...
    def __array__(self) -> numpy.ndarray[numpy.float64]: ...
    def __getitem__(self, idx: int) -> typing.Union[APyFixedArray, APyFixed]: ...
    def __init__(
        self,
        bit_pattern_sequence: typing.Sequence,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        bits: int | None = None,
    ) -> None: ...
    def __iter__(self) -> APyFixedArrayIterator: ...
    def __len__(self) -> int: ...
    def __lshift__(self, shift_amnt: int) -> APyFixedArray: ...
    def __matmul__(self, rhs: APyFixedArray) -> APyFixedArray: ...
    @typing.overload
    def __mul__(self, arg0: APyFixedArray) -> APyFixedArray: ...
    @typing.overload
    def __mul__(self, arg0: APyFixed) -> APyFixedArray: ...
    @typing.overload
    def __mul__(self, arg0: int) -> APyFixedArray: ...
    @typing.overload
    def __mul__(self, arg0: float) -> APyFixedArray: ...
    def __neg__(self) -> APyFixedArray: ...
    @typing.overload
    def __radd__(self, arg0: int) -> APyFixedArray: ...
    @typing.overload
    def __radd__(self, arg0: float) -> APyFixedArray: ...
    @typing.overload
    def __radd__(self, arg0: APyFixed) -> APyFixedArray: ...
    def __repr__(self) -> str: ...
    @typing.overload
    def __rmul__(self, arg0: APyFixed) -> APyFixedArray: ...
    @typing.overload
    def __rmul__(self, arg0: int) -> APyFixedArray: ...
    @typing.overload
    def __rmul__(self, arg0: float) -> APyFixedArray: ...
    def __rshift__(self, shift_amnt: int) -> APyFixedArray: ...
    @typing.overload
    def __sub__(self, arg0: APyFixedArray) -> APyFixedArray: ...
    @typing.overload
    def __sub__(self, arg0: int) -> APyFixedArray: ...
    @typing.overload
    def __sub__(self, arg0: float) -> APyFixedArray: ...
    @typing.overload
    def __sub__(self, arg0: APyFixed) -> APyFixedArray: ...
    @typing.overload
    def __truediv__(self, arg0: APyFixedArray) -> APyFixedArray: ...
    # @typing.overload
    # def __truediv__(self, arg0: int) -> APyFixedArray: ...
    # @typing.overload
    # def __truediv__(self, arg0: float) -> APyFixedArray: ...
    @typing.overload
    def __truediv__(self, arg0: APyFixed) -> APyFixedArray: ...
    @typing.overload
    def __rtruediv__(self, arg0: APyFixed) -> APyFixedArray: ...
    def cast(
        self,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        quantization: QuantizationMode | None = None,
        overflow: OverflowMode | None = None,
        bits: int | None = None,
    ) -> APyFixedArray:
        """
        Create a new resized fixed-point array based on the bit pattern in this
        fixed-point array.

        This is the primary method for performing quantization, truncation, overflowing,
        and saturation when dealing with APyTypes fixed-point numbers.

        Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must
        be set.

        Parameters
        ----------
        int_bits : int, optional
            Number of integer bits in the created fixed-point array-
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point array.
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use in this cast.
        overflow : :class:`OverflowMode`, optional
            Overflowing mode to use in this cast.
        bits : int, optional
            Total number of bits in the created fixed-point array.

        Returns
        -------
        :class:`APyFixedArray`
        """

    def broadcast_to(self, shape: tuple | int) -> APyFixedArray: ...
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

    def to_numpy(self) -> numpy.ndarray[numpy.float64]:
        """
        Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

        The returned array has the same `shape` and stored value as `self`. This
        method rounds away from infinity on ties.

        Returns
        -------
        :class:`numpy.ndarray`
        """

    def transpose(self) -> APyFixedArray:
        """
        Return the transposition of the array.

        If the dimension of `self` is one, this method returns the a copy of `self`.
        If the dimension of `self` is two, this method returns the matrix
        transposition of `self`.

        Higher order transposition has not been implemented and will raise a
        `NotImplementedException`.

        Returns
        -------
        :class:`APyFixedArray`
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

    @property
    def ndim(self) -> int:
        """
        Number of dimensions in the array.

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

class APyFixedArrayIterator:
    def __iter__(self) -> APyFixedArrayIterator: ...
    def __next__(self) -> typing.Union[APyFixedArray, APyFixed]: ...

class APyFloat:
    """

    Class for configurable floating-point formats.
    """

    __hash__: typing.ClassVar[None] = None
    @staticmethod
    def from_bits(
        bits: int, exp_bits: int, man_bits: int, bias: int | None = None
    ) -> APyFloat: ...
    @staticmethod
    def from_float(
        value: float,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
        quantization: QuantizationMode | None = None,
    ) -> APyFloat:
        """
        Create an :class:`APyFloat` object from a :class:`float`.

        Parameters
        ----------
        float_sequence : float
            Floating-point value to initialize from.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : int, optional
            Bias.
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.

        Returns
        -------
        :class:`APyFloat`

        Examples
        --------

        .. code-block:: python

            from apytypes import APyFloat

            # `a`, initialized from floating-point values.
            a = APyFloat.from_float(1.35, exp_bits=10, man_bits=15)
        """

    def __abs__(self) -> APyFloat: ...
    @typing.overload
    def __add__(self, arg0: APyFloat) -> APyFloat: ...
    @typing.overload
    def __add__(self, arg0: int) -> APyFloat: ...
    @typing.overload
    def __add__(self, arg0: float) -> APyFloat: ...
    def __and__(self, arg0: APyFloat) -> APyFloat: ...
    @typing.overload
    def __eq__(self, arg0: APyFloat) -> bool: ...
    @typing.overload
    def __eq__(self, arg0: float) -> bool: ...
    def __float__(self) -> float: ...
    @typing.overload
    def __ge__(self, arg0: APyFloat) -> bool: ...
    @typing.overload
    def __ge__(self, arg0: float) -> bool: ...
    @typing.overload
    def __gt__(self, arg0: APyFloat) -> bool: ...
    @typing.overload
    def __gt__(self, arg0: float) -> bool: ...
    def __init__(
        self,
        sign: bool,
        exp: int,
        man: int,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
    ) -> None: ...
    def __invert__(self) -> APyFloat: ...
    @typing.overload
    def __le__(self, arg0: APyFloat) -> bool: ...
    @typing.overload
    def __le__(self, arg0: float) -> bool: ...
    @typing.overload
    def __lt__(self, arg0: APyFloat) -> bool: ...
    @typing.overload
    def __lt__(self, arg0: float) -> bool: ...
    @typing.overload
    def __mul__(self, arg0: APyFloat) -> APyFloat: ...
    @typing.overload
    def __mul__(self, arg0: int) -> APyFloat: ...
    @typing.overload
    def __mul__(self, arg0: float) -> APyFloat: ...
    @typing.overload
    def __truediv__(self, arg0: APyFloat) -> APyFloat: ...
    @typing.overload
    def __truediv__(self, arg0: int) -> APyFloat: ...
    @typing.overload
    def __truediv__(self, arg0: float) -> APyFloat: ...
    @typing.overload
    def __ne__(self, arg0: APyFloat) -> bool: ...
    @typing.overload
    def __ne__(self, arg0: float) -> bool: ...
    def __neg__(self) -> APyFloat: ...
    def __or__(self, arg0: APyFloat) -> APyFloat: ...
    @typing.overload
    def __pow__(self, arg0: APyFloat) -> APyFloat: ...
    @typing.overload
    def __pow__(self, arg0: int) -> APyFloat: ...
    @typing.overload
    def __radd__(self, arg0: int) -> APyFloat: ...
    @typing.overload
    def __radd__(self, arg0: float) -> APyFloat: ...
    def __repr__(self) -> str: ...
    @typing.overload
    def __rmul__(self, arg0: int) -> APyFloat: ...
    @typing.overload
    def __rmul__(self, arg0: float) -> APyFloat: ...
    @typing.overload
    def __rsub__(self, arg0: int) -> APyFloat: ...
    @typing.overload
    def __rsub__(self, arg0: float) -> APyFloat: ...
    def __str__(self) -> str: ...
    @typing.overload
    def __sub__(self, arg0: APyFloat) -> APyFloat: ...
    @typing.overload
    def __sub__(self, arg0: int) -> APyFloat: ...
    @typing.overload
    def __sub__(self, arg0: float) -> APyFloat: ...
    def __truediv__(self, arg0: APyFloat) -> APyFloat: ...
    def __xor__(self, arg0: APyFloat) -> APyFloat: ...
    def _repr_latex_(self) -> str: ...
    def cast(
        self,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        quantization: QuantizationMode | None = None,
    ) -> APyFloat: ...
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

    def is_identical(self, other: APyFloat) -> bool:
        """
        Test if two `APyFloat` objects are identical.

        Two `APyFixed` objects are considered identical if, and only if,  they have
        the same sign, exponent, mantissa, and format.

        Returns
        -------
        :class:`bool`
        """

    def to_bits(self) -> int: ...
    def next_up(self) -> APyFloat:
        """
        Get the smallest floating-point number in the same format that compares greater.

        Returns
        -------
        :class:`APyFloat`
        """

    def next_down(self) -> APyFloat:
        """
        Get the largest floating-point number in the same format that compares less.

        Returns
        -------
        :class:`APyFloat`
        """

    @property
    def bias(self) -> int: ...
    @property
    def exp(self) -> int: ...
    @property
    def exp_bits(self) -> int: ...
    @property
    def is_finite(self) -> bool:
        """
        True if and only if x is zero, subnormal, or normal.

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_inf(self) -> bool:
        """
        True if and only if x is infinite.

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_nan(self) -> bool:
        """
        True if and only if x is NaN.

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_normal(self) -> bool:
        """
        True if and only if x is normal (not zero, subnormal, infinite, or NaN).

        Returns
        -------
        :class:`bool`
        """

    @property
    def is_subnormal(self) -> bool:
        """
        True if and only if x is normal (not zero, subnormal, infinite, or NaN).

        Returns
        -------
        :class:`bool`
        """

    @property
    def man(self) -> int: ...
    @property
    def man_bits(self) -> int: ...
    @property
    def sign(self) -> bool: ...

class APyFloatArray:
    @staticmethod
    def from_float(
        float_sequence: typing.Sequence,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
        quantization: QuantizationMode | None = None,
    ) -> APyFloatArray:
        """
        Create an :class:`APyFloatArray` object from a sequence of :class:`float`.

        Parameters
        ----------
        float_sequence : sequence of float
            Floating point values to initialize from. The tensor shape will be taken
            from the sequence shape.
        exp_bits : int
            Number of exponent bits in the created fixed-point tensor
        man_bits : int
            Number of mantissa bits in the created fixed-point tensor
        bias : int, optional
            Bias in the created fixed-point tensor
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.

        Returns
        -------
        :class:`APyFloatArray`

        Examples
        --------

        .. code-block:: python

            from apytypes import APyFloatArray

            # Array `a`, initialized from floating-point values.
            a = APyFloatArray.from_float([1.0, 1.25, 1.49], exp_bits=10, man_bits=15)

            # Array `b` (2 x 3 matrix), initialized from floating-point values.
            b = APyFloatArray.from_float(
                [
                    [1.0, 2.0, 3.0],
                    [4.0, 5.0, 6.0],
                ],
                exp_bits=5,
                man_bits=2
            )
        """

    @staticmethod
    def from_array(
        float_sequence,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
        quantization: QuantizationMode | None = None,
    ) -> APyFloatArray:
        """
        Create an :class:`APyFloatArray` object from a sequence of :class:`float`.

        Parameters
        ----------
        float_sequence : sequence of float
            Floating point values to initialize from. The tensor shape will be taken
            from the sequence shape.
        exp_bits : int
            Number of exponent bits in the created fixed-point tensor
        man_bits : int
            Number of mantissa bits in the created fixed-point tensor
        bias : int, optional
            Bias in the created fixed-point tensor
        quantization : :class:`QuantizationMode`, optional
            Quantization mode to use. If not provided, the global mode,
            see :func:`get_float_quantization_mode`, is used.

        Returns
        -------
        :class:`APyFloatArray`

        Examples
        --------

        .. code-block:: python

            from apytypes import APyFloatArray
            import numpy as np

            # Array `a`, initialized from NumPy ndarray
            a = APyFloatArray.from_array(
                np.array([
                    [1.0, 2.0, 3.0],
                    [4.0, 5.0, 6.0],
                ]),
                man_bits=10,
                exp_bits=10
            )
        """

    @typing.overload
    def __add__(self, arg0: APyFloatArray) -> APyFloatArray: ...
    @typing.overload
    def __add__(self, arg0: int) -> APyFloatArray: ...
    @typing.overload
    def __add__(self, arg0: float) -> APyFloatArray: ...
    @typing.overload
    def __add__(self, arg0: APyFloat) -> APyFloatArray: ...
    def __array__(self) -> numpy.ndarray[numpy.float64]: ...
    def __getitem__(self, idx: int) -> typing.Union[APyFloatArray, APyFloat]: ...
    def __init__(
        self,
        signs: typing.Sequence,
        exps: typing.Sequence,
        mans: typing.Sequence,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
    ) -> None: ...
    def __iter__(self) -> APyFloatArrayIterator: ...
    def __len__(self) -> int: ...
    def __matmul__(self, rhs: APyFloatArray) -> APyFloatArray: ...
    @typing.overload
    def __mul__(self, arg0: APyFloatArray) -> APyFloatArray: ...
    @typing.overload
    def __mul__(self, arg0: int) -> APyFloatArray: ...
    @typing.overload
    def __mul__(self, arg0: float) -> APyFloatArray: ...
    @typing.overload
    def __mul__(self, arg0: APyFloat) -> APyFloatArray: ...
    @typing.overload
    def __radd__(self, arg0: int) -> APyFloatArray: ...
    @typing.overload
    def __radd__(self, arg0: float) -> APyFloatArray: ...
    @typing.overload
    def __radd__(self, arg0: APyFloat) -> APyFloatArray: ...
    def __repr__(self) -> str: ...
    @typing.overload
    def __rmul__(self, arg0: int) -> APyFloatArray: ...
    @typing.overload
    def __rmul__(self, arg0: float) -> APyFloatArray: ...
    @typing.overload
    def __rmul__(self, arg0: APyFloat) -> APyFloatArray: ...
    @typing.overload
    def __sub__(self, arg0: APyFloatArray) -> APyFloatArray: ...
    @typing.overload
    def __sub__(self, arg0: int) -> APyFloatArray: ...
    @typing.overload
    def __sub__(self, arg0: float) -> APyFloatArray: ...
    @typing.overload
    def __sub__(self, arg0: APyFloat) -> APyFloatArray: ...
    @typing.overload
    def __truediv__(self, arg0: APyFloatArray) -> APyFloatArray: ...
    @typing.overload
    def __truediv__(self, arg0: int) -> APyFloatArray: ...
    @typing.overload
    def __truediv__(self, arg0: float) -> APyFloatArray: ...
    @typing.overload
    def __truediv__(self, arg0: APyFloat) -> APyFloatArray: ...
    def cast(
        self,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        quantization: QuantizationMode | None = None,
    ) -> APyFloatArray: ...
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

    def broadcast_to(self, shape: tuple | int) -> APyFloatArray: ...
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

    def to_numpy(self) -> numpy.ndarray[numpy.float64]:
        """
        Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

        The returned array has the same `shape` and stored value as `self`. This
        method rounds away from infinity on ties.

        Returns
        -------
        :class:`numpy.ndarray`
        """

    def transpose(self) -> APyFloatArray:
        """
        Return the transposition of the array.

        If the dimension of `self` is one, this method returns the a copy of `self`.
        If the dimension of `self` is two, this method returns the matrix
        transposition of `self`.

        Higher order transposition has not been implemented and will raise a
        `NotImplementedException`.

        Returns
        -------
        :class:`APyFloatArray`
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

    @property
    def bias(self) -> int:
        """
        Bias.

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
    def man_bits(self) -> int:
        """
        Number of mantissa bits.

        Returns
        -------
        :class:`int`
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
    def shape(self) -> tuple:
        """
        The shape of the array.

        Returns
        -------
        :class:`tuple` of :class:`int`
        """

class APyFloatArrayIterator:
    def __iter__(self) -> APyFloatArrayIterator: ...
    def __next__(self) -> typing.Union[APyFloatArray, APyFloat]: ...

class APyFloatAccumulatorContext(ContextManager):
    def __enter__(self: ContextManager) -> None: ...
    def __exit__(
        self: ContextManager,
        arg0: type | None,
        arg1: typing.Any | None,
        arg2: typing.Any | None,
    ) -> None: ...
    def __init__(
        self,
        *,
        exp_bits: int | None = None,
        man_bits: int | None = None,
        bias: int | None = None,
        quantization: QuantizationMode | None = None,
    ) -> None: ...

class APyFixedAccumulatorContext(ContextManager):
    def __enter__(self: ContextManager) -> None: ...
    def __exit__(
        self: ContextManager,
        arg0: type | None,
        arg1: typing.Any | None,
        arg2: typing.Any | None,
    ) -> None: ...
    def __init__(
        self,
        *,
        int_bits: int | None = None,
        frac_bits: int | None = None,
        quantization: QuantizationMode | None = None,
        overflow: OverflowMode | None = None,
        bits: int | None = None,
    ) -> None: ...

class APyFixedCastContext(ContextManager):
    def __enter__(self: ContextManager) -> None: ...
    def __exit__(
        self: ContextManager,
        arg0: type | None,
        arg1: typing.Any | None,
        arg2: typing.Any | None,
    ) -> None: ...
    def __init__(
        self,
        *,
        quantization: QuantizationMode | None = None,
        overflow: OverflowMode | None = None,
    ) -> None: ...

class ContextManager:
    pass

class OverflowMode:
    """
    Members:

      WRAP : Two's complement overflow. Remove MSBs.

      SAT : Saturate to the closest of most positive and most negative value.

      NUMERIC_STD : Remove MSBs, but keep the most significant bit. As ieee.numeric_std resize for signed.
    """

    NUMERIC_STD: typing.ClassVar[OverflowMode]  # value = <OverflowMode.NUMERIC_STD: 2>
    SAT: typing.ClassVar[OverflowMode]  # value = <OverflowMode.SAT: 1>
    WRAP: typing.ClassVar[OverflowMode]  # value = <OverflowMode.WRAP: 0>
    __members__: typing.ClassVar[
        dict[str, OverflowMode]
    ]  # value = {'WRAP': <OverflowMode.WRAP: 0>, 'SAT': <OverflowMode.SAT: 1>, 'NUMERIC_STD': <OverflowMode.NUMERIC_STD: 2>}
    def __eq__(self, other: typing.Any) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: typing.Any) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    def __str__(self) -> str: ...
    @property
    def name(self) -> str: ...
    @property
    def value(self) -> int: ...

class APyFloatQuantizationContext(ContextManager):
    def __enter__(self: ContextManager) -> None: ...
    def __exit__(
        self: ContextManager,
        arg0: type | None,
        arg1: typing.Any | None,
        arg2: typing.Any | None,
    ) -> None: ...
    def __init__(
        self, quantization: QuantizationMode, quantization_seed: int | None = None
    ) -> None: ...

class QuantizationMode:
    """
    Members:

      TRN : Truncation (remove bits, round towards negative infinity

      TRN_ZERO : Magnitude truncation (round towards zero).

      TRN_INF : Round towards positive infinity.

      RND : Round to nearest, ties towards positive infinity (standard 'round' for fixed-point).

      RND_ZERO : Round to nearest, ties toward zero.

      RND_INF : Round to nearest, ties away from zero.

      RND_MIN_INF : Round to nearest, ties toward negative infinity.

      RND_CONV : Round to nearest, ties to even.

      RND_CONV_ODD : Round to nearest, ties to odd.

      JAM : Jamming/von Neumann rounding. Set LSB to 1.

      JAM_UNBIASED : Unbiased jamming/von Neumann rounding. Set LSB to 1 unless the previous LSB and all the removed bits are 0.

      STOCH_WEIGHTED : Stochastic rounding. Probability depends on the bits to remove.

      STOCH_EQUAL : Stochastic rounding with equal probability.

      TO_NEG : Alias. Round towards negative infinity.

      TO_ZERO : Alias. Round towards zero.

      TO_POS : Alias. Round towards positive infinity.

      TIES_ZERO : Alias. Round to nearest, ties toward zero.

      TIES_AWAY : Alias. Round to nearest, ties away from zero.

      TIES_EVEN : Alias. Round to nearest, ties to even.

      TIES_ODD : Alias. Round to nearest, ties to odd.

      TIES_NEG : Alias. Round to nearest, ties towards negative infinity.

      TIES_POS : Alias. Round to nearest, ties towards positive infinity.
    """

    JAM: typing.ClassVar[QuantizationMode]  # value = <QuantizationMode.JAM: 9>
    JAM_UNBIASED: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.JAM_UNBIASED: 10>
    RND: typing.ClassVar[QuantizationMode]  # value = <QuantizationMode.RND: 3>
    RND_CONV: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.RND_CONV: 7>
    RND_CONV_ODD: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.RND_CONV_ODD: 8>
    RND_INF: typing.ClassVar[QuantizationMode]  # value = <QuantizationMode.RND_INF: 5>
    RND_MIN_INF: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.RND_MIN_INF: 6>
    RND_ZERO: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.RND_ZERO: 4>
    STOCH_EQUAL: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.STOCH_EQUAL: 12>
    STOCH_WEIGHTED: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.STOCH_WEIGHTED: 11>
    TIES_AWAY: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.RND_INF: 5>
    TIES_EVEN: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.RND_CONV: 7>
    TIES_NEG: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.RND_MIN_INF: 6>
    TIES_ODD: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.RND_CONV_ODD: 8>
    TIES_POS: typing.ClassVar[QuantizationMode]  # value = <QuantizationMode.RND: 3>
    TIES_ZERO: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.RND_ZERO: 4>
    TO_NEG: typing.ClassVar[QuantizationMode]  # value = <QuantizationMode.TRN: 0>
    TO_POS: typing.ClassVar[QuantizationMode]  # value = <QuantizationMode.TRN_INF: 1>
    TO_ZERO: typing.ClassVar[QuantizationMode]  # value = <QuantizationMode.TRN_ZERO: 2>
    TRN: typing.ClassVar[QuantizationMode]  # value = <QuantizationMode.TRN: 0>
    TRN_INF: typing.ClassVar[QuantizationMode]  # value = <QuantizationMode.TRN_INF: 1>
    TRN_MAG: typing.ClassVar[QuantizationMode]
    TRN_ZERO: typing.ClassVar[
        QuantizationMode
    ]  # value = <QuantizationMode.TRN_ZERO: 2>
    __members__: typing.ClassVar[
        dict[str, QuantizationMode]
    ]  # value = {'TRN': <QuantizationMode.TRN: 0>, 'TRN_ZERO': <QuantizationMode.TRN_ZERO: 2>, 'TRN_INF': <QuantizationMode.TRN_INF: 1>, 'RND': <QuantizationMode.RND: 3>, 'RND_ZERO': <QuantizationMode.RND_ZERO: 4>, 'RND_INF': <QuantizationMode.RND_INF: 5>, 'RND_MIN_INF': <QuantizationMode.RND_MIN_INF: 6>, 'RND_CONV': <QuantizationMode.RND_CONV: 7>, 'RND_CONV_ODD': <QuantizationMode.RND_CONV_ODD: 8>, 'JAM': <QuantizationMode.JAM: 9>, 'JAM_UNBIASED': <QuantizationMode.JAM_UNBIASED: 10>, 'STOCH_WEIGHTED': <QuantizationMode.STOCH_WEIGHTED: 11>, 'STOCH_EQUAL': <QuantizationMode.STOCH_EQUAL: 12>, 'TO_NEG': <QuantizationMode.TRN: 0>, 'TO_ZERO': <QuantizationMode.TRN_ZERO: 2>, 'TO_POS': <QuantizationMode.TRN_INF: 1>, 'TIES_ZERO': <QuantizationMode.RND_ZERO: 4>, 'TIES_AWAY': <QuantizationMode.RND_INF: 5>, 'TIES_EVEN': <QuantizationMode.RND_CONV: 7>, 'TIES_ODD': <QuantizationMode.RND_CONV_ODD: 8>, 'TIES_NEG': <QuantizationMode.RND_MIN_INF: 6>, 'TIES_POS': <QuantizationMode.RND: 3>}
    def __eq__(self, other: typing.Any) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: typing.Any) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    def __str__(self) -> str: ...
    @property
    def name(self) -> str: ...
    @property
    def value(self) -> int: ...

def get_float_quantization_mode() -> QuantizationMode: ...
def get_float_quantization_seed() -> int: ...
def set_float_quantization_mode(arg0: QuantizationMode) -> None: ...
def set_float_quantization_seed(arg0: int) -> None: ...
