import math

import pytest

from apytypes import APyCFixed, APyFixed


@pytest.mark.parametrize("apyfixed", [APyFixed, APyCFixed])
def test_arithmetic_operations(apyfixed: type[APyCFixed]):
    a = apyfixed.from_float(-1.25, int_bits=2, frac_bits=4)
    b = apyfixed.from_float(0.75, int_bits=2, frac_bits=3)

    cb = apyfixed == APyCFixed  # Complex multiplication result in one extra `int_bit`
    assert (a * b).is_identical(
        apyfixed.from_float(-0.9375, int_bits=4 + cb, frac_bits=7)
    )
    assert not (
        (a * b).is_identical(apyfixed.from_float(-0.9375, int_bits=4 + cb, frac_bits=5))
    )
    assert (a + b).is_identical(apyfixed.from_float(-0.5, int_bits=3, frac_bits=4))
    assert (b + a).is_identical(apyfixed.from_float(-0.5, int_bits=3, frac_bits=4))
    assert (a - b).is_identical(apyfixed.from_float(-2, int_bits=3, frac_bits=4))
    assert (b - a).is_identical(apyfixed.from_float(2, int_bits=3, frac_bits=4))
    assert (a / b).is_identical(apyfixed.from_float(-1.65625, int_bits=6, frac_bits=6))
    assert (b / a).is_identical(apyfixed.from_float(-0.59375, int_bits=7, frac_bits=5))

    # Division-by-zero throws a ZeroDivisionError
    with pytest.raises(ZeroDivisionError, match="fixed-point division by zero"):
        _ = apyfixed.from_float(1.0, 10, 10) / apyfixed.from_float(0.0, 10, 10)


@pytest.mark.parametrize("apyfixed", [APyFixed, APyCFixed])
def test_overflow_twos_complement(apyfixed: type[APyCFixed]):
    assert apyfixed(1 << 127, 1, 127) == -1.0
    assert apyfixed(1 << 126, 1, 127) == 0.5
    assert apyfixed(0xFFFFFFFF0 << 92, 1, 95) == 0.0
    assert apyfixed(0xFFFFFFFF8 << 92, 1, 95) == -1.0
    assert apyfixed(0xFFFFFFFF4 << 92, 1, 95) == 0.5


@pytest.mark.parametrize("apyfixed", [APyFixed, APyCFixed])
def test_binary_comparison_orderless(apyfixed: type[APyCFixed]):
    assert apyfixed.from_float(0.0, 256, 128) != apyfixed.from_float(1.0, 256, 128)
    assert apyfixed.from_float(0.0, 256, 128) != apyfixed.from_float(1.0, 256, 128)
    assert apyfixed.from_float(1.0, 256, 128) == apyfixed.from_float(1.0, 256, 128)
    assert apyfixed.from_float(1.0, 256, 128) == apyfixed.from_float(1.0, 140, 128)
    assert apyfixed.from_float(-1.0, 256, 128) != apyfixed.from_float(-3.0, 140, 128)
    assert apyfixed.from_float(-1.0, 256, 128) != apyfixed.from_float(-3.0, 256, 128)


def test_binary_comparison_total_order():
    assert APyFixed.from_float(0.0, 256, 128) < APyFixed.from_float(1.0, 256, 128)
    assert APyFixed.from_float(0.0, 256, 128) <= APyFixed.from_float(1.0, 256, 128)
    assert not (APyFixed.from_float(0.0, 256, 128) > APyFixed.from_float(1.0, 256, 128))
    assert not (
        APyFixed.from_float(0.0, 256, 128) >= APyFixed.from_float(1.0, 256, 128)
    )

    assert not (APyFixed.from_float(1.0, 256, 128) < APyFixed.from_float(1.0, 140, 128))
    assert APyFixed.from_float(1.0, 256, 128) <= APyFixed.from_float(1.0, 256, 128)
    assert not (APyFixed.from_float(1.0, 256, 128) > APyFixed.from_float(1.0, 256, 128))
    assert APyFixed.from_float(1.0, 256, 128) >= APyFixed.from_float(1.0, 256, 128)

    assert not (
        APyFixed.from_float(-1.0, 256, 128) < APyFixed.from_float(-3.0, 140, 128)
    )
    assert not (
        APyFixed.from_float(-1.0, 256, 128) <= APyFixed.from_float(-3.0, 256, 128)
    )
    assert APyFixed.from_float(-1.0, 256, 128) > APyFixed.from_float(-3.0, 256, 128)
    assert APyFixed.from_float(-1.0, 256, 128) >= APyFixed.from_float(-3.0, 256, 128)


