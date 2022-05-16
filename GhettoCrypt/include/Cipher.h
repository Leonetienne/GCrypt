#pragma once
#include "Feistel.h"
#include "Flexblock.h"

namespace GhettoCipher {
  /** Class to apply a block cipher to messages of arbitrary length in a distributed manner
  */
  class Cipher {
  public:
    explicit Cipher(const Block& key);
    explicit Cipher(const std::string& password);

    Cipher(const Cipher& other) = delete;
    Cipher(Cipher&& other) noexcept = delete;

    ~Cipher();

    //! Will set the key
    void SetKey(const Block& key);

    //! Will set the key from a password
    void SetPassword(const std::string& password);

    //! Will encipher a flexblock of data
    Flexblock Encipher(const Flexblock& data, bool printProgress = false) const;

    //! Will decipher a flexblock of data
    Flexblock Decipher(const Flexblock& data, bool printProgress = false) const;

  private:
    Block key;

    //! Will zero the memory used by the key
    void ZeroKeyMemory();

    // Initial value for cipher block chaining
    Block initializationVector;
  };
}
