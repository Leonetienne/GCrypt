#include <GeneralUtility/BaseConversion.h>
#include "Catch2.h"

using namespace Leonetienne::GeneralUtility;

// Tests base 10 to 10
TEST_CASE(__FILE__"/Base10_to_10", "[BaseConversion][Base10_2_X]")
{
    // Setup
    const std::string set = "0123456789";
    const std::uint64_t in         =  1990381;
    const std::string expected_out = "1990381";

    // Exercise
    const std::string out = BaseConversion::Base10_2_X(in, set);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 10 to 16
TEST_CASE(__FILE__"/Base10_to_16", "[BaseConversion][Base10_2_X]")
{
    // Setup
    const std::string set = "0123456789abcdef";
    const std::uint64_t in         =  0x1990381;
    const std::string expected_out = "1990381";

    // Exercise
    const std::string out = BaseConversion::Base10_2_X(in, set);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 10 to 2
TEST_CASE(__FILE__"/Base10_to_2", "[BaseConversion][Base10_2_X]")
{
    // Setup
    const std::string set = "01";
    const std::uint64_t in         = 0b10111011;
    const std::string expected_out =  "10111011";

    // Exercise
    const std::string out = BaseConversion::Base10_2_X(in, set);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 10 to fucking big
TEST_CASE(__FILE__"/Base10_to_FuckingBig", "[BaseConversion][Base10_2_X]")
{
    // Setup
    const std::string set = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::uint64_t in =  189434143264827;
    const std::string expected_out = "rn5qZuTD";

    // Exercise
    const std::string out = BaseConversion::Base10_2_X(in, set);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests that padding works
TEST_CASE(__FILE__"/TestPadding", "[BaseConversion][Base10_2_X]")
{
    // Setup
    const std::string set = "01";
    const std::uint64_t in         = 0b101;
    const std::string expected_out =  "00000101";

    // Exercise
    const std::string out = BaseConversion::Base10_2_X(in, set, 8);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests that conversion with more complex 'digits' works. Weird-ass usecase
TEST_CASE(__FILE__"/BaseWeird_to_10", "[BaseConversion][Base10_2_X]")
{
    // Setup

    // Yes... That is actually our base...
    const std::vector<std::string> set = { "Banana", "Apple", "Peach", "Strawberry", "Tomato", "Cherry" };

    const std::uint64_t in = 69;

    // Yes, that is a fucking number. It's 69 in whatever the fuck that base is
    const std::vector<std::string> expected_out = { "Apple", "Cherry", "Strawberry" };

    // Exercise
    const std::vector<std::string> out = BaseConversion::Base10_2_X(in, set);

    // Verify
    REQUIRE(out == expected_out);
}
