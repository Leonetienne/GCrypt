#pragma once
#include "GCrypt/Feistel.h"
#include "GCrypt/Flexblock.h"

namespace Leonetienne::GCrypt {
  /** Class to apply a block/-stream cipher to messages of arbitrary length in a distributed manner
  */
  class Cipher {
  public:
    //! Describes the direction the cipher runs in
    enum class CIPHER_DIRECTION {
      ENCIPHER,
      DECIPHER
    };

    //! Will initialize this cipher with a key
    explicit Cipher(const Block& key, const CIPHER_DIRECTION direction);

    //! Will initialize this cipher with a key
    explicit Cipher(const std::string& password, const CIPHER_DIRECTION direction);

    // Disable copying
    Cipher(const Cipher& other) = delete;
    Cipher(Cipher&& other) noexcept = delete;

    ~Cipher();

    //! Will digest a data block, and return it
    Block Digest(const Block& input);

  private:
    Block key;
    const CIPHER_DIRECTION direction;

    //! The feistel instance to be used
    Feistel feistel;

    //! The last block, required for CBC.
    Block lastBlock;

    //! Will zero the memory used by the key
    void ZeroKeyMemory();
  };
}
