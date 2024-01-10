/*
 * Arithmetic tests for the APyFixed data type.
 */


#include "../apy_fixed.h"
#include "catch.hpp"
#include <iostream>

TEST_CASE("General binary arithmetic tests")
{
    {  /* Test #1 */
        APyFixed op_a( 1.5, 3, 2);
        APyFixed op_b(-2.0, 3, 2);
        REQUIRE((op_a + op_b).to_string() == "-0.5");
        REQUIRE((op_a + op_b).int_bits()  ==      3);
        REQUIRE((op_a + op_b).bits()      ==      4);
        REQUIRE((op_a - op_b).to_string() ==  "3.5");
        REQUIRE((op_a - op_b).int_bits()  ==      3);
        REQUIRE((op_a - op_b).bits()      ==      4);
        REQUIRE((op_a * op_b).to_string() ==   "-3");
        REQUIRE((op_a * op_b).int_bits()  ==      4);
        REQUIRE((op_a * op_b).bits()      ==      6);
        REQUIRE((op_a / op_b).to_string() == "-0.5");
        REQUIRE((op_a / op_b).int_bits()  ==      4);
        REQUIRE((op_a / op_b).bits()      ==      5);
    }

    {  /* Test #2 */
        APyFixed op_a(-1.0, 1, 1);
        APyFixed op_b(-0.5, 1, 0);
        REQUIRE((op_a + op_b).to_string() == "-1.5");
        REQUIRE((op_a + op_b).int_bits()  ==      2);
        REQUIRE((op_a + op_b).bits()      ==      3);
        REQUIRE((op_a - op_b).to_string() == "-0.5");
        REQUIRE((op_a - op_b).int_bits()  ==      2);
        REQUIRE((op_a - op_b).bits()      ==      3);
        REQUIRE((op_a * op_b).to_string() ==  "0.5");
        REQUIRE((op_a * op_b).int_bits()  ==      1);
        REQUIRE((op_a * op_b).bits()      ==      2);
        REQUIRE((op_a / op_b).to_string() ==    "2");
        REQUIRE((op_a / op_b).int_bits()  ==      3);
        REQUIRE((op_a / op_b).bits()      ==      3);
    }

    {  /* Test #3 */
        APyFixed op_a(100, 40);
        op_a.from_string("306538369.1376953125");
        
        APyFixed op_b(140, 10);
        op_b.from_string(
            "68.5000000000000005176294573636282259077034950678353197872638702392578125"
        );

        REQUIRE((op_a + op_b).bits()     == 171);
        REQUIRE((op_a + op_b).int_bits() == 41);
        REQUIRE(
            (op_a + op_b).to_string() ==
            "306538437."
            "6376953125000005176294573636282259077034950678353197872638702392578125"
        );
        REQUIRE((op_a - op_b).bits()     == 171);
        REQUIRE((op_a - op_b).int_bits() == 41);
        REQUIRE(
            (op_a - op_b).to_string() ==
            "306538300."
            "6376953124999994823705426363717740922965049321646802127361297607421875"
        );
        REQUIRE((op_a * op_b).bits()     == 240);
        REQUIRE((op_a * op_b).int_bits() == 50);
        REQUIRE(
            (op_a * op_b).to_string() ==
            "20997878285."
            "9321290649232896778767861830011886138756121988890868124144617468118667"
            "6025390625"
        );
        REQUIRE((op_a / op_b).bits()     == 231);
        REQUIRE((op_a / op_b).int_bits() == 171);
        REQUIRE(
            (op_a / op_b).to_string() ==
            "4475012."
            "688141537374943088913037403386852020048536360263824462890625"
       );
    }  /* End test #3 */
}


TEST_CASE("Binary arithmetic: APyFixed::operator-(const APyFixed &rhs)")
{
    { /* Test #1 */
        APyFixed op_a(512, 200, to_limb_vec({0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}));
        APyFixed op_b(512, 100, to_limb_vec({0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}));
        REQUIRE( (op_a - op_b).to_string() == "0" );
    }

    { /* Test #2 */
        APyFixed op_a(200, 100, to_limb_vec({0x0,0x0,0x0,0x0}));
        APyFixed op_b(512, 200, to_limb_vec({0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}));
        REQUIRE( (op_a - op_b).to_string() == "0" );
    }
}


