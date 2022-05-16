#include <GeneralUtility/BaseConversion.h>
#include "Catch2.h"
#include <string>
#include <vector>
#include <time.h>
#include <stdexcept>

using namespace Leonetienne::GeneralUtility;

// Tests that basic division (base10) is working, with oracle values
TEST_CASE(__FILE__"/Base10", "[BaseConversion][DigitstringDivision]")
{
    const std::string set = "0123456789";

    SECTION("No rest") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("200", 10, set);
        REQUIRE(res.first == "20");
        REQUIRE(res.second == 0);
    }

    SECTION("With rest") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("205", 10, set);
        REQUIRE(res.first == "20");
        REQUIRE(res.second == 5);
    }

    SECTION("With rest, and divisor > dividend") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("205", 299, set);
        REQUIRE(res.first == "0");
        REQUIRE(res.second == 205);
    }
}

// Tests that basic division (base10) is working, by doing a lot of random, precalculated divisions
TEST_CASE(__FILE__"/Base10_Randoms", "[BaseConversion][DigitstringDivision]")
{
    srand(time(0));
    const std::string set = "0123456789";

    // Test 1000 random numbers
    for (std::size_t i = 0; i < 1000; i++) {
        // Generate random input
        const std::uint64_t i_dividend = rand();
        const std::uint32_t i_divisor = rand() % 1000000;

        // Precalculate expected output
        const std::uint32_t expected_result = i_dividend / i_divisor;
        const std::uint32_t expected_rest   = i_dividend % i_divisor;

        // Convert dividend to a string
        std::stringstream ss;
        ss << i_dividend;
        const std::string dividend = ss.str();

        // Compute results
        const auto res = BaseConversion::DigitstringDivision(dividend, i_divisor, set);
        const std::uint32_t actual_result = std::stoi(res.first);
        const std::uint32_t actual_rest   = res.second;

        // Assertions
        REQUIRE(actual_result == expected_result);
        REQUIRE(actual_rest == expected_rest);
    }
}

// Tests that base16 division is working, with oracle values
TEST_CASE(__FILE__"/Base16", "[BaseConversion][DigitstringDivision]")
{
    const std::string set = "0123456789abcdef";

    SECTION("No rest") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("1f4", 10, set);
        REQUIRE(res.first == "32");
        REQUIRE(res.second == 0);
    }

    SECTION("With rest") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("1fd", 10, set);
        REQUIRE(res.first == "32");
        REQUIRE(res.second == 9);
    }

    SECTION("With rest, and divisor > dividend") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("1f4", 999, set);
        REQUIRE(res.first == "0");
        REQUIRE(res.second == 500);
    }
}

// Tests that base2 division is working, with oracle values
TEST_CASE(__FILE__"/Base2", "[BaseConversion][DigitstringDivision]")
{
    const std::string set = "01";

    SECTION("No rest") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("111110100", 10, set);
        REQUIRE(res.first == "110010");
        REQUIRE(res.second == 0);
    }

    SECTION("With rest") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("111111011", 10, set);
        REQUIRE(res.first == "110010");
        REQUIRE(res.second == 7);
    }
    SECTION("With rest, and divisor > dividend") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("111110100", 599, set);
        REQUIRE(res.first == "0");
        REQUIRE(res.second == 500);
    }
}

// Tests that fucking big bases are working, with oracle values
TEST_CASE(__FILE__"/BaseFuckingBig", "[BaseConversion][DigitstringDivision]")
{
    const std::string set = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    SECTION("No rest") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("9RieQV", 29915, set);
        REQUIRE(res.first == "1DGL");
        REQUIRE(res.second == 0);
    }

    SECTION("With rest") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("9RieQZ", 29915, set);
        REQUIRE(res.first == "1DGL");
        REQUIRE(res.second == 4);
    }
    SECTION("With rest, and divisor > dividend") {
        const auto res = BaseConversion::DigitstringDivision<std::string>("8x", 600, set);
        REQUIRE(res.first == "0");
        REQUIRE(res.second == 555);
    }
}

// Tests that having a dividend of size 0 returns 0
TEST_CASE(__FILE__"/DividendLengthZero", "[BaseConversion][DigitstringDivision]")
{
    const auto& res = BaseConversion::DigitstringDivision<std::string>("", 19, "0123456789");
    REQUIRE(res.first == "0");
    REQUIRE(res.second == 0);
}

// Tests that a division by zero exception is thrown when appropriate
TEST_CASE(__FILE__"/DivisionByZero", "[BaseConversion][DigitstringDivision]")
{
    REQUIRE_THROWS_AS(
            BaseConversion::DigitstringDivision<std::string>("699", 0, "0123456789")
    , std::overflow_error);
}

// Tests that a logic error is thrown when the supplied set is empty
TEST_CASE(__FILE__"/NoSetSupplied", "[BaseConversion][DigitstringDivision]")
{
    REQUIRE_THROWS_AS(
            BaseConversion::DigitstringDivision<std::string>("699", 5, "")
    , std::logic_error);
}

// Tests that a logic error is thrown if the dividend contains a character not present in the set
TEST_CASE(__FILE__"/InvalidDigitsInDividend", "[BaseConversion][DigitstringDivision]")
{
    REQUIRE_THROWS_AS(
            BaseConversion::DigitstringDivision<std::string>("699z", 5, "0123465789")
    , std::logic_error);
}

// Tests that weird, abstract bases are working, with oracle values
TEST_CASE(__FILE__"/BaseWeird", "[BaseConversion][DigitstringDivision]")
{
    const std::vector<std::string> set = { "apple", "apricot", "avocado", "banana", "bell pepper", "bilberry" };

    SECTION("No rest") {
        const auto res = BaseConversion::DigitstringDivision<std::vector<std::string>>({"apricot", "bilberry", "bell pepper"}, 10, set);
        REQUIRE(res.first == std::vector<std::string>({"apricot", "apricot"}));
        REQUIRE(res.second == 0);
    }
    SECTION("With rest") {
       const auto res = BaseConversion::DigitstringDivision<std::vector<std::string>>({"avocado", "apple", "banana"}, 10, set);
       REQUIRE(res.first == std::vector<std::string>({"apricot", "apricot"}));
       REQUIRE(res.second == 5);
    }
    SECTION("With rest, and divisor > dividend") {
       const auto res = BaseConversion::DigitstringDivision<std::vector<std::string>>({"apricot", "bilberry", "bell pepper"}, 100, set);
       REQUIRE(res.first == std::vector<std::string>({"apple"}));
       REQUIRE(res.second == 70);
    }
}
