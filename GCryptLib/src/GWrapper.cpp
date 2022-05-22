#include "GCrypt/GWrapper.h"
#include "GCrypt/GCipher.h"
#include "GCrypt/Util.h"

namespace Leonetienne::GCrypt {

  std::string GWrapper::EncryptString(const std::string& cleartext, const std::string& password) {
    // Transform the password to a key
    const Block key = PasswordToKey(password);

    // Recode the ascii-string to bits
    const Flexblock cleartext_bits = StringToBits(cleartext);

    // Encrypt our cleartext bits
    const Flexblock ciphertext_bits = CipherFlexblock(cleartext_bits, key, GCipher::DIRECTION::ENCIPHER);

    // Recode the ciphertext bits to a hex-string
    const std::string ciphertext = BitsToHexstring(ciphertext_bits);

    // Return it
    return ciphertext;
  }

  std::string GWrapper::DecryptString(const std::string& ciphertext, const std::string& password) {
    // Transform the password to a key
    const Block key = PasswordToKey(password);

    // Recode the hex-string to bits
    const Flexblock ciphertext_bits = HexstringToBits(ciphertext);

    // Decrypt the ciphertext bits
    const std::string cleartext_bits = CipherFlexblock(ciphertext_bits, key, GCipher::DIRECTION::DECIPHER);

    // Recode the cleartext bits to an ascii-string
    const std::string cleartext = BitsToString(cleartext_bits);

    // Return it
    return cleartext;
  }

  bool GWrapper::EncryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport) {
    try {
      // Read the file to bits
      const Flexblock cleartext_bits = ReadFileToBits(filename_in);

      // Transform the password to a key
      const Block key = PasswordToKey(password);

      // Encrypt our cleartext bits
      const Flexblock ciphertext_bits = CipherFlexblock(cleartext_bits, key, GCipher::DIRECTION::ENCIPHER);

      // Write our ciphertext bits to file
      WriteBitsToFile(filename_out, ciphertext_bits);

      return true;
    }
    catch (std::runtime_error&) {
      return false;
    }
  }

  bool GWrapper::DecryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport) {
    try {
      // Read the file to bits
      const Flexblock ciphertext_bits = ReadFileToBits(filename_in);

      // Transform the password to a key
      const Block key = PasswordToKey(password);

      // Decrypt the ciphertext bits
      const Flexblock cleartext_bits = CipherFlexblock(ciphertext_bits, key, GCipher::DIRECTION::DECIPHER);

      // Write our cleartext bits to file
      WriteBitsToFile(filename_out, cleartext_bits);

      return true;
    }
    catch (std::runtime_error&) {
      return false;
    }
  }

  Flexblock GWrapper::CipherFlexblock(
      const Flexblock& data,
      const Block& key,
      const GCipher::DIRECTION direction)
  {
    // Split input into blocks
    std::vector<Block> blocks;

    for (std::size_t i = 0; i < data.size(); i += BLOCK_SIZE) {
      blocks.push_back(Block(
        PadStringToLength(data.substr(i, BLOCK_SIZE), BLOCK_SIZE, '0', false))
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

