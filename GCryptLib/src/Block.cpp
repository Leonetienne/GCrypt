#include "GCrypt/Block.h"
#include "GCrypt/Config.h"
#include <sstream>
#include <bitset>
#include <cassert>
#include <cstring>

namespace Leonetienne::GCrypt {

  Block::Block() {
  }

  Block::Block(const std::string& str) {
    FromString(str);
  }

  Block::Block(const Block& other) {
    data = other.data;
  }

  void Block::FromString(const std::string& str) {

    assert(str.length() == BLOCK_SIZE);

    for (std::size_t i = 0; i < data.size(); i++) {
      data[i] = std::bitset<CHUNK_SIZE_BITS>(
          str.substr(i*CHUNK_SIZE_BITS, CHUNK_SIZE_BITS)
      ).to_ulong();
    }

    return;
  }

  std::string Block::ToString() const {

    std::stringstream ss;
    for (std::size_t i = 0; i < data.size(); i++) {
      ss << std::bitset<CHUNK_SIZE_BITS>(data[i]).to_string();
    }
    return ss.str();
  }

  Block Block::MMul(const Block& o) const {

    Block m;

    // Maybe pre-calculate the 1d-index...?

    m.Get(0, 0) = (this->Get(0, 0) * o.Get(0, 0)) + (this->Get(0, 1) * o.Get(1, 0)) + (this->Get(0, 2) * o.Get(2, 0)) + (this->Get(0, 3) * o.Get(3, 0));
    m.Get(0, 1) = (this->Get(0, 0) * o.Get(0, 1)) + (this->Get(0, 1) * o.Get(1, 1)) + (this->Get(0, 2) * o.Get(2, 1)) + (this->Get(0, 3) * o.Get(3, 1));
    m.Get(0, 2) = (this->Get(0, 0) * o.Get(0, 2)) + (this->Get(0, 1) * o.Get(1, 2)) + (this->Get(0, 2) * o.Get(2, 2)) + (this->Get(0, 3) * o.Get(3, 2));
    m.Get(0, 3) = (this->Get(0, 0) * o.Get(0, 3)) + (this->Get(0, 1) * o.Get(1, 3)) + (this->Get(0, 2) * o.Get(2, 3)) + (this->Get(0, 3) * o.Get(3, 3));

    m.Get(1, 0) = (this->Get(1, 0) * o.Get(0, 0)) + (this->Get(1, 1) * o.Get(1, 0)) + (this->Get(1, 2) * o.Get(2, 0)) + (this->Get(1, 3) * o.Get(3, 0));
    m.Get(1, 1) = (this->Get(1, 0) * o.Get(0, 1)) + (this->Get(1, 1) * o.Get(1, 1)) + (this->Get(1, 2) * o.Get(2, 1)) + (this->Get(1, 3) * o.Get(3, 1));
    m.Get(1, 2) = (this->Get(1, 0) * o.Get(0, 2)) + (this->Get(1, 1) * o.Get(1, 2)) + (this->Get(1, 2) * o.Get(2, 2)) + (this->Get(1, 3) * o.Get(3, 2));
    m.Get(1, 3) = (this->Get(1, 0) * o.Get(0, 3)) + (this->Get(1, 1) * o.Get(1, 3)) + (this->Get(1, 2) * o.Get(2, 3)) + (this->Get(1, 3) * o.Get(3, 3));

    m.Get(2, 0) = (this->Get(2, 0) * o.Get(0, 0)) + (this->Get(2, 1) * o.Get(1, 0)) + (this->Get(2, 2) * o.Get(2, 0)) + (this->Get(2, 3) * o.Get(3, 0));
    m.Get(2, 1) = (this->Get(2, 0) * o.Get(0, 1)) + (this->Get(2, 1) * o.Get(1, 1)) + (this->Get(2, 2) * o.Get(2, 1)) + (this->Get(2, 3) * o.Get(3, 1));
    m.Get(2, 2) = (this->Get(2, 0) * o.Get(0, 2)) + (this->Get(2, 1) * o.Get(1, 2)) + (this->Get(2, 2) * o.Get(2, 2)) + (this->Get(2, 3) * o.Get(3, 2));
    m.Get(2, 3) = (this->Get(2, 0) * o.Get(0, 3)) + (this->Get(2, 1) * o.Get(1, 3)) + (this->Get(2, 2) * o.Get(2, 3)) + (this->Get(2, 3) * o.Get(3, 3));

    m.Get(3, 0) = (this->Get(3, 0) * o.Get(0, 0)) + (this->Get(3, 1) * o.Get(1, 0)) + (this->Get(3, 2) * o.Get(2, 0)) + (this->Get(3, 3) * o.Get(3, 0));
    m.Get(3, 1) = (this->Get(3, 0) * o.Get(0, 1)) + (this->Get(3, 1) * o.Get(1, 1)) + (this->Get(3, 2) * o.Get(2, 1)) + (this->Get(3, 3) * o.Get(3, 1));
    m.Get(3, 2) = (this->Get(3, 0) * o.Get(0, 2)) + (this->Get(3, 1) * o.Get(1, 2)) + (this->Get(3, 2) * o.Get(2, 2)) + (this->Get(3, 3) * o.Get(3, 2));
    m.Get(3, 3) = (this->Get(3, 0) * o.Get(0, 3)) + (this->Get(3, 1) * o.Get(1, 3)) + (this->Get(3, 2) * o.Get(2, 3)) + (this->Get(3, 3) * o.Get(3, 3));

    return m;
  }

