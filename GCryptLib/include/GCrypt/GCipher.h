#ifndef GCRYPT_GCIPHER_H
#define GCRYPT_GCIPHER_H

#include "GCrypt/Feistel.h"

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

    //! Empty initializer. If you use this, you must call Initialize()!
    GCipher();

    //! Will initialize this cipher with a key
    explicit GCipher(const Key& key, const DIRECTION direction);

    // Disable copying
    GCipher(const GCipher& other) = delete;
    GCipher(GCipher&& other) noexcept = delete;

    //! Will digest a data block, and return it
    Block Digest(const Block& input);

    //! Will update the base key used
    void SetKey(const Key& key);

    void operator=(const GCipher& other);


    //! Will initialize the cipher with a key, and a mode.
    //! If called on an existing object, it will reset its state.
    void Initialize(const Key& key, const DIRECTION direction);

  private:
    DIRECTION direction;

    //! The feistel instance to be used
    Feistel feistel;

    //! The last block, required for CBC.
    Block lastBlock;

    bool isInitialized = false;
  };
}

#endif

