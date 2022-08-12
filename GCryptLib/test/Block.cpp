#include <GCrypt/Block.h>
#include <GCrypt/Key.h>
#include "Catch2.h"
#include <cstdlib>
#include <time.h>
#include <sstream>
#include <iostream>

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

// Tests that converting to, and from, binary strings works
TEST_CASE(__FILE__"/BinaryStringConversion", "[Block]") {

  // Setup
  srand(time(0));
  std::stringstream ss;

  for (std::size_t i = 0; i < 512; i++) {
    ss << (rand()%2 == 0 ? '1' : '0');
  }

  // Exercise
  Block block(ss.str());

  // Verify
  REQUIRE(block.ToBinaryString() == ss.str());
}

// Tests that converting to, and from, hexstrings works
TEST_CASE(__FILE__"/HexStringConversion", "[Block]") {

  // Setup
  srand(time(0));
  std::stringstream ss;

  const std::string charset = "0123456789abcdef";
  for (std::size_t i = 0; i < 128; i++) {
    ss << charset[rand() % charset.length()];
  }

  // Exercise
  Block block;
  block.FromHexString(ss.str());

  // Verify
  REQUIRE(block.ToHexString() == ss.str());
}

// Tests that converting to, and from, bytestrings works
TEST_CASE(__FILE__"/ByteStringConversion", "[Block]") {

  // Setup
  srand(time(0));
  std::stringstream ss;

  for (std::size_t i = 0; i < 64; i++) {
    ss << (char)(rand() % 256);
  }

  // Exercise
  Block block;
  block.FromByteString(ss.str());

  // Verify
  REQUIRE(block.ToByteString() == ss.str());
}

// Tests that converting to, and from, textstrings works
TEST_CASE(__FILE__"/TextStringConversion", "[Block]") {

  // Setup
  const std::string textstr = "Hello, World :3";

  // Exercise
  Block block;
  block.FromTextString(textstr);

  // Verify
  REQUIRE(block.ToTextString() == textstr);
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

// Tests that operator+ (add) works
TEST_CASE(__FILE__"/add", "[Block]") {

  // Setup
  Block block;
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i * 1024;
  }

  Block addRH;
  for (std::size_t i = 0; i < 16; i++) {
    addRH.Get(i) = i * 5099;
  }

  // Exercise
  Block result = block + addRH;

  Block manualResult;
  for (std::size_t i = 0; i < 16; i++) {
    manualResult.Get(i) = (i * 1024) + (i * 5099);
  }

  // Verify
  REQUIRE(result == manualResult);
}

// Tests that operator+ is the same as +=
TEST_CASE(__FILE__"/operator+&=", "[Block]") {

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
  Block block3 = block1 + block2;
  block1 += block2;

  // Verify
  REQUIRE(block1 == block3);
}

// Tests that operator- (subtract) works
TEST_CASE(__FILE__"/subtract", "[Block]") {

  // Setup
  Block block;
  for (std::size_t i = 0; i < 16; i++) {
    block.Get(i) = i * 1024;
  }

  Block subRH;
  for (std::size_t i = 0; i < 16; i++) {
    subRH.Get(i) = i * 5099;
  }

  // Exercise
  Block result = block - subRH;

  Block manualResult;
  for (std::size_t i = 0; i < 16; i++) {
    manualResult.Get(i) = (i * 1024) - (i * 5099);
  }

  // Verify
  REQUIRE(result == manualResult);
}

// Tests that operator- is the same as -=
TEST_CASE(__FILE__"/operator-&=", "[Block]") {

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
  Block block3 = block1 - block2;
  block1 -= block2;

  // Verify
  REQUIRE(block1 == block3);
}

