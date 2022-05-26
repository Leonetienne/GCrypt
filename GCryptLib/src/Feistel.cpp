#include <unordered_map>
#include "GCrypt/Feistel.h"
#include "GCrypt/Util.h"
#include "GCrypt/Config.h"
#include "GCrypt/SBoxLookup.h"

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

  Block Feistel::Run(const Block& data, bool modeEncrypt) {
    const auto splitData = FeistelSplit(data);
    Halfblock l = splitData.first;
    Halfblock r = splitData.second;

    Halfblock tmp;

    for (std::size_t i = 0; i < N_ROUNDS; i++) {

      // Encryption
      if (modeEncrypt) {
        const std::size_t keyIndex = i;

        // Do a feistel round
        tmp = r;
        r = l ^ F(r, roundKeys[keyIndex]);
        l = tmp;

        // Jumble it up a bit more
        l.ShiftRowsUpInplace();
        l.ShiftCellsRightInplace();
        l.ShiftBitsLeftInplace();
        l.ShiftColumnsLeftInplace();
        // Seal all these operations with a key
        l += ReductionFunction(roundKeys[keyIndex]);
      }

      // Decryption
      else {
        // Decryption needs keys in reverse order
        const std::size_t keyIndex = N_ROUNDS - i - 1;

        // Unjumble the jumble
        r -= ReductionFunction(roundKeys[keyIndex]);
        r.ShiftColumnsRightInplace();
        r.ShiftBitsRightInplace();
        r.ShiftCellsLeftInplace();
        r.ShiftRowsDownInplace();

        // Do a feistel round
        tmp = r;
        r = l ^ F(r, roundKeys[keyIndex]);
        l = tmp;
      }

    }

    // Block has finished de*ciphering.
    // Let's generate a new set of round keys.
    GenerateRoundKeys(roundKeys.back());

    return FeistelCombine(r, l);
  }

  Halfblock Feistel::F(Halfblock m, const Key& key) {

    // Made-up F function:
    // Expand to full bitwidth
    Block m_expanded = ExpansionFunction(m);

    // Mix up the block a bit
    m_expanded.ShiftCellsRightInplace();
    m_expanded.ShiftRowsUpInplace();

    // Matrix-mult with key (this is irreversible)
    m_expanded *= key;

    // Now do a bitshift
    m_expanded.ShiftBitsLeftInplace();

    // Apply the sbox
    SBox(m_expanded);

    // Reduce back to a halfblock
    Halfblock hb = ReductionFunction(m_expanded);

    // To jumble it up a last time,
    // matrix-multiply it with the input halfblock
    hb *= m;

    return hb;
  }

  std::pair<Halfblock, Halfblock> Feistel::FeistelSplit(const Block& block) {
    Halfblock l;
    Halfblock r;

    memcpy(l.Data(), block.Data(), Halfblock::BLOCK_SIZE);
    memcpy(r.Data(), block.Data() + 8, Halfblock::BLOCK_SIZE);
    // +8, because 8 is HALF the number of elements in the array. We only want to copy HALF a full-sized block.

    return std::make_pair(l, r);
  }

  Block Feistel::FeistelCombine(const Halfblock& l, const Halfblock& r) {
    Block b;

    memcpy(b.Data(), l.Data(), Halfblock::BLOCK_SIZE);
    memcpy(b.Data() + 8, r.Data(), Halfblock::BLOCK_SIZE);
    // +8, because 8 is HALF the number of elements in the array. We only want to copy HALF a full-sized block.

    return b;
  }

  Block Feistel::ExpansionFunction(const Halfblock& hb) {
    Block b;

    // Copy the bits over
    for (std::size_t i = 0; i < 16; i++) {
      b[i] = hb[i];
    }

    // Multiply the block a few tims with a bitshifted version
    // This is irriversible, too
    for (std::size_t i = 0; i < 3; i++) {
      b *= b.ShiftBitsRight();
    }

    return b;
  }

  Halfblock Feistel::ReductionFunction(const Block& block) {

    // Just apply a modulo operation, remapping a 32bit integer
    // onto 16bit space (default configuration).
    // Without saying, modulo is irreversible.
    Halfblock hb;
    for (std::size_t i = 0; i < 16; i++) {
      hb[i] = block[i] % (1 << (Halfblock::CHUNK_SIZE_BITS - 1));
    }

    return hb;
  }

  void Feistel::SBox(Block& block) {

    std::uint8_t* curByte = (std::uint8_t*)(void*)block.Data();

    // Iterate over all bytes in the block
    for (std::size_t i = 0; i < Block::BLOCK_SIZE; i++) {
      curByte++;

      // Subsitute byte
      *curByte = sboxLookup[*curByte];
    }

    return;
  }

  /*
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
  */

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

