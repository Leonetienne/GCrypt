#include "InitializationVector.h"
#include "Feistel.h"

namespace Leonetienne::GCrypt {

  InitializationVector::InitializationVector(const Block& seed) {
    // We'll generate our initialization vector by encrypting our seed with itself as a key
    // iv = E(M=seed, K=seed)
    iv = Feistel(seed).Encipher(seed);
  }

  InitializationVector::operator Block() const {
    return iv;
  }

}

