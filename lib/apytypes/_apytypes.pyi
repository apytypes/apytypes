from __future__ import annotations
import typing

__all__ = [
    "APyFixed",
    "APyFixedOverflowMode",
    "APyFixedRoundingMode",
    "APyFloat",
    "ContextManager",
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
        Create an :class:`APyFixed` object and initialize its value from a
        :class:`float`.

        The initialized fixed-point value is the one closest to the
        input floating-point value, rounded away from zero on ties. Exactly two of
        the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) has to be set.

        Examples
        --------

        .. code-block:: python

            from apytypes import APyFixed

            # Fixed-point `fx_a` from float, initialized from the floating-point
            # value 1.234, rounded to 1.25 as it is the closest representable number
            fx_a = APyFixed.from_float(1.234, int_bits=2, frac_bits=2)

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
        """
    @staticmethod
    def from_string(
        string_value: str,
        bits: int | None = None,
        int_bits: int | None = None,
        base: int = 10,
        frac_bits: int | None = None,
    ) -> APyFixed: ...
    def __abs__(self) -> APyFixed: ...
    def __add__(self, arg0: APyFixed) -> APyFixed: ...
    def __eq__(self, arg0: APyFixed) -> bool: ...
    def __float__(self) -> float: ...
    def __ge__(self, arg0: APyFixed) -> bool: ...
    def __gt__(self, arg0: APyFixed) -> bool: ...
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
    def __le__(self, arg0: APyFixed) -> bool: ...
    def __lshift__(self, shift_amnt: int) -> APyFixed: ...
    def __lt__(self, arg0: APyFixed) -> bool: ...
    def __mul__(self, arg0: APyFixed) -> APyFixed: ...
    def __ne__(self, arg0: APyFixed) -> bool: ...
    @typing.overload
    def __neg__(self) -> APyFixed: ...
    @typing.overload
    def __neg__(self) -> APyFixed: ...
    def __repr__(self) -> str: ...
    def __rshift__(self, shift_amnt: int) -> APyFixed: ...
    def __str__(self, base: int = 10) -> str: ...
    def __sub__(self, arg0: APyFixed) -> APyFixed: ...
    def __truediv__(self, arg0: APyFixed) -> APyFixed: ...
    def assign(self, arg0: APyFixed) -> APyFixed: ...
    def bit_pattern_to_int(self, allow_negative_return_value: bool = False) -> int:
        """
        Retrieve the underlying bit-pattern in an :class:`int` object.

        Parameters
        ----------
        allow_negative_return_value : bool, default=False
            Allow returning a negative integer bit-pattern. See example.

        Examples
        --------

        .. code-block:: python

            from apytypes import APyFixed

            # Create fixed-point number `fx_a` of value -5.75
            fx_a = APyFixed.from_float(-5.75, int_bits=4, frac_bits=4)

            ### Returns: 164 == 0xa4 == 0b10100100
            fx_a.bit_pattern_to_int()

            ### Returns: -92 == -0x5C == -0b1011100
            fx_a.bit_pattern_to_int(allow_negative_return_value=True)

        Returns
        -------
        :class:`int`
        """
    def bit_pattern_to_string_dec(self) -> str:
        """
        Retrieve the underlying bit-pattern as a :class:`str` object formated in
        base-10.

        Returns
        -------
        :class:`str`
        """
    def increment_lsb(self) -> int: ...
    def is_identical(self, other: APyFixed) -> bool:
        """
        Test if two `APyFixed` objects are identical.

        Two `APyFixed` objects are considered identical if, and only if,  they store
        the same fixed-point value and have exactly the same bit-specification
        (`bits`, `int_bits`, and `frac_bits` are all equal). This is a more
        restrictive test than `__eq__`,  that only tests equality of the fixed-point
        value.

        Returns
        -------
        :class:`bool`
        """
    def resize(
        self,
        bits: int | None = None,
        int_bits: int | None = None,
        rounding_mode: APyFixedRoundingMode = ...,
        overflow_mode: APyFixedOverflowMode = ...,
        frac_bits: int | None = None,
    ) -> APyFixed: ...
    def set_from_float(self, arg0: float) -> None: ...
    def set_from_string(self, str: str, base: int = 10) -> None: ...
    def to_string(self, base: int = 10) -> str: ...
    def to_string_dec(self) -> str: ...
    def to_string_hex(self) -> str: ...
    def to_string_oct(self) -> str: ...
    @property
    def bits(self) -> int:
        """
        Total number of bits in this :class:`APyFixed` object.

        Returns
        -------
        :class:`int`
        """
    @property
    def frac_bits(self) -> int:
        """
        Number of fractional bits in this :class:`APyFixed` object.

        Returns
        -------
        :class:`int`
        """
    @property
    def int_bits(self) -> int:
        """
        Number of integer bits in this :class:`APyFixed` object.

        Returns
        -------
        :class:`int`
        """
    @property
    def is_negative(self) -> bool: ...
    @property
    def is_positive(self) -> bool: ...
    @property
    def is_zero(self) -> bool: ...
    @property
    def vector_size(self) -> int: ...

class APyFixedOverflowMode:
    """
    Members:

      TWOS_OVERFLOW

      SATURATE
    """

    SATURATE: typing.ClassVar[
        APyFixedOverflowMode
    ]  # value = <APyFixedOverflowMode.SATURATE: 1>
    TWOS_OVERFLOW: typing.ClassVar[
        APyFixedOverflowMode
    ]  # value = <APyFixedOverflowMode.TWOS_OVERFLOW: 0>
    __members__: typing.ClassVar[
        dict[str, APyFixedOverflowMode]
    ]  # value = {'TWOS_OVERFLOW': <APyFixedOverflowMode.TWOS_OVERFLOW: 0>, 'SATURATE': <APyFixedOverflowMode.SATURATE: 1>}
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

class APyFixedRoundingMode:
    """
    Members:

      TRN

      RND

      RND_TO_ZERO

      RND_AWAY_ZERO

      RND_CONV

      RND_CONV_ODD
    """

    RND: typing.ClassVar[APyFixedRoundingMode]  # value = <APyFixedRoundingMode.RND: 1>
    RND_AWAY_ZERO: typing.ClassVar[
        APyFixedRoundingMode
    ]  # value = <APyFixedRoundingMode.RND_AWAY_ZERO: 3>
    RND_CONV: typing.ClassVar[
        APyFixedRoundingMode
    ]  # value = <APyFixedRoundingMode.RND_CONV: 4>
    RND_CONV_ODD: typing.ClassVar[
        APyFixedRoundingMode
    ]  # value = <APyFixedRoundingMode.RND_CONV_ODD: 5>
    RND_TO_ZERO: typing.ClassVar[
        APyFixedRoundingMode
    ]  # value = <APyFixedRoundingMode.RND_TO_ZERO: 2>
    TRN: typing.ClassVar[APyFixedRoundingMode]  # value = <APyFixedRoundingMode.TRN: 0>
    __members__: typing.ClassVar[
        dict[str, APyFixedRoundingMode]
    ]  # value = {'TRN': <APyFixedRoundingMode.TRN: 0>, 'RND': <APyFixedRoundingMode.RND: 1>, 'RND_TO_ZERO': <APyFixedRoundingMode.RND_TO_ZERO: 2>, 'RND_AWAY_ZERO': <APyFixedRoundingMode.RND_AWAY_ZERO: 3>, 'RND_CONV': <APyFixedRoundingMode.RND_CONV: 4>, 'RND_CONV_ODD': <APyFixedRoundingMode.RND_CONV_ODD: 5>}
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

class APyFloat:
    """

    Class for configurable floating-point formats.
    """

    __hash__: typing.ClassVar[None] = None
    @staticmethod
    def from_bits(arg0: int, arg1: int, arg2: int) -> APyFloat: ...
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
    def __eq__(self, arg0: APyFloat) -> bool: ...
    def __float__(self) -> float: ...
    def __ge__(self, arg0: APyFloat) -> bool: ...
    def __gt__(self, arg0: APyFloat) -> bool: ...
    def __init__(
        self,
        sign: bool,
        exp: int,
        man: int,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
    ) -> None: ...
    def __le__(self, arg0: APyFloat) -> bool: ...
    def __lt__(self, arg0: APyFloat) -> bool: ...
    def __mul__(self, arg0: APyFloat) -> APyFloat: ...
    def __ne__(self, arg0: APyFloat) -> bool: ...
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
    def cast_to(
        self,
        exp_bits: int,
        man_bits: int,
        bias: int | None = None,
        rounding_mode: RoundingMode | None = None,
    ) -> APyFloat: ...
    def pretty_string(self) -> str: ...
    def to_bits(self) -> int: ...
    def to_float(self) -> float: ...
    def update_from_bits(self, arg0: int) -> APyFloat: ...
    def update_from_float(
        self, value: float, rounding_mode: RoundingMode | None = None
    ) -> APyFloat: ...
    @property
    def exp(self) -> int: ...
    @property
    def exp_bits(self) -> int: ...
    @property
    def is_finite(self) -> bool: ...
    @property
    def is_inf(self) -> bool: ...
    @property
    def is_nan(self) -> bool: ...
    @property
    def is_normal(self) -> bool: ...
    @property
    def is_sign_neg(self) -> bool: ...
    @property
    def is_subnormal(self) -> bool: ...
    @property
    def is_zero(self) -> bool: ...
    @property
    def man(self) -> int: ...
    @property
    def man_bits(self) -> int: ...
    @property
    def sign(self) -> bool: ...

class ContextManager:
    pass

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

      TO_POSITIVE

      TO_NEGATIVE

      TO_ZERO

      TIES_TO_EVEN

      TIES_TO_AWAY

      JAMMING
    """

    JAMMING: typing.ClassVar[RoundingMode]  # value = <RoundingMode.JAMMING: 5>
    TIES_TO_AWAY: typing.ClassVar[
        RoundingMode
    ]  # value = <RoundingMode.TIES_TO_AWAY: 4>
    TIES_TO_EVEN: typing.ClassVar[
        RoundingMode
    ]  # value = <RoundingMode.TIES_TO_EVEN: 3>
    TO_NEGATIVE: typing.ClassVar[RoundingMode]  # value = <RoundingMode.TO_NEGATIVE: 1>
    TO_POSITIVE: typing.ClassVar[RoundingMode]  # value = <RoundingMode.TO_POSITIVE: 0>
    TO_ZERO: typing.ClassVar[RoundingMode]  # value = <RoundingMode.TO_ZERO: 2>
    __members__: typing.ClassVar[
        dict[str, RoundingMode]
    ]  # value = {'TO_POSITIVE': <RoundingMode.TO_POSITIVE: 0>, 'TO_NEGATIVE': <RoundingMode.TO_NEGATIVE: 1>, 'TO_ZERO': <RoundingMode.TO_ZERO: 2>, 'TIES_TO_EVEN': <RoundingMode.TIES_TO_EVEN: 3>, 'TIES_TO_AWAY': <RoundingMode.TIES_TO_AWAY: 4>, 'JAMMING': <RoundingMode.JAMMING: 5>}
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

__version__: str = "0.0.1"
