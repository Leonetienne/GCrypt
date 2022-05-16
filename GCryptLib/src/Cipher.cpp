#include <iostream>
#include <vector>
#include "GCrypt/Cipher.h"
#include "GCrypt/Util.h"
#include "GCrypt/InitializationVector.h"

namespace Leonetienne::GCrypt {

  Cipher::Cipher(const Block& key)
    :
    key { key },
    initializationVector(InitializationVector(key)) {

    return;
  }

  Cipher::Cipher(const std::string& password)
    :
    key { PasswordToKey(password) },
    initializationVector(InitializationVector(key)) {
    return;
  }

  Cipher::~Cipher() {
    // Clear key memory
    ZeroKeyMemory();

    return;
  }

  void Cipher::SetKey(const Block& key) {
    ZeroKeyMemory();

    this->key = key;
    return;
  }

  void Cipher::SetPassword(const std::string& password) {
    ZeroKeyMemory();

    key = PasswordToKey(password);
    return;
  }

  Flexblock Cipher::Encipher(const Flexblock& data, bool printProgress) const {
    // Split cleartext into blocks
    std::vector<Block> blocks;

    for (std::size_t i = 0; i < data.size(); i += BLOCK_SIZE) {
      blocks.push_back(Block(
        PadStringToLength(data.substr(i, BLOCK_SIZE), BLOCK_SIZE, '0', false))
      );
    }

    // Encrypt individual blocks using cipher block chaining
    Feistel feistel(key);

    for (std::size_t i = 0; i < blocks.size(); i++) {
      // Print reports if desired. If we have > 1000 blocks, print one report every 100 blocks. Otherwise for every 10th block.
      if ((i % ((blocks.size() > 1000)? 100 : 10) == 0) && (printProgress)) {
        std::cout << "Encrypting... (Block " << i << " / " << blocks.size() << " - " << ((float)i*100 / blocks.size()) << "%)" << std::endl;
      }

      const Block& lastBlock = (i>0) ? blocks[i-1] : initializationVector;
      blocks[i] = feistel.Encipher(blocks[i] ^ lastBlock); // Xor last cipher block with new clear text block before E()
    }

    // Concatenate ciphertext blocks back into a flexblock
    std::stringstream ss;
    for (Block& b : blocks) {
      ss << b;
    }

    // Return it
    return ss.str();
  }

  Flexblock Cipher::Decipher(const Flexblock& data, bool printProgress) const {
    // Split ciphertext into blocks
    std::vector<Block> blocks;

    for (std::size_t i = 0; i < data.size(); i += BLOCK_SIZE) {
      blocks.push_back(Block(
        PadStringToLength(data.substr(i, BLOCK_SIZE), BLOCK_SIZE, '0', false))
      );
    }

    // Decrypt individual blocks
    Feistel feistel(key);

    // We can't do this in-loop for decryption, because we are decrypting the blocks in-place.
    Block lastBlock = initializationVector;

    for (std::size_t i = 0; i < blocks.size(); i++) {
      // Print reports if desired. If we have > 1000 blocks, print one report every 100 blocks. Otherwise for every 10th block.
      if ((i % ((blocks.size() > 1000) ? 100 : 10) == 0) && (printProgress)) {
        std::cout << "Decrypting... (Block " << i << " / " << blocks.size() << " - " << ((float)i*100/ blocks.size()) << "%)" << std::endl;
      }

      Block tmpCopy = blocks[i];

      blocks[i] = feistel.Decipher(blocks[i]) ^ lastBlock; // Decipher cipher block [i] and then xor it with the last cipher block [i-1] we've had

      lastBlock = std::move(tmpCopy);
    }

    // Concatenate ciphertext blocks back into a flexblock
    std::stringstream ss;
    for (Block& b : blocks) {
      ss << b;
    }

    // Return it
    return ss.str();
  }

  // These pragmas only work for MSVC and g++, as far as i know. Beware!!!
#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
  void Cipher::ZeroKeyMemory() {
    key.reset();
    return;
  }
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif

}