  Block Block::operator*(const Block& other) const {
    return this->MMul(other);
  }

  void Block::MMulInplace(const Block& o) {

    Block m = *this;

    // Maybe pre-calculate the 1d-index...?

    this->Get(0, 0) = (m.Get(0, 0) * o.Get(0, 0)) + (m.Get(0, 1) * o.Get(1, 0)) + (m.Get(0, 2) * o.Get(2, 0)) + (m.Get(0, 3) * o.Get(3, 0));
    this->Get(0, 1) = (m.Get(0, 0) * o.Get(0, 1)) + (m.Get(0, 1) * o.Get(1, 1)) + (m.Get(0, 2) * o.Get(2, 1)) + (m.Get(0, 3) * o.Get(3, 1));
    this->Get(0, 2) = (m.Get(0, 0) * o.Get(0, 2)) + (m.Get(0, 1) * o.Get(1, 2)) + (m.Get(0, 2) * o.Get(2, 2)) + (m.Get(0, 3) * o.Get(3, 2));
    this->Get(0, 3) = (m.Get(0, 0) * o.Get(0, 3)) + (m.Get(0, 1) * o.Get(1, 3)) + (m.Get(0, 2) * o.Get(2, 3)) + (m.Get(0, 3) * o.Get(3, 3));

    this->Get(1, 0) = (m.Get(1, 0) * o.Get(0, 0)) + (m.Get(1, 1) * o.Get(1, 0)) + (m.Get(1, 2) * o.Get(2, 0)) + (m.Get(1, 3) * o.Get(3, 0));
    this->Get(1, 1) = (m.Get(1, 0) * o.Get(0, 1)) + (m.Get(1, 1) * o.Get(1, 1)) + (m.Get(1, 2) * o.Get(2, 1)) + (m.Get(1, 3) * o.Get(3, 1));
    this->Get(1, 2) = (m.Get(1, 0) * o.Get(0, 2)) + (m.Get(1, 1) * o.Get(1, 2)) + (m.Get(1, 2) * o.Get(2, 2)) + (m.Get(1, 3) * o.Get(3, 2));
    this->Get(1, 3) = (m.Get(1, 0) * o.Get(0, 3)) + (m.Get(1, 1) * o.Get(1, 3)) + (m.Get(1, 2) * o.Get(2, 3)) + (m.Get(1, 3) * o.Get(3, 3));

    this->Get(2, 0) = (m.Get(2, 0) * o.Get(0, 0)) + (m.Get(2, 1) * o.Get(1, 0)) + (m.Get(2, 2) * o.Get(2, 0)) + (m.Get(2, 3) * o.Get(3, 0));
    this->Get(2, 1) = (m.Get(2, 0) * o.Get(0, 1)) + (m.Get(2, 1) * o.Get(1, 1)) + (m.Get(2, 2) * o.Get(2, 1)) + (m.Get(2, 3) * o.Get(3, 1));
    this->Get(2, 2) = (m.Get(2, 0) * o.Get(0, 2)) + (m.Get(2, 1) * o.Get(1, 2)) + (m.Get(2, 2) * o.Get(2, 2)) + (m.Get(2, 3) * o.Get(3, 2));
    this->Get(2, 3) = (m.Get(2, 0) * o.Get(0, 3)) + (m.Get(2, 1) * o.Get(1, 3)) + (m.Get(2, 2) * o.Get(2, 3)) + (m.Get(2, 3) * o.Get(3, 3));

    this->Get(3, 0) = (m.Get(3, 0) * o.Get(0, 0)) + (m.Get(3, 1) * o.Get(1, 0)) + (m.Get(3, 2) * o.Get(2, 0)) + (m.Get(3, 3) * o.Get(3, 0));
    this->Get(3, 1) = (m.Get(3, 0) * o.Get(0, 1)) + (m.Get(3, 1) * o.Get(1, 1)) + (m.Get(3, 2) * o.Get(2, 1)) + (m.Get(3, 3) * o.Get(3, 1));
    this->Get(3, 2) = (m.Get(3, 0) * o.Get(0, 2)) + (m.Get(3, 1) * o.Get(1, 2)) + (m.Get(3, 2) * o.Get(2, 2)) + (m.Get(3, 3) * o.Get(3, 2));
    this->Get(3, 3) = (m.Get(3, 0) * o.Get(0, 3)) + (m.Get(3, 1) * o.Get(1, 3)) + (m.Get(3, 2) * o.Get(2, 3)) + (m.Get(3, 3) * o.Get(3, 3));

    return;
  }

