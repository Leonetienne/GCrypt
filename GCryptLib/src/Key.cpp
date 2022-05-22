#include "GCrypt/Key.h"
#include "GCrypt/GHash.h"
#include "GCrypt/Util.h"
#include <random>
#include <cassert>

namespace Leonetienne::GCrypt {

  Key Key::FromPassword(const std::string& password) {
    return GHash::CalculateHashsum(
      StringToBits(password)
    );
  }

  Key Key::Random() {
    // Create our truly-random rng
    std::random_device rng;
    constexpr std::size_t bitsPerCall = sizeof(std::random_device::result_type) * 8;

    // Fetch BLOCK_SIZE bits
    std::stringstream ss;
    for (std::size_t i = 0; i < BLOCK_SIZE / bitsPerCall; i++) {
      ss << std::bitset<bitsPerCall>(rng());
    }

    // Verify that we actually have the correct size
    assert(ss.str().length() == BLOCK_SIZE);

    // Return them as a key
    return Key(Block(ss.str()));
  }

  Key::Key() : Block() {
  }

  Key::Key(const Block& b) : Block(b) {
  }

  Key::Key(const Key& k) : Block(k) {
  }
}

