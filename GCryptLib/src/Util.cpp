#include "GCrypt/Util.h"
#include "GCrypt/GHash.h"
#include <vector>

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
    return Block(PadStringToLength(ss.str(), Block::BLOCK_SIZE_BITS, '0', padLeft));
  }

  Flexblock StringToBits(const std::string& s) {
    std::stringstream ss;

    for (std::size_t i = 0; i < s.size(); i++) {
      ss << std::bitset<8>(s[i]);
    }

    return Flexblock(ss.str());
  }

  std::string BitblockToBytes(const Block& block) {
    std::stringstream ss;

    std::uint8_t* curByte = (std::uint8_t*)(void*)block.Data();
    for (std::size_t j = 0; j < Block::BLOCK_SIZE; j++) {
      ss << *curByte++;
    }

    return ss.str();
  }

  std::string BitblocksToBytes(const std::vector<Block>& blocks) {
    std::stringstream ss;

    for (const Block& block : blocks) {
      ss << BitblockToBytes(block);
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

  std::string BitblocksToString(const std::vector<Block>& blocks) {
    // Decode to bytes
    std::string text = BitblocksToBytes(blocks);

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
    const std::string bstr = b.ToBinaryString();

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

  std::vector<Block> ReadFileToBlocks(const std::string& filepath, std::size_t& bytes_read) {
    // Read file
    bytes_read = 0;

    // "ate" specifies that the read-pointer is already at the end of the file
    // this allows to estimate the file size
    std::ifstream ifs(filepath, std::ios::binary | std::ios::ate);

    if (!ifs.good()) {
      throw std::runtime_error("Unable to open ifilestream!");
    }

    // Create our vector of blocks, and resorve a good guess
    // of memory
    std::vector<Block> blocks;
    blocks.reserve((ifs.tellg() / Block::BLOCK_SIZE) + 1);

    // Move read head to the file beginning
    ifs.seekg(std::ios_base::beg);

    // Whilst not reached eof, read into blocks
    while (!ifs.eof()) {
      // Create a new block, and zero it
      Block block;
      block.Reset();

      // Read data into the block
      ifs.read((char*)(void*)block.Data(), Block::BLOCK_SIZE);
      const std::size_t n_bytes_read_block = ifs.gcount();
      bytes_read += n_bytes_read_block;

      if (n_bytes_read_block > 0) {
        // Append the block to our vector
        blocks.emplace_back(block);
      }
    }

    // Close the filehandle
    ifs.close();

    return blocks;
  }

  std::vector<Block> ReadFileToBlocks(const std::string& filepath) {
    std::size_t bytes_read_dummy; // Create a dumme for the parameter
    return ReadFileToBlocks(filepath, bytes_read_dummy);
  }

  void WriteBlocksToFile(
      const std::string& filepath,
      const std::vector<Block>& blocks
  ){

    // Create outfile file handle
    std::ofstream ofs(filepath, std::ios::binary);

    if (!ofs.good()) {
      throw std::runtime_error("Unable to open ofilestream!");
    }

    // Write all the blocks
    for (const Block& block : blocks) {
      ofs.write((char*)(void*)block.Data(), Block::BLOCK_SIZE);
    }

    // Close the filehandle
    ofs.close();

    return;
  }

  std::vector<Block> StringToBitblocks(const std::string& str) {

    // Create our block vector, and reserve exactly
    // how many blocks are required to store this string
    const std::size_t num_blocks = (str.length() / Block::BLOCK_SIZE) + 1;
    std::vector<Block> blocks;
    blocks.reserve(num_blocks);

    for (std::size_t i = 0; i < str.length(); i += Block::BLOCK_SIZE) {
      Block block;
      block.FromTextString(str.substr(i, Block::BLOCK_SIZE));

      blocks.emplace_back(block);
    }

    return blocks;
  }
}

