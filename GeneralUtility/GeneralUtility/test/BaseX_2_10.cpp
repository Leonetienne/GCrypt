#include <GeneralUtility/BaseConversion.h>
#include "Catch2.h"

using namespace Leonetienne::GeneralUtility;

// Tests base 10 to 10
TEST_CASE(__FILE__"/Base10_to_10", "[BaseConversion][BaseX_2_10]")
{
    // Setup
    const std::string set = "0123456789";
    const std::string in             = "1990381";
    const std::uint64_t expected_out =  1990381;

    // Exercise
    const std::uint64_t out = BaseConversion::BaseX_2_10(in, set);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 16 to 10
TEST_CASE(__FILE__"/Base16_to_10", "[BaseConversion][BaseX_2_10]")
{
    // Setup
    const std::string set = "0123456789abcdef";
    const std::string in             =  "1a83f9cefa";
    const std::uint64_t expected_out = 0x1a83f9cefa;

    // Exercise
    const std::uint64_t out = BaseConversion::BaseX_2_10(in, set);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base 2 to 10
TEST_CASE(__FILE__"/Base2_to_10", "[BaseConversion][BaseX_2_10]")
{
    // Setup
    const std::string set = "01";
    const std::string in             =  "10101010110000111111";
    const std::uint64_t expected_out = 0b10101010110000111111;

    // Exercise
    const std::uint64_t out = BaseConversion::BaseX_2_10(in, set);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests base fucking big to 10
TEST_CASE(__FILE__"/BaseFuckingBig_to_10", "[BaseConversion][BaseX_2_10]")
{
    // Setup
    const std::string set = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::string in =  "rn5qZuTD";
    const std::uint64_t expected_out = 189434143264827;

    // Exercise
    const std::uint64_t out = BaseConversion::BaseX_2_10(in, set);

    // Verify
    REQUIRE(out == expected_out);
}

// Tests that a logic error is thrown when the supplied set is empty
TEST_CASE(__FILE__"/NoSetSupplied", "[BaseConversion][BaseX_2_10]")
{
    REQUIRE_THROWS_AS(
            BaseConversion::BaseX_2_10<std::string>("699", "")
    , std::logic_error);
}

// Tests that conversion with more complex 'digits' works. Weird-ass usecase
TEST_CASE(__FILE__"/BaseWeird_to_10", "[BaseConversion][BaseX_2_10]")
{
    // Setup

    // Yes... That is actually our base...
    const std::vector<std::string> set = { "Banana", "Apple", "Peach", "Strawberry", "Tomato", "Cherry" };

    // Yes, that is a fucking number. It's 69 in whatever the fuck that base is
    const std::vector<std::string> in = { "Apple", "Cherry", "Strawberry" };

    const std::uint64_t expected_out =  69;

    // Exercise
    const std::uint64_t out = BaseConversion::BaseX_2_10(in, set);

    // Verify
    REQUIRE(out == expected_out);
}