@pytest.mark.parametrize("py_type", [int, float])
@pytest.mark.parametrize("apyfixed", [APyFixed, APyCFixed])
def test_float_and_int_comparison_orderless(
    py_type: type[float], apyfixed: type[APyCFixed]
):
    assert apyfixed.from_float(0.0, 256, 128) != py_type(1.0)
    assert apyfixed.from_float(0.0, 256, 128) != py_type(1.0)

    assert apyfixed.from_float(1.0, 256, 128) == py_type(1.0)
    assert apyfixed.from_float(1.0, 256, 128) == py_type(1.0)

    assert apyfixed.from_float(-1.0, 256, 128) != py_type(-3.0)
    assert apyfixed.from_float(-1.0, 256, 128) != py_type(-3.0)


@pytest.mark.parametrize("py_type", [int, float])
def test_float_and_int_comparison_total_order(py_type: type[float]):
    assert not APyFixed.from_float(1.0, 256, 128) < py_type(1.0)
    assert APyFixed.from_float(1.0, 256, 128) <= py_type(1.0)
    assert not APyFixed.from_float(1.0, 256, 128) > py_type(1.0)
    assert APyFixed.from_float(1.0, 256, 128) >= py_type(1.0)

    assert not APyFixed.from_float(-1.0, 256, 128) < py_type(-3.0)
    assert not APyFixed.from_float(-1.0, 256, 128) <= py_type(-3.0)
    assert APyFixed.from_float(-1.0, 256, 128) > py_type(-3.0)
    assert APyFixed.from_float(-1.0, 256, 128) >= py_type(-3.0)

    assert APyFixed.from_float(0.0, 256, 128) < py_type(1.0)
    assert APyFixed.from_float(0.0, 256, 128) <= py_type(1.0)
    assert not APyFixed.from_float(0.0, 256, 128) > py_type(1.0)
    assert not APyFixed.from_float(0.0, 256, 128) >= py_type(1.0)


@pytest.mark.parametrize("apyfixed", [APyFixed, APyCFixed])
def test_narrow_add_sub(apyfixed: type[APyCFixed]):
    for a_bits in range(10, 30):
        for b_bits in range(10, 30):
            a = apyfixed.from_float(a_bits + b_bits, bits=a_bits + 20, int_bits=b_bits)
            b = apyfixed.from_float(a_bits + b_bits, bits=b_bits + 20, int_bits=a_bits)
            assert a + b == float(2 * (a_bits + b_bits))
            assert a - b == 0.0
            assert a * b == float(a_bits + b_bits) ** 2


