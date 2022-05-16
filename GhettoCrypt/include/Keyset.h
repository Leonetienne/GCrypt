#pragma once
#include <array>
#include "Block.h"
#include "Config.h"

namespace GhettoCipher {
  typedef std::array<Block, N_ROUNDS> Keyset;
}