// Tests that subtraction undoes addition, and vica versa
TEST_CASE(__FILE__"/subtraction-undoes-addition", "[Block]") {
  // Setup
  const Block a = Key::Random();
  const Block b = Key::Random();

  // Exercise
  const Block a_plus_b = a + b;
  const Block a_plus_b_minus_b = a_plus_b - b;

  // Verify
  REQUIRE(a == a_plus_b_minus_b);
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

// Tests that shift rows up works
TEST_CASE(__FILE__"/shift-rows-up", "[Block]") {

  // Setup
  Block a;
  a.Get(0,0) = 10; a.Get(0,1) = 11; a.Get(0,2) = 12; a.Get(0,3) = 13;
  a.Get(1,0) = 20; a.Get(1,1) = 21; a.Get(1,2) = 22; a.Get(1,3) = 23;
  a.Get(2,0) = 30; a.Get(2,1) = 31; a.Get(2,2) = 32; a.Get(2,3) = 33;
  a.Get(3,0) = 40; a.Get(3,1) = 41; a.Get(3,2) = 42; a.Get(3,3) = 43;

  Block e; /* expected */
  e.Get(0,0) = 20; e.Get(0,1) = 21; e.Get(0,2) = 22; e.Get(0,3) = 23;
  e.Get(1,0) = 30; e.Get(1,1) = 31; e.Get(1,2) = 32; e.Get(1,3) = 33;
  e.Get(2,0) = 40; e.Get(2,1) = 41; e.Get(2,2) = 42; e.Get(2,3) = 43;
  e.Get(3,0) = 10; e.Get(3,1) = 11; e.Get(3,2) = 12; e.Get(3,3) = 13;

  // Exercise
  a.ShiftRowsUpInplace();

  // Verify
  REQUIRE(a == e);
}

// Tests that ShiftRowsUpInplace() does the exact same thing as ShiftRowsUp()
TEST_CASE(__FILE__"/shift-rows-up-same-as-inplace", "[Block]") {
  // Setup
  Block a = Key::Random();
  const Block initial_a = a;

  // Exercise and verify
  a.ShiftRowsUpInplace();
  REQUIRE(a == initial_a.ShiftRowsUp());
}

// Tests that shift rows down works
TEST_CASE(__FILE__"/shift-rows-down", "[Block]") {

  // Setup
  Block a;
  a.Get(0,0) = 10; a.Get(0,1) = 11; a.Get(0,2) = 12; a.Get(0,3) = 13;
  a.Get(1,0) = 20; a.Get(1,1) = 21; a.Get(1,2) = 22; a.Get(1,3) = 23;
  a.Get(2,0) = 30; a.Get(2,1) = 31; a.Get(2,2) = 32; a.Get(2,3) = 33;
  a.Get(3,0) = 40; a.Get(3,1) = 41; a.Get(3,2) = 42; a.Get(3,3) = 43;

  Block e; /* expected */
  e.Get(0,0) = 40; e.Get(0,1) = 41; e.Get(0,2) = 42; e.Get(0,3) = 43;
  e.Get(1,0) = 10; e.Get(1,1) = 11; e.Get(1,2) = 12; e.Get(1,3) = 13;
  e.Get(2,0) = 20; e.Get(2,1) = 21; e.Get(2,2) = 22; e.Get(2,3) = 23;
  e.Get(3,0) = 30; e.Get(3,1) = 31; e.Get(3,2) = 32; e.Get(3,3) = 33;

  // Exercise
  a.ShiftRowsDownInplace();

  // Verify
  REQUIRE(a == e);
}

// Tests that ShiftRowsDownInplace() does the exact same thing as ShiftRowsDown()
TEST_CASE(__FILE__"/shift-rows-down-same-as-inplace", "[Block]") {
  // Setup
  Block a = Key::Random();
  const Block initial_a = a;

  // Exercise and verify
  a.ShiftRowsDownInplace();
  REQUIRE(a == initial_a.ShiftRowsDown());
}

// Tests that shift columns left works
TEST_CASE(__FILE__"/shift-columns-left", "[Block]") {

  // Setup
  Block a;
  a.Get(0,0) = 10; a.Get(0,1) = 11; a.Get(0,2) = 12; a.Get(0,3) = 13;
  a.Get(1,0) = 20; a.Get(1,1) = 21; a.Get(1,2) = 22; a.Get(1,3) = 23;
  a.Get(2,0) = 30; a.Get(2,1) = 31; a.Get(2,2) = 32; a.Get(2,3) = 33;
  a.Get(3,0) = 40; a.Get(3,1) = 41; a.Get(3,2) = 42; a.Get(3,3) = 43;

  Block e; /* expected */
  e.Get(0,0) = 11; e.Get(0,1) = 12; e.Get(0,2) = 13; e.Get(0,3) = 10;
  e.Get(1,0) = 21; e.Get(1,1) = 22; e.Get(1,2) = 23; e.Get(1,3) = 20;
  e.Get(2,0) = 31; e.Get(2,1) = 32; e.Get(2,2) = 33; e.Get(2,3) = 30;
  e.Get(3,0) = 41; e.Get(3,1) = 42; e.Get(3,2) = 43; e.Get(3,3) = 40;

  // Exercise
  a.ShiftColumnsLeftInplace();

  // Verify
  REQUIRE(a == e);
}

// Tests that ShiftColumnsLeftInplace()() does the exact same thing as ShiftColumnsLeft()
TEST_CASE(__FILE__"/shift-columns-left-same-as-inplace", "[Block]") {
  // Setup
  Block a = Key::Random();
  const Block initial_a = a;

  // Exercise and verify
  a.ShiftColumnsLeftInplace();
  REQUIRE(a == initial_a.ShiftColumnsLeft());
}

// Tests that shift columns right works
TEST_CASE(__FILE__"/shift-columns-right", "[Block]") {

  // Setup
  Block a;
  a.Get(0,0) = 10; a.Get(0,1) = 11; a.Get(0,2) = 12; a.Get(0,3) = 13;
  a.Get(1,0) = 20; a.Get(1,1) = 21; a.Get(1,2) = 22; a.Get(1,3) = 23;
  a.Get(2,0) = 30; a.Get(2,1) = 31; a.Get(2,2) = 32; a.Get(2,3) = 33;
  a.Get(3,0) = 40; a.Get(3,1) = 41; a.Get(3,2) = 42; a.Get(3,3) = 43;

  Block e; /* expected */
  e.Get(0,0) = 13; e.Get(0,1) = 10; e.Get(0,2) = 11; e.Get(0,3) = 12;
  e.Get(1,0) = 23; e.Get(1,1) = 20; e.Get(1,2) = 21; e.Get(1,3) = 22;
  e.Get(2,0) = 33; e.Get(2,1) = 30; e.Get(2,2) = 31; e.Get(2,3) = 32;
  e.Get(3,0) = 43; e.Get(3,1) = 40; e.Get(3,2) = 41; e.Get(3,3) = 42;

  // Exercise
  a.ShiftColumnsRightInplace();

  // Verify
  REQUIRE(a == e);
}

// Tests that ShiftColumnsRightInplace()() does the exact same thing as ShiftColumnsRight()
TEST_CASE(__FILE__"/shift-columns-right-same-as-inplace", "[Block]") {
  // Setup
  Block a = Key::Random();
  const Block initial_a = a;

  // Exercise and verify
  a.ShiftColumnsRightInplace();
  REQUIRE(a == initial_a.ShiftColumnsRight());
}

// Tests that shift cells left works
TEST_CASE(__FILE__"/shift-cells-left", "[Block]") {

  // Setup
  Block a;
  for (std::size_t i = 0; i < 16; i++) {
    a.Get(i) = i;
  }

  Block expected;
  for (std::size_t i = 0; i < 15; i++) {
    expected.Get(i) = i+1;
  }
  expected.Get(15) = 0;

  // Exercise
  a.ShiftCellsLeftInplace();

  // Verify
  REQUIRE(a == expected);
}

// Tests that ShiftCellsLeftInplace()() does the exact same thing as ShiftCellsLeft()
TEST_CASE(__FILE__"/shift-cells-left-same-as-inplace", "[Block]") {
  // Setup
  Block a = Key::Random();
  const Block initial_a = a;

  // Exercise and verify
  a.ShiftCellsLeftInplace();
  REQUIRE(a == initial_a.ShiftCellsLeft());
}

// Tests that shift cells right works
TEST_CASE(__FILE__"/shift-cells-right", "[Block]") {

  // Setup
  Block a;
  for (std::size_t i = 0; i < 16; i++) {
    a.Get(i) = i;
  }

  Block expected;
  for (std::size_t i = 1; i < 16; i++) {
    expected.Get(i) = i-1;
  }
  expected.Get(0) = 15;

  // Exercise
  a.ShiftCellsRightInplace();

  // Verify
  REQUIRE(a == expected);
}

// Tests that ShiftCellsRightInplace()() does the exact same thing as ShiftCellsRight()
TEST_CASE(__FILE__"/shift-cells-right-same-as-inplace", "[Block]") {
  // Setup
  Block a = Key::Random();
  const Block initial_a = a;

  // Exercise and verify
  a.ShiftCellsRightInplace();
  REQUIRE(a == initial_a.ShiftCellsRight());
}

// Tests that shifting down undoes shifting up, and vica versa
TEST_CASE(__FILE__"/shift-down-undoes-shift-up", "[Block]") {
  // Setup
  Block a = Key::Random();
  const Block initial_a = a;

  // Exercise
  a.ShiftRowsUpInplace();
  a.ShiftRowsDownInplace();

  // Verify
  REQUIRE(a == initial_a);
}

// Tests that shifting left undoes shifting right, and vica versa
TEST_CASE(__FILE__"/shift-left-undoes-shift-right", "[Block]") {
  // Setup
  Block a = Key::Random();
  const Block initial_a = a;

  // Exercise
  a.ShiftColumnsRightInplace();
  a.ShiftColumnsLeftInplace();

  // Verify
  REQUIRE(a == initial_a);
}

// Tests that shifting cells left undoes shifting cells right, and vica versa
TEST_CASE(__FILE__"/cellshift-left-undoes-cellshift-right", "[Block]") {
  // Setup
  Block a = Key::Random();
  const Block initial_a = a;

  // Exercise
  a.ShiftCellsRightInplace();
  a.ShiftCellsLeftInplace();

  // Verify
  REQUIRE(a == initial_a);
}

// Tests that multiple, combined shifts and additions can be undone
TEST_CASE(__FILE__"/multiple-combined-shifts-and-additions-can-be-undone", "[Block]") {
  // Setup
  Block a = Key::Random();
  Block key = Key::FromPassword("Papaya");

  const Block initial_a = a;

  // Exercise (mix-up)
  for (std::size_t i = 0; i < 64; i++) {
    a.ShiftRowsUpInplace();
    a.ShiftColumnsLeftInplace();
    a += key;
    a.ShiftCellsRightInplace();
  }

  // Exercise (un-mix)
  for (std::size_t i = 0; i < 64; i++) {
    a.ShiftCellsLeftInplace();
    a -= key;
    a.ShiftColumnsRightInplace();
    a.ShiftRowsDownInplace();
  }

  // Verify
  REQUIRE(a == initial_a);
}

// Tests that the get-bit method works
TEST_CASE(__FILE__"/get-bit", "[Block]") {
  for (std::size_t i = 0; i < 100; i++) {
    // Setup
    Block a = Key::Random();

    // Exercise
    std::stringstream ss;
    for (std::size_t i = 0; i < 512; i++) {
      ss << a.GetBit(i);
    }

    // Verify
    REQUIRE(ss.str() == a.ToBinaryString());
  }
}

// Tests that the set-bit method works
TEST_CASE(__FILE__"/set-bit", "[Block]") {
  for (std::size_t i = 0; i < 100; i++) {
    // Setup
    const std::string a_bits = Key::Random().ToBinaryString();

    // Exercise
    Block a;
    for (std::size_t i = 0; i < 512; i++) {
      a.SetBit(i, a_bits[i] == '1');
    }

    // Verify
    REQUIRE(a_bits == a.ToBinaryString());
  }
}

// Tests that the set-bit to-false method works
TEST_CASE(__FILE__"/set-bit-to-false", "[Block]") {
  // Setup
  Block a = Key::Random();

  // Exercise
  a.SetBit(5, 0);
  a.SetBit(15, 0);
  a.SetBit(105, 0);
  a.SetBit(205, 0);

  // Verify
  REQUIRE(a.GetBit(5) == false);
  REQUIRE(a.GetBit(15) == false);
  REQUIRE(a.GetBit(105) == false);
  REQUIRE(a.GetBit(205) == false);
}

// Tests that the set-bit to-true method works
TEST_CASE(__FILE__"/set-bit-to-true", "[Block]") {
  // Setup
  Block a = Key::Random();

  // Exercise
  a.SetBit(5, 1);
  a.SetBit(15, 1);
  a.SetBit(105, 1);
  a.SetBit(205, 1);

  // Verify
  REQUIRE(a.GetBit(5) == true);
  REQUIRE(a.GetBit(15) == true);
  REQUIRE(a.GetBit(105) == true);
  REQUIRE(a.GetBit(205) == true);
}

// Tests that the flip-bit method works
TEST_CASE(__FILE__"/flip-bit", "[Block]") {
  // Setup
  Block a = Key::Random();

  std::string compare = a.ToBinaryString();
  compare[5]   = compare[5]   == '1' ? '0' : '1';
  compare[15]  = compare[15]  == '1' ? '0' : '1';
  compare[105] = compare[105] == '1' ? '0' : '1';
  compare[205] = compare[205] == '1' ? '0' : '1';

  // Exercise
  a.FlipBit(5);
  a.FlipBit(15);
  a.FlipBit(105);
  a.FlipBit(205);

  // Verify
  REQUIRE(a.ToBinaryString() == compare);
}

// Tests that bitshifts (to the left) work
TEST_CASE(__FILE__"/bitshift-left", "[Block]") {

  // Setup
  srand(time(0));
  std::stringstream ss;

  for (std::size_t i = 0; i < 512; i++) {
    ss << (rand()%2 == 0 ? '1' : '0');
  }
  const std::string originalBits = ss.str();
  ss.str("");

  // Shift string manually
  std::string shiftedBits = originalBits;
  shiftedBits.erase(0, 1);
  ss << shiftedBits << originalBits[0];
  shiftedBits = ss.str();

  // Create block of original bits
  Block block(originalBits);

  // Exercise
  block = block.ShiftBitsLeft();

  // Verify
  REQUIRE(block.ToBinaryString().length() == shiftedBits.length());
  REQUIRE(block.ToBinaryString() == shiftedBits);
}

// Tests that inplace-bitshifts to the left do the exact same as copy bitshifts
TEST_CASE(__FILE__"/bitshift-left-inplace", "[Block]") {

  // Setup
  srand(time(0));
  std::stringstream ss;

  for (std::size_t i = 0; i < 512; i++) {
    ss << (rand()%2 == 0 ? '1' : '0');
  }
  Block a(ss.str());

  // Exercise
  Block b = a.ShiftBitsLeft();
  a.ShiftBitsLeftInplace();

  // Verify
  REQUIRE(a == b);
}

// Tests that bitshifts (to the right) work
TEST_CASE(__FILE__"/bitshift-right", "[Block]") {

  // Setup
  srand(time(0));
  std::stringstream ss;

  for (std::size_t i = 0; i < 512; i++) {
    ss << (rand()%2 == 0 ? '1' : '0');
  }
  const std::string originalBits = ss.str();
  ss.str("");

  // Shift string manually
  std::string shiftedBits = originalBits;
  shiftedBits.erase(shiftedBits.length() - 1);
  ss << originalBits[originalBits.length()-1] << shiftedBits;
  shiftedBits = ss.str();

  // Create block of original bits
  Block block(originalBits);

  // Exercise
  block = block.ShiftBitsRight();

  // Verify
  REQUIRE(block.ToBinaryString().length() == shiftedBits.length());
  REQUIRE(block.ToBinaryString() == shiftedBits);
}

// Tests that inplace-bitshifts to the right do the exact same as copy bitshifts
TEST_CASE(__FILE__"/bitshift-right-inplace", "[Block]") {

  // Setup
  srand(time(0));
  std::stringstream ss;

  for (std::size_t i = 0; i < 512; i++) {
    ss << (rand()%2 == 0 ? '1' : '0');
  }
  Block a(ss.str());

  // Exercise
  Block b = a.ShiftBitsRight();
  a.ShiftBitsRightInplace();

  // Verify
  REQUIRE(a == b);
}

// Tests that bitshifting undoes itself
TEST_CASE(__FILE__"/bitshifting-undoes-itself", "[Block]") {

  // Setup
  Block a = Key::Random();

  const Block initial_a = a;

  // Exercise (mix-up)
  for (std::size_t i = 0; i < 100; i++) {
    a.ShiftBitsLeftInplace();
  }

  // Exercise (un-mix)
  for (std::size_t i = 0; i < 100; i++) {
    a.ShiftBitsRightInplace();
  }

  // Verify
  REQUIRE(a == initial_a);
}