TEST_CASE("Unary arithmetic arithmetic: APyFixed::operator-()")
{
    { /* Test #1 */
        APyFixed operand(64, 32, to_limb_vec({ 0x8000000000000000 }));
        REQUIRE( (-operand)._data == to_limb_vec({ 0x8000000000000000, 0 }));
    }

    { /* Test #2 */
        APyFixed operand(64, 32, to_limb_vec({ 0x7FFFFFFFFFFFFFFF }));
        REQUIRE( (-operand)._data == to_limb_vec({ 0x8000000000000001, uint64_t(-1) }));
    }

    { /* Test #3 */
        APyFixed operand(
            512, 100,
            to_limb_vec({
                0xF51A10EEB1241512,
                0xF51A10EF8AFF1512,
                0x151A10EF8AFF1512,
                0x051A10EF8AFF1512,
                0x051A10EEB1241512,
                0xF51A10EEB1241512,
                0xF51A10EEB1241512,
                0x101A10EEB1241512,
            })
        );
        REQUIRE( (-operand).bits()     == 513 );
        REQUIRE( (-operand).int_bits() == 101 );
        REQUIRE(std::string("-") + operand.to_string() == (-operand).to_string());
    }
}

TEST_CASE("APyFixed::operator+(rhs) and APyFixed::operator-(rhs) [large]")
{
    { /* Test #1 */

        // -1376018206341311063223476816643087998331620501540496640.
        // 021222579872958058370179355618716816066859017361262100333952697594702
        // 314679773970519809467311447652539955943903993200932791396783892142688
        // 708904952458654442554723081083186210082207584128592922850820472478833
        // 257136662269306798708182072507551281664490003441493733349403017982015
        // 56238154807942919433116912841796875
        APyFixed op_a(
            512, 200,
            to_limb_vec({
                0x1429404125123152,
                0x1245451212312342,
                0xFFF4F0A218B18CCC,
                0xFFF011012151FEEA,
                0xFFFA912831824717,
                0xFFFA91B39812391A,
                0x123ABCA124150012,
                0xFFFFF1A23A231245,
            })
        );

        // 104491155816182282723870707435.
        // 071309157022941811996487335267222452817725256730408336761400307905788
        // 797456145543984209859839960406624306885282563866331997053887403535237
        // 669472286295439923033245419525969513108528001363095759209857885470986
        // 321274651839001942356532319797348447878578921637577688261230250110887
        // 656142652390037359273932290457150614714630623155135326791655321661502
        // 949476980808972834127647429103280529716357705183327198028564453125
        APyFixed op_b(
            512, 100,
            to_limb_vec({
                0x151A10EEB1241512,
                0xFFAA10EEB1241512,
                0xFFAA10EEB1241512,
                0xFFAA10EEB1241512,
                0xFFAA10EEB1241512,
                0x151A10EEB1241512,
                0x151A10EEB1241512,
                0x151A10EEB1241512,
            })
        );
        REQUIRE((op_a + op_b).to_string() ==
                "-1376018206341311063223476712151932182149337777669789204."
                "94991342285001624637369202035149436324913376063085376357255238"
                "96889135172236284265355996074714872459156490586214293346007943"
                "42896488607451039432666163214519521477661557216696973679582765"
                "49716364096258700784693586201043030485635164975271020283378591"
                "10818039160450881727678711279062388956893918350572368379608181"
                "35285369376844864673208344678338497050523019191027165872352570"
                "896719470283642294816672801971435546875");
        REQUIRE((op_a - op_b).to_string() ==
                "-1376018206341311063223476921134243814513903225411204075."
                "09253173689589987036666669088593926888458427409167043709535300"
                "55004911121359195145040193271514080591642628291865570672647884"
                "50671295677926378377238754094365587968500609155723190735585491"
                "68868206067835794981957841131410830874106471439230489972954306"
                "89250790714216106332680929032185242004694665536051014188751193"
                "64714630623155135326791655321661502949476980808972834127647429"
                "103280529716357705183327198028564453125");
        REQUIRE((op_a+op_b).bits()      == 613);
        REQUIRE((op_a+op_b).int_bits()  == 201);
        REQUIRE((op_a-op_b).bits()      == 613);
        REQUIRE((op_a-op_b).int_bits()  == 201);
    }

    { /* Test #2 */

        // -1085486967854999652283885.
        // 7448729760947930374960952188947589709537631140390587787067857834922569
        // 8593119855531200749403496121503716792997680618362464712806046112732391
        // 3062308705241286170134557973407921303069833918178512551600480466327405
        // 8610777142902994459196267966570891762710916869640642680309043084935670
        // 3461527201803803803026149333988253072319552556869060605661235965322211
        // 0076395744803539537663705782932765941950492560863494873046875
        APyFixed op_a(
            512, 100,
            to_limb_vec({
                0x1429404125123152,
                0x1245451212312342,
                0xFFF4F0A218B18CCC,
                0xFFF011012151FEEA,
                0xFFFA912831824717,
                0xFFFA91B39812391A,
                0x123ABCA124150012,
                0xFFFFF1A23A231245,
            })
        );

        // 132458276388924914357910377122715071335085446375728229978879.
        // 664320867774297149614525689744528428324923121855502041915090146517009
        // 690050948493327275505512705229951370519178223895243419834128949003300
        // 054840360790844456369066397718311100233752085743664256491380347940962
        // 859216008018161064320203894400395799216476079288214421276109787964370
        // 13471631871652789413928985595703125
        APyFixed op_b(
            512, 200,
            to_limb_vec({
                0x151A10EEB1241512,
                0xFFAA10EEB1241512,
                0xFFAA10EEB1241512,
                0xFFAA10EEB1241512,
                0xFFAA10EEB1241512,
                0x151A10EEB1241512,
                0x151A10EEB1241512,
                0x151A10EEB1241512,
            })
        );
        REQUIRE((op_a + op_b).to_string() ==
                "132458276388924914357910377122715070249598478520728577694993."
                "91944789167950411211843047084976945737116000781644326320830436"
                "30247527041197499380152680114777440149142025892014177116187727"
                "06068487875976141778052085603170198931839744903178930682251825"
                "48574393977986747463545335493030387076487428426760373871004020"
                "49876012503570080788836558765676817034445098561012595944636913"
                "67469276804474431309393943387640346777889923604255196460462336"
                "294217067234058049507439136505126953125");
        REQUIRE((op_a - op_b).to_string() ==
                "-132458276388924914357910377122715072420572414230727882262765."
                "40919384386909018711062090863928739927868623589456082062187593"
                "00092666759821470486392829995476664449885384491550300788680669"
                "62189410130623967902669496085742539200955691719021536821919661"
                "84276904298082840729026507708573245136376612352119705288839274"
                "71709751784855441406922728637017509339885459321773201174503711"
                "32530723195525568690606056612359653222110076395744803539537663"
                "705782932765941950492560863494873046875");
        REQUIRE((op_a+op_b).bits()      == 613);
        REQUIRE((op_a+op_b).int_bits()  == 201);
        REQUIRE((op_a-op_b).bits()      == 613);
        REQUIRE((op_a-op_b).int_bits()  == 201);
    }

}


