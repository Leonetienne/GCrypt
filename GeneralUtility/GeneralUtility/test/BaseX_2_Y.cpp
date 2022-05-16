#include <GeneralUtility/BaseConversion.h>
#include "Catch2.h"

using namespace Leonetienne::GeneralUtility;

// Tests base 10 to 10
TEST_CASE(__FILE__"/Base10_to_10", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::string set_in  = "0123456789";
    const std::string set_out = "0123456789";
    const std::string in           = "1990381";
    const std::string expected_out = "1990381";

    // Exercise
    const std::string out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 10 to 16
TEST_CASE(__FILE__"/Base10_to_16", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::string set_in  = "0123456789";
    const std::string set_out = "0123456789abcdef";
    const std::string in           = "1990381";
    const std::string expected_out = "1e5eed";

    // Exercise
    const std::string out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 16 to 10
TEST_CASE(__FILE__"/Base16_to_10", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::string set_in  = "0123456789abcdef";
    const std::string set_out = "0123456789";
    const std::string in           = "1e5eed";
    const std::string expected_out = "1990381";

    // Exercise
    const std::string out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 2 to 16
TEST_CASE(__FILE__"/Base2_to_16", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::string set_in  = "01";
    const std::string set_out = "0123456789abcdef";
    const std::string in           = "110000110001110011010111101011111";
    const std::string expected_out = "18639af5f";

    // Exercise
    const std::string out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 16 to 2
TEST_CASE(__FILE__"/Base16_to_2", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::string set_in  = "0123456789abcdef";
    const std::string set_out = "01";
    const std::string in           = "18639af5f";
    const std::string expected_out = "110000110001110011010111101011111";

    // Exercise
    const std::string out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 16 to fucking big
TEST_CASE(__FILE__"/Base16_to_FuckingBig", "[BaseConversion][Base10_2_X]")
{
    // Setup
    const std::string set_in  = "0123456789abcdef";
    const std::string set_out = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::string in = "2091f258";
    const std::string expected_out = "aymsa";

    // Exercise
    const std::string out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests that padding works
TEST_CASE(__FILE__"/TestPadding_ResultSmallEnough", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::string set_in  = "0123456789";
    const std::string set_out = "01";
    const std::string in           = "3";
    const std::string expected_out = "00000011"; // We are expecting the output to be padded to 8 digits.

    // Exercise
    const std::string out = BaseConversion::BaseX_2_Y(in, set_in, set_out, 8);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests that nothing gets padded if the result is longer than the pad-length
TEST_CASE(__FILE__"/TestPadding_ResultLargerThanPad", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::string set_in  = "0123456789";
    const std::string set_out = "01";
    const std::string in           = "500";
    const std::string expected_out = "111110100";

    // Exercise
    const std::string out = BaseConversion::BaseX_2_Y(in, set_in, set_out, 8);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests that BaseX_2_Y is not limited by integer boundaries
TEST_CASE(__FILE__"/NotLimitedByIntegerLimits", "[BaseConversion][BaseX_2_Y]")
{
    // So what we're doing is the following:
    // 1) Store a 4096-bit integer in a string (hex)
    // 2) Convert that to base62
    // 3) Convert the base62 number back to hex
    // 4) Check if the strings are equal
    //
    // If we have integer limitations, the result should differ.

    // Setup
    const std::string set_hex  = "0123456789ABCDEF";
    const std::string set_base62 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    // Create a 4096-bit number
    const std::string hex_initial =
       "655468576D5A7134743777397A24432646294A404E635266556A586E32723575"
       "38782F4125442A472D4B6150645367566B59703373367639792442264528482B"
       "4D6251655468576D5A7134743777217A25432A462D4A404E635266556A586E32"
       "72357538782F413F4428472B4B6250645367566B597033733676397924422645"
       "2948404D6351665468576D5A7134743777217A25432A462D4A614E645267556B"
       "586E3272357538782F413F4428472B4B6250655368566D597133733676397924"
       "4226452948404D635166546A576E5A7234753777217A25432A462D4A614E6452"
       "67556B58703273357638792F413F4428472B4B6250655368566D597133743677"
       "397A244326452948404D635166546A576E5A7234753778214125442A472D4B61"
       "4E645267556B58703273357638792F423F4528482B4D6251655368566D597133"
       "743677397A24432646294A404E635266556A576E5A7234753778214125442A47"
       "2D4B6150645367566B59703273357638792F423F4528482B4D6251655468576D"
       "5A7134743677397A24432646294A404E635266556A586E327235753878214125"
       "442A472D4B6150645367566B597033733676397924423F4528482B4D62516554"
       "68576D5A7134743777217A25432A46294A404E635266556A586E327235753878"
       "2F413F4428472B4B6150645367566B5970337336763979244226452948404D63"
    ;

    // Convert to base62
    const std::string base62 = BaseConversion::BaseX_2_Y(hex_initial, set_hex, set_base62);

    // Convert back to hex
    const std::string hex_convertedBack = BaseConversion::BaseX_2_Y(base62, set_base62, set_hex);

    // Verify that hex_initial equals hex_converted
    REQUIRE(hex_initial == hex_convertedBack);
}

// Test to convert weird bases (longer to shorter)
TEST_CASE(__FILE__"/BaseLongCustom_to_ShortCustom", "[BaseConversion][BaseX_2_Y]")
{
    // Let's convert from fruits to beers

    // Setup
    const std::vector<std::string> set_in  = { "apple", "apricot", "avocado", "banana", "bell pepper", "bilberry" };
    const std::vector<std::string> set_out = { "heinecken", "corona", "flensburger", "rothaus" };
    const std::vector<std::string> in           = { "apricot", "apple", "apple" };
    const std::vector<std::string> expected_out = { "flensburger", "corona", "heinecken" };

    // Exercise
    const std::vector<std::string> out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Test to convert weird bases (shorter to longer)
TEST_CASE(__FILE__"/BaseShortCustom_to_LongCustom", "[BaseConversion][BaseX_2_Y]")
{
    // Let's convert from fruits to beers

    // Setup
    const std::vector<std::string> set_in  = { "heinecken", "corona", "flensburger", "rothaus" };
    const std::vector<std::string> set_out = { "apple", "apricot", "avocado", "banana", "bell pepper", "bilberry" };
    const std::vector<std::string> in           = { "flensburger", "corona", "heinecken" };
    const std::vector<std::string> expected_out = { "apricot", "apple", "apple" };

    // Exercise
    const std::vector<std::string> out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests that converting from a normal base to a custom base works
TEST_CASE(__FILE__"/Base10_to_fruits", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::string set_in  = "0123456789";
    const std::vector<std::string> set_out = { "apple", "apricot", "avocado", "banana", "bell pepper", "bilberry" };
    const std::string in = "599";
    const std::vector<std::string> expected_out = { "avocado", "bell pepper", "banana", "bilberry" };

    // Exercise
    const std::vector<std::string> out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests that converting from a custom base to a normal base works
TEST_CASE(__FILE__"/Base1_fruits_to_10", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::vector<std::string> set_in = { "apple", "apricot", "avocado", "banana", "bell pepper", "bilberry" };
    const std::string set_out = "0123456789";
    const std::vector<std::string> in = { "avocado", "bell pepper", "banana", "bilberry" };
    const std::string expected_out = "599";

    // Exercise
    const std::string out = BaseConversion::BaseX_2_Y(in, set_in, set_out);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests that padding with custom digit types works
TEST_CASE(__FILE__"/TestPadding_CustomDigitTypes", "[BaseConversion][BaseX_2_Y]")
{
    // Setup
    const std::string set_in  = "0123456789";
    const std::vector<std::string> set_out = { "apple", "banana", "tomato" };
    const std::string in           = "50";
    const std::vector<std::string> expected_out = { "apple", "apple", "apple", "apple", "banana", "tomato", "banana", "tomato" };

    // Exercise
    const std::vector<std::string> out = BaseConversion::BaseX_2_Y(in, set_in, set_out, 8);

    // Verify
    REQUIRE(out == expected_out);
}
