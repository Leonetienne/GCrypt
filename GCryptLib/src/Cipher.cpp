#include <iostream>
#include <vector>
#include <stdexcept>
#include "GCrypt/Cipher.h"
#include "GCrypt/Util.h"
#include "GCrypt/InitializationVector.h"

namespace Leonetienne::GCrypt {

  Cipher::Cipher(const Block& key, const CIPHER_DIRECTION direction)
    :
    key { key },
    direction { direction },
    lastBlock(InitializationVector(key)), // Initialize our lastBlock with some deterministic initial value, based on the key
    feistel(key) {

    return;
  }

  Cipher::Cipher(const std::string& password, const CIPHER_DIRECTION direction)
    :
    key { PasswordToKey(password) },
    direction { direction },
    lastBlock(InitializationVector(key)),  // Initialize our lastBlock with some deterministic initial value, based on the key   feistel(key) {
    feistel(key) {
    return;
  }

  Cipher::~Cipher() {
    // Clear key memory
    ZeroKeyMemory();

    return;
  }

  Block Cipher::Digest(const Block& input) {

    switch (direction) {
      case CIPHER_DIRECTION::ENCIPHER: {
        // Rename our input to cleartext
        const Block& cleartext = input;

        // First, xor our cleartext with the last block, and then encipher it
        Block ciphertext = feistel.Encipher(cleartext ^ lastBlock);

        // Now set our lastBlock to the ciphertext of this block
        lastBlock = ciphertext;

        // Now return the ciphertext
        return ciphertext;
      }

      case CIPHER_DIRECTION::DECIPHER: {
        // Rename our input into ciphertext
        const Block& ciphertext = input;

        // First, decipher our ciphertext, and then xor it with our last block
        Block cleartext = feistel.Decipher(ciphertext) ^ lastBlock;

        // Now set our lastBLock to the ciphertext of this block
        lastBlock = ciphertext;

        // Now return the cleartext
        return cleartext;
      }
    }

    throw std::runtime_error("Unreachable branch reached.");
  }

  /*
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
*/

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

