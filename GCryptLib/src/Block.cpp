#include "GCrypt/Block.h"
#include "GCrypt/Config.h"
#include "GCrypt/Util.h"
#include <sstream>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <ios>

// Just to be sure, the compiler will optimize this
// little formula out, let's do it in the preprocessor
namespace {
  constexpr std::size_t MAT_INDEX(const std::size_t row, const std::size_t column) {
    return column*4 + row;
  }
}

namespace Leonetienne::GCrypt {

  template <typename T>
  Basic_Block<T>::Basic_Block() {
  }

  template <typename T>
  Basic_Block<T>::Basic_Block(const std::string& str) {
    FromBinaryString(str);
  }

  template <typename T>
  Basic_Block<T>::Basic_Block(const Basic_Block<T>& other) {
    data = other.data;
  }

  template <typename T>
  void Basic_Block<T>::FromBinaryString(const std::string& str) {

    if (str.length() != BLOCK_SIZE_BITS) {
      throw std::invalid_argument(
        std::string("Unable to read binary block: \"") + str + "\": Length is not BLOCK_SIZE_BITS."
      );
    }

    for (std::size_t i = 0; i < data.size(); i++) {
      data[i] = std::bitset<CHUNK_SIZE_BITS>(
          str.substr(i*CHUNK_SIZE_BITS, CHUNK_SIZE_BITS)
      ).to_ulong();
    }

    return;
  }

  template <typename T>
  void Basic_Block<T>::FromHexString(const std::string& str) {

    if (str.length() != BLOCK_SIZE*2) {
      throw std::invalid_argument(
        std::string("Unable to read hex block: \"") + str + "\": Length is not BLOCK_SIZE*2."
      );
    }

    for (std::size_t i = 0; i < str.length(); i += CHUNK_SIZE*2) {
      const std::string hexChunk = str.substr(i, CHUNK_SIZE*2);
      try {
          data[i / (CHUNK_SIZE*2)] = std::stoul(hexChunk, NULL, 16);
        }
      catch (std::invalid_argument&) {
          throw std::invalid_argument(
              std::string("Unable to read hex block: \"") + hexChunk + "\"."
          );
      }
    }

    return;
  }

  template <typename T>
  void Basic_Block<T>::FromByteString(const std::string& str) {

    if (str.length() != BLOCK_SIZE) {
      throw std::invalid_argument(
        std::string("Unable to read byte block: \"") + str + "\": Length is not BLOCK_SIZE."
      );
    }

    // Iterate over all bytes in the block
    std::uint8_t* curByte = (std::uint8_t*)(void*)Data();
    const char* strIt = 0;
    for (std::size_t i = 0; i < BLOCK_SIZE; i++) {
      *curByte++ = str[i];
    }

    return;
  }

  template <typename T>
  void Basic_Block<T>::FromTextString(const std::string& str) {
    // Just pad the input string to lenght, and treat it as a byte string
    FromByteString(
        PadStringToLength(str, BLOCK_SIZE, '\0', false)
    );
  }

  template <typename T>
  std::string Basic_Block<T>::ToBinaryString() const {

    std::stringstream ss;
    for (std::size_t i = 0; i < data.size(); i++) {
      ss << std::bitset<CHUNK_SIZE_BITS>(data[i]).to_string();
    }
    return ss.str();
  }

  template <typename T>
  std::string Basic_Block<T>::ToHexString() const {

    std::stringstream ss;
    for (std::size_t i = 0; i < data.size(); i++) {
      ss
        << std::setfill('0')
        << std::setw(CHUNK_SIZE*2)
        << std::hex
        << data[i]
      ;
    }

    return ss.str();
  }

  template <typename T>
  std::string Basic_Block<T>::ToByteString() const {

    std::stringstream ss;
    ss.write((const char*)(void*)Data(), BLOCK_SIZE);
    return ss.str();
  }

