#include "GCrypt/Hasher.h"
#include "GCrypt/Util.h"
#include "GCrypt/InitializationVector.h"

namespace Leonetienne::GCrypt {

  Hasher::Hasher() :
    // Initialize our cipher with a static, but randomly distributed key.
    cipher(
        StringToBitblock("CfRtNdMTP4Y5CWRd"),
        Cipher::CIPHER_DIRECTION::ENCIPHER
    ) {
      block = InitializationVector(StringToBitblock("3J7IipfQTDJbO8jtasz9PgWui6faPaEMOuVuAqyhB1S2CRcLw5caawewgDUEG1WN"));

      return;
  }
 
  void Hasher::Digest(const Block& data) {
    // Encipher the current block, and xor it on the current hashsum
    block ^= cipher.Digest(data);
    return;
  }

  const Block& Hasher::GetHashsum() const {
    return block;
  }

  Block Hasher::CalculateHashsum(const Flexblock& data) {
    // Split input into blocks
    std::vector<Block> blocks;

    for (std::size_t i = 0; i < data.size(); i += BLOCK_SIZE) {
      blocks.push_back(Block(
        PadStringToLength(data.substr(i, BLOCK_SIZE), BLOCK_SIZE, '0', false))
      );
    }

    // Add an additional block, containing the length of the input
    std::stringstream ss;
    ss << data.length();
    const Block lengthBlock = StringToBitblock(ss.str());
    blocks.push_back(lengthBlock);

    // Create hasher instance
    Hasher hasher;

    // Digest all blocks
    for (Block& block : blocks) {
      hasher.Digest(block);
    }

    // Return the total hashsum
    return hasher.GetHashsum();
  }

}