TEST_CASE("Binary arithmetic: APyFixed::operator*/(APyFixed &rhs)")
{
    // -1085486967854999652283885.
    // 7448729760947930374960952188947589709537631140390587787067857834922569
    // 8593119855531200749403496121503716792997680618362464712806046112732391
    // 3062308705241286170134557973407921303069833918178512551600480466327405
    // 8610777142902994459196267966570891762710916869640642680309043084935670
    // 3461527201803803803026149333988253072319552556869060605661235965322211
    // 0076395744803539537663705782932765941950492560863494873046875
    APyFixed op_a(
        512, 100,
        to_limb_vec({
            0x1429404125123152,
            0x1245451212312342,
            0xFFF4F0A218B18CCC,
            0xFFF011012151FEEA,
            0xFFFA912831824717,
            0xFFFA91B39812391A,
            0x123ABCA124150012,
            0xFFFFF1A23A231245,
        })
    );

    // 132458276388924914357910377122715071335085446375728229978879.
    // 664320867774297149614525689744528428324923121855502041915090146517009
    // 690050948493327275505512705229951370519178223895243419834128949003300
    // 054840360790844456369066397718311100233752085743664256491380347940962
    // 859216008018161064320203894400395799216476079288214421276109787964370
    // 13471631871652789413928985595703125
    APyFixed op_b(
        512, 200,
        to_limb_vec({
            0x151A10EEB1241512,
            0xFFAA10EEB1241512,
            0xFFAA10EEB1241512,
            0xFFAA10EEB1241512,
            0xFFAA10EEB1241512,
            0x151A10EEB1241512,
            0x151A10EEB1241512,
            0x151A10EEB1241512,
        })
    );

    REQUIRE(
        (op_a * op_b).to_string() ==
        "-14378173280471359793175652260816583699347283529370074406003684748"
        "1874028068127844759."
        "016595668886460375186623178841992969362848313453571029224354702831"
        "490463717969073392643044880348208446455087857738493876686848561681"
        "809833044214776451743329705854295876542876810172902345207337900392"
        "438906279960351818481385542967807793796224234468459085395360774678"
        "994347012274651936962832344031929334376765299139295185206945982946"
        "367607665831151169445418287400641639061549270423726568173859720256"
        "223941074691188417648020443974631016237843957199506152243019578289"
        "070045142028345404026673865347600681259158824811586579251365902015"
        "572046441783266421061817749228191070949127455047033369563083092108"
        "895792786441945277872071425455765918986368187031375649791989805309"
        "59416814195373670432452628542563388691633008420467376708984375"
    );
    REQUIRE(
        (op_a / op_b).to_string() ==
        "-0."
        "0000000000000000000000000000000000081949350198985320592667882188680263"
        "1404908543387836706736596653031922315529439906414609775728304830271469"
        "4447502502414438879994792623364382676904853317193557803728727306488670"
        "7689673910841119861514475799047154125636716109496964509445895253514308"
        "4126472969072655710037829606428766865257148577231179633243452844950211"
        "24529392901893504901934817130637611626298166811466217041015625"
    );
}


