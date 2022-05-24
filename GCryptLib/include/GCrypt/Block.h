#ifndef GCRYPT_BLOCK_H
#define GCRYPT_BLOCK_H

#include <cstdint>
#include <array>
#include <string>
#include <ostream>

namespace Leonetienne::GCrypt {

  /* This class represents a block of data,
   * and provides functions to manipulate it
  */
  class Block {
    public:
      //! Will constuct an uninitialized data block
      Block();

      //! Will construct this block from a string like "101010".. Length MUST be 512.
      Block(const std::string& other);

      //! Copy-ctor
      Block(const Block& other);

      ~Block();

      //! Will construct this block from a string like "011101..". Length MUST be 512.
      void FromString(const std::string& str);

      //! Will create a bitset-compatible string ("0101110..") representation
      //! of this block. Length will always be 512.
      std::string ToString() const;

      //! Will matrix-multiply two blocks together.
      //! Since the matrices values are pretty much sudo-random,
      //! they will most likely integer-overflow.
      //! So see this as a one-way function.
      [[nodiscard]] Block MMul(const Block& other) const;
      [[nodiscard]] Block operator*(const Block& other) const;

      //! Will matrix-multiply two blocks together,
      //! and directly write into this same block.
      //! Since the matrices values are pretty much sudo-random,
      //! they will most likely integer-overflow.
      //! So see this as a one-way function.
      void MMulInplace(const Block& other);
      Block& operator*=(const Block& other);

      //! Will xor two blocks together
      [[nodiscard]] Block Xor(const Block& other) const;
      //! Will xor two blocks together
      [[nodiscard]] Block operator^(const Block& other) const;

      //! Will xor two blocks together, inplace
      void XorInplace(const Block& other);
      //! Will xor two blocks together, inplace
      Block& operator^=(const Block& other);

      //! Will add all the integer making up this block, one by one
      [[nodiscard]] Block Add(const Block& other) const;
      //! Will add all the integer making up this block, one by one
      [[nodiscard]] Block operator+(const Block& other) const;

      //! Will add all the integer making up this block, one by one, inplace
      void AddInplace(const Block& other);
      //! Will add all the integer making up this block, one by one, inplace
      Block& operator+=(const Block& other);

      //! Will subtract all the integer making up this block, one by one
      [[nodiscard]] Block Sub(const Block& other) const;
      //! Will subtract all the integer making up this block, one by one
      [[nodiscard]] Block operator-(const Block& other) const;

      //! Will subtract all the integer making up this block, one by one, inplace
      void SubInplace(const Block& other);
      //! Will subtract all the integer making up this block, one by one, inplace
      Block& operator-=(const Block& other);

      //! Will shift rows upwards by 1
      [[nodiscard]] Block ShiftRowsUp() const;

      //! Will shift rows upwards by 1
      void ShiftRowsUpInplace();

      //! Will shift matrix rows downwards by 1
      [[nodiscard]] Block ShiftRowsDown() const;

      //! Will shift matrix rows downwards by 1
      void ShiftRowsDownInplace();

      //! Will shift matrix columns to the left by 1
      [[nodiscard]] Block ShiftColumnsLeft() const;

      //! Will shift matrix columns to the left by 1
      void ShiftColumnsLeftInplace();

      //! Will shift matrix columns to the right by 1
      [[nodiscard]] Block ShiftColumnsRight() const;

      //! Will shift matrix columns to the right by 1
      void ShiftColumnsRightInplace();

      //! Will shift array cells to the left by 1
      [[nodiscard]] Block ShiftCellsLeft() const;

      //! Will shift array cells to the left by 1
      void ShiftCellsLeftInplace();

      //! Will shift array cells to the right by 1
      [[nodiscard]] Block ShiftCellsRight() const;

      //! Will shift array cells to the right by 1
      void ShiftCellsRightInplace();

      //! Will copy a block
      Block& operator=(const Block& other);

      //! Will compare whether or not two blocks are equal
      [[nodiscard]] bool operator==(const Block& other) const;
      //! Will compare whether or not two blocks are unequal
      [[nodiscard]] bool operator!=(const Block& other) const;

      //! Will zero all data
      void Reset();

      //! Will return the state of any given bit
      [[nodiscard]] bool GetBit(const std::size_t index) const;

      //! Returns 32-bit chunks of data, indexed by matrix coordinates (0-3)
      [[nodiscard]] std::uint32_t& Get(const std::uint8_t row, const std::uint8_t column);
      //! Returns 32-bit chunks of data, indexed by matrix coordinates (0-3)
      [[nodiscard]] const std::uint32_t& Get(const std::uint8_t row, const std::uint8_t column) const;

      //! Returns 32-bit chunks of data, indexed by a 1d-index (0-16)
      [[nodiscard]] std::uint32_t& Get(const std::uint8_t index);

      //! Returns 32-bit chunks of data, indexed by a 1d-index (0-16)
      [[nodiscard]] const std::uint32_t& Get(const std::uint8_t index) const;

      //! Returns 32-bit chunks of data, indexed by a 1d-index (0-16)
      [[nodiscard]] std::uint32_t& operator[](const std::uint8_t index);

      //! Returns 32-bit chunks of data, indexed by a 1d-index (0-16)
      [[nodiscard]] const std::uint32_t& operator[](const std::uint8_t index) const;

      static constexpr std::size_t CHUNK_SIZE = sizeof(std::uint32_t);
      static constexpr std::size_t CHUNK_SIZE_BITS = CHUNK_SIZE * 8;

      friend std::ostream& operator<<(std::ostream& os, const Block& b);

    private:

      std::array<std::uint32_t, 16> data;
  };

}


#endif

