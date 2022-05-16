#pragma once
#include <cstdint>
#include "GCrypt/SecureBitset.h"
#include "GCrypt/Config.h"

namespace Leonetienne::GCrypt {
  constexpr std::size_t HALFBLOCK_SIZE = (BLOCK_SIZE / 2);
  typedef SecureBitset<HALFBLOCK_SIZE> Halfblock;
}
