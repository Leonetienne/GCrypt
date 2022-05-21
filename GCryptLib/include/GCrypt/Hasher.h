#ifndef GCRYPT_HASHER_H
#define GCRYPT_HASHER_H

#include "GCrypt/Flexblock.h"
#include "GCrypt/Block.h"
#include "GCrypt/Cipher.h"

namespace Leonetienne::GCrypt {
  /** This class implements a hash function, based on the GCrypt cipher
  */
  class Hasher {
  public:
    Hasher();

    //! Will add the hash value of `data` to the hashsum
    void Digest(const Block& data);

    //! Will return the current hashsum
    const Block& GetHashsum() const;

    //! Will calculate a hashsum for `data`.
    static Block CalculateHashsum(const Flexblock& data);

  private:
    //! The cipher to use
    Cipher cipher;

    //! The current state of the hashsum
    Block block;
  };
}

#endif

