#ifndef GCRYPT_KEYSET_H
#define GCRYPT_KEYSET_H

#include <array>
#include "GCrypt/Key.h"
#include "GCrypt/Config.h"

namespace Leonetienne::GCrypt {
  typedef std::array<Key, N_ROUNDS> Keyset;
}

#endif

