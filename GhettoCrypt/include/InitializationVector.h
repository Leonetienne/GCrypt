#pragma once
#include "Config.h"
#include "Block.h"

namespace GhettoCipher {
  /** Will create a sudo-random Block based on a seed
  */
  class InitializationVector {
  public:
    InitializationVector(const GhettoCipher::Block& seed);

    operator GhettoCipher::Block() const;

  private:
    GhettoCipher::Block iv;
  };
}
