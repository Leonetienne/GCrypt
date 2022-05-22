#include "GCrypt/GHash.h"
#include "GCrypt/Util.h"
#include "GCrypt/InitializationVector.h"

namespace Leonetienne::GCrypt {

  GHash::GHash() :
    // Initialize our cipher with a static, but randomly distributed key.
    cipher(
        // Can't use Key::FromPassword here, because it depends on GHash.
        // Instead use a hardcoded key.
        Key(StringToBitblock("nsoCZfvdqpRkeVTt9wzvPR3TT26peOW9E2kTHh3pdPCq2M7BpskvUljJHSrobUTI")),
        GCipher::DIRECTION::ENCIPHER
    ) {
      block = InitializationVector(StringToBitblock("3J7IipfQTDJbO8jtasz9PgWui6faPaEMOuVuAqyhB1S2CRcLw5caawewgDUEG1WN"));

      return;
  }

  void GHash::DigestBlock(const Block& data) {
    // Encipher the current block, and xor it on the current hashsum
    block ^= cipher.Digest(data);
    return;
  }

  const Block& GHash::GetHashsum() const {
    return block;
  }

  Block GHash::CalculateHashsum(const Flexblock& data) {
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
    GHash hasher;

    // Digest all blocks
    for (Block& block : blocks) {
      hasher.DigestBlock(block);
    }

    // Return the total hashsum
    return hasher.GetHashsum();
  }

  void GHash::operator=(const GHash& other) {
    cipher = other.cipher;

    return;
  }
}

