#include "GCrypt/BlockMatrix.h"
#include <sstream>

namespace Leonetienne::GCrypt {

  BlockMatrix::BlockMatrix() {
  }

  BlockMatrix::BlockMatrix(const Block& block) {
    FromBlock(block);
  }

  BlockMatrix::BlockMatrix(const BlockMatrix& other) {
    data = other.data;
  }

  void BlockMatrix::FromBlock(const Block& block) {

    const std::string blockstr = block.to_string();
    constexpr std::size_t cellSize = sizeof(std::uint32_t)*8;

    for (std::size_t i = 0; i < 16; i++) {
      data[i] = std::bitset<cellSize>(blockstr.substr(i*cellSize, cellSize)).to_ulong();
    }

    return;
  }

  Block BlockMatrix::ToBlock() const {

    std::stringstream ss;
    constexpr std::size_t cellSize = sizeof(std::uint32_t)*8;

    for (std::size_t i = 0; i < 16; i++) {
      ss << std::bitset<cellSize>(data[i]).to_string();
    }
    return Block(ss.str());
  }

  BlockMatrix BlockMatrix::MMult(const BlockMatrix& o) const {

    BlockMatrix m;

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

  BlockMatrix BlockMatrix::operator*(const BlockMatrix& other) const {
    return this->MMult(other);
  }

  BlockMatrix BlockMatrix::MXor(const BlockMatrix& o) const {

    BlockMatrix m;

    m.Get(0, 0) = this->Get(0, 0) ^ o.Get(0, 0) ^ this->Get(0, 1) ^ o.Get(1, 0) ^ this->Get(0, 2) ^ o.Get(2, 0) ^ this->Get(0, 3) ^ o.Get(3, 0);
    m.Get(0, 1) = this->Get(0, 0) ^ o.Get(0, 1) ^ this->Get(0, 1) ^ o.Get(1, 1) ^ this->Get(0, 2) ^ o.Get(2, 1) ^ this->Get(0, 3) ^ o.Get(3, 1);
    m.Get(0, 2) = this->Get(0, 0) ^ o.Get(0, 2) ^ this->Get(0, 1) ^ o.Get(1, 2) ^ this->Get(0, 2) ^ o.Get(2, 2) ^ this->Get(0, 3) ^ o.Get(3, 2);
    m.Get(0, 3) = this->Get(0, 0) ^ o.Get(0, 3) ^ this->Get(0, 1) ^ o.Get(1, 3) ^ this->Get(0, 2) ^ o.Get(2, 3) ^ this->Get(0, 3) ^ o.Get(3, 3);

    m.Get(1, 0) = this->Get(1, 0) ^ o.Get(0, 0) ^ this->Get(1, 1) ^ o.Get(1, 0) ^ this->Get(1, 2) ^ o.Get(2, 0) ^ this->Get(1, 3) ^ o.Get(3, 0);
    m.Get(1, 1) = this->Get(1, 0) ^ o.Get(0, 1) ^ this->Get(1, 1) ^ o.Get(1, 1) ^ this->Get(1, 2) ^ o.Get(2, 1) ^ this->Get(1, 3) ^ o.Get(3, 1);
    m.Get(1, 2) = this->Get(1, 0) ^ o.Get(0, 2) ^ this->Get(1, 1) ^ o.Get(1, 2) ^ this->Get(1, 2) ^ o.Get(2, 2) ^ this->Get(1, 3) ^ o.Get(3, 2);
    m.Get(1, 3) = this->Get(1, 0) ^ o.Get(0, 3) ^ this->Get(1, 1) ^ o.Get(1, 3) ^ this->Get(1, 2) ^ o.Get(2, 3) ^ this->Get(1, 3) ^ o.Get(3, 3);

    m.Get(2, 0) = this->Get(2, 0) ^ o.Get(0, 0) ^ this->Get(2, 1) ^ o.Get(1, 0) ^ this->Get(2, 2) ^ o.Get(2, 0) ^ this->Get(2, 3) ^ o.Get(3, 0);
    m.Get(2, 1) = this->Get(2, 0) ^ o.Get(0, 1) ^ this->Get(2, 1) ^ o.Get(1, 1) ^ this->Get(2, 2) ^ o.Get(2, 1) ^ this->Get(2, 3) ^ o.Get(3, 1);
    m.Get(2, 2) = this->Get(2, 0) ^ o.Get(0, 2) ^ this->Get(2, 1) ^ o.Get(1, 2) ^ this->Get(2, 2) ^ o.Get(2, 2) ^ this->Get(2, 3) ^ o.Get(3, 2);
    m.Get(2, 3) = this->Get(2, 0) ^ o.Get(0, 3) ^ this->Get(2, 1) ^ o.Get(1, 3) ^ this->Get(2, 2) ^ o.Get(2, 3) ^ this->Get(2, 3) ^ o.Get(3, 3);

    m.Get(3, 0) = this->Get(3, 0) ^ o.Get(0, 0) ^ this->Get(3, 1) ^ o.Get(1, 0) ^ this->Get(3, 2) ^ o.Get(2, 0) ^ this->Get(3, 3) ^ o.Get(3, 0);
    m.Get(3, 1) = this->Get(3, 0) ^ o.Get(0, 1) ^ this->Get(3, 1) ^ o.Get(1, 1) ^ this->Get(3, 2) ^ o.Get(2, 1) ^ this->Get(3, 3) ^ o.Get(3, 1);
    m.Get(3, 2) = this->Get(3, 0) ^ o.Get(0, 2) ^ this->Get(3, 1) ^ o.Get(1, 2) ^ this->Get(3, 2) ^ o.Get(2, 2) ^ this->Get(3, 3) ^ o.Get(3, 2);
    m.Get(3, 3) = this->Get(3, 0) ^ o.Get(0, 3) ^ this->Get(3, 1) ^ o.Get(1, 3) ^ this->Get(3, 2) ^ o.Get(2, 3) ^ this->Get(3, 3) ^ o.Get(3, 3);

    return m;
  }

  const std::uint32_t& BlockMatrix::Get(const std::uint8_t row, const std::uint8_t column) const {
    return data[column*4 + row];
  }

  std::uint32_t& BlockMatrix::Get(const std::uint8_t row, const std::uint8_t column){
    return data[column*4 + row];
  }

  bool BlockMatrix::operator==(const BlockMatrix& other) const {
    return data == other.data;
  }

  bool BlockMatrix::operator!=(const BlockMatrix& other) const {
    return data != other.data;
  }
}

