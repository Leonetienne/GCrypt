#include "InitializationVector.h"
#include "Feistel.h"

GhettoCipher::InitializationVector::InitializationVector(const Block& seed) {
  // We'll generate our initialization vector by encrypting our seed with itself as a key
  // iv = E(M=seed, K=seed)
  iv = Feistel(seed).Encipher(seed);
}

GhettoCipher::InitializationVector::operator GhettoCipher::Block() const {
  return iv;
}

