#include "GCrypt/GWrapper.h"
#include "GCrypt/GCipher.h"
#include "GCrypt/Util.h"
#include <vector>

namespace Leonetienne::GCrypt {

  std::string GWrapper::EncryptString(
      const std::string& cleartext,
      const Key& key)
  {
    // Recode the ascii-string to bits
    const Flexblock cleartext_bits = StringToBits(cleartext);

    // Encrypt our cleartext bits
    const Flexblock ciphertext_bits = CipherFlexblock(cleartext_bits, key, GCipher::DIRECTION::ENCIPHER);

    // Recode the ciphertext bits to a hex-string
    const std::string ciphertext = BitsToHexstring(ciphertext_bits);

    // Return it
    return ciphertext;
  }

  std::string GWrapper::DecryptString(
      const std::string& ciphertext,
      const Key& key)
  {
    // Recode the hex-string to bits
    const Flexblock ciphertext_bits = HexstringToBits(ciphertext);

    // Decrypt the ciphertext bits
    const std::string cleartext_bits = CipherFlexblock(ciphertext_bits, key, GCipher::DIRECTION::DECIPHER);

    // Recode the cleartext bits to an ascii-string
    const std::string cleartext = BitsToString(cleartext_bits);

    // Return it
    return cleartext;
  }

  bool GWrapper::EncryptFile(
      const std::string& filename_in,
      const std::string& filename_out,
      const Key& key,
      bool printProgressReport)
  {
    try {
      // Read the file to blocks
      const std::vector<Block> cleartext_blocks = ReadFileToBlocks(filename_in);

      // Encrypt our cleartext blocks
      std::vector<Block> ciphertext_blocks;
      ciphertext_blocks.reserve(cleartext_blocks.size());

      // Create cipher instance
      GCipher cipher(key, GCipher::DIRECTION::ENCIPHER);

      // Encrypt all blocks
      for (const Block& clearBlock : cleartext_blocks) {
        ciphertext_blocks.emplace_back(cipher.Digest(clearBlock));
      }

      // Write our ciphertext blocks to file
      WriteBlocksToFile(filename_out, ciphertext_blocks);

      return true;
    }
    catch (std::runtime_error&) {
      return false;
    }
  }

  bool GWrapper::DecryptFile(
      const std::string& filename_in,
      const std::string& filename_out,
      const Key& key,
      bool printProgressReport)
  {
    try {
      // Read the file to blocks
      const std::vector<Block> ciphertext_blocks = ReadFileToBlocks(filename_in);

      // Decrypt our cleartext blocks
      std::vector<Block> cleartext_blocks;
      cleartext_blocks.reserve(ciphertext_blocks.size());

      // Create cipher instance
      GCipher cipher(key, GCipher::DIRECTION::DECIPHER);

      // Decrypt all blocks
      for (const Block& cipherBlock : ciphertext_blocks) {
        cleartext_blocks.emplace_back(cipher.Digest(cipherBlock));
      }

      // Write our cleartext blocks to file
      WriteBlocksToFile(filename_out, cleartext_blocks);

      return true;
    }
    catch (std::runtime_error&) {
      return false;
    }
  }

  Flexblock GWrapper::CipherFlexblock(
      const Flexblock& data,
      const Key& key,
      const GCipher::DIRECTION direction)
  {
    // Split input into blocks
    std::vector<Block> blocks;

    for (std::size_t i = 0; i < data.size(); i += Block::BLOCK_SIZE_BITS) {
      blocks.push_back(Block(
        PadStringToLength(data.substr(i, Block::BLOCK_SIZE_BITS), Block::BLOCK_SIZE_BITS, '0', false))
      );
    }

    // Create cipher instance
    GCipher cipher(key, direction);

    for (Block& block : blocks) {
      block = cipher.Digest(block);
    }

    // Concatenate ciphertext blocks back into a flexblock
    std::stringstream ss;
    for (Block& b : blocks) {
      ss << b;
    }

    // Return it
    return ss.str();
  }
}

