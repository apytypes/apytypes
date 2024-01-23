from apytypes import APyFixed
from apytypes import APyFixedRoundingMode
from apytypes import APyFixedOverflowMode


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


def test_wide_operations():
    """
    Tests for wider additions and subtractions
    """
    fx_a = APyFixed.from_string(
        "-1376018206341311063223476816643087998331620501540496640."
        "021222579872958058370179355618716816066859017361262100333952697594702"
        "314679773970519809467311447652539955943903993200932791396783892142688"
        "708904952458654442554723081083186210082207584128592922850820472478833"
        "257136662269306798708182072507551281664490003441493733349403017982015"
        "56238154807942919433116912841796875",
        bits=511,
        int_bits=199,
    )
    fx_b = APyFixed.from_string(
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
        (fx_a + fx_b).to_string()
        == "-1376018206341311063223476712151932182149337777669789204."
        "94991342285001624637369202035149436324913376063085376357255238"
        "96889135172236284265355996074714872459156490586214293346007943"
        "42896488607451039432666163214519521477661557216696973679582765"
        "49716364096258700784693586201043030485635164975271020283378591"
        "10818039160450881727678711279062388956893918350572368379608181"
        "35285369376844864673208344678338497050523019191027165872352570"
        "896719470283642294816672801971435546875"
    )
    assert (
        (fx_a - fx_b).to_string()
        == "-1376018206341311063223476921134243814513903225411204075."
        "09253173689589987036666669088593926888458427409167043709535300"
        "55004911121359195145040193271514080591642628291865570672647884"
        "50671295677926378377238754094365587968500609155723190735585491"
        "68868206067835794981957841131410830874106471439230489972954306"
        "89250790714216106332680929032185242004694665536051014188751193"
        "64714630623155135326791655321661502949476980808972834127647429"
        "103280529716357705183327198028564453125"
    )
    assert (
        (fx_a * fx_b).to_string()
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
        (fx_b / fx_a).to_string()
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


def test_resize():
    """
    Test rounding and truncation
    """
    Mode = APyFixedRoundingMode
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, Mode.TRN)) == 0.5
    assert float(APyFixed.from_float(0.75, 3, 1).resize(2, 1, Mode.RND)) == -1.0
    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, Mode.TRN)) == 0.75
    assert float(APyFixed.from_float(0.75, 4, 1).resize(3, 1, Mode.RND)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, Mode.TRN)) == 0.75
    assert float(APyFixed.from_float(0.875, 4, 1).resize(3, 1, Mode.RND)) == -1.0
