#ifndef GCRYPT_CONFIG_H
#define GCRYPT_CONFIG_H

#include "GCrypt/SecureBitset.h"
#include "GCrypt/Config.h"

namespace Leonetienne::GCrypt {
  typedef SecureBitset<BLOCK_SIZE> Block;
}

#endif

