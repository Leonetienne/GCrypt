#pragma once
#include <array>
#include "GCrypt/Block.h"
#include "GCrypt/Config.h"

namespace Leonetienne::GCrypt {
  typedef std::array<Block, N_ROUNDS> Keyset;
}
