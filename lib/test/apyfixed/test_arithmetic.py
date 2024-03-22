from apytypes import APyFixed

import math


def test_arithmetic_operations():
    a = APyFixed.from_float(-1.25, int_bits=2, frac_bits=4)
    b = APyFixed.from_float(0.75, int_bits=2, frac_bits=3)
    assert (a * b).is_identical(APyFixed.from_float(-0.9375, int_bits=4, frac_bits=7))
    assert not (
        (a * b).is_identical(APyFixed.from_float(-0.9375, int_bits=4, frac_bits=5))
    )
    assert (a + b).is_identical(APyFixed.from_float(-0.5, int_bits=3, frac_bits=4))
    assert (b + a).is_identical(APyFixed.from_float(-0.5, int_bits=3, frac_bits=4))
    assert (a - b).is_identical(APyFixed.from_float(-2, int_bits=3, frac_bits=4))
    assert (b - a).is_identical(APyFixed.from_float(2, int_bits=3, frac_bits=4))
    assert (a / b).is_identical(APyFixed.from_float(-1.625, int_bits=6, frac_bits=4))
    assert (b / a).is_identical(APyFixed.from_float(-0.5, int_bits=7, frac_bits=3))


def test_twos_complement_overflow():
    assert float(APyFixed(1 << 127, 128, 1)) == -1.0
    assert float(APyFixed(1 << 126, 128, 1)) == 0.5
    assert float(APyFixed(0xFFFFFFFF0 << 92, 96, 1)) == 0.0
    assert float(APyFixed(0xFFFFFFFF8 << 92, 96, 1)) == -1.0
    assert float(APyFixed(0xFFFFFFFF4 << 92, 96, 1)) == 0.5


def test_binary_comparison():
    assert not (
        APyFixed.from_float(0.0, 256, 128) == APyFixed.from_float(1.0, 256, 128)
    )
    assert APyFixed.from_float(0.0, 256, 128) != APyFixed.from_float(1.0, 256, 128)
    assert APyFixed.from_float(0.0, 256, 128) < APyFixed.from_float(1.0, 256, 128)
    assert APyFixed.from_float(0.0, 256, 128) <= APyFixed.from_float(1.0, 256, 128)
    assert not (APyFixed.from_float(0.0, 256, 128) > APyFixed.from_float(1.0, 256, 128))
    assert not (
        APyFixed.from_float(0.0, 256, 128) >= APyFixed.from_float(1.0, 256, 128)
    )

    assert APyFixed.from_float(1.0, 256, 128) == APyFixed.from_float(1.0, 256, 128)
    assert not (
        APyFixed.from_float(1.0, 256, 128) != APyFixed.from_float(1.0, 140, 128)
    )
    assert not (APyFixed.from_float(1.0, 256, 128) < APyFixed.from_float(1.0, 140, 128))
    assert APyFixed.from_float(1.0, 256, 128) <= APyFixed.from_float(1.0, 256, 128)
    assert not (APyFixed.from_float(1.0, 256, 128) > APyFixed.from_float(1.0, 256, 128))
    assert APyFixed.from_float(1.0, 256, 128) >= APyFixed.from_float(1.0, 256, 128)

    assert not (
        APyFixed.from_float(-1.0, 256, 128) == APyFixed.from_float(-3.0, 140, 128)
    )
    assert APyFixed.from_float(-1.0, 256, 128) != APyFixed.from_float(-3.0, 256, 128)
    assert not (
        APyFixed.from_float(-1.0, 256, 128) < APyFixed.from_float(-3.0, 140, 128)
    )
    assert not (
        APyFixed.from_float(-1.0, 256, 128) <= APyFixed.from_float(-3.0, 256, 128)
    )
    assert APyFixed.from_float(-1.0, 256, 128) > APyFixed.from_float(-3.0, 256, 128)
    assert APyFixed.from_float(-1.0, 256, 128) >= APyFixed.from_float(-3.0, 256, 128)


def test_float_comparison():
    assert not APyFixed.from_float(0.0, 256, 128) == 1.0
    assert APyFixed.from_float(0.0, 256, 128) != 1.0
    assert APyFixed.from_float(0.0, 256, 128) < 1.0
    assert APyFixed.from_float(0.0, 256, 128) <= 1.0
    assert not APyFixed.from_float(0.0, 256, 128) > 1.0
    assert not APyFixed.from_float(0.0, 256, 128) >= 1.0

    assert APyFixed.from_float(1.0, 256, 128) == 1.0
    assert not APyFixed.from_float(1.0, 256, 128) != 1.0
    assert not APyFixed.from_float(1.0, 256, 128) < 1.0
    assert APyFixed.from_float(1.0, 256, 128) <= 1.0
    assert not APyFixed.from_float(1.0, 256, 128) > 1.0
    assert APyFixed.from_float(1.0, 256, 128) >= 1.0

    assert not APyFixed.from_float(-1.0, 256, 128) == -3.0
    assert APyFixed.from_float(-1.0, 256, 128) != -3.0
    assert not APyFixed.from_float(-1.0, 256, 128) < -3.0
    assert not APyFixed.from_float(-1.0, 256, 128) <= -3.0
    assert APyFixed.from_float(-1.0, 256, 128) > -3.0
    assert APyFixed.from_float(-1.0, 256, 128) >= -3.0


