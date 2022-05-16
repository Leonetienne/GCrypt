#pragma once
#include <array>
#include "Block.h"
#include "Config.h"

namespace Leonetienne::GCrypt {
  typedef std::array<Block, N_ROUNDS> Keyset;
}
