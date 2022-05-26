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
    return hasher.GetHashsum().GetBit(nextBit++);
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

    // Performance improvement over the previous method:
    // (generating 100.000 blocks):
    // 12 seconds -> 0.12 seconds

    // Fetch our current block
    Block hashsum = hasher.GetHashsum();

    // Derive/'hash' it to hashsum'
    hashsum *= seed;
    hashsum.ShiftBitsLeftInplace();
    hashsum *= seed;

    // Advance the block, so that the following block will be a new block
    AdvanceBlock();

    // Return our hashsum
    return hashsum;
  }

  std::uint32_t GPrng::operator()() {
    // Tactic:
    // A block intrinsically consists of 16 32-bit uints.
    // We'll just skip all the bits until the next whole integer,
    // fetch this complete int, and then advance our pointer
    // by 32 bits.

    // Performance improvement over the previous method:
    // (generating 1.000.000 integers):
    // 5.26 seconds -> 3.75 seconds


    // Advance our pointer to the next whole uint32

    // Do we even have >= 32 bits left in our block?
    if (nextBit > Block::BLOCK_SIZE_BITS - Block::CHUNK_SIZE_BITS) {
      // No: Create a new block
      AdvanceBlock();
    }

    // We don't have to do this, if our pointer is already at
    // the beginning of a whole uint32
    if (nextBit % Block::CHUNK_SIZE_BITS != 0) {
      nextBit = ((nextBit / Block::CHUNK_SIZE_BITS) + 1) * Block::CHUNK_SIZE_BITS;
    }

    // Fetch our integer from the block
    const std::uint32_t randint =
      hasher.GetHashsum().Get(nextBit / Block::CHUNK_SIZE_BITS);

    // Advance our pointer
    nextBit += Block::CHUNK_SIZE_BITS;

    // New state of the pointer:
    // It ow may be more now than the size of a block, but that
    // gets checked at the begin of a function.
    // Not at the end, like here.

    // Return our integer
    return randint;
  }

}

