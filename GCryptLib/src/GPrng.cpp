#include "GCrypt/GPrng.h"
#include <cassert>

namespace Leonetienne::GCrypt {

  GPrng::GPrng(const Block& seed) {
    this->seed = seed;
    hasher.DigestBlock(seed);
  }

  GPrng::GPrng() {
  }

  void GPrng::Seed(const Block& seed) {
    hasher = GHash();
    this->seed = seed;
    hasher.DigestBlock(seed);

    return;
  }

  bool GPrng::GetBit() {
    // If we have no more bits to go, create new ones
    if (nextBit >= BLOCK_SIZE) {
      AdvanceBlock();
    }

    // Return the next bit.
    return hasher.GetHashsum()[nextBit++];
  }

  void GPrng::AdvanceBlock() {
    // To prevent an attacker from being able
    // to predict block n, by knowing block n-1, and block n-2,
    // we will advance the hash function by block n-1 XOR seed.
    // This way it is impossible for an attacker to know the
    // state of the hash function, unless the seed is known.

    // Advance the block (Add the current hashsum XOR seed to the hasher)
    hasher.DigestBlock(Block(hasher.GetHashsum() ^ seed));

    // Reset the pointer
    nextBit = 0;

    return;
  }

  Block GPrng::GetBlock() {
    // Getting a block is a bit troublesome.
    // Just fetching 512 bits would be too much of a performance hog.

    // Slurp up the rest of the current block
    std::stringstream ss;
    const std::size_t bitsLeft = BLOCK_SIZE - nextBit;
    ss << hasher.GetHashsum().ToString().substr(nextBit, bitsLeft);

    // Now we have to advance to the next block
    AdvanceBlock();

    // Now, grab the remaining bits
    const std::size_t remainingBits = BLOCK_SIZE - bitsLeft;
    ss << hasher.GetHashsum().ToString().substr(0, remainingBits);

    // Assert that we have the correct number of bits
    assert(ss.str().length() == BLOCK_SIZE);

    // Set out bitpointer
    nextBit = remainingBits;

    // Return our block
    return Block(ss.str());
  }

}

