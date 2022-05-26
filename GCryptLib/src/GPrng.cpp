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
    if (nextBit >= Block::BLOCK_SIZE_BITS) {
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
    // Tactic on efficiently generating a new block:
    // 1) Fetch complete current hashsum (it might have been partially given out already)
    // 2) Bitshift it, and matrix-mult it with the seed (that is irreversible)
    //    That should be a one-way function, and create a new unique block.
    //    We don't even have to AdvanceBlock(), because we've only given out
    //    hashsum', not hashsum.

    // Fetch our current block
    Block hashsum = hasher.GetHashsum();

    // Derive/'hash' it to hashsum'
    hashsum *= seed;
    hashsum.ShiftBitsLeftInplace();
    hashsum *= seed;

    // Return our hashsum
    return hashsum;
  }

}