  Block& Block::operator*=(const Block& other) {
    MMulInplace(other);
    return *this;
  }

  Block Block::Xor(const Block& other) const {

    Block m;
    for (std::size_t i = 0; i < data.size(); i++) {
      m.Get(i) = this->Get(i) ^ other.Get(i);
    }
    return m;
  }

  Block Block::operator^(const Block& other) const {
    return Xor(other);
  }

  void Block::XorInplace(const Block& other) {
    for (std::size_t i = 0; i < data.size(); i++) {
      this->Get(i) ^= other.Get(i);
    }
    return;
  }

  Block& Block::operator^=(const Block& other) {
    XorInplace(other);
    return *this;
  }

  void ShiftRowsUp(const std::size_t n) {
    // TO BE IMPLEMENTED
  }

  void ShiftRowsDown(const std::size_t n) {
    // TO BE IMPLEMENTED
  }

  void ShiftColumnsLeft(const std::size_t n) {
    // TO BE IMPLEMENTED
  }

  void ShiftColumnsRight(const std::size_t n) {
    // TO BE IMPLEMENTED
  }

  void ShiftCellsLeft(const std::size_t n) {
    // TO BE IMPLEMENTED
  }

  void ShiftCellsRight(const std::size_t n) {
    // TO BE IMPLEMENTED
  }

  Block& Block::operator=(const Block& other) {
    data = other.data;
    return *this;
  }

  std::uint32_t& Block::Get(const std::uint8_t row, const std::uint8_t column){
    return data[column*4 + row];
  }

  const std::uint32_t& Block::Get(const std::uint8_t row, const std::uint8_t column) const {
    return data[column*4 + row];
  }

  std::uint32_t& Block::Get(const std::uint8_t index) {
    return data[index];
  }

  const std::uint32_t& Block::Get(const std::uint8_t index) const {
    return data[index];
  }

  std::uint32_t& Block::operator[](const std::uint8_t index) {
    return data[index];
  }

  const std::uint32_t& Block::operator[](const std::uint8_t index) const {
    return data[index];
  }

  bool Block::operator==(const Block& other) const {
    return data == other.data;
  }

  bool Block::operator!=(const Block& other) const {
    return data != other.data;
  }

  std::ostream& operator<<(std::ostream& os, const Block& b) {
    for (std::size_t i = 0; i < b.data.size(); i++) {
      os << std::bitset<b.CHUNK_SIZE_BITS>(b.data[i]).to_string();
    }
    return os;
  }

#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
  Block::~Block() {
    Reset();
  }

  void Block::Reset() {
    memset(data.data(), 0, CHUNK_SIZE*data.size());
    return;
  }

#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif
}

