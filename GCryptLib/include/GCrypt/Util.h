#ifndef GCRYPT_UTIL_H
#define GCRYPT_UTIL_H

#include <bitset>
#include <sstream>
#include <fstream>
#include <cstring>
#include "GCrypt/SecureBitset.h"
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

  inline Block Shiftl(const Block& bits, const std::size_t amount) {
      std::stringstream ss;
      const std::string bitss = bits.ToString();

      for (std::size_t i = 0; i < bitss.size(); i++) {
        ss << bitss[Mod((int)(i + amount), (int)bitss.size())];
      }

      return Block(ss.str());
  }

  //! Will perform a wrapping left-bitshift on a bitset
  template <std::size_t T>
    inline SecureBitset<T> Shiftl(const SecureBitset<T>& bits, const std::size_t amount) {
      std::stringstream ss;
      const std::string bitss = bits.to_string();

      for (std::size_t i = 0; i < bitss.size(); i++) {
        ss << bitss[Mod((int)(i + amount), (int)bitss.size())];
      }

      return SecureBitset<T>(ss.str());
    }

  //! Will perform a wrapping right-bitshift on a bitset
  template <std::size_t T>
    inline SecureBitset<T> Shiftr(const SecureBitset<T>& bits, const  std::size_t amount) {
      std::stringstream ss;
      const std::string bitss = bits.to_string();

      for (std::size_t i = 0; i < bitss.size(); i++) {
        ss << bitss[Mod((i - amount), bitss.size())];
      }

      return SecureBitset<T>(ss.str());
    }

  //! Will pad a string to a set length with a certain character
  std::string PadStringToLength(const std::string& str, const std::size_t len, const char pad, const bool padLeft = true);

  //! Will convert a string to a fixed-size data block
  //! @s: The string to pad
  //! padLeft: should padding be added to the left? If not, to the right.
  Block StringToBitblock(const std::string& s, bool padLeft = true);

  //! Will convert a string to a flexible data block
  Flexblock StringToBits(const std::string& s);

  //! Will convert a fixed-size data block to a bytestring
  std::string BitblockToBytes(const Block& bits);

  //! Will convert a fixed-size data block to a string
  //! The difference to BitblockToBytes() is, that it strips excess nullbytes
  std::string BitblockToString(const Block& bits);

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
}

#endif

