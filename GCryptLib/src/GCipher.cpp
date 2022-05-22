#include <iostream>
#include <vector>
#include <stdexcept>
#include "GCrypt/GCipher.h"
#include "GCrypt/Util.h"
#include "GCrypt/InitializationVector.h"

namespace Leonetienne::GCrypt {

  GCipher::GCipher(const Key& key, const DIRECTION direction)
    :
    direction { direction },
    lastBlock(InitializationVector(key)), // Initialize our lastBlock with some deterministic initial value, based on the key
    feistel(key) {

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

  void GCipher::operator=(const GCipher& other) {
    direction = other.direction;
    feistel = other.feistel;
    lastBlock = other.lastBlock;

    return;
  }

}

