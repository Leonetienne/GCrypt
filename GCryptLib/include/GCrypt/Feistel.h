#ifndef GCRYPT_FEISTEL_H
#define GCRYPT_FEISTEL_H

#include "GCrypt/Keyset.h"
#include "GCrypt/Block.h"
#include "GCrypt/Key.h"

namespace Leonetienne::GCrypt {
  /** Class to perform a feistel block chipher
  */
  class Feistel {
  public:
    explicit Feistel(const Key& key);

    Feistel(const Feistel& other) = delete;
    Feistel(Feistel&& other) noexcept = delete;

    ~Feistel();

    //! Will set the seed-key for this feistel network.
    //! Roundkeys will be derived from this.
    void SetKey(const Key& key);

    //! Will encipher a data block via the set seed-key
    Block Encipher(const Block& data);

    //! Will decipher a data block via the set seed-key
    Block Decipher(const Block& data);

    void operator=(const Feistel& other);

  private:
    //! Will run the feistel rounds, with either regular key
    //! order or reversed key order
    Block Run(const Block& data, bool reverseKeys);

    //! Arbitrary cipher function
    static Halfblock F(Halfblock m, const Key& key);

    //! Split a data block into two half blocks (into L and R)
    static std::pair<Halfblock, Halfblock> FeistelSplit(const Block& block);

    //! Combine two half blocks (L and R) into a regular data block
    static Block FeistelCombine(const Halfblock& l, const Halfblock& r);

    //! Will expand a halfblock to a fullblock
    static Block ExpansionFunction(const Halfblock& block);

    //! Will compress a fullblock to a halfblock
    static Halfblock CompressionFunction(const Block& block);

    //! Substitutes four bits by static random others
    static std::string SBox(const std::string& in);

    //! Will generate a the round keys
    void GenerateRoundKeys(const Key& seedKey);

    //! Will zero the memory used by the keyset
    void ZeroKeyMemory();

    Keyset roundKeys;
  };
}

#endif

