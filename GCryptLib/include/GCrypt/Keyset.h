#pragma once
#include <array>
#include "GCrypt/Key.h"
#include "GCrypt/Config.h"

namespace Leonetienne::GCrypt {
  typedef std::array<Key, N_ROUNDS> Keyset;
}