def test_wide_operations():
    """
    Tests for wider additions and subtractions
    """
    fx_a = APyFixed.from_str(
        "-1376018206341311063223476816643087998331620501540496640."
        "021222579872958058370179355618716816066859017361262100333952697594702"
        "314679773970519809467311447652539955943903993200932791396783892142688"
        "708904952458654442554723081083186210082207584128592922850820472478833"
        "257136662269306798708182072507551281664490003441493733349403017982015"
        "56238154807942919433116912841796875",
        bits=511,
        int_bits=199,
    )
    fx_b = APyFixed.from_str(
        "104491155816182282723870707435."
        "071309157022941811996487335267222452817725256730408336761400307905788"
        "797456145543984209859839960406624306885282563866331997053887403535237"
        "669472286295439923033245419525969513108528001363095759209857885470986"
        "321274651839001942356532319797348447878578921637577688261230250110887"
        "656142652390037359273932290457150614714630623155135326791655321661502"
        "949476980808972834127647429103280529716357705183327198028564453125",
        bits=512,
        int_bits=100,
    )
    assert (
        str(fx_a + fx_b) == "-1376018206341311063223476712151932182149337777669789204."
        "94991342285001624637369202035149436324913376063085376357255238"
        "96889135172236284265355996074714872459156490586214293346007943"
        "42896488607451039432666163214519521477661557216696973679582765"
        "49716364096258700784693586201043030485635164975271020283378591"
        "10818039160450881727678711279062388956893918350572368379608181"
        "35285369376844864673208344678338497050523019191027165872352570"
        "896719470283642294816672801971435546875"
    )
    assert (
        str(fx_a - fx_b) == "-1376018206341311063223476921134243814513903225411204075."
        "09253173689589987036666669088593926888458427409167043709535300"
        "55004911121359195145040193271514080591642628291865570672647884"
        "50671295677926378377238754094365587968500609155723190735585491"
        "68868206067835794981957841131410830874106471439230489972954306"
        "89250790714216106332680929032185242004694665536051014188751193"
        "64714630623155135326791655321661502949476980808972834127647429"
        "103280529716357705183327198028564453125"
    )
    assert (
        str(fx_a * fx_b)
        == "-1437817328047135979317565226081658369934728352937007440600368"
        "47481874028068127844759.01659566888646037518662317884199296936"
        "28483134535710292243547028314904637179690733926430448803482084"
        "46455087857738493876686848561681809833044214776451743329705854"
        "29587654287681017290234520733790039243890627996035181848138554"
        "29678077937962242344684590853953607746789943470122746519369628"
        "32344031929334376765299139295185206945982946367607665831151169"
        "44541828740064163906154927042372656817385972025622394107469118"
        "84176480204439746310162378439571995061522430195782890700451420"
        "28345404026673865347600681259158824811586579251365902015572046"
        "44178326642106181774922819107094912745504703336956308309210889"
        "57927864419452778720714254557659189863681870313756497919898053"
        "09594168141953736704324526285425633886916330084204673767089843"
        "75"
    )
    assert (
        str(fx_a / fx_b)
        == "-13168752853705256664641460.154699927299868178494673213020163268029024396564097348867620536148608051377939374405769258389374582601824578761720551679290918141817518103067961101475530661088410553732230720431121814264170962540309744289867699231599682791612131882266885621220168075281855455166215222327932319205085175828486942171835130524287413875373672121755959467487090622541658023790962843835921302314273528821786385378800332546234130859375"
    )
    assert (
        str(fx_b / fx_a)
        == "-0.00000000000000000000000007593733522902532737164561219082471162117941053179830993334762476346448047724916444178110980891382351234853711842276403919205100103319973748441469938356153516676552484840432730125299732215457184330985179287340274676023884036498932468730856406230716140700632956188930761042015410081479704273150082273947051231565176551724963700473737910501716104810029975554599602486522797342070346268541033706171502414177198326705362450397865853514406205241004900193695723977004399771561843697392012158912383372951201123526755685207014841161173565699664966764637595575404560577226220630109310150146484375"
    )
    assert (fx_a / fx_b).is_identical(
        APyFixed(
            179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377628608902058037607122737562380431202427980764679928997651726279044249830618985901641828817582211479134659343523646971962599873836423873396413918755082,
            bits=1024,
            int_bits=612,
        )
    )
    assert (fx_b / fx_a).is_identical(
        APyFixed(
            179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430638828793116753517593272714700119155884513163112591979992022276398946432130363171884903235021859936193161365280782561200379382710821175400477141530990327829075421,
            bits=1024,
            int_bits=413,
        )
    )

    assert fx_a + fx_b == fx_b + fx_a
    assert fx_a - fx_b == -(fx_b - fx_a)
    assert fx_a * fx_b == fx_b * fx_a


def test_wide_operations_special_cases():
    """
    Tests for wider operations to try to trigger specific branches of the code.
    """
    fx_b = APyFixed(5444517870735015415395546974834581831681, bits=136, frac_bits=75)
    # Two most significant lambs are the same as fx_b
    fx_c = APyFixed(5444517870735015415395546974834581831680, bits=136, frac_bits=75)
    assert (fx_c / fx_b).is_identical(
        APyFixed(87112285931760246646623899502532662132719, bits=273, int_bits=137)
    )
    assert (fx_b / fx_c).is_identical(
        APyFixed(87112285931760246646623899502532662132752, bits=273, int_bits=137)
    )

    # The same as the 3/2 inverse of fx_c
    fx_a = APyFixed(340282366920938463462221685927161364480, bits=136, frac_bits=75)
    assert (fx_a / fx_c).is_identical(
        APyFixed(5444517870735015415413993718908291383296, bits=273, int_bits=137)
    )


