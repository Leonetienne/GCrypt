#pragma once
#include "GCrypt/Config.h"
#include "GCrypt/Block.h"

namespace Leonetienne::GCrypt {
  /** Will create a sudo-random Block based on a seed
  */
  class InitializationVector {
  public:
    InitializationVector(const Block& seed);

    operator Block() const;

  private:
    Block iv;
  };
}
