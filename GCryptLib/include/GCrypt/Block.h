#ifndef GCRYPT_BLOCK_H
#define GCRYPT_BLOCK_H

#include <cstdint>
#include <array>
#include <string>
#include <ostream>
#include <bitset>

namespace Leonetienne::GCrypt {

  /* This class represents a block of data,
   * and provides functions to manipulate it
  */
  template <typename T>
  class Basic_Block {
    public:
      //! Will constuct an uninitialized data block
      Basic_Block();

      //! Will construct this block from a string like "101010".. Length MUST be 512.
      Basic_Block(const std::string& other);

      //! Copy-ctor
      Basic_Block(const Basic_Block<T>& other);

      ~Basic_Block();

      //! Will construct this block from a string like "011101..". Length MUST be 512.
      void FromString(const std::string& str);

      //! Will create a bitset-compatible string ("0101110..") representation
      //! of this block. Length will always be 512.
      std::string ToString() const;

      //! Will matrix-multiply two blocks together.
      //! Since the matrices values are pretty much sudo-random,
      //! they will most likely integer-overflow.
      //! So see this as a one-way function.
      [[nodiscard]] Basic_Block<T> MMul(const Basic_Block<T>& other) const;
      [[nodiscard]] Basic_Block<T> operator*(const Basic_Block<T>& other) const;

      //! Will matrix-multiply two blocks together,
      //! and directly write into this same block.
      //! Since the matrices values are pretty much sudo-random,
      //! they will most likely integer-overflow.
      //! So see this as a one-way function.
      void MMulInplace(const Basic_Block<T>& other);
      Basic_Block<T>& operator*=(const Basic_Block<T>& other);

      //! Will xor two blocks together
      [[nodiscard]] Basic_Block<T> Xor(const Basic_Block<T>& other) const;
      //! Will xor two blocks together
      [[nodiscard]] Basic_Block<T> operator^(const Basic_Block<T>& other) const;

      //! Will xor two blocks together, inplace
      void XorInplace(const Basic_Block<T>& other);
      //! Will xor two blocks together, inplace
      Basic_Block<T>& operator^=(const Basic_Block<T>& other);

      //! Will add all the integer making up this block, one by one
      [[nodiscard]] Basic_Block<T> Add(const Basic_Block<T>& other) const;
      //! Will add all the integer making up this block, one by one
      [[nodiscard]] Basic_Block<T> operator+(const Basic_Block<T>& other) const;

      //! Will add all the integer making up this block, one by one, inplace
      void AddInplace(const Basic_Block<T>& other);
      //! Will add all the integer making up this block, one by one, inplace
      Basic_Block<T>& operator+=(const Basic_Block<T>& other);

      //! Will subtract all the integer making up this block, one by one
      [[nodiscard]] Basic_Block<T> Sub(const Basic_Block<T>& other) const;
      //! Will subtract all the integer making up this block, one by one
      [[nodiscard]] Basic_Block<T> operator-(const Basic_Block<T>& other) const;

      //! Will subtract all the integer making up this block, one by one, inplace
      void SubInplace(const Basic_Block<T>& other);
      //! Will subtract all the integer making up this block, one by one, inplace
      Basic_Block<T>& operator-=(const Basic_Block<T>& other);

      //! Will shift rows upwards by 1
      [[nodiscard]] Basic_Block<T> ShiftRowsUp() const;

      //! Will shift rows upwards by 1
      void ShiftRowsUpInplace();

      //! Will shift matrix rows downwards by 1
      [[nodiscard]] Basic_Block<T> ShiftRowsDown() const;

      //! Will shift matrix rows downwards by 1
      void ShiftRowsDownInplace();

      //! Will shift matrix columns to the left by 1
      [[nodiscard]] Basic_Block<T> ShiftColumnsLeft() const;

      //! Will shift matrix columns to the left by 1
      void ShiftColumnsLeftInplace();

      //! Will shift matrix columns to the right by 1
      [[nodiscard]] Basic_Block<T> ShiftColumnsRight() const;

      //! Will shift matrix columns to the right by 1
      void ShiftColumnsRightInplace();

      //! Will shift array cells to the left by 1
      [[nodiscard]] Basic_Block<T> ShiftCellsLeft() const;

      //! Will shift array cells to the left by 1
      void ShiftCellsLeftInplace();

      //! Will shift array cells to the right by 1
      [[nodiscard]] Basic_Block<T> ShiftCellsRight() const;

      //! Will shift array cells to the right by 1
      void ShiftCellsRightInplace();

      //! Will copy a block
      Basic_Block<T>& operator=(const Basic_Block<T>& other);

      //! Will compare whether or not two blocks are equal
      [[nodiscard]] bool operator==(const Basic_Block<T>& other) const;
      //! Will compare whether or not two blocks are unequal
      [[nodiscard]] bool operator!=(const Basic_Block<T>& other) const;

      //! Will zero all data
      void Reset();

      //! Will return the state of any given bit
      [[nodiscard]] bool GetBit(const std::size_t index) const;

      //! Will set the state of any given bit
      void SetBit(const std::size_t index, const bool state);

      //! Will flip the state of any given bit
      void FlipBit(const std::size_t index);

      //! Will shift all bits to the left by 1
      [[nodiscard]] Basic_Block<T> ShiftBitsLeft() const;

      //! Will shift all bits to the left by 1, inplace
      void ShiftBitsLeftInplace();

      //! Will shift all bits to the right by 1
      [[nodiscard]] Basic_Block<T> ShiftBitsRight() const;

      //! Will shift all bits to the right by 1, inplace
      void ShiftBitsRightInplace();

      //! Returns 32-bit chunks of data, indexed by matrix coordinates (0-3)
      [[nodiscard]] T& Get(const std::uint8_t row, const std::uint8_t column);
      //! Returns 32-bit chunks of data, indexed by matrix coordinates (0-3)
      [[nodiscard]] const T& Get(const std::uint8_t row, const std::uint8_t column) const;

      //! Returns 32-bit chunks of data, indexed by a 1d-index (0-16)
      [[nodiscard]] T& Get(const std::uint8_t index);

      //! Returns 32-bit chunks of data, indexed by a 1d-index (0-16)
      [[nodiscard]] const T& Get(const std::uint8_t index) const;

      //! Returns 32-bit chunks of data, indexed by a 1d-index (0-16)
      [[nodiscard]] T& operator[](const std::uint8_t index);

      //! Returns 32-bit chunks of data, indexed by a 1d-index (0-16)
      [[nodiscard]] const T& operator[](const std::uint8_t index) const;

      static constexpr std::size_t CHUNK_SIZE = sizeof(T);
      static constexpr std::size_t CHUNK_SIZE_BITS = CHUNK_SIZE * 8;

      friend std::ostream& operator<<(std::ostream& os, const Basic_Block<T>& b) {
        for (std::size_t i = 0; i < b.data.size(); i++) {
          os << std::bitset<Basic_Block<T>::CHUNK_SIZE_BITS>(b.data[i]).to_string();
        }
        return os;
      }

    private:

      std::array<T, 16> data;
  };

  // Instantiate templates
  template class Basic_Block<std::uint32_t>;
  //template class Basic_Block<std::uint16_t>;

  //! This a full-sizes 512-bit block
  typedef Basic_Block<std::uint32_t> Block;

  //! This is a half-block used within the feistel class
  //typedef Basic_Block<std::uint16_t> Halfblock;
}

#endif

