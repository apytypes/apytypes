from __future__ import annotations
import numpy
import typing

__all__ = [
    "APyFixed",
    "APyFixedArray",
    "APyFixedArrayIterator",
    "APyFloat",
    "ContextManager",
    "OverflowMode",
    "RoundingContext",
    "RoundingMode",
    "get_rounding_mode",
    "set_rounding_mode",
]

class APyFixed:
    """

    Class for configurable fixed-point formats.
    """

    __hash__: typing.ClassVar[None] = None
    @staticmethod
    def from_float(
        value: float,
        bits: int | None = None,
        int_bits: int | None = None,
        frac_bits: int | None = None,
    ) -> APyFixed:
        """
        Create an :class:`APyFixed` object from :class:`float`.

        The initialized fixed-point value is the one closest to the
        input floating-point value, rounded away from zero on ties. Exactly two of
        the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) needs to be set.

        Exactly two of three bit-specifiers (*bits*, *int_bits*, *frac_bits*) needs
        to be set.

        Parameters
        ----------
        value : float
            Floating point value to initialize from
        bits : int, optional
            Total number of bits in the created fixed-point object
        int_bits : int, optional
            Number of integer bits in the created fixed-point object
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point object

        Examples
        --------
        .. code-block:: python

            from apytypes import APyFixed

            # Fixed-point `fx_a`, initialized from the floating-point value 1.234,
            # rounded to 1.25 as it is the closest representable number
            fx_a = APyFixed.from_float(1.234, int_bits=2, frac_bits=2)
        """

    @staticmethod
    def from_str(
        string_value: str,
        bits: int | None = None,
        int_bits: int | None = None,
        base: int = 10,
        frac_bits: int | None = None,
    ) -> APyFixed:
        """
        Create an :class:`APyFixed` object from :class:`str`.

        Exactly two of three bit-specifiers (*bits*, *int_bits*, *frac_bits*) needs
        to be set.

        Parameters
        ----------
        string_value : str
            String to initialize the value from
        bits : int, optional
            Total number of bits in the created fixed-point object
        int_bits : int, optional
            Number of integer bits in the created fixed-point object
        base : int, default: 10
            Numeric base used in `string_value`
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point object

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
    def __float__(self) -> float: ...
    @typing.overload
    def __ge__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __ge__(self, arg0: float) -> bool: ...
    @typing.overload
    def __gt__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __gt__(self, arg0: float) -> bool: ...
    @typing.overload
    def __init__(
        self,
        other: APyFixed,
        bits: int | None = None,
        int_bits: int | None = None,
        frac_bits: int | None = None,
    ) -> None: ...
    @typing.overload
    def __init__(
        self,
        bit_pattern: int,
        bits: int | None = None,
        int_bits: int | None = None,
        frac_bits: int | None = None,
    ) -> None: ...
    @typing.overload
    def __le__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __le__(self, arg0: float) -> bool: ...
    def __lshift__(self, shift_amnt: int) -> APyFixed: ...
    @typing.overload
    def __lt__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __lt__(self, arg0: float) -> bool: ...
    @typing.overload
    def __mul__(self, arg0: APyFixed) -> APyFixed: ...
    @typing.overload
    def __mul__(self, arg0: int) -> APyFixed: ...
    @typing.overload
    def __ne__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __ne__(self, arg0: float) -> bool: ...
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
    def is_identical(self, other: APyFixed) -> bool:
        """
        Test if two fixed-point objects are exactly identical.

        Two `APyFixed` objects are considered exactly identical if, and only if,
        they store the same fixed-point value, and have the exact same
        bit-specification (*bits*, *int_bits*, and *frac_bits*). This is a more
        restrictive test than ``==``,  that only tests equality of the stored
        fixed-point value.

        Paramters
        ---------
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

    def resize(
        self,
        bits: int | None = None,
        int_bits: int | None = None,
        rounding: RoundingMode = ...,
        overflow: OverflowMode = ...,
        frac_bits: int | None = None,
    ) -> APyFixed:
        """
        Create a new resized fixed-point number based on the bit pattern in this
        fixed-point number.

        This is the primary method for performing rounding, truncation, overflowing,
        and saturation when dealing with APyTypes fixed-point numbers.

        Exactly two of three bit-specifiers (*bits*, *int_bits*, *frac_bits*) needs
        to be set.

        Parameters
        ----------
        bits : int, optional
            Total number of bits in the created fixed-point object
        int_bits : int, optional
            Number of integer bits in the created fixed-point object
        rounding : RoundingMode, default: RoundingMode.TRN
            Rounding mode to use in this resize
        overflow : OverflowMode, default: OverflowMode.WRAP
            Overflowing mode to use in this resize
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point object

        Returns
        -------
        :class:`APyFixed`

        Examples
        --------
        .. code-block:: python

            from apytypes import APyFixed
            from apytypes import RoundingMode
            from apytypes import OverflowMode

            fx = APyFixed.from_float(2.125, int_bits=3, frac_bits=3)

            # Truncation (fx_a == 2.0)
            fx_a = fx.resize(int_bits=3, frac_bits=2, rounding=RoundingMode.TRN)

            # Rounding (fx_b == 2.25)
            fx_b = fx.resize(int_bits=3, frac_bits=2, rounding=RoundingMode.RND)

            # Two's complement overflowing (fx_c == -1.875)
            fx_c = fx.resize(int_bits=2, frac_bits=3, overflow=OverflowMode.WRAP)
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
        bits: int | None = None,
        int_bits: int | None = None,
        frac_bits: int | None = None,
    ) -> APyFixedArray:
        """
        Create an :class:`APyFixedArray` object from a sequence of :class:`float`.

        The initialized fixed-point values are the one closest to the
        input floating-point value, rounded away from zero on ties. Exactly two of
        the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) has to be set.

        Parameters
        ----------
        float_sequence : sequence of float
            Floating point values to initialize from. The tensor shape will be taken
            from the sequence shape.
        bits : int, optional
            Total number of bits in the created fixed-point tensor
        int_bits : int, optional
            Number of integer bits in the created fixed-point tensor
        frac_bits : int, optional
            Number of fractional bits in the created fixed-point tensor

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

    def __add__(self, arg0: APyFixedArray) -> APyFixedArray: ...
    def __array__(self) -> numpy.ndarray[numpy.float64]: ...
    def __getitem__(self, idx: int) -> APyFixedArray: ...
    def __init__(
        self,
        bit_pattern_sequence: typing.Sequence,
        bits: int | None = None,
        int_bits: int | None = None,
        frac_bits: int | None = None,
    ) -> None: ...
    def __iter__(self) -> APyFixedArrayIterator: ...
    def __len__(self) -> int: ...
    def __lshift__(self, shift_amnt: int) -> APyFixedArray: ...
    def __matmul__(self, rhs: APyFixedArray) -> APyFixedArray: ...
    def __mul__(self, arg0: APyFixedArray) -> APyFixedArray: ...
    def __repr__(self) -> str: ...
    def __rshift__(self, shift_amnt: int) -> APyFixedArray: ...
    def __sub__(self, arg0: APyFixedArray) -> APyFixedArray: ...
    def is_identical(self, other: APyFixedArray) -> bool:
        """
        Test if two :class:`APyFixedArray` objects are identical.

        Two :class:`APyFixedArray` objects are considered identical if, and only if:
          * They represent exatly the same tensor shape
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
    def __next__(self) -> APyFixedArray: ...

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
        rounding_mode: RoundingMode | None = None,
    ) -> APyFloat: ...
    def __abs__(self) -> APyFloat: ...
    def __add__(self, arg0: APyFloat) -> APyFloat: ...
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
    @typing.overload
    def __le__(self, arg0: APyFloat) -> bool: ...
    @typing.overload
    def __le__(self, arg0: float) -> bool: ...
    @typing.overload
    def __lt__(self, arg0: APyFloat) -> bool: ...
    @typing.overload
    def __lt__(self, arg0: float) -> bool: ...
    def __mul__(self, arg0: APyFloat) -> APyFloat: ...
    @typing.overload
    def __ne__(self, arg0: APyFloat) -> bool: ...
    @typing.overload
    def __ne__(self, arg0: float) -> bool: ...
    def __neg__(self) -> APyFloat: ...
    @typing.overload
    def __pow__(self, arg0: APyFloat) -> APyFloat: ...
    @typing.overload
    def __pow__(self, arg0: int) -> APyFloat: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    def __sub__(self, arg0: APyFloat) -> APyFloat: ...
    def __truediv__(self, arg0: APyFloat) -> APyFloat: ...
    def _repr_latex_(self) -> str: ...
    def is_identical(self, other: APyFloat) -> bool:
        """
        Test if two `APyFloat` objects are identical.

        Two `APyFixed` objects are considered identical if, and only if,  they have
        the same sign, exponent, mantissa, and format.

        Returns
        -------
        :class:`bool`
        """

    def pretty_string(self) -> str: ...
    def resize(
        self,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
        rounding_mode: RoundingMode | None = None,
    ) -> APyFloat: ...
    def to_bits(self) -> int: ...
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

class ContextManager:
    pass

class OverflowMode:
    """
    Members:

      WRAP

      SAT
    """

    SAT: typing.ClassVar[OverflowMode]  # value = <OverflowMode.SAT: 1>
    WRAP: typing.ClassVar[OverflowMode]  # value = <OverflowMode.WRAP: 0>
    __members__: typing.ClassVar[
        dict[str, OverflowMode]
    ]  # value = {'WRAP': <OverflowMode.WRAP: 0>, 'SAT': <OverflowMode.SAT: 1>}
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

class RoundingContext(ContextManager):
    def __enter__(self: ContextManager) -> None: ...
    def __exit__(
        self: ContextManager,
        arg0: type | None,
        arg1: typing.Any | None,
        arg2: typing.Any | None,
    ) -> None: ...
    def __init__(self, rounding_mode: RoundingMode) -> None: ...

class RoundingMode:
    """
    Members:

      TRN

      TRN_ZERO

      TRN_INF

      RND

      RND_ZERO

      RND_INF

      RND_MIN_INF

      RND_CONV

      RND_CONV_ODD

      JAM

      STOCHASTIC_WEIGHTED

      STOCHASTIC_EQUAL

      TO_NEG

      TO_ZERO

      TO_POS

      TIES_ZERO

      TIES_AWAY

      TIES_EVEN

      TIES_ODD
    """

    JAM: typing.ClassVar[RoundingMode]  # value = <RoundingMode.JAM: 9>
    RND: typing.ClassVar[RoundingMode]  # value = <RoundingMode.RND: 3>
    RND_CONV: typing.ClassVar[RoundingMode]  # value = <RoundingMode.RND_CONV: 7>
    RND_CONV_ODD: typing.ClassVar[
        RoundingMode
    ]  # value = <RoundingMode.RND_CONV_ODD: 8>
    RND_INF: typing.ClassVar[RoundingMode]  # value = <RoundingMode.RND_INF: 5>
    RND_MIN_INF: typing.ClassVar[RoundingMode]  # value = <RoundingMode.RND_MIN_INF: 6>
    RND_ZERO: typing.ClassVar[RoundingMode]  # value = <RoundingMode.RND_ZERO: 4>
    STOCHASTIC_EQUAL: typing.ClassVar[
        RoundingMode
    ]  # value = <RoundingMode.STOCHASTIC_EQUAL: 11>
    STOCHASTIC_WEIGHTED: typing.ClassVar[
        RoundingMode
    ]  # value = <RoundingMode.STOCHASTIC_WEIGHTED: 10>
    TIES_AWAY: typing.ClassVar[RoundingMode]  # value = <RoundingMode.RND_INF: 5>
    TIES_EVEN: typing.ClassVar[RoundingMode]  # value = <RoundingMode.RND_CONV: 7>
    TIES_ODD: typing.ClassVar[RoundingMode]  # value = <RoundingMode.RND_CONV_ODD: 8>
    TIES_ZERO: typing.ClassVar[RoundingMode]  # value = <RoundingMode.RND_ZERO: 4>
    TO_NEG: typing.ClassVar[RoundingMode]  # value = <RoundingMode.TRN: 0>
    TO_POS: typing.ClassVar[RoundingMode]  # value = <RoundingMode.TRN_INF: 1>
    TO_ZERO: typing.ClassVar[RoundingMode]  # value = <RoundingMode.TRN_ZERO: 2>
    TRN: typing.ClassVar[RoundingMode]  # value = <RoundingMode.TRN: 0>
    TRN_INF: typing.ClassVar[RoundingMode]  # value = <RoundingMode.TRN_INF: 1>
    TRN_ZERO: typing.ClassVar[RoundingMode]  # value = <RoundingMode.TRN_ZERO: 2>
    __members__: typing.ClassVar[
        dict[str, RoundingMode]
    ]  # value = {'TRN': <RoundingMode.TRN: 0>, 'TRN_ZERO': <RoundingMode.TRN_ZERO: 2>, 'TRN_INF': <RoundingMode.TRN_INF: 1>, 'RND': <RoundingMode.RND: 3>, 'RND_ZERO': <RoundingMode.RND_ZERO: 4>, 'RND_INF': <RoundingMode.RND_INF: 5>, 'RND_MIN_INF': <RoundingMode.RND_MIN_INF: 6>, 'RND_CONV': <RoundingMode.RND_CONV: 7>, 'RND_CONV_ODD': <RoundingMode.RND_CONV_ODD: 8>, 'JAM': <RoundingMode.JAM: 9>, 'STOCHASTIC_WEIGHTED': <RoundingMode.STOCHASTIC_WEIGHTED: 10>, 'STOCHASTIC_EQUAL': <RoundingMode.STOCHASTIC_EQUAL: 11>, 'TO_NEG': <RoundingMode.TRN: 0>, 'TO_ZERO': <RoundingMode.TRN_ZERO: 2>, 'TO_POS': <RoundingMode.TRN_INF: 1>, 'TIES_ZERO': <RoundingMode.RND_ZERO: 4>, 'TIES_AWAY': <RoundingMode.RND_INF: 5>, 'TIES_EVEN': <RoundingMode.RND_CONV: 7>, 'TIES_ODD': <RoundingMode.RND_CONV_ODD: 8>}
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

@typing.overload
def get_rounding_mode() -> RoundingMode: ...
@typing.overload
def get_rounding_mode() -> RoundingMode: ...
@typing.overload
def set_rounding_mode(arg0: RoundingMode) -> None: ...
@typing.overload
def set_rounding_mode(arg0: RoundingMode) -> None: ...
