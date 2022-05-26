#include "GCrypt/GHash.h"
#include "GCrypt/Util.h"
#include "GCrypt/InitializationVector.h"
#include <vector>

namespace Leonetienne::GCrypt {

  GHash::GHash() {
    // Initialize our cipher with a static, but randomly distributed key.
    Block ivSeed;
    ivSeed.FromByteString("3J7IipfQTDJbO8jtasz9PgWui6faPaEMOuVuAqyhB1S2CRcLw5caawewgDUEG1WN");
    block = InitializationVector(ivSeed);

    Key key;
    key.FromByteString("nsoCZfvdqpRkeVTt9wzvPR3TT26peOW9E2kTHh3pdPCq2M7BpskvUljJHSrobUTI");

    cipher = GCipher(
      // The key really does not matter, as it gets changed
      // each time before digesting anything.
      key,
      GCipher::DIRECTION::ENCIPHER
    );

    return;
  }

  void GHash::DigestBlock(const Block& data) {
    // Set the cipher key to the current data to be hashed
    cipher.SetKey(data);

    // Encipher the current block, and matrix-mult it with the current hashsum
    block ^= cipher.Digest(data);

    return;
  }

  const Block& GHash::GetHashsum() const {
    return block;
  }

  Block GHash::CalculateHashsum(const std::vector<Block>& data, std::size_t n_bytes) {

    // If we have no supplied n_bytes, let's just assume sizeof(data).
    if (n_bytes == std::string::npos) {
      n_bytes = data.size() * Block::BLOCK_SIZE;
    }

    // Create hasher instance
    GHash hasher;

    // Digest all blocks
    for (const Block& block : data) {
      hasher.DigestBlock(block);
    }

    // Add an additional block, containing the length of the input

    // Here it is actually good to use a binary string ("10011"),
    // because std::size_t is not fixed to 32-bits. It may aswell
    // be 64 bits, depending on the platform.
    // Then it would be BAD to just cram it into a 32-bit uint32.
    // This way, in case of 64-bits, it would just occupy 2 uint32's.

    // Also, this operation gets done ONCE per n blocks. This won't
    // hurt performance.

    // I know that we are first converting n_bytes to str(n_bytes),
    // and then converting this to a binstring, making it unnecessarily large,
    // but who cares. It has a whole 512 bit block to itself.
    // The max size (2^64) would occupy 155 bits at max. (log10(2^64)*8 = 155)

    std::stringstream ss;
    ss << n_bytes;
    Block lengthBlock;
    lengthBlock.FromTextString(ss.str());

    // Digest the length block
    hasher.DigestBlock(lengthBlock);

    // Return the total hashsum
    return hasher.GetHashsum();
  }

  Block GHash::HashString(const std::string& str) {
    const std::vector<Block> blocks = StringToBitblocks(str);
    const std::size_t n_bytes = str.length();

    return CalculateHashsum(blocks, n_bytes);
  }

  void GHash::operator=(const GHash& other) {
    cipher = other.cipher;

    return;
  }
}

