#ifndef GCRYPT_BLOCK_H
#define GCRYPT_BLOCK_H

#include "GCrypt/SecureBitset.h"
#include "GCrypt/Config.h"

namespace Leonetienne::GCrypt {
  typedef SecureBitset<BLOCK_SIZE> Block;
}

#endif

