#include <unordered_map>
#include "GCrypt/Feistel.h"
#include "GCrypt/Util.h"
#include "GCrypt/Config.h"

namespace Leonetienne::GCrypt {

  Feistel::Feistel(const Key& key) {
    SetKey(key);
    return;
  }

  Feistel::~Feistel() {
    ZeroKeyMemory();

    return;
  }

  void Feistel::SetKey(const Key& key) {
    GenerateRoundKeys(key);
    return;
  }

  Block Feistel::Encipher(const Block& data) {
    return Run(data, false);
  }

  Block Feistel::Decipher(const Block& data) {
    return Run(data, true);
  }

  Block Feistel::Run(const Block& data, bool reverseKeys) {
    const auto splitData = FeistelSplit(data);
    Halfblock l = splitData.first;
    Halfblock r = splitData.second;

    Halfblock tmp;

    for (std::size_t i = 0; i < N_ROUNDS; i++) {
      // Calculate key index
      std::size_t keyIndex;
      if (reverseKeys) {
        keyIndex = N_ROUNDS - i - 1;
      }
      else {
        keyIndex = i;
      }

      // Do a feistel round
      tmp = r;
      r = l ^ F(r, roundKeys[keyIndex]);
      l = tmp;
    }

    // Block has finished de*ciphering.
    // Let's generate a new set of round keys.
    GenerateRoundKeys((Key)roundKeys.back());

    return FeistelCombine(r, l);
  }

  Halfblock Feistel::F(Halfblock m, const Key& key) {

    // Made-up F function
    // Expand to full bitwidth
    Block m_expanded = ExpansionFunction(m);

    // Mix up the block a bit
    m_expanded.ShiftCellsRightInplace();
    m_expanded.ShiftRowsUpInplace();

    // Matrix-mult with key (this is irreversible)
    m_expanded *= key;

    // Now do a bitshift
    m_expanded.ShiftBitsLeftInplace();

    // Non-linearly apply subsitution boxes
    std::stringstream ss;
    const std::string m_str = m_expanded.ToString();
    for (std::size_t i = 0; i < BLOCK_SIZE; i += 4) {
      ss << SBox(m_str.substr(i, 4));
    }
    m_expanded = Block(ss.str());

    // Return the compressed version, shifted by 3
    //return Shiftl(CompressionFunction(m_expanded), 3);
    return (CompressionFunction(m_expanded));
  }

  std::pair<Halfblock, Halfblock> Feistel::FeistelSplit(const Block& block) {
    const std::string bits = block.ToString();

    Halfblock l(bits.substr(0, bits.size() / 2));
    Halfblock r(bits.substr(bits.size() / 2));

    return std::make_pair(l, r);
  }

  Block Feistel::FeistelCombine(const Halfblock& l, const Halfblock& r) {
    return Block(l.to_string() + r.to_string());
  }

  Block Feistel::ExpansionFunction(const Halfblock& block) {
    std::stringstream ss;
    const std::string bits = block.to_string();

    std::unordered_map<std::string, std::string> expansionMap;
    expansionMap["00"] = "1101";
    expansionMap["01"] = "1000";
    expansionMap["10"] = "0010";
    expansionMap["11"] = "0111";

    // We have to double the bits!
    for (std::size_t i = 0; i < HALFBLOCK_SIZE; i += 2) {
      const std::string sub = bits.substr(i, 2);
      ss << expansionMap[sub];
    }

    return Block(ss.str());
  }

  Halfblock Feistel::CompressionFunction(const Block& block) {
    std::stringstream ss;
    const std::string bits = block.ToString();

    std::unordered_map<std::string, std::string> compressionMap;
    compressionMap["0000"] = "10";
    compressionMap["0001"] = "01";
    compressionMap["0010"] = "11";
    compressionMap["0011"] = "10";
    compressionMap["0100"] = "11";
    compressionMap["0101"] = "01";
    compressionMap["0110"] = "00";
    compressionMap["0111"] = "01";
    compressionMap["1000"] = "11";
    compressionMap["1001"] = "00";
    compressionMap["1010"] = "11";
    compressionMap["1011"] = "00";
    compressionMap["1100"] = "11";
    compressionMap["1101"] = "10";
    compressionMap["1110"] = "00";
    compressionMap["1111"] = "01";

    // We have to half the bits!
    for (std::size_t i = 0; i < BLOCK_SIZE; i += 4) {
      const std::string sub = bits.substr(i, 4);
      ss << compressionMap[sub];
    }

    return Halfblock(ss.str());
  }

  std::string Feistel::SBox(const std::string& in) {
    static std::unordered_map<std::string, std::string> subMap;
    static bool mapInitialized = false;
    if (!mapInitialized) {
      subMap["0000"] = "1100";
      subMap["0001"] = "1000";
      subMap["0010"] = "0001";
      subMap["0011"] = "0111";
      subMap["0100"] = "1011";
      subMap["0101"] = "0011";
      subMap["0110"] = "1101";
      subMap["0111"] = "1111";
      subMap["1000"] = "0000";
      subMap["1001"] = "1010";
      subMap["1010"] = "0100";
      subMap["1011"] = "1001";
      subMap["1100"] = "0010";
      subMap["1101"] = "1110";
      subMap["1110"] = "0101";
      subMap["1111"] = "0110";
      mapInitialized = true;
    }

    return subMap[in];
  }

  void Feistel::GenerateRoundKeys(const Key& seedKey) {
    // Clear initial key memory
    ZeroKeyMemory();
    roundKeys = Keyset();

    // Derive all round keys with simple matrix operations
    roundKeys[0] = seedKey;

    for (std::size_t i = 1; i < roundKeys.size(); i++) {
      // Initialize new round key with last round key
      const Key& lastKey = roundKeys[i - 1];
      roundKeys[i] = lastKey;

      // Stir it good
      roundKeys[i].ShiftRowsUpInplace();

      // Bitshift and matrix-mult 3 times
      // (each time jumbles it up pretty good)
      // This is irreversible
      roundKeys[i].ShiftBitsRightInplace();
      roundKeys[i] *= lastKey;
      roundKeys[i].ShiftBitsRightInplace();
      roundKeys[i] *= lastKey;
      roundKeys[i].ShiftBitsRightInplace();
      roundKeys[i] *= lastKey;

      // Lastly, do apply some cell shifting, and other mutations
      roundKeys[i].ShiftCellsRightInplace();
      roundKeys[i] += lastKey;
      roundKeys[i].ShiftColumnsRightInplace();
      roundKeys[i] ^= lastKey;
    }

    return;
  }

  void Feistel::operator=(const Feistel& other) {
    roundKeys = other.roundKeys;

    return;
  }

  // These pragmas only work for MSVC and g++, as far as i know. Beware!!!
#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
  void Feistel::ZeroKeyMemory() {
    for (Key& key : roundKeys) {
      key.Reset();
    }

    return;
  }
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif

}