def test_narrow_add_sub():
    for a_bits in range(10, 30):
        for b_bits in range(10, 30):
            a = APyFixed.from_float(a_bits + b_bits, bits=a_bits + 20, int_bits=b_bits)
            b = APyFixed.from_float(a_bits + b_bits, bits=b_bits + 20, int_bits=a_bits)
            assert float(a + b) == float(2 * (a_bits + b_bits))
            assert float(a - b) == 0.0
            assert float(a * b) == float(a_bits + b_bits) ** 2


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
        str(fx_b / fx_a)
        == "-0.00000000000000000000000007593733522902532737164561219082471"
        "16211794105317983099333476247634644804772491644417811098089138"
        "23507667083775290336210270855184793714621993102245603749482911"
        "71667408411795422085067091072570996522783139632231323071901487"
        "88982202344898850628344770239698064998436062342984081358263354"
        "97658796230365731928363689560796027286843061797633335593749927"
        "0012225327519672646303661167621612548828125"
    )
    assert fx_a + fx_b == fx_b + fx_a
    assert fx_a - fx_b == -(fx_b - fx_a)
    assert fx_a * fx_b == fx_b * fx_a


def test_unary_minus():
    a = APyFixed(-3, 3, 2)
    assert a._is_negative
    assert not a._is_positive
    assert (-a).is_identical(APyFixed(3, 4, 3))
    assert (--a).is_identical(APyFixed(-3, 5, 4))
    assert not (-a)._is_negative

    a = APyFixed(-3, 300, 2)
    assert a._is_negative
    assert not a._is_positive
    assert (-a).is_identical(APyFixed(3, 301, 3))
    assert (--a).is_identical(APyFixed(-3, 302, 4))
    assert not (-a)._is_negative

    a = APyFixed(-3, 64, 5)
    assert a._is_negative
    assert not a._is_positive
    assert (-a).is_identical(APyFixed(3, 65, 6))
    assert (--a).is_identical(APyFixed(-3, 66, 7))
    assert not (-a)._is_negative


def test_abs():
    a = APyFixed(-3, 3, 2)
    assert abs(a).is_identical(APyFixed(3, 4, 3))
    assert not abs(a)._is_negative
    a = APyFixed(-4, 3, 2)
    assert a._is_negative
    assert abs(a).is_identical(APyFixed(4, 4, 3))
    assert not abs(a)._is_negative
    assert abs(a)._is_positive

    # Both positive and negative fixed-point values have word length increased on
    # absolute value. Related GitHub issue: #15
    a = APyFixed(3, 3, 2)
    b = APyFixed(-3, 3, 2)
    assert abs(a).to_bits() == 3
    assert abs(b).to_bits() == 3
    assert abs(a).is_identical(abs(b))

    a = APyFixed(-3, 300, 2)
    assert abs(a).is_identical(APyFixed(3, 301, 3))


def test_shift():
    a = APyFixed(-3, bits=3, int_bits=2)
    assert (a >> 2).is_identical(APyFixed(-3, 3, 0))
    assert (a << 7).is_identical(APyFixed(-3, 3, 9))
    assert (a >> -2).is_identical(APyFixed(-3, 3, 4))
    assert (a << -7).is_identical(APyFixed(-3, 3, -5))

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
    assert math.prod(s).is_identical(APyFixed(900, bits=18, int_bits=6))


def test_issue_198():
    # Smoke test for hang:
    # https://github.com/apytypes/apytypes/issues/198
    a = APyFixed.from_float(0.37, 320, 160)
    b = APyFixed.from_float(-1.54, 320, 160)
    assert (a / b).is_identical(
        APyFixed(
            6243497100631984462763194459586332611497196285329942301718313919250743477639531240240612206126983591179650087934981576964032895475068432139224293,
            bits=481,
            int_bits=321,
        )
    )


