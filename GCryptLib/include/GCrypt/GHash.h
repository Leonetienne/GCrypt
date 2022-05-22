#ifndef GCRYPT_GHASH_H
#define GCRYPT_GHASH_H

#include "GCrypt/Flexblock.h"
#include "GCrypt/Block.h"
#include "GCrypt/GCipher.h"

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
    void DigestBlock(const Block& data);

    //! Will return the current hashsum
    const Block& GetHashsum() const;

    //! Will calculate a hashsum for `data`.
    static Block CalculateHashsum(const Flexblock& data);

  private:
    //! The cipher to use
    GCipher cipher;

    //! The current state of the hashsum
    Block block;
  };
}

#endif