@pytest.mark.parametrize("apyfixed", [APyFixed, APyCFixed])
def test_mixed_width_operations(apyfixed: type[APyCFixed]):
    """
    Tests for mixed-width operations additions and subtractions
    """
    fx_a = apyfixed(1779, int_bits=8, frac_bits=8)
    fx_b = apyfixed(17777779, int_bits=80, frac_bits=20)
    assert (fx_a + fx_b).is_identical(apyfixed(25064563, bits=101, int_bits=81))
    assert (fx_b + fx_a).is_identical(fx_a + fx_b)

    cb = apyfixed == APyCFixed  # Complex multiplication result in one extra `int_bit`
    assert (fx_a * fx_b).is_identical(
        apyfixed(31626668841, bits=116 + cb, int_bits=88 + cb)
    )
    assert (fx_b * fx_a).is_identical(fx_a * fx_b)

    assert (fx_a / fx_b).is_identical(
        apyfixed(126852202280499724136667184, bits=117, int_bits=29)
    )
    assert (fx_b / fx_a).is_identical(apyfixed(654909794, bits=117, int_bits=89))
    assert (fx_a / fx_a).is_identical(apyfixed(65536, bits=33, int_bits=17))
    assert (fx_b / fx_b).is_identical(
        apyfixed(1267650600228229401496703205376, bits=201, int_bits=101)
    )


@pytest.mark.parametrize("apyfixed", [APyFixed, APyCFixed])
def test_negative_fractional_bits_operations(apyfixed: type[APyCFixed]):
    """
    Tests for mixed-width operations additions and subtractions
    """
    fx_a = apyfixed(1779, int_bits=20, frac_bits=-4)
    fx_b = apyfixed(1779, int_bits=30, frac_bits=-14)
    s = apyfixed(1823475, bits=27, int_bits=31)
    assert (fx_a + fx_b).is_identical(s)
    assert (fx_b + fx_a).is_identical(s)

    cb = apyfixed == APyCFixed  # Complex multiplication result in one extra `int_bit`
    p = apyfixed(3164841, bits=32 + cb, int_bits=50 + cb)
    assert (fx_a * fx_b).is_identical(p)
    assert (fx_b * fx_a).is_identical(p)

    assert (fx_a / fx_b).is_identical(apyfixed(65536, bits=33, int_bits=7))
    assert (fx_b / fx_a).is_identical(apyfixed(65536, bits=33, int_bits=27))
    assert (fx_a / fx_a).is_identical(apyfixed(65536, bits=33, int_bits=17))
    assert (fx_b / fx_b).is_identical(apyfixed(65536, bits=33, int_bits=17))


def test_unary_minus():
    a = APyFixed(-3, int_bits=2, frac_bits=1)
    assert a._is_negative
    assert not a._is_positive
    assert (-a).is_identical(APyFixed(3, 3, 1))
    assert (--a).is_identical(APyFixed(-3, 4, 1))
    assert not (-a)._is_negative

    a = APyFixed(-3, int_bits=2, frac_bits=298)
    assert a._is_negative
    assert not a._is_positive
    assert (-a).is_identical(APyFixed(3, int_bits=3, frac_bits=298))
    assert (--a).is_identical(APyFixed(-3, int_bits=4, frac_bits=298))
    assert not (-a)._is_negative

    a = APyFixed(-3, int_bits=5, frac_bits=59)
    assert a._is_negative
    assert not a._is_positive
    assert (-a).is_identical(APyFixed(3, int_bits=6, frac_bits=59))
    assert (--a).is_identical(APyFixed(-3, int_bits=7, frac_bits=59))
    assert not (-a)._is_negative


def test_unary_plus():
    a = APyFixed(-3, int_bits=2, frac_bits=1)
    assert (+a).is_identical(a)
    assert (+a)._is_negative


def test_positive_negative_zero():
    assert not APyFixed(0, int_bits=4, frac_bits=4)._is_positive
    assert APyFixed(0, int_bits=4, frac_bits=4).is_zero
    assert not APyFixed(0, int_bits=4, frac_bits=4)._is_negative

    assert APyFixed(1, int_bits=4, frac_bits=4)._is_positive
    assert not APyFixed(1, int_bits=4, frac_bits=4).is_zero
    assert not APyFixed(1, int_bits=4, frac_bits=4)._is_negative

    assert not APyFixed(-1, int_bits=4, frac_bits=4)._is_positive
    assert not APyFixed(-1, int_bits=4, frac_bits=4).is_zero
    assert APyFixed(-1, int_bits=4, frac_bits=4)._is_negative

    assert not APyFixed(0, int_bits=4, frac_bits=10000)._is_positive
    assert APyFixed(0, int_bits=4, frac_bits=10000).is_zero
    assert not APyFixed(0, int_bits=4, frac_bits=10000)._is_negative

    assert APyFixed(1, int_bits=4, frac_bits=10000)._is_positive
    assert not APyFixed(1, int_bits=4, frac_bits=10000).is_zero
    assert not APyFixed(1, int_bits=4, frac_bits=10000)._is_negative

    assert not APyFixed(-1, int_bits=4, frac_bits=10000)._is_positive
    assert not APyFixed(-1, int_bits=4, frac_bits=10000).is_zero
    assert APyFixed(-1, int_bits=4, frac_bits=10000)._is_negative


