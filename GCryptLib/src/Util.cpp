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

  std::string BitblocksToBytes(const std::vector<Block>& blocks) {
    std::stringstream ss;

    for (const Block& block : blocks) {
      ss << block.ToByteString();
    }

    return ss.str();
  }

  std::string BitblocksToString(const std::vector<Block>& blocks) {
    // Decode to bytes
    std::string text = BitblocksToBytes(blocks);

    // Dümp excess nullbytes
    text.resize(strlen(text.data()));

    return text;
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

