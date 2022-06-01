#ifndef GCRYPT_UTIL_H
#define GCRYPT_UTIL_H

#include <bitset>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include "GCrypt/Block.h"
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

  //! Will convert a string to a vector of blocks
  std::vector<Block> StringToBitblocks(const std::string& str);

  //! Will convert an array of data blocks to a bytestring
  std::string BitblocksToBytes(const std::vector<Block>& bits);

  //! Will convert an array of blocks to a character-string
  //! The difference to BitblocksToBytes() is, that it strips excess nullbytes
  std::string BitblocksToString(const std::vector<Block>& blocks);

  //! Will read a file directly to data blocks, and yield the amount of bytes read
  std::vector<Block> ReadFileToBlocks(const std::string& filepath, std::size_t& bytes_read);

  //! Will read a file directly to data blocks
  std::vector<Block> ReadFileToBlocks(const std::string& filepath);

  //! Will write data blocks directly to a file
  void WriteBlocksToFile(const std::string& filepath, const std::vector<Block>& blocks);
}

#endif

