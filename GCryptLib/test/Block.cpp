#include <GCrypt/Block.h>
#include "Catch2.h"
#include <cstdlib>
#include <time.h>
#include <sstream>

using namespace Leonetienne::GCrypt;

// Tests that writing and retrieving data from a block works
TEST_CASE(__FILE__"/Write-read", "[Block]") {

  // Setup
  Block block;

  // Exercise
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i * 1024;
  }

  // Verify
  for (std::size_t i = 0; i < 16; i++) {
    REQUIRE(block.Get(i) == i * 1024);
  }
}

// Tests that the copy constructor works
TEST_CASE(__FILE__"/CCtor", "[Block]") {

  // Setup
  Block block;
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i * 1024;
  }

  // Exercise
  Block block2(block);

  // Verify
  for (std::size_t i = 0; i < 16; i++) {
    REQUIRE(block2.Get(i) == i * 1024);
  }
}

// Tests that operator= works
TEST_CASE(__FILE__"/operator=", "[Block]") {

  // Setup
  Block block;
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i * 1024;
  }

  // Exercise
  Block block2;
  block2 = block;

  // Verify
  for (std::size_t i = 0; i < 16; i++) {
    REQUIRE(block2.Get(i) == i * 1024);
  }
}

// Tests that converting to, and from, strings works
TEST_CASE(__FILE__"/StringConversion", "[Block]") {

  // Setup
  srand(time(0));
  std::stringstream ss;

  for (std::size_t i = 0; i < 512; i++) {
    ss << (rand()%2 == 0 ? '1' : '0');
  }

  // Exercise
  Block block(ss.str());

  // Verify
  REQUIRE(block.ToString() == ss.str());
}

// Tests that operator* is the same as *=
TEST_CASE(__FILE__"/operator*&=", "[Block]") {

  // Setup
  Block block1;
  for (std::size_t i = 0; i < 16; i++) {
    block1.Get(i) = i * 1024;
  }

  Block block2;
  for (std::size_t i = 0; i < 16; i++) {
    block2.Get(i) = i * 1024 * 2;
  }

  // Exercise
  Block block3 = block1 * block2;
  block1 *= block2;

  // Verify
  REQUIRE(block1 == block3);
}

// Tests that operator^ (xor) works
TEST_CASE(__FILE__"/xor", "[Block]") {

  // Setup
  Block block;
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i * 1024;
  }

  Block xorRH;
  for (std::size_t i = 0; i < 16; i++) {
    xorRH.Get(i) = i * 5099;
  }

  // Exercise
  Block result = block ^ xorRH;

  Block manualResult;
  for (std::size_t i = 0; i < 16; i++) {
    manualResult.Get(i) = (i * 1024) ^ (i * 5099);
  }

  // Verify
  REQUIRE(result == manualResult);
}

// Tests that operator^ is the same as ^=
TEST_CASE(__FILE__"/operator^&=", "[Block]") {

  // Setup
  Block block1;
  for (std::size_t i = 0; i < 16; i++) {
    block1.Get(i) = i * 1024;
  }

  Block block2;
  for (std::size_t i = 0; i < 16; i++) {
    block2.Get(i) = i * 5099 * 2;
  }

  // Exercise
  Block block3 = block1 ^ block2;
  block1 ^= block2;

  // Verify
  REQUIRE(block1 == block3);
}

// Tests that operator== works correctly
TEST_CASE(__FILE__"/operator==", "[Block]") {

  // Setup
  Block block;
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i * 1024;
  }

  SECTION("Expected true") {
    Block sameBlock;
    for (std::size_t i = 0; i < 16; i++) {
      sameBlock.Get(i) = i * 1024;
    }

    REQUIRE(block == sameBlock);
  }

  SECTION("Expected false") {
    Block otherBlock;
    for (std::size_t i = 0; i < 16; i++) {
      otherBlock.Get(i) = i * 1024 + 1;
    }

    REQUIRE_FALSE(block == otherBlock);
  }
}

// Tests that operator!= works correctly
TEST_CASE(__FILE__"/operator!=", "[Block]") {

  // Setup
  Block block;
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i * 1024;
  }

  SECTION("Expected false") {
    Block sameBlock;
    for (std::size_t i = 0; i < 16; i++) {
      sameBlock.Get(i) = i * 1024;
    }

    REQUIRE_FALSE(block != sameBlock);
  }

  SECTION("Expected true") {
    Block otherBlock;
    for (std::size_t i = 0; i < 16; i++) {
      otherBlock.Get(i) = i * 1024 + 1;
    }

    REQUIRE(block != otherBlock);
  }
}

// Tests that getting the data via the matrix accessor works
TEST_CASE(__FILE__"/matrix-accessor", "[Block]") {

  // Setup
  Block block;
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i;
  }

  // Exercise
  REQUIRE(block.Get(0,0) == 0);
  REQUIRE(block.Get(1,0) == 1);
  REQUIRE(block.Get(2,0) == 2);
  REQUIRE(block.Get(3,0) == 3);
  REQUIRE(block.Get(0,1) == 4);
  REQUIRE(block.Get(1,1) == 5);
  REQUIRE(block.Get(2,1) == 6);
  REQUIRE(block.Get(3,1) == 7);
  REQUIRE(block.Get(0,2) == 8);
  REQUIRE(block.Get(1,2) == 9);
  REQUIRE(block.Get(2,2) == 10);
  REQUIRE(block.Get(3,2) == 11);
  REQUIRE(block.Get(0,3) == 12);
  REQUIRE(block.Get(1,3) == 13);
  REQUIRE(block.Get(2,3) == 14);
  REQUIRE(block.Get(3,3) == 15);
}

// Tests that the reset method works
TEST_CASE(__FILE__"/reset", "[Block]") {

  // Setup
  Block block;
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i + 33;
  }

  // Exercise
  block.Reset();

  // Verify
  for (std::size_t i = 0; i < 16; i++) {
    REQUIRE(block[i] == 0);
  }
}
