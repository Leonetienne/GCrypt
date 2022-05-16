#pragma once
#include <cstddef>

namespace GhettoCipher {
  // MUST BE A POWER OF 2 > 4
  constexpr std::size_t BLOCK_SIZE = 512;

  // MUST BE > 2
  constexpr std::size_t N_ROUNDS = 64;
}
