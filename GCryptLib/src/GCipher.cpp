#include <iostream>
#include <vector>
#include <stdexcept>
#include "GCrypt/GCipher.h"
#include "GCrypt/Util.h"
#include "GCrypt/InitializationVector.h"

namespace Leonetienne::GCrypt {

  GCipher::GCipher(const Block& key, const DIRECTION direction)
    :
    key { key },
    direction { direction },
    lastBlock(InitializationVector(key)), // Initialize our lastBlock with some deterministic initial value, based on the key
    feistel(key) {

    return;
  }

  GCipher::GCipher(const std::string& password, const DIRECTION direction)
    :
    key { PasswordToKey(password) },
    direction { direction },
    lastBlock(InitializationVector(key)),  // Initialize our lastBlock with some deterministic initial value, based on the key   feistel(key) {
    feistel(key) {
    return;
  }

  GCipher::~GCipher() {
    // Clear key memory
    ZeroKeyMemory();

    return;
  }

  Block GCipher::Digest(const Block& input) {

    switch (direction) {
      case DIRECTION::ENCIPHER: {
        // Rename our input to cleartext
        const Block& cleartext = input;

        // First, xor our cleartext with the last block, and then encipher it
        Block ciphertext = feistel.Encipher(cleartext ^ lastBlock);

        // Now set our lastBlock to the ciphertext of this block
        lastBlock = ciphertext;

        // Now return the ciphertext
        return ciphertext;
      }

      case DIRECTION::DECIPHER: {
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

  // These pragmas only work for MSVC and g++, as far as i know. Beware!!!
#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
  void GCipher::ZeroKeyMemory() {
    key.reset();
    return;
  }
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif

}