def test_leading_zeros():
    # Less than full-limb test
    assert APyFixed(0x000, bits=12, int_bits=0).leading_zeros() == 12
    assert APyFixed(0x001, bits=12, int_bits=0).leading_zeros() == 11
    assert APyFixed(0x002, bits=12, int_bits=0).leading_zeros() == 10
    assert APyFixed(0x003, bits=12, int_bits=0).leading_zeros() == 10
    assert APyFixed(0x004, bits=12, int_bits=0).leading_zeros() == 9
    assert APyFixed(0x7FF, bits=12, int_bits=0).leading_zeros() == 1
    assert APyFixed(0x800, bits=12, int_bits=0).leading_zeros() == 0
    assert APyFixed(0xFFF, bits=12, int_bits=0).leading_zeros() == 0

    # Full-limb test
    assert APyFixed(0x0000000000000000, bits=64, int_bits=0).leading_zeros() == 64
    assert APyFixed(0x0000000000000001, bits=64, int_bits=0).leading_zeros() == 63
    assert APyFixed(0x0000000000000002, bits=64, int_bits=0).leading_zeros() == 62
    assert APyFixed(0x0000000000000003, bits=64, int_bits=0).leading_zeros() == 62
    assert APyFixed(0x0000000000000004, bits=64, int_bits=0).leading_zeros() == 61
    assert APyFixed(0x1000000000000000, bits=64, int_bits=0).leading_zeros() == 3
    assert APyFixed(0x2000000000000000, bits=64, int_bits=0).leading_zeros() == 2
    assert APyFixed(0x4000000000000000, bits=64, int_bits=0).leading_zeros() == 1
    assert APyFixed(0x8000000000000000, bits=64, int_bits=0).leading_zeros() == 0
    assert APyFixed(0xFFFFFFFFFFFFFFFF, bits=64, int_bits=0).leading_zeros() == 0

    # Two-limb test
    assert APyFixed(0x000000000000000000, bits=72, int_bits=0).leading_zeros() == 72
    assert APyFixed(0x000000000000000001, bits=72, int_bits=0).leading_zeros() == 71
    assert APyFixed(0x000000000000000002, bits=72, int_bits=0).leading_zeros() == 70
    assert APyFixed(0x000000000000000003, bits=72, int_bits=0).leading_zeros() == 70
    assert APyFixed(0x000000000000000004, bits=72, int_bits=0).leading_zeros() == 69
    assert APyFixed(0x00FFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_zeros() == 8
    assert APyFixed(0x01FFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_zeros() == 7
    assert APyFixed(0xFFFFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_zeros() == 0


def test_leading_signs():
    # Less than full-limb test
    assert APyFixed(0xFF0, bits=12, int_bits=0).leading_signs() == 8
    assert APyFixed(0x000, bits=12, int_bits=0).leading_signs() == 12
    assert APyFixed(0x001, bits=12, int_bits=0).leading_signs() == 11
    assert APyFixed(0x002, bits=12, int_bits=0).leading_signs() == 10
    assert APyFixed(0x003, bits=12, int_bits=0).leading_signs() == 10
    assert APyFixed(0x004, bits=12, int_bits=0).leading_signs() == 9
    assert APyFixed(0x7FF, bits=12, int_bits=0).leading_signs() == 1
    assert APyFixed(0x800, bits=12, int_bits=0).leading_signs() == 1
    assert APyFixed(0xFF0, bits=12, int_bits=0).leading_signs() == 8
    assert APyFixed(0xFFF, bits=12, int_bits=0).leading_signs() == 12

    # Full-limb test
    assert APyFixed(0x0000000000000000, bits=64, int_bits=0).leading_signs() == 64
    assert APyFixed(0x0000000000000001, bits=64, int_bits=0).leading_signs() == 63
    assert APyFixed(0x0000000000000002, bits=64, int_bits=0).leading_signs() == 62
    assert APyFixed(0x0000000000000003, bits=64, int_bits=0).leading_signs() == 62
    assert APyFixed(0x0000000000000004, bits=64, int_bits=0).leading_signs() == 61
    assert APyFixed(0x1000000000000000, bits=64, int_bits=0).leading_signs() == 3
    assert APyFixed(0x2000000000000000, bits=64, int_bits=0).leading_signs() == 2
    assert APyFixed(0x4000000000000000, bits=64, int_bits=0).leading_signs() == 1
    assert APyFixed(0x8000000000000000, bits=64, int_bits=0).leading_signs() == 1
    assert APyFixed(0xC000000000000000, bits=64, int_bits=0).leading_signs() == 2
    assert APyFixed(0xFFFFFFFFFFFFFF00, bits=64, int_bits=0).leading_signs() == 56
    assert APyFixed(0xFFFFFFFFFFFFFFFF, bits=64, int_bits=0).leading_signs() == 64

    # Two-limb test
    assert APyFixed(0x000000000000000000, bits=72, int_bits=0).leading_signs() == 72
    assert APyFixed(0x000000000000000001, bits=72, int_bits=0).leading_signs() == 71
    assert APyFixed(0x000000000000000002, bits=72, int_bits=0).leading_signs() == 70
    assert APyFixed(0x000000000000000003, bits=72, int_bits=0).leading_signs() == 70
    assert APyFixed(0x000000000000000004, bits=72, int_bits=0).leading_signs() == 69
    assert APyFixed(0x00FFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_signs() == 8
    assert APyFixed(0x01FFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_signs() == 7
    assert APyFixed(0xFFFFFFFFFFFFFFFFFF, bits=72, int_bits=0).leading_signs() == 72
    assert APyFixed(0xFFFFFFFFFFFFFFFFF0, bits=72, int_bits=0).leading_signs() == 68
