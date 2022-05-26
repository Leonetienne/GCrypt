#ifndef GCRYPT_UTIL_H
#define GCRYPT_UTIL_H

#include <bitset>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include "GCrypt/Block.h"
#include "GCrypt/Flexblock.h"
#include "GCrypt/Config.h"
#include "GCrypt/GCipher.h"
#include "GCrypt/InitializationVector.h"

namespace Leonetienne::GCrypt {
  //! Mod-operator that works with negative values
  inline int Mod(const int numerator, const int denominator) {
    return (denominator + (numerator % denominator)) % denominator;
  }

  //! Will pad a string to a set length with a certain character
  std::string PadStringToLength(const std::string& str, const std::size_t len, const char pad, const bool padLeft = true);

  //! Will convert a string to a fixed-size data block
  //! @s: The string to pad
  //! padLeft: should padding be added to the left? If not, to the right.
  Block StringToBitblock(const std::string& s, bool padLeft = true);

  //! Will convert a string to a flexible data block
  Flexblock StringToBits(const std::string& s);

  //! Will convert a string to a vector of blocks
  std::vector<Block> StringToBitblocks(const std::string& s);

  //! Will convert a fixed-size data block to a bytestring
  std::string BitblockToBytes(const Block& block);

  //! Will convert an array of data blocks to a bytestring
  std::string BitblocksToBytes(const std::vector<Block>& bits);

  //! Will convert a fixed-size data blocks to a textstring
  //! The difference to BitblockToBytes() is, that it strips excess nullbytes
  std::string BitblockToString(const Block& block);

  //! Will convert an array of blocks to a character-string
  //! The difference to BitblocksToBytes() is, that it strips excess nullbytes
  std::string BitblocksToString(const std::vector<Block>& blocks);

  //! Will convert a flexible data block to a bytestring
  std::string BitsToBytes(const Flexblock& bits);

  //! Will convert a flexible data block to a string
  //! The difference to BitsToBytes() is, that it strips excess nullbytes
  std::string BitsToString(const Flexblock& bits);

  //! Turns a fixed-size data block into a hex-string
  std::string BitblockToHexstring(const Block& b);

  //! Turns a flexible data block into a hex-string
  std::string BitsToHexstring(const Flexblock& b);

  //! Turns a hex string into a fixed-size data block
  Block HexstringToBitblock(const std::string& hexstring);

  //! Turns a hex string into a flexible data block
  Flexblock HexstringToBits(const std::string& hexstring);

  //! Will read a file into a flexblock
  Flexblock ReadFileToBits(const std::string& filepath);

  //! Will save bits to a binary file
  void WriteBitsToFile(const std::string& filepath, const Flexblock& bits);

  //! Will read a file directly to data blocks
  std::vector<Block> ReadFileToBlocks(const std::string& filepath);

  //! Will write data blocks directly to a file
  void WriteBlocksToFile(const std::string& filepath, const std::vector<Block>& blocks);
}

#endif

