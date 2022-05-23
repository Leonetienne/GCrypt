#include "GCrypt/Util.h"
#include "GCrypt/GHash.h"

namespace Leonetienne::GCrypt {

  std::string PadStringToLength(const std::string& str, const std::size_t len, const char pad, const bool padLeft) {
    // Fast-reject: Already above padded length
    if (str.length() >= len) {
      return str;
    }

    std::stringstream ss;

    // Pad left:
    if (padLeft) {
      for (std::size_t i = 0; i < len - str.size(); i++) {
        ss << pad;
      }
      ss << str;
    }
    // Pad right:
    else {
      ss << str;
      for (std::size_t i = 0; i < len - str.size(); i++) {
        ss << pad;
      }
    }

    return ss.str();
  }

  Block StringToBitblock(const std::string& s, bool padLeft) {
    std::stringstream ss;

    for (std::size_t i = 0; i < s.size(); i++) {
      ss << std::bitset<8>(s[i]);
    }

    // Pad rest with zeores
    return Block(PadStringToLength(ss.str(), BLOCK_SIZE, '0', padLeft));
  }

  Flexblock StringToBits(const std::string& s) {
    std::stringstream ss;

    for (std::size_t i = 0; i < s.size(); i++) {
      ss << std::bitset<8>(s[i]);
    }

    return Flexblock(ss.str());
  }

  std::string BitblockToBytes(const Block& bits) {
    std::stringstream ss;

    const std::string bitstring = bits.ToString();

    for (std::size_t i = 0; i < BLOCK_SIZE; i += 8) {
      ss << (char)std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
    }

    return ss.str();
  }

  std::string BitblockToString(const Block& bits) {
    // Decode to bytes
    std::string text = BitblockToBytes(bits);

    // Dümp excess nullbytes
    text.resize(strlen(text.data()));

    return text;
  }

  std::string BitsToBytes(const Flexblock& bits) {
    std::stringstream ss;

    const std::string bitstring = bits;

    for (std::size_t i = 0; i < bits.size(); i += 8) {
      ss << (char)std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
    }

    return ss.str();
  }

  std::string BitsToString(const Flexblock& bits) {
    // Decode to bytes
    std::string text = BitsToBytes(bits);

    // Dümp excess nullbytes
    text.resize(strlen(text.data()));

    return text;
  }

  std::string BitblockToHexstring(const Block& b) {
    std::stringstream ss;
    const std::string charset = "0123456789abcdef";
    const std::string bstr = b.ToString();

    for (std::size_t i = 0; i < bstr.size(); i += 4) {
      ss << charset[std::bitset<4>(bstr.substr(i, 4)).to_ulong()];
    }

    return ss.str();
  }

  std::string BitsToHexstring(const Flexblock& b) {
    std::stringstream ss;
    const std::string charset = "0123456789abcdef";
    const std::string bstr = b;

    for (std::size_t i = 0; i < bstr.size(); i += 4) {
      ss << charset[std::bitset<4>(bstr.substr(i, 4)).to_ulong()];
    }

    return ss.str();
  }

  Block HexstringToBitblock(const std::string& hexstring) {
    std::stringstream ss;

    for (std::size_t i = 0; i < hexstring.size(); i++) {
      const char c = hexstring[i];

      // Get value
      std::size_t value;
      if ((c >= '0') && (c <= '9')) {
        // Is it a number?
        value = ((std::size_t)c - '0') + 0;
      }
      else if ((c >= 'a') && (c <= 'f')) {
        // Else, it is a lowercase letter
        value = ((std::size_t)c - 'a') + 10;
      }
      else {
        throw std::logic_error("non-hex string detected in HexstringToBits()");
      }

      // Append to our bits
      ss << std::bitset<4>(value).to_string();
    }

    return Block(ss.str());
  }

  Flexblock HexstringToBits(const std::string& hexstring) {
    std::stringstream ss;

    for (std::size_t i = 0; i < hexstring.size(); i++) {
      const char c = hexstring[i];

      // Get value
      std::size_t value;
      if ((c >= '0') && (c <= '9')) {
        // Is it a number?
        value = ((std::size_t)c - '0') + 0;
      }
      else if ((c >= 'a') && (c <= 'f')) {
        // Else, it is a lowercase letter
        value = ((std::size_t)c - 'a') + 10;
      }
      else {
        throw std::logic_error("non-hex string detected in HexstringToBits()");
      }

      // Append to our bits
      ss << std::bitset<4>(value).to_string();
    }

    return ss.str();
  }

  Flexblock ReadFileToBits(const std::string& filepath) {
    // Read file
    std::ifstream ifs(filepath, std::ios::binary);

    if (!ifs.good()) {
      throw std::runtime_error("Unable to open ifilestream!");
    }

    std::stringstream ss;
    std::copy(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(ss)
        );

    ifs.close();

    const std::string bytes = ss.str();

    // Convert bytes to bits
    return StringToBits(bytes);
  }

  void WriteBitsToFile(const std::string& filepath, const Flexblock& bits) {
    // Convert bits to bytes
    const std::string bytes = BitsToBytes(bits);

    // Write bits to file
    std::ofstream ofs(filepath, std::ios::binary);

    if (!ofs.good()) {
      throw std::runtime_error("Unable to open ofilestream!");
    }

    ofs.write(bytes.data(), bytes.length());
    ofs.close();

    return;
  }
}

