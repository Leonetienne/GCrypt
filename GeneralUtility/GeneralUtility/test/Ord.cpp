#include <GeneralUtility/ContainerUtility.h>
#include "Catch2.h"
#include <string>
#include <vector>

using namespace Leonetienne::GeneralUtility;

// Tests that the Ord method works with characters in a string
TEST_CASE(__FILE__"/WorksWithCharsInString", "[ContainerUtility][Ord]")
{
    const std::string set = "0123456789abcdef";

    REQUIRE(ContainerUtility::Ord('0', set) == 0);
    REQUIRE(ContainerUtility::Ord('1', set) == 1);
    REQUIRE(ContainerUtility::Ord('2', set) == 2);
    REQUIRE(ContainerUtility::Ord('3', set) == 3);
    REQUIRE(ContainerUtility::Ord('4', set) == 4);
    REQUIRE(ContainerUtility::Ord('5', set) == 5);
    REQUIRE(ContainerUtility::Ord('6', set) == 6);
    REQUIRE(ContainerUtility::Ord('7', set) == 7);
    REQUIRE(ContainerUtility::Ord('8', set) == 8);
    REQUIRE(ContainerUtility::Ord('9', set) == 9);
    REQUIRE(ContainerUtility::Ord('a', set) == 10);
    REQUIRE(ContainerUtility::Ord('b', set) == 11);
    REQUIRE(ContainerUtility::Ord('c', set) == 12);
    REQUIRE(ContainerUtility::Ord('d', set) == 13);
    REQUIRE(ContainerUtility::Ord('e', set) == 14);
    REQUIRE(ContainerUtility::Ord('f', set) == 15);
}

// Tests that, if an object is not found, -1 is returned
TEST_CASE(__FILE__"/ReturnsNeg1IfNotFound", "[ContainerUtility][Ord]")
{
    const std::string set = "0123456789abcdef";

    REQUIRE(ContainerUtility::Ord('z', set) == -1);
}

// Tests that Ord works with vectors <string>
TEST_CASE(__FILE__"/WorksWithVector_String", "[ContainerUtility][Ord]")
{
    const std::vector<std::string> vec = { "Apple", "Banana", "Tomato", "Olives" };

    REQUIRE(ContainerUtility::Ord(std::string("Apple"), vec) == 0);
    REQUIRE(ContainerUtility::Ord(std::string("Banana"), vec) == 1);
    REQUIRE(ContainerUtility::Ord(std::string("Tomato"), vec) == 2);
    REQUIRE(ContainerUtility::Ord(std::string("Olives"), vec) == 3);

    INFO("Now testing that unknown is -1");
    REQUIRE(ContainerUtility::Ord(std::string("Pepper"), vec) == -1);
}

// Tests that Ord works with vectors <int>
TEST_CASE(__FILE__"/WorksWithVector_Int", "[ContainerUtility][Ord]")
{
    const std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (std::size_t i = 0 ; i < 10; i++)
        REQUIRE(ContainerUtility::Ord((int)i, vec) == i);

    INFO("Now testing that unknown is -1");
    REQUIRE(ContainerUtility::Ord((int)99, vec) == -1);
}