def test_abs():
    a = APyFixed(-3, 2, 1)
    assert abs(a).is_identical(APyFixed(3, 3, 1))
    assert not abs(a)._is_negative
    a = APyFixed(-4, 2, 1)
    assert a._is_negative
    assert abs(a).is_identical(APyFixed(4, 3, 1))
    assert not abs(a)._is_negative
    assert abs(a)._is_positive

    # Both positive and negative fixed-point values have word length increased on
    # absolute value. Related GitHub issue: #15
    a = APyFixed(3, 2, 1)
    b = APyFixed(-3, 2, 1)
    assert abs(a).to_bits() == 3
    assert abs(b).to_bits() == 3
    assert abs(a).is_identical(abs(b))

    a = APyFixed(-3, 2, 298)
    assert abs(a).is_identical(APyFixed(3, 3, 298))


def test_shift():
    a = APyFixed(-3, bits=3, int_bits=2)
    assert (a >> 2).is_identical(APyFixed(-3, bits=3, int_bits=0))
    assert (a << 7).is_identical(APyFixed(-3, bits=3, int_bits=9))
    assert (a >> -2).is_identical(APyFixed(-3, bits=3, int_bits=4))
    assert (a << -7).is_identical(APyFixed(-3, bits=3, int_bits=-5))

    a = APyFixed(-3, bits=3, int_bits=2)
    b = a << 2
    assert not a.is_identical(b)
    a <<= 2
    assert a.is_identical(b)
    a = APyFixed(-3, bits=3, int_bits=2)
    b = a >> 2
    assert not a.is_identical(b)
    a >>= 2
    assert a.is_identical(b)


def test_comparisons_failing():
    a = APyFixed(3, 3, 2)
    assert a > 0
    assert a > 0.0
    a = APyFixed(0, 3, 2)
    assert a == 0
    assert a == 0.0


def test_sum():
    s = [
        APyFixed.from_float(0.3, int_bits=2, frac_bits=5),
        APyFixed.from_float(0.7, int_bits=4, frac_bits=7),
    ]
    assert sum(s).is_identical(APyFixed(130, bits=12, int_bits=5))


def test_prod():
    s = [
        APyFixed.from_float(0.3, int_bits=2, frac_bits=5),
        APyFixed.from_float(0.7, int_bits=4, frac_bits=7),
    ]
    assert math.prod(s).is_identical(APyFixed(28800, bits=25, int_bits=8))


@pytest.mark.parametrize("fixed_type", [APyFixed, APyCFixed])
def test_issue_198(fixed_type):
    # Smoke test for hang:
    # https://github.com/apytypes/apytypes/issues/198
    a = fixed_type.from_float(0.37, bits=320, int_bits=160)
    b = fixed_type.from_float(-1.54, bits=320, int_bits=160)
    assert (a / b).is_identical(
        fixed_type(
            9124881235244390437282343211400582649786457014497119861158385035798550334417354773011825622634742286365594170616862341739107325461683793130579692109045501048385049337084316871381549559677925492,
            bits=641,
            int_bits=321,
        )
    )
    assert (b / a).is_identical(
        fixed_type(
            9124881235244390437282343211400582649786457014497119861158385035798550334417354773011825622634733909232864840223831369193001809343494660536116672315905590613784976847626072664256077133740188487,
            bits=641,
            int_bits=321,
        )
    )


