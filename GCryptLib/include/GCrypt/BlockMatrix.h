#ifndef GCRYPT_BLOCKMATRIX_H
#define GCRYPT_BLOCKMATRIX_H

#include "GCrypt/Block.h"
#include <cstdint>
#include <array>

namespace Leonetienne::GCrypt {

  /* This class represents a block as a matrix,
   * providing typical matrix operations
  */
  class BlockMatrix {
    public:
      BlockMatrix();
      BlockMatrix(const Block& block);
      BlockMatrix(const BlockMatrix& other);

      //! Will calculate the product of two matrices.
      //! Since the matrices values are pretty much sudo-random,
      //! they will most likely integer-overflow.
      //! So see this as a one-way function.
      BlockMatrix MMult(const BlockMatrix& other) const;
      BlockMatrix operator*(const BlockMatrix& other) const;

      //! Will do a regular matrix-mult, but instead of
      //! adding, and multiplying, all ints get xored.
      BlockMatrix MXor(const BlockMatrix& other) const;

      bool operator==(const BlockMatrix& other) const;
      bool operator!=(const BlockMatrix& other) const;

      void FromBlock(const Block& block);
      Block ToBlock() const;

    private:
      //! Returns items of data, indexed by 4x4 coordinates
      std::uint32_t& Get(const std::uint8_t row, const std::uint8_t column);
      //! Returns items of data, indexed by 4x4 coordinates
      const std::uint32_t& Get(const std::uint8_t row, const std::uint8_t column) const;

      std::array<std::uint32_t, 16> data;
  };

}


#endif

