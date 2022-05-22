#pragma once
#include "GCrypt/Feistel.h"
#include "GCrypt/Flexblock.h"

namespace Leonetienne::GCrypt {
  /** Class to apply a block/-stream cipher to messages of arbitrary length in a distributed manner
  */
  class GCipher {
  public:
    //! Describes the direction the cipher runs in
    enum class DIRECTION {
      ENCIPHER,
      DECIPHER
    };

    //! Will initialize this cipher with a key
    explicit GCipher(const Key& key, const DIRECTION direction);

    // Disable copying
    GCipher(const GCipher& other) = delete;
    GCipher(GCipher&& other) noexcept = delete;

    //! Will digest a data block, and return it
    Block Digest(const Block& input);

  private:
    const DIRECTION direction;

    //! The feistel instance to be used
    Feistel feistel;

    //! The last block, required for CBC.
    Block lastBlock;
  };
}