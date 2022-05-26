#ifndef GCRYPT_GHASH_H
#define GCRYPT_GHASH_H

#include "GCrypt/Block.h"
#include "GCrypt/GCipher.h"
#include <vector>

namespace Leonetienne::GCrypt {
  /** This class implements a hash function, based on the GCrypt cipher
  */
  class GHash {
  public:
    GHash();

    //! Will add the hash value of the block `data` to the hashsum.
    //! WARNING: If you compute hashes using this digestive method,
    //! you REALLY REALLY should add a trailing block just containing the cleartext size!
    //! You MOST LIKELY just want to use the wrapper function GHash::CalculateHashsum(Flexblock const&) instead!
    void Digest(const Block& data);

    //! Will return the current hashsum
    const Block& GetHashsum() const;

    //! Will calculate a hashsum for `blocks`.
    //! Whilst n_bytes is optional, it is HIGHLY recommended to supply.
    //! Without specifying the size of the input (doesn't always have to be 512*n bits)
    //! b'293eff' would hash to the exact same values as b'293eff0000'
    static Block CalculateHashsum(const std::vector<Block>& blocks, std::size_t n_bytes = std::string::npos);

    //! Will calculate a hashsum for a string
    static Block HashString(const std::string& str);

    void operator=(const GHash& other);

  private:
    //! The cipher to use
    GCipher cipher;

    //! The current state of the hashsum
    Block block;
  };
}

#endif