  template <typename T>
  std::string Basic_Block<T>::ToTextString() const {

    std::string bytes = ToByteString();

    // Trim extra nullterminators
    bytes.resize(strlen(bytes.data()));

    return bytes;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::MMul(const Basic_Block<T>& o) const {

    Basic_Block<T> m;

    // Maybe pre-calculate the 1d-index...?

    m.Get(MAT_INDEX(0, 0)) = (this->Get(MAT_INDEX(0, 0)) * o.Get(MAT_INDEX(0, 0))) + (this->Get(MAT_INDEX(0, 1)) * o.Get(MAT_INDEX(1, 0))) + (this->Get(MAT_INDEX(0, 2)) * o.Get(MAT_INDEX(2, 0))) + (this->Get(MAT_INDEX(0, 3)) * o.Get(MAT_INDEX(3, 0)));
    m.Get(MAT_INDEX(0, 1)) = (this->Get(MAT_INDEX(0, 0)) * o.Get(MAT_INDEX(0, 1))) + (this->Get(MAT_INDEX(0, 1)) * o.Get(MAT_INDEX(1, 1))) + (this->Get(MAT_INDEX(0, 2)) * o.Get(MAT_INDEX(2, 1))) + (this->Get(MAT_INDEX(0, 3)) * o.Get(MAT_INDEX(3, 1)));
    m.Get(MAT_INDEX(0, 2)) = (this->Get(MAT_INDEX(0, 0)) * o.Get(MAT_INDEX(0, 2))) + (this->Get(MAT_INDEX(0, 1)) * o.Get(MAT_INDEX(1, 2))) + (this->Get(MAT_INDEX(0, 2)) * o.Get(MAT_INDEX(2, 2))) + (this->Get(MAT_INDEX(0, 3)) * o.Get(MAT_INDEX(3, 2)));
    m.Get(MAT_INDEX(0, 3)) = (this->Get(MAT_INDEX(0, 0)) * o.Get(MAT_INDEX(0, 3))) + (this->Get(MAT_INDEX(0, 1)) * o.Get(MAT_INDEX(1, 3))) + (this->Get(MAT_INDEX(0, 2)) * o.Get(MAT_INDEX(2, 3))) + (this->Get(MAT_INDEX(0, 3)) * o.Get(MAT_INDEX(3, 3)));

    m.Get(MAT_INDEX(1, 0)) = (this->Get(MAT_INDEX(1, 0)) * o.Get(MAT_INDEX(0, 0))) + (this->Get(MAT_INDEX(1, 1)) * o.Get(MAT_INDEX(1, 0))) + (this->Get(MAT_INDEX(1, 2)) * o.Get(MAT_INDEX(2, 0))) + (this->Get(MAT_INDEX(1, 3)) * o.Get(MAT_INDEX(3, 0)));
    m.Get(MAT_INDEX(1, 1)) = (this->Get(MAT_INDEX(1, 0)) * o.Get(MAT_INDEX(0, 1))) + (this->Get(MAT_INDEX(1, 1)) * o.Get(MAT_INDEX(1, 1))) + (this->Get(MAT_INDEX(1, 2)) * o.Get(MAT_INDEX(2, 1))) + (this->Get(MAT_INDEX(1, 3)) * o.Get(MAT_INDEX(3, 1)));
    m.Get(MAT_INDEX(1, 2)) = (this->Get(MAT_INDEX(1, 0)) * o.Get(MAT_INDEX(0, 2))) + (this->Get(MAT_INDEX(1, 1)) * o.Get(MAT_INDEX(1, 2))) + (this->Get(MAT_INDEX(1, 2)) * o.Get(MAT_INDEX(2, 2))) + (this->Get(MAT_INDEX(1, 3)) * o.Get(MAT_INDEX(3, 2)));
    m.Get(MAT_INDEX(1, 3)) = (this->Get(MAT_INDEX(1, 0)) * o.Get(MAT_INDEX(0, 3))) + (this->Get(MAT_INDEX(1, 1)) * o.Get(MAT_INDEX(1, 3))) + (this->Get(MAT_INDEX(1, 2)) * o.Get(MAT_INDEX(2, 3))) + (this->Get(MAT_INDEX(1, 3)) * o.Get(MAT_INDEX(3, 3)));

    m.Get(MAT_INDEX(2, 0)) = (this->Get(MAT_INDEX(2, 0)) * o.Get(MAT_INDEX(0, 0))) + (this->Get(MAT_INDEX(2, 1)) * o.Get(MAT_INDEX(1, 0))) + (this->Get(MAT_INDEX(2, 2)) * o.Get(MAT_INDEX(2, 0))) + (this->Get(MAT_INDEX(2, 3)) * o.Get(MAT_INDEX(3, 0)));
    m.Get(MAT_INDEX(2, 1)) = (this->Get(MAT_INDEX(2, 0)) * o.Get(MAT_INDEX(0, 1))) + (this->Get(MAT_INDEX(2, 1)) * o.Get(MAT_INDEX(1, 1))) + (this->Get(MAT_INDEX(2, 2)) * o.Get(MAT_INDEX(2, 1))) + (this->Get(MAT_INDEX(2, 3)) * o.Get(MAT_INDEX(3, 1)));
    m.Get(MAT_INDEX(2, 2)) = (this->Get(MAT_INDEX(2, 0)) * o.Get(MAT_INDEX(0, 2))) + (this->Get(MAT_INDEX(2, 1)) * o.Get(MAT_INDEX(1, 2))) + (this->Get(MAT_INDEX(2, 2)) * o.Get(MAT_INDEX(2, 2))) + (this->Get(MAT_INDEX(2, 3)) * o.Get(MAT_INDEX(3, 2)));
    m.Get(MAT_INDEX(2, 3)) = (this->Get(MAT_INDEX(2, 0)) * o.Get(MAT_INDEX(0, 3))) + (this->Get(MAT_INDEX(2, 1)) * o.Get(MAT_INDEX(1, 3))) + (this->Get(MAT_INDEX(2, 2)) * o.Get(MAT_INDEX(2, 3))) + (this->Get(MAT_INDEX(2, 3)) * o.Get(MAT_INDEX(3, 3)));

    m.Get(MAT_INDEX(3, 0)) = (this->Get(MAT_INDEX(3, 0)) * o.Get(MAT_INDEX(0, 0))) + (this->Get(MAT_INDEX(3, 1)) * o.Get(MAT_INDEX(1, 0))) + (this->Get(MAT_INDEX(3, 2)) * o.Get(MAT_INDEX(2, 0))) + (this->Get(MAT_INDEX(3, 3)) * o.Get(MAT_INDEX(3, 0)));
    m.Get(MAT_INDEX(3, 1)) = (this->Get(MAT_INDEX(3, 0)) * o.Get(MAT_INDEX(0, 1))) + (this->Get(MAT_INDEX(3, 1)) * o.Get(MAT_INDEX(1, 1))) + (this->Get(MAT_INDEX(3, 2)) * o.Get(MAT_INDEX(2, 1))) + (this->Get(MAT_INDEX(3, 3)) * o.Get(MAT_INDEX(3, 1)));
    m.Get(MAT_INDEX(3, 2)) = (this->Get(MAT_INDEX(3, 0)) * o.Get(MAT_INDEX(0, 2))) + (this->Get(MAT_INDEX(3, 1)) * o.Get(MAT_INDEX(1, 2))) + (this->Get(MAT_INDEX(3, 2)) * o.Get(MAT_INDEX(2, 2))) + (this->Get(MAT_INDEX(3, 3)) * o.Get(MAT_INDEX(3, 2)));
    m.Get(MAT_INDEX(3, 3)) = (this->Get(MAT_INDEX(3, 0)) * o.Get(MAT_INDEX(0, 3))) + (this->Get(MAT_INDEX(3, 1)) * o.Get(MAT_INDEX(1, 3))) + (this->Get(MAT_INDEX(3, 2)) * o.Get(MAT_INDEX(2, 3))) + (this->Get(MAT_INDEX(3, 3)) * o.Get(MAT_INDEX(3, 3)));

    return m;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::operator*(const Basic_Block<T>& other) const {
    return this->MMul(other);
  }

  template <typename T>
  void Basic_Block<T>::MMulInplace(const Basic_Block<T>& o) {

    Basic_Block<T> m = *this;

    // Maybe pre-calculate the 1d-index...?

    this->Get(MAT_INDEX(0, 0)) = (m.Get(MAT_INDEX(0, 0)) * o.Get(MAT_INDEX(0, 0))) + (m.Get(MAT_INDEX(0, 1)) * o.Get(MAT_INDEX(1, 0))) + (m.Get(MAT_INDEX(0, 2)) * o.Get(MAT_INDEX(2, 0))) + (m.Get(MAT_INDEX(0, 3)) * o.Get(MAT_INDEX(3, 0)));
    this->Get(MAT_INDEX(0, 1)) = (m.Get(MAT_INDEX(0, 0)) * o.Get(MAT_INDEX(0, 1))) + (m.Get(MAT_INDEX(0, 1)) * o.Get(MAT_INDEX(1, 1))) + (m.Get(MAT_INDEX(0, 2)) * o.Get(MAT_INDEX(2, 1))) + (m.Get(MAT_INDEX(0, 3)) * o.Get(MAT_INDEX(3, 1)));
    this->Get(MAT_INDEX(0, 2)) = (m.Get(MAT_INDEX(0, 0)) * o.Get(MAT_INDEX(0, 2))) + (m.Get(MAT_INDEX(0, 1)) * o.Get(MAT_INDEX(1, 2))) + (m.Get(MAT_INDEX(0, 2)) * o.Get(MAT_INDEX(2, 2))) + (m.Get(MAT_INDEX(0, 3)) * o.Get(MAT_INDEX(3, 2)));
    this->Get(MAT_INDEX(0, 3)) = (m.Get(MAT_INDEX(0, 0)) * o.Get(MAT_INDEX(0, 3))) + (m.Get(MAT_INDEX(0, 1)) * o.Get(MAT_INDEX(1, 3))) + (m.Get(MAT_INDEX(0, 2)) * o.Get(MAT_INDEX(2, 3))) + (m.Get(MAT_INDEX(0, 3)) * o.Get(MAT_INDEX(3, 3)));

    this->Get(MAT_INDEX(1, 0)) = (m.Get(MAT_INDEX(1, 0)) * o.Get(MAT_INDEX(0, 0))) + (m.Get(MAT_INDEX(1, 1)) * o.Get(MAT_INDEX(1, 0))) + (m.Get(MAT_INDEX(1, 2)) * o.Get(MAT_INDEX(2, 0))) + (m.Get(MAT_INDEX(1, 3)) * o.Get(MAT_INDEX(3, 0)));
    this->Get(MAT_INDEX(1, 1)) = (m.Get(MAT_INDEX(1, 0)) * o.Get(MAT_INDEX(0, 1))) + (m.Get(MAT_INDEX(1, 1)) * o.Get(MAT_INDEX(1, 1))) + (m.Get(MAT_INDEX(1, 2)) * o.Get(MAT_INDEX(2, 1))) + (m.Get(MAT_INDEX(1, 3)) * o.Get(MAT_INDEX(3, 1)));
    this->Get(MAT_INDEX(1, 2)) = (m.Get(MAT_INDEX(1, 0)) * o.Get(MAT_INDEX(0, 2))) + (m.Get(MAT_INDEX(1, 1)) * o.Get(MAT_INDEX(1, 2))) + (m.Get(MAT_INDEX(1, 2)) * o.Get(MAT_INDEX(2, 2))) + (m.Get(MAT_INDEX(1, 3)) * o.Get(MAT_INDEX(3, 2)));
    this->Get(MAT_INDEX(1, 3)) = (m.Get(MAT_INDEX(1, 0)) * o.Get(MAT_INDEX(0, 3))) + (m.Get(MAT_INDEX(1, 1)) * o.Get(MAT_INDEX(1, 3))) + (m.Get(MAT_INDEX(1, 2)) * o.Get(MAT_INDEX(2, 3))) + (m.Get(MAT_INDEX(1, 3)) * o.Get(MAT_INDEX(3, 3)));

    this->Get(MAT_INDEX(2, 0)) = (m.Get(MAT_INDEX(2, 0)) * o.Get(MAT_INDEX(0, 0))) + (m.Get(MAT_INDEX(2, 1)) * o.Get(MAT_INDEX(1, 0))) + (m.Get(MAT_INDEX(2, 2)) * o.Get(MAT_INDEX(2, 0))) + (m.Get(MAT_INDEX(2, 3)) * o.Get(MAT_INDEX(3, 0)));
    this->Get(MAT_INDEX(2, 1)) = (m.Get(MAT_INDEX(2, 0)) * o.Get(MAT_INDEX(0, 1))) + (m.Get(MAT_INDEX(2, 1)) * o.Get(MAT_INDEX(1, 1))) + (m.Get(MAT_INDEX(2, 2)) * o.Get(MAT_INDEX(2, 1))) + (m.Get(MAT_INDEX(2, 3)) * o.Get(MAT_INDEX(3, 1)));
    this->Get(MAT_INDEX(2, 2)) = (m.Get(MAT_INDEX(2, 0)) * o.Get(MAT_INDEX(0, 2))) + (m.Get(MAT_INDEX(2, 1)) * o.Get(MAT_INDEX(1, 2))) + (m.Get(MAT_INDEX(2, 2)) * o.Get(MAT_INDEX(2, 2))) + (m.Get(MAT_INDEX(2, 3)) * o.Get(MAT_INDEX(3, 2)));
    this->Get(MAT_INDEX(2, 3)) = (m.Get(MAT_INDEX(2, 0)) * o.Get(MAT_INDEX(0, 3))) + (m.Get(MAT_INDEX(2, 1)) * o.Get(MAT_INDEX(1, 3))) + (m.Get(MAT_INDEX(2, 2)) * o.Get(MAT_INDEX(2, 3))) + (m.Get(MAT_INDEX(2, 3)) * o.Get(MAT_INDEX(3, 3)));

    this->Get(MAT_INDEX(3, 0)) = (m.Get(MAT_INDEX(3, 0)) * o.Get(MAT_INDEX(0, 0))) + (m.Get(MAT_INDEX(3, 1)) * o.Get(MAT_INDEX(1, 0))) + (m.Get(MAT_INDEX(3, 2)) * o.Get(MAT_INDEX(2, 0))) + (m.Get(MAT_INDEX(3, 3)) * o.Get(MAT_INDEX(3, 0)));
    this->Get(MAT_INDEX(3, 1)) = (m.Get(MAT_INDEX(3, 0)) * o.Get(MAT_INDEX(0, 1))) + (m.Get(MAT_INDEX(3, 1)) * o.Get(MAT_INDEX(1, 1))) + (m.Get(MAT_INDEX(3, 2)) * o.Get(MAT_INDEX(2, 1))) + (m.Get(MAT_INDEX(3, 3)) * o.Get(MAT_INDEX(3, 1)));
    this->Get(MAT_INDEX(3, 2)) = (m.Get(MAT_INDEX(3, 0)) * o.Get(MAT_INDEX(0, 2))) + (m.Get(MAT_INDEX(3, 1)) * o.Get(MAT_INDEX(1, 2))) + (m.Get(MAT_INDEX(3, 2)) * o.Get(MAT_INDEX(2, 2))) + (m.Get(MAT_INDEX(3, 3)) * o.Get(MAT_INDEX(3, 2)));
    this->Get(MAT_INDEX(3, 3)) = (m.Get(MAT_INDEX(3, 0)) * o.Get(MAT_INDEX(0, 3))) + (m.Get(MAT_INDEX(3, 1)) * o.Get(MAT_INDEX(1, 3))) + (m.Get(MAT_INDEX(3, 2)) * o.Get(MAT_INDEX(2, 3))) + (m.Get(MAT_INDEX(3, 3)) * o.Get(MAT_INDEX(3, 3)));

    return;
  }

  template <typename T>
  Basic_Block<T>& Basic_Block<T>::operator*=(const Basic_Block<T>& other) {
    MMulInplace(other);
    return *this;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::Xor(const Basic_Block<T>& other) const {

    Basic_Block<T> m;
    for (std::size_t i = 0; i < data.size(); i++) {
      m.Get(i) = this->Get(i) ^ other.Get(i);
    }
    return m;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::operator^(const Basic_Block<T>& other) const {
    return Xor(other);
  }

  template <typename T>
  void Basic_Block<T>::XorInplace(const Basic_Block<T>& other) {
    for (std::size_t i = 0; i < data.size(); i++) {
      this->Get(i) ^= other.Get(i);
    }
    return;
  }

  template <typename T>
  Basic_Block<T>& Basic_Block<T>::operator^=(const Basic_Block<T>& other) {
    XorInplace(other);
    return *this;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::Add(const Basic_Block<T>& other) const {

    Basic_Block<T> m;
    for (std::size_t i = 0; i < data.size(); i++) {
      m.Get(i) = this->Get(i) + other.Get(i);
    }
    return m;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::operator+(const Basic_Block<T>& other) const {
    return Add(other);
  }

  template <typename T>
  void Basic_Block<T>::AddInplace(const Basic_Block<T>& other) {
    for (std::size_t i = 0; i < data.size(); i++) {
      this->Get(i) += other.Get(i);
    }
    return;
  }

  template <typename T>
  Basic_Block<T>& Basic_Block<T>::operator+=(const Basic_Block<T>& other) {
    AddInplace(other);
    return *this;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::Sub(const Basic_Block<T>& other) const {

    Basic_Block<T> m;
    for (std::size_t i = 0; i < data.size(); i++) {
      m.Get(i) = this->Get(i) - other.Get(i);
    }
    return m;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::operator-(const Basic_Block<T>& other) const {
    return Sub(other);
  }

  template <typename T>
  void Basic_Block<T>::SubInplace(const Basic_Block<T>& other) {
    for (std::size_t i = 0; i < data.size(); i++) {
      this->Get(i) -= other.Get(i);
    }
    return;
  }

  template <typename T>
  Basic_Block<T>& Basic_Block<T>::operator-=(const Basic_Block<T>& other) {
    SubInplace(other);
    return *this;
  }

  template <typename T>
  void Basic_Block<T>::ShiftRowsUpInplace() {
    Basic_Block<T> tmp = *this;

    Get(MAT_INDEX(0, 0)) = tmp.Get(MAT_INDEX(1, 0));
    Get(MAT_INDEX(0, 1)) = tmp.Get(MAT_INDEX(1, 1));
    Get(MAT_INDEX(0, 2)) = tmp.Get(MAT_INDEX(1, 2));
    Get(MAT_INDEX(0, 3)) = tmp.Get(MAT_INDEX(1, 3));

    Get(MAT_INDEX(1, 0)) = tmp.Get(MAT_INDEX(2, 0));
    Get(MAT_INDEX(1, 1)) = tmp.Get(MAT_INDEX(2, 1));
    Get(MAT_INDEX(1, 2)) = tmp.Get(MAT_INDEX(2, 2));
    Get(MAT_INDEX(1, 3)) = tmp.Get(MAT_INDEX(2, 3));

    Get(MAT_INDEX(2, 0)) = tmp.Get(MAT_INDEX(3, 0));
    Get(MAT_INDEX(2, 1)) = tmp.Get(MAT_INDEX(3, 1));
    Get(MAT_INDEX(2, 2)) = tmp.Get(MAT_INDEX(3, 2));
    Get(MAT_INDEX(2, 3)) = tmp.Get(MAT_INDEX(3, 3));

    Get(MAT_INDEX(3, 0)) = tmp.Get(MAT_INDEX(0, 0));
    Get(MAT_INDEX(3, 1)) = tmp.Get(MAT_INDEX(0, 1));
    Get(MAT_INDEX(3, 2)) = tmp.Get(MAT_INDEX(0, 2));
    Get(MAT_INDEX(3, 3)) = tmp.Get(MAT_INDEX(0, 3));

    return;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::ShiftRowsUp() const {
    Basic_Block<T> b;

    b.Get(MAT_INDEX(0, 0)) = Get(MAT_INDEX(1, 0));
    b.Get(MAT_INDEX(0, 1)) = Get(MAT_INDEX(1, 1));
    b.Get(MAT_INDEX(0, 2)) = Get(MAT_INDEX(1, 2));
    b.Get(MAT_INDEX(0, 3)) = Get(MAT_INDEX(1, 3));

    b.Get(MAT_INDEX(1, 0)) = Get(MAT_INDEX(2, 0));
    b.Get(MAT_INDEX(1, 1)) = Get(MAT_INDEX(2, 1));
    b.Get(MAT_INDEX(1, 2)) = Get(MAT_INDEX(2, 2));
    b.Get(MAT_INDEX(1, 3)) = Get(MAT_INDEX(2, 3));

    b.Get(MAT_INDEX(2, 0)) = Get(MAT_INDEX(3, 0));
    b.Get(MAT_INDEX(2, 1)) = Get(MAT_INDEX(3, 1));
    b.Get(MAT_INDEX(2, 2)) = Get(MAT_INDEX(3, 2));
    b.Get(MAT_INDEX(2, 3)) = Get(MAT_INDEX(3, 3));

    b.Get(MAT_INDEX(3, 0)) = Get(MAT_INDEX(0, 0));
    b.Get(MAT_INDEX(3, 1)) = Get(MAT_INDEX(0, 1));
    b.Get(MAT_INDEX(3, 2)) = Get(MAT_INDEX(0, 2));
    b.Get(MAT_INDEX(3, 3)) = Get(MAT_INDEX(0, 3));

    return b;
  }

  template <typename T>
  void Basic_Block<T>::ShiftRowsDownInplace() {
    Basic_Block<T> tmp = *this;

    Get(MAT_INDEX(0, 0)) = tmp.Get(MAT_INDEX(3, 0));
    Get(MAT_INDEX(0, 1)) = tmp.Get(MAT_INDEX(3, 1));
    Get(MAT_INDEX(0, 2)) = tmp.Get(MAT_INDEX(3, 2));
    Get(MAT_INDEX(0, 3)) = tmp.Get(MAT_INDEX(3, 3));

    Get(MAT_INDEX(1, 0)) = tmp.Get(MAT_INDEX(0, 0));
    Get(MAT_INDEX(1, 1)) = tmp.Get(MAT_INDEX(0, 1));
    Get(MAT_INDEX(1, 2)) = tmp.Get(MAT_INDEX(0, 2));
    Get(MAT_INDEX(1, 3)) = tmp.Get(MAT_INDEX(0, 3));

    Get(MAT_INDEX(2, 0)) = tmp.Get(MAT_INDEX(1, 0));
    Get(MAT_INDEX(2, 1)) = tmp.Get(MAT_INDEX(1, 1));
    Get(MAT_INDEX(2, 2)) = tmp.Get(MAT_INDEX(1, 2));
    Get(MAT_INDEX(2, 3)) = tmp.Get(MAT_INDEX(1, 3));

    Get(MAT_INDEX(3, 0)) = tmp.Get(MAT_INDEX(2, 0));
    Get(MAT_INDEX(3, 1)) = tmp.Get(MAT_INDEX(2, 1));
    Get(MAT_INDEX(3, 2)) = tmp.Get(MAT_INDEX(2, 2));
    Get(MAT_INDEX(3, 3)) = tmp.Get(MAT_INDEX(2, 3));

    return;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::ShiftRowsDown() const {
    Basic_Block<T> b;

    b.Get(MAT_INDEX(0, 0)) = Get(MAT_INDEX(3, 0));
    b.Get(MAT_INDEX(0, 1)) = Get(MAT_INDEX(3, 1));
    b.Get(MAT_INDEX(0, 2)) = Get(MAT_INDEX(3, 2));
    b.Get(MAT_INDEX(0, 3)) = Get(MAT_INDEX(3, 3));

    b.Get(MAT_INDEX(1, 0)) = Get(MAT_INDEX(0, 0));
    b.Get(MAT_INDEX(1, 1)) = Get(MAT_INDEX(0, 1));
    b.Get(MAT_INDEX(1, 2)) = Get(MAT_INDEX(0, 2));
    b.Get(MAT_INDEX(1, 3)) = Get(MAT_INDEX(0, 3));

    b.Get(MAT_INDEX(2, 0)) = Get(MAT_INDEX(1, 0));
    b.Get(MAT_INDEX(2, 1)) = Get(MAT_INDEX(1, 1));
    b.Get(MAT_INDEX(2, 2)) = Get(MAT_INDEX(1, 2));
    b.Get(MAT_INDEX(2, 3)) = Get(MAT_INDEX(1, 3));

    b.Get(MAT_INDEX(3, 0)) = Get(MAT_INDEX(2, 0));
    b.Get(MAT_INDEX(3, 1)) = Get(MAT_INDEX(2, 1));
    b.Get(MAT_INDEX(3, 2)) = Get(MAT_INDEX(2, 2));
    b.Get(MAT_INDEX(3, 3)) = Get(MAT_INDEX(2, 3));

    return b;
  }

  template <typename T>
  void Basic_Block<T>::ShiftColumnsLeftInplace() {
    Basic_Block<T> tmp = *this;

    Get(MAT_INDEX(0, 0)) = tmp.Get(MAT_INDEX(0, 1));
    Get(MAT_INDEX(1, 0)) = tmp.Get(MAT_INDEX(1, 1));
    Get(MAT_INDEX(2, 0)) = tmp.Get(MAT_INDEX(2, 1));
    Get(MAT_INDEX(3, 0)) = tmp.Get(MAT_INDEX(3, 1));

    Get(MAT_INDEX(0, 1)) = tmp.Get(MAT_INDEX(0, 2));
    Get(MAT_INDEX(1, 1)) = tmp.Get(MAT_INDEX(1, 2));
    Get(MAT_INDEX(2, 1)) = tmp.Get(MAT_INDEX(2, 2));
    Get(MAT_INDEX(3, 1)) = tmp.Get(MAT_INDEX(3, 2));

    Get(MAT_INDEX(0, 2)) = tmp.Get(MAT_INDEX(0, 3));
    Get(MAT_INDEX(1, 2)) = tmp.Get(MAT_INDEX(1, 3));
    Get(MAT_INDEX(2, 2)) = tmp.Get(MAT_INDEX(2, 3));
    Get(MAT_INDEX(3, 2)) = tmp.Get(MAT_INDEX(3, 3));

    Get(MAT_INDEX(0, 3)) = tmp.Get(MAT_INDEX(0, 0));
    Get(MAT_INDEX(1, 3)) = tmp.Get(MAT_INDEX(1, 0));
    Get(MAT_INDEX(2, 3)) = tmp.Get(MAT_INDEX(2, 0));
    Get(MAT_INDEX(3, 3)) = tmp.Get(MAT_INDEX(3, 0));

    return;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::ShiftColumnsLeft() const {
    Basic_Block<T> b;

    b.Get(MAT_INDEX(0, 0)) = Get(MAT_INDEX(0, 1));
    b.Get(MAT_INDEX(1, 0)) = Get(MAT_INDEX(1, 1));
    b.Get(MAT_INDEX(2, 0)) = Get(MAT_INDEX(2, 1));
    b.Get(MAT_INDEX(3, 0)) = Get(MAT_INDEX(3, 1));

    b.Get(MAT_INDEX(0, 1)) = Get(MAT_INDEX(0, 2));
    b.Get(MAT_INDEX(1, 1)) = Get(MAT_INDEX(1, 2));
    b.Get(MAT_INDEX(2, 1)) = Get(MAT_INDEX(2, 2));
    b.Get(MAT_INDEX(3, 1)) = Get(MAT_INDEX(3, 2));

    b.Get(MAT_INDEX(0, 2)) = Get(MAT_INDEX(0, 3));
    b.Get(MAT_INDEX(1, 2)) = Get(MAT_INDEX(1, 3));
    b.Get(MAT_INDEX(2, 2)) = Get(MAT_INDEX(2, 3));
    b.Get(MAT_INDEX(3, 2)) = Get(MAT_INDEX(3, 3));

    b.Get(MAT_INDEX(0, 3)) = Get(MAT_INDEX(0, 0));
    b.Get(MAT_INDEX(1, 3)) = Get(MAT_INDEX(1, 0));
    b.Get(MAT_INDEX(2, 3)) = Get(MAT_INDEX(2, 0));
    b.Get(MAT_INDEX(3, 3)) = Get(MAT_INDEX(3, 0));

    return b;
  }

  template <typename T>
  void Basic_Block<T>::ShiftColumnsRightInplace() {
    Basic_Block<T> tmp = *this;

    Get(MAT_INDEX(0, 1)) = tmp.Get(MAT_INDEX(0, 0));
    Get(MAT_INDEX(1, 1)) = tmp.Get(MAT_INDEX(1, 0));
    Get(MAT_INDEX(2, 1)) = tmp.Get(MAT_INDEX(2, 0));
    Get(MAT_INDEX(3, 1)) = tmp.Get(MAT_INDEX(3, 0));

    Get(MAT_INDEX(0, 2)) = tmp.Get(MAT_INDEX(0, 1));
    Get(MAT_INDEX(1, 2)) = tmp.Get(MAT_INDEX(1, 1));
    Get(MAT_INDEX(2, 2)) = tmp.Get(MAT_INDEX(2, 1));
    Get(MAT_INDEX(3, 2)) = tmp.Get(MAT_INDEX(3, 1));

    Get(MAT_INDEX(0, 3)) = tmp.Get(MAT_INDEX(0, 2));
    Get(MAT_INDEX(1, 3)) = tmp.Get(MAT_INDEX(1, 2));
    Get(MAT_INDEX(2, 3)) = tmp.Get(MAT_INDEX(2, 2));
    Get(MAT_INDEX(3, 3)) = tmp.Get(MAT_INDEX(3, 2));

    Get(MAT_INDEX(0, 0)) = tmp.Get(MAT_INDEX(0, 3));
    Get(MAT_INDEX(1, 0)) = tmp.Get(MAT_INDEX(1, 3));
    Get(MAT_INDEX(2, 0)) = tmp.Get(MAT_INDEX(2, 3));
    Get(MAT_INDEX(3, 0)) = tmp.Get(MAT_INDEX(3, 3));

    return;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::ShiftColumnsRight() const {
    Basic_Block<T> b;

    b.Get(MAT_INDEX(0, 1)) = Get(MAT_INDEX(0, 0));
    b.Get(MAT_INDEX(1, 1)) = Get(MAT_INDEX(1, 0));
    b.Get(MAT_INDEX(2, 1)) = Get(MAT_INDEX(2, 0));
    b.Get(MAT_INDEX(3, 1)) = Get(MAT_INDEX(3, 0));

    b.Get(MAT_INDEX(0, 2)) = Get(MAT_INDEX(0, 1));
    b.Get(MAT_INDEX(1, 2)) = Get(MAT_INDEX(1, 1));
    b.Get(MAT_INDEX(2, 2)) = Get(MAT_INDEX(2, 1));
    b.Get(MAT_INDEX(3, 2)) = Get(MAT_INDEX(3, 1));

    b.Get(MAT_INDEX(0, 3)) = Get(MAT_INDEX(0, 2));
    b.Get(MAT_INDEX(1, 3)) = Get(MAT_INDEX(1, 2));
    b.Get(MAT_INDEX(2, 3)) = Get(MAT_INDEX(2, 2));
    b.Get(MAT_INDEX(3, 3)) = Get(MAT_INDEX(3, 2));

    b.Get(MAT_INDEX(0, 0)) = Get(MAT_INDEX(0, 3));
    b.Get(MAT_INDEX(1, 0)) = Get(MAT_INDEX(1, 3));
    b.Get(MAT_INDEX(2, 0)) = Get(MAT_INDEX(2, 3));
    b.Get(MAT_INDEX(3, 0)) = Get(MAT_INDEX(3, 3));

    return b;
  }

  template <typename T>
  void Basic_Block<T>::ShiftCellsLeftInplace() {
    Basic_Block<T> tmp = *this;

    Get(15) = tmp.Get(0);

    for (std::size_t i = 0; i < 15; i++) {
      Get(i) = tmp.Get(i+1);
    }

    return;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::ShiftCellsLeft() const {
    Basic_Block<T> b;

    b.Get(15) = Get(0);

    for (std::size_t i = 0; i < 15; i++) {
      b.Get(i) = Get(i+1);
    }

    return b;
  }

  template <typename T>
  void Basic_Block<T>::ShiftCellsRightInplace() {
    Basic_Block<T> tmp = *this;

    Get(0) = tmp.Get(15);

    for (std::size_t i = 1; i < 16; i++) {
      Get(i) = tmp.Get(i-1);
    }

    return;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::ShiftCellsRight() const {
    Basic_Block<T> b;

    b.Get(0) = Get(15);

    for (std::size_t i = 1; i < 16; i++) {
      b.Get(i) = Get(i-1);
    }

    return b;
  }

  template <typename T>
  Basic_Block<T>& Basic_Block<T>::operator=(const Basic_Block<T>& other) {
    data = other.data;
    return *this;
  }

  template <typename T>
  bool Basic_Block<T>::GetBit(const std::size_t index) const {
    // Fetch index of integer the bit is located in
    const std::size_t intIndex = index / CHUNK_SIZE_BITS;

    // Fetch bit index relative to that int
    const std::size_t relBitIndex = index - (intIndex * CHUNK_SIZE_BITS);

    // Pre-calculate the bitmask to use
    const std::size_t bitmask = 1 << (CHUNK_SIZE_BITS - relBitIndex - 1);

    return data[intIndex] & bitmask;
  }

  template <typename T>
  void Basic_Block<T>::SetBit(const std::size_t index, const bool state) {
    // Fetch index of integer the bit is located in
    const std::size_t intIndex = index / CHUNK_SIZE_BITS;

    // Fetch bit index relative to that int
    const std::size_t relBitIndex = index - (intIndex * CHUNK_SIZE_BITS);

    // Pre-calculate the bitmask to use
    const std::size_t bitmask = 1 << (CHUNK_SIZE_BITS - relBitIndex - 1);

    // Set the bit
    if (state) {
      data[intIndex] |= bitmask;
    }
    // Clear the bit
    else {
      data[intIndex] &= ~bitmask;
    }

    return;
  }

  template <typename T>
  void Basic_Block<T>::FlipBit(const std::size_t index) {
    // Fetch index of integer the bit is located in
    const std::size_t intIndex = index / CHUNK_SIZE_BITS;

    // Fetch bit index relative to that int
    const std::size_t relBitIndex = index - (intIndex * CHUNK_SIZE_BITS);

    // Pre-calculate the bitmask to use
    const std::size_t bitmask = 1 << (CHUNK_SIZE_BITS - relBitIndex - 1);

    data[intIndex] ^= bitmask;

    return;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::ShiftBitsLeft() const {
    Basic_Block<T> b;

    // First, copy this block over
    b = *this;

    // Then, shift all integers individually
    for (std::size_t i = 0; i < data.size(); i++) {
      b.data[i] <<= 1;
    }

    // Current state: the LSB is zero everywhere. We have to carry
    // it over manually from the previous state.

    // Carry over the MSB of data[i] to LSB of data[i-1]
    constexpr std::size_t bitmaskMsb = 1 << (CHUNK_SIZE_BITS - 1);
    constexpr std::size_t bitmaskLsb = 1;
    for (int i = 0; i < data.size(); i++) {
      const bool msb = data[i] & bitmaskMsb;

      // Set the lsb
      if (msb) {
        b.data[Mod(i-1, data.size())] |= bitmaskLsb;
      }
      // Clear the lsb
      else {
        b.data[Mod(i-1, data.size())] &= ~bitmaskLsb;
      }
    }

    return b;
  }

  template <typename T>
  void Basic_Block<T>::ShiftBitsLeftInplace() {
    Basic_Block<T> tmp = *this;

    // Then, shift all integers individually
    for (std::size_t i = 0; i < data.size(); i++) {
      data[i] <<= 1;
    }

    // Current state: the LSB is zero everywhere. We have to carry
    // it over manually from the previous state.

    // Carry over the MSB of data[i] to LSB of data[i-1]
    constexpr std::size_t bitmaskMsb = 1 << (CHUNK_SIZE_BITS - 1);
    constexpr std::size_t bitmaskLsb = 1;
    for (int i = 0; i < data.size(); i++) {
      const bool msb = tmp.data[i] & bitmaskMsb;

      // Set the lsb
      if (msb) {
        data[Mod(i-1, data.size())] |= bitmaskLsb;
      }
      // Clear the lsb
      else {
        data[Mod(i-1, data.size())] &= ~bitmaskLsb;
      }
    }

    return;
  }

  template <typename T>
  Basic_Block<T> Basic_Block<T>::ShiftBitsRight() const {
    Basic_Block<T> b;

    // First, copy this block over
    b = *this;

    // Then, shift all integers individually
    for (std::size_t i = 0; i < data.size(); i++) {
      b.data[i] >>= 1;
    }

    // Current state: the LSB is zero everywhere. We have to carry
    // it over manually from the previous state.

    // Carry over the LSB of data[i] to MSB of data[i+1]
    constexpr std::size_t bitmaskMsb = 1 << (CHUNK_SIZE_BITS - 1);
    constexpr std::size_t bitmaskLsb = 1;
    for (int i = 0; i < data.size(); i++) {
      const bool lsb = data[i] & bitmaskLsb;

      // Set the msb
      if (lsb) {
        b.data[Mod(i+1, data.size())] |= bitmaskMsb;
      }
      // Clear the msb
      else {
        b.data[Mod(i+1, data.size())] &= ~bitmaskMsb;
      }
    }

    return b;
  }

  template <typename T>
  void Basic_Block<T>::ShiftBitsRightInplace() {
    Basic_Block<T> tmp = *this;

    // Then, shift all integers individually
    for (std::size_t i = 0; i < data.size(); i++) {
      data[i] >>= 1;
    }

    // Current state: the LSB is zero everywhere. We have to carry
    // it over manually from the previous state.

    // Carry over the LSB of data[i] to MSB of data[i+1]
    constexpr std::size_t bitmaskMsb = 1 << (CHUNK_SIZE_BITS - 1);
    constexpr std::size_t bitmaskLsb = 1;
    for (int i = 0; i < data.size(); i++) {
      const bool lsb = tmp.data[i] & bitmaskLsb;

      // Set the msb
      if (lsb) {
        data[Mod(i+1, data.size())] |= bitmaskMsb;
      }
      // Clear the msb
      else {
        data[Mod(i+1, data.size())] &= ~bitmaskMsb;
      }
    }

    return;
  }

  template <typename T>
  T& Basic_Block<T>::Get(const std::uint8_t row, const std::uint8_t column){
    return data[MAT_INDEX(row, column)];
  }

  template <typename T>
  const T& Basic_Block<T>::Get(const std::uint8_t row, const std::uint8_t column) const {
    return data[MAT_INDEX(row, column)];
  }

  template <typename T>
  T& Basic_Block<T>::Get(const std::uint8_t index) {
    return data[index];
  }

  template <typename T>
  const T& Basic_Block<T>::Get(const std::uint8_t index) const {
    return data[index];
  }

  template <typename T>
  T& Basic_Block<T>::operator[](const std::uint8_t index) {
    return data[index];
  }

  template <typename T>
  const T& Basic_Block<T>::operator[](const std::uint8_t index) const {
    return data[index];
  }

  template <typename T>
  T* Basic_Block<T>::Data() noexcept {
    return data.data();
  }

  template <typename T>
  const T* Basic_Block<T>::Data() const noexcept {
    return data.data();
  }

  template <typename T>
  bool Basic_Block<T>::operator==(const Basic_Block<T>& other) const {
    return data == other.data;
  }

  template <typename T>
  bool Basic_Block<T>::operator!=(const Basic_Block<T>& other) const {
    return data != other.data;
  }

#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
  template <typename T>
  Basic_Block<T>::~Basic_Block() {
    Reset();
  }

  template <typename T>
  void Basic_Block<T>::Reset() {
    memset(data.data(), 0, CHUNK_SIZE*data.size());
    return;
  }
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif

  // Instantiate templates
  template class Basic_Block<std::uint32_t>;
  template class Basic_Block<std::uint16_t>;
}

