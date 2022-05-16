#pragma once
#include <bitset>
#include <ostream>
#include <istream>

namespace Leonetienne::GCrypt {
  /** Wrapper for std::bitset<T> that zeroes memory upon deletion.
  * This does not include ALL methods, but the ones needed.
  *
  * Just creating a specialization of std::bitset<T> does not work.
  */
  template <std::size_t T>
  class SecureBitset {
  public:
    explicit SecureBitset();
    explicit SecureBitset(const std::string& str);
    explicit SecureBitset(const long long int i);

    ~SecureBitset();

    bool operator==(const SecureBitset<T>& other) const;
    bool operator!=(const SecureBitset<T>& other) const;
    bool operator[](const std::size_t) const;
    bool test(const std::size_t index) const;
    bool all() const;
    bool any() const;
    bool none() const;
    std::size_t count() const;
    std::size_t size() const;
    SecureBitset<T>& operator&=(const SecureBitset<T>& other);
    SecureBitset<T>& operator|=(const SecureBitset<T>& other);
    SecureBitset<T>& operator^=(const SecureBitset<T>& other);
    SecureBitset<T> operator&(const SecureBitset<T>& other);
    SecureBitset<T> operator|(const SecureBitset<T>& other);
    SecureBitset<T> operator^(const SecureBitset<T>& other);
    SecureBitset<T> operator~() const;
    SecureBitset<T>& operator<<=(const std::size_t offset);
    SecureBitset<T>& operator>>=(const std::size_t offset);
    SecureBitset<T> operator<<(const std::size_t offset) const;
    SecureBitset<T> operator>>(const std::size_t offset) const;
    SecureBitset<T>& set();
    SecureBitset<T>& set(const std::size_t index, bool value = true);
    SecureBitset<T>& reset();
    SecureBitset<T>& reset(const std::size_t index);
    SecureBitset<T>& flip();
    SecureBitset<T>& flip(const std::size_t index);
    std::string to_string() const;
    unsigned long to_ulong() const;
    unsigned long long to_ullong() const;

    std::bitset<T>& Get();
    const std::bitset<T>& Get() const;

  private:
    std::bitset<T> bitset;
};

  template<std::size_t T>
  inline SecureBitset<T>::SecureBitset()
    :
    bitset() {
    return;
  }

  template<std::size_t T>
  inline SecureBitset<T>::SecureBitset(const std::string& str)
    :
    bitset(str) {
    return;
  }

  template<std::size_t T>
  inline SecureBitset<T>::SecureBitset(const long long int i)
    :
    bitset(i) {
    return;
  }


  // Don't optimize the destructor out!!!
  // These pragmas only work for MSVC and g++, as far as i know. Beware!!!
#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
  template<std::size_t T>
  inline SecureBitset<T>::~SecureBitset() {
    bitset.reset();
    return;
  }
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif

  template<std::size_t T>
  inline bool SecureBitset<T>::operator==(const SecureBitset<T>& other) const {
    return bitset == other.bitset;
  }

  template<std::size_t T>
  inline bool SecureBitset<T>::operator!=(const SecureBitset<T>& other) const {
    return bitset != other.bitset;
  }

  template<std::size_t T>
  inline bool SecureBitset<T>::operator[](const std::size_t index) const {
    return bitset[index];
  }

  template<std::size_t T>
  inline bool SecureBitset<T>::test(const std::size_t index) const {
    return bitset.test(index);
  }

  template<std::size_t T>
  inline bool SecureBitset<T>::all() const {
    return bitset.all();
  }

  template<std::size_t T>
  inline bool SecureBitset<T>::any() const {
    return bitset.any();
  }

  template<std::size_t T>
  inline bool SecureBitset<T>::none() const {
    return bitset.none();
  }

  template<std::size_t T>
  inline std::size_t SecureBitset<T>::count() const {
    return bitset.count();
  }

  template<std::size_t T>
  inline std::size_t SecureBitset<T>::size() const {
    return bitset.count();
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::operator&=(const SecureBitset<T>& other) {
    bitset &= other.bitset;
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::operator|=(const SecureBitset<T>& other) {
    bitset |= other.bitset;
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::operator^=(const SecureBitset<T>& other) {
    bitset ^= other.bitset;
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T> SecureBitset<T>::operator&(const SecureBitset<T>& other) {
    SecureBitset bs;
    bs.bitset = bitset & other.bitset;
    return bs;
  }

  template<std::size_t T>
  inline SecureBitset<T> SecureBitset<T>::operator|(const SecureBitset<T>& other) {
    SecureBitset bs;
    bs.bitset = bitset | other.bitset;
    return bs;
  }

  template<std::size_t T>
  inline SecureBitset<T> SecureBitset<T>::operator^(const SecureBitset<T>& other) {
    SecureBitset bs;
    bs.bitset = bitset ^ other.bitset;
    return bs;
  }

  template<std::size_t T>
  inline SecureBitset<T> SecureBitset<T>::operator~() const {
    SecureBitset bs;
    bs.bitset = ~bitset;
    return bs;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::operator<<=(const std::size_t offset) {
    bitset <<= offset;
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::operator>>=(const std::size_t offset) {
    bitset >>= offset;
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T> SecureBitset<T>::operator<<(const std::size_t offset) const {
    SecureBitset bs;
    bs.bitset = bitset << offset;
    return bs;
  }

  template<std::size_t T>
  inline SecureBitset<T> SecureBitset<T>::operator>>(const std::size_t offset) const {
    SecureBitset bs;
    bs.bitset = bitset >> offset;
    return bs;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::set() {
    bitset.set();
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::set(const std::size_t index, bool value) {
    bitset.set(index, value);
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::reset() {
    bitset.reset();
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::reset(const std::size_t index) {
    bitset.reset(index);
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::flip() {
    bitset.flip();
    return *this;
  }

  template<std::size_t T>
  inline SecureBitset<T>& SecureBitset<T>::flip(const std::size_t index) {
    bitset.flip(index);
    return *this;
  }

  template<std::size_t T>
  inline std::string SecureBitset<T>::to_string() const {
    return bitset.to_string();
  }

  template<std::size_t T>
  inline unsigned long SecureBitset<T>::to_ulong() const {
    return bitset.to_ulong();
  }

  template<std::size_t T>
  inline unsigned long long SecureBitset<T>::to_ullong() const {
    return bitset.to_ullong();
  }

  template<std::size_t T>
  inline std::bitset<T>& SecureBitset<T>::Get() {
    return bitset;
  }

  template<std::size_t T>
  inline const std::bitset<T>& SecureBitset<T>::Get() const {
    return bitset;
  }

  template <std::size_t T>
  inline std::ostream& operator<<(std::ostream& ofs, const SecureBitset<T>& bs) {
    return ofs << bs.Get();
  }

  template <std::size_t T>
  inline std::istream& operator>>(std::istream& ifs, const SecureBitset<T>& bs) {
    return ifs >> bs.Get();
  }
}