TEST_CASE("Comparison operators")
{
    REQUIRE_FALSE(  APyFixed(0.0, 256, 128)  == APyFixed(1.0, 256, 128)  );
    REQUIRE(        APyFixed(0.0, 256, 128)  != APyFixed(1.0, 256, 128)  );
    REQUIRE(        APyFixed(0.0, 256, 128)  <  APyFixed(1.0, 256, 128)  );
    REQUIRE(        APyFixed(0.0, 256, 128)  <= APyFixed(1.0, 256, 128)  );
    REQUIRE_FALSE(  APyFixed(0.0, 256, 128)  >  APyFixed(1.0, 256, 128)  );
    REQUIRE_FALSE(  APyFixed(0.0, 256, 128)  >= APyFixed(1.0, 256, 128)  );

    REQUIRE(        APyFixed(1.0, 256, 128)  == APyFixed(1.0, 256, 128)  );
    REQUIRE_FALSE(  APyFixed(1.0, 256, 128)  != APyFixed(1.0, 140, 128)  );
    REQUIRE_FALSE(  APyFixed(1.0, 256, 128)  <  APyFixed(1.0, 140, 128)  );
    REQUIRE(        APyFixed(1.0, 256, 128)  <= APyFixed(1.0, 256, 128)  );
    REQUIRE_FALSE(  APyFixed(1.0, 256, 128)  >  APyFixed(1.0, 256, 128)  );
    REQUIRE(        APyFixed(1.0, 256, 128)  >= APyFixed(1.0, 256, 128)  );

    REQUIRE_FALSE(  APyFixed(-1.0, 256, 128) == APyFixed(-3.0, 140, 128) );
    REQUIRE(        APyFixed(-1.0, 256, 128) != APyFixed(-3.0, 256, 128) );
    REQUIRE_FALSE(  APyFixed(-1.0, 256, 128) <  APyFixed(-3.0, 140, 128) );
    REQUIRE_FALSE(  APyFixed(-1.0, 256, 128) <= APyFixed(-3.0, 256, 128) );
    REQUIRE(        APyFixed(-1.0, 256, 128) >  APyFixed(-3.0, 256, 128) );
    REQUIRE(        APyFixed(-1.0, 256, 128) >= APyFixed(-3.0, 256, 128) );
}