def test_leading_zeros():
    # Less than full-limb test
    assert APyFixed(0x000, bits=12, int_bits=0).leading_zeros == 12
    assert APyFixed(0x001, bits=12, int_bits=0).leading_zeros == 11
    assert APyFixed(0x002, bits=12, int_bits=0).leading_zeros == 10
    assert APyFixed(0x003, bits=12, int_bits=0).leading_zeros == 10
    assert APyFixed(0x004, bits=12, int_bits=0).leading_zeros == 9
    assert APyFixed(0x7FF, bits=12, int_bits=0).leading_zeros == 1
    assert APyFixed(0x800, bits=12, int_bits=0).leading_zeros == 0
    assert APyFixed(0xFFF, bits=12, int_bits=0).leading_zeros == 0

    # Full-limb test
    assert APyFixed(0x0000000000000000, bits=64, int_bits=0).leading_zeros == 64
    assert APyFixed(0x0000000000000001, bits=64, int_bits=0).leading_zeros == 63
    assert APyFixed(0x0000000000000002, bits=64, int_bits=0).leading_zeros == 62
    assert APyFixed(0x0000000000000003, bits=64, int_bits=0).leading_zeros == 62
    assert APyFixed(0x0000000000000004, bits=64, int_bits=0).leading_zeros == 61
    assert APyFixed(0x1000000000000000, bits=64, int_bits=0).leading_zeros == 3
    assert APyFixed(0x2000000000000000, bits=64, int_bits=0).leading_zeros == 2
    assert APyFixed(0x4000000000000000, bits=64, int_bits=0).leading_zeros == 1
    assert APyFixed(0x8000000000000000, bits=64, int_bits=0).leading_zeros == 0
    assert APyFixed(0xFFFFFFFFFFFFFFFF, bits=64, int_bits=0).leading_zeros == 0

    # Two-limb test
    assert APyFixed(0x000000000000000000, bits=72, int_bits=0).leading_zeros == 72
    assert APyFixed(0x000000000000000001, bits=72, int_bits=0).leading_zeros == 71
    assert APyFixed(0x000000000000000002, bits=72, int_bits=0).leading_zeros == 70
    assert APyFixed(0x000000000000000003, bits=72, int_bits=0).leading_zeros == 70
    assert APyFixed(0x000000000000000004, bits=72, int_bits=0).leading_zeros == 69
    assert APyFixed(0x00FFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_zeros == 8
    assert APyFixed(0x01FFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_zeros == 7
    assert APyFixed(0xFFFFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_zeros == 0


def test_leading_signs():
    # Less than full-limb test
    assert APyFixed(0xFF0, bits=12, int_bits=0).leading_signs == 8
    assert APyFixed(0x000, bits=12, int_bits=0).leading_signs == 12
    assert APyFixed(0x001, bits=12, int_bits=0).leading_signs == 11
    assert APyFixed(0x002, bits=12, int_bits=0).leading_signs == 10
    assert APyFixed(0x003, bits=12, int_bits=0).leading_signs == 10
    assert APyFixed(0x004, bits=12, int_bits=0).leading_signs == 9
    assert APyFixed(0x7FF, bits=12, int_bits=0).leading_signs == 1
    assert APyFixed(0x800, bits=12, int_bits=0).leading_signs == 1
    assert APyFixed(0xFF0, bits=12, int_bits=0).leading_signs == 8
    assert APyFixed(0xFFF, bits=12, int_bits=0).leading_signs == 12

    # Full-limb test
    assert APyFixed(0x0000000000000000, bits=64, int_bits=0).leading_signs == 64
    assert APyFixed(0x0000000000000001, bits=64, int_bits=0).leading_signs == 63
    assert APyFixed(0x0000000000000002, bits=64, int_bits=0).leading_signs == 62
    assert APyFixed(0x0000000000000003, bits=64, int_bits=0).leading_signs == 62
    assert APyFixed(0x0000000000000004, bits=64, int_bits=0).leading_signs == 61
    assert APyFixed(0x1000000000000000, bits=64, int_bits=0).leading_signs == 3
    assert APyFixed(0x2000000000000000, bits=64, int_bits=0).leading_signs == 2
    assert APyFixed(0x4000000000000000, bits=64, int_bits=0).leading_signs == 1
    assert APyFixed(0x8000000000000000, bits=64, int_bits=0).leading_signs == 1
    assert APyFixed(0xC000000000000000, bits=64, int_bits=0).leading_signs == 2
    assert APyFixed(0xFFFFFFFFFFFFFF00, bits=64, int_bits=0).leading_signs == 56
    assert APyFixed(0xFFFFFFFFFFFFFFFF, bits=64, int_bits=0).leading_signs == 64

    # Two-limb test
    assert APyFixed(0x000000000000000000, bits=72, int_bits=0).leading_signs == 72
    assert APyFixed(0x000000000000000001, bits=72, int_bits=0).leading_signs == 71
    assert APyFixed(0x000000000000000002, bits=72, int_bits=0).leading_signs == 70
    assert APyFixed(0x000000000000000003, bits=72, int_bits=0).leading_signs == 70
    assert APyFixed(0x000000000000000004, bits=72, int_bits=0).leading_signs == 69
    assert APyFixed(0x00FFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_signs == 8
    assert APyFixed(0x01FFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_signs == 7
    assert APyFixed(0xFFFFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_signs == 72
    assert APyFixed(0xFFFFFFFFFFFFFFFFF0, bits=72, int_bits=0).leading_signs == 68


def test_leading_fractional_zeros():
    # Code coverage test
    assert APyFixed(0x00, bits=8, frac_bits=0).leading_fractional_zeros == 0

    # Less than full-limb test
    assert APyFixed(0xFF_F, bits=12, frac_bits=4).leading_fractional_zeros == 0
    assert APyFixed(0xFF_7, bits=12, frac_bits=4).leading_fractional_zeros == 1
    assert APyFixed(0x00_3, bits=12, frac_bits=4).leading_fractional_zeros == 2
    assert APyFixed(0x0F_1, bits=12, frac_bits=4).leading_fractional_zeros == 3
    assert APyFixed(0xFF_0, bits=12, frac_bits=4).leading_fractional_zeros == 4
    assert APyFixed(0x00_0, bits=12, frac_bits=4).leading_fractional_zeros == 4

    # Multi-limb boundary test
    assert (
        APyFixed(0x0_0000000000000000, bits=68, frac_bits=64).leading_fractional_zeros
        == 64
    )
    assert (
        APyFixed(0x1_0000000000000001, bits=68, frac_bits=64).leading_fractional_zeros
        == 63
    )
    assert (
        APyFixed(0x2_0000000000000002, bits=68, frac_bits=64).leading_fractional_zeros
        == 62
    )
    assert (
        APyFixed(0x3_0000000000000003, bits=68, frac_bits=64).leading_fractional_zeros
        == 62
    )
    assert (
        APyFixed(0x0_1FFFFFFFFFFFFFFF, bits=68, frac_bits=64).leading_fractional_zeros
        == 3
    )
    assert (
        APyFixed(0x1_200FF00FF00FF00F, bits=68, frac_bits=64).leading_fractional_zeros
        == 2
    )
    assert (
        APyFixed(0x1_300FF00FF00FF00F, bits=68, frac_bits=64).leading_fractional_zeros
        == 2
    )
    assert (
        APyFixed(0xF_400FF00FF00FF00F, bits=68, frac_bits=64).leading_fractional_zeros
        == 1
    )
    assert (
        APyFixed(0xE_800FF00FF00FF00F, bits=68, frac_bits=64).leading_fractional_zeros
        == 0
    )


def test_issue_224():
    # Smoke test for faulty number of leading zeros
    # https://github.com/apytypes/apytypes/issues/224
    x = APyFixed(39726548548812, bits=55, int_bits=3)  # approx 0.008821065777556036
    assert x.leading_zeros == 9
    assert x.leading_fractional_zeros == 6


@pytest.mark.parametrize("fixed_type", [APyFixed, APyCFixed])
def test_operation_with_integers(fixed_type):
    a = fixed_type(5, 6, 2)
    one = fixed_type(4, 6, 2)
    zero = fixed_type(0, 6, 2)

    # Integer identities
    assert (_ := a + 0).is_identical(_ := a + zero)
    assert (_ := 0 + a).is_identical(_ := zero + a)
    assert (_ := a - 0).is_identical(_ := a - zero)
    assert (_ := 0 - a).is_identical(_ := zero - a)
    assert (_ := a * 1).is_identical(_ := a * one)
    assert (_ := 1 * a).is_identical(_ := one * a)
    assert (_ := a / 1).is_identical(_ := a / one)

    # Other integer operations
    neg_two = fixed_type(248, bits=8, int_bits=6)
    assert (_ := a + (-2)).is_identical(_ := a + neg_two)
    assert (_ := (-2) + a).is_identical(_ := neg_two + a)
    assert (_ := a - (-2)).is_identical(_ := a - neg_two)
    assert (_ := (-2) - a).is_identical(_ := neg_two - a)
    assert (_ := a * (-2)).is_identical(_ := a * neg_two)
    assert (_ := (-2) * a).is_identical(_ := neg_two * a)
    assert (_ := a / (-2)).is_identical(_ := a / neg_two)
    assert (_ := (-2) / a).is_identical(_ := neg_two / a)


def test_operation_with_floats():
    a = APyFixed(5, 6, 2)
    one = APyFixed(4, 6, 2)
    zero = APyFixed(0, 6, 2)

    # Identities
    assert (_ := a + 0.0).is_identical(_ := a + zero)
    assert (_ := 0.0 + a).is_identical(_ := zero + a)
    assert (_ := a - 0.0).is_identical(_ := a - zero)
    assert (_ := 0.0 - a).is_identical(_ := zero - a)
    assert (_ := a * 1.0).is_identical(_ := a * one)
    assert (_ := 1.0 * a).is_identical(_ := one * a)
    assert (_ := a / 1.0).is_identical(_ := a / one)

    # Other operations. 2.125 should quantize to -2.25
    b = APyFixed(247, bits=8, int_bits=6)
    assert (_ := a + (-2.125)).is_identical(_ := a + b)
    assert (_ := (-2.125) + a).is_identical(_ := b + a)
    assert (_ := a - (-2.125)).is_identical(_ := a - b)
    assert (_ := (-2.125) - a).is_identical(_ := b - a)
    assert (_ := a * (-2.125)).is_identical(_ := a * b)
    assert (_ := (-2.125) * a).is_identical(_ := b * a)
    assert (_ := a / (-2.125)).is_identical(_ := a / b)
    assert (_ := (-2.125) / a).is_identical(_ := b / a)


def test_rdiv_int():
    """
    R-division with an integer. As long as the integer fits into a float, Python chooses
    to promote the integer to float first, and only performs `__rtruediv__` with the
    integer if it does not fit into a float.
    """
    a = APyFixed.from_float(2**1000, int_bits=4000, frac_bits=0)
    assert (2**2000 / a).is_identical(
        APyFixed.from_float(2**1000, int_bits=4001, frac_bits=4000)
    )


def test_pow():
    a = APyFixed.from_float(0.2, 2, 8)
    a2 = APyFixed(2601, 4, 16)
    assert (a**2).is_identical(a2)
    assert (a**1).is_identical(a)
    assert (a**0).is_identical(APyFixed.from_float(1, 2, 8))

    a = APyFixed.from_float(-0.2, 2, 8)
    assert (a**2).is_identical(a2)
    a3 = APyFixed(1073609173, 6, 24)
    assert (a**3).is_identical(a3)

    assert (a**1).is_identical(a)
    assert (a**0).is_identical(APyFixed.from_float(1, 2, 8))

    b = APyFixed(124, bits=7, int_bits=10)
    b5 = APyFixed(34359737344, bits=35, int_bits=50)
    assert (b**5).is_identical(b5)
    assert (b**1).is_identical(b)
    assert (b**0).is_identical(APyFixed(1, 10, 0))


@pytest.mark.parametrize("bits", range(13, 90))
def test_addition_different_wordlengths(bits: int):
    a = APyFixed(3, bits=bits, int_bits=7)
    b = APyFixed(18, bits=bits, int_bits=7)
    assert (a + b).is_identical(APyFixed(21, bits=bits + 1, int_bits=8))


@pytest.mark.parametrize("bits", range(13, 90))
def test_addition_different_wordlengths_different_fracbits(bits: int):
    a = APyFixed(3, bits=bits, int_bits=8)
    b = APyFixed(18, bits=bits, int_bits=6)
    assert (a + b).is_identical(APyFixed(30, bits=bits + 3, int_bits=9))


@pytest.mark.parametrize("bits", range(13, 90))
def test_multiplication_different_wordlengths(bits: int):
    a = APyFixed(3, bits=bits, int_bits=7)
    b = APyFixed(18, bits=bits, int_bits=7)
    assert (a * b).is_identical(APyFixed(54, bits=2 * bits, int_bits=14))


@pytest.mark.parametrize("bits", range(13, 90))
def test_multiplication_different_wordlengths_different_fracbits(bits: int):
    a = APyFixed(3, bits=bits, int_bits=8)
    b = APyFixed(18, bits=bits, int_bits=6)
    assert (a * b).is_identical(APyFixed(54, bits=2 * bits, int_bits=14))
