#include <GCrypt/GWrapper.h>
#include <GCrypt/Util.h>
#include "Catch2.h"

using namespace Leonetienne::GCrypt;

// THESE TESTS ASSUME BLOCK_SIZE == 512

// Tests that encrypting a message of exactly BLOCK_SIZE yields the exact message back
TEST_CASE(__FILE__"/SingleBlock", "[Encryption/Decryption consistency]") {

  // Instanciate our cipher and supply a key
  const Key key = Key::FromPassword("1234");

  // Recode the ascii-string to bits
  const std::string cleartext_bits =
    "1011110011010110000010110001111000111010111101001010100100011101"
    "0101110101010010100000110100001000011000111010001001110101111111"
    "1110110101100101110001010101011110001010000010111110011011010111"
    "1100110100111000000011100101010100110010001110010011000010111001"
    "0000010000010000011001111010011110111001000000000110101000110001"
    "0110111110110110100000010100000011010001000011100100111001001011"
    "1101100100000100010000001011100010010001101111100100101100010001"
    "0000011110010110111010110110111110011110011010001100100111110101";

  std::vector<Block> cleartext_blocks({Block(cleartext_bits)});

  // Encrypt our cleartext bits
  const std::vector<Block> ciphertext_blocks = GWrapper::CipherBlocks(cleartext_blocks, key, GCipher::DIRECTION::ENCIPHER);

  // Decipher it again
  const std::vector<Block> decrypted_blocks = GWrapper::CipherBlocks(ciphertext_blocks, key, GCipher::DIRECTION::DECIPHER);

  // Assert that the decrypted text equals the plaintext
  REQUIRE(
    cleartext_blocks ==
    decrypted_blocks
  );
}

// Tests that a decrypted ciphertext equals its plaintext version, using a cleartext that requires A LOT of blocks
TEST_CASE(__FILE__"MultiBlock_NoPadding/", "[Encryption/Decryption consistency]") {

  // Instanciate our cipher and supply a key
  const Key key = Key::FromPassword("1234");

  std::stringstream ss;
  const std::string charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  // Read 10 blocks worth of characters
  srand(time(0));
  for (std::size_t i = 0; i < 512*10; i++) {
    ss << charset[rand() % charset.length()];
  }
  const std::string cleartext_str = ss.str();

  std::vector<Block> cleartext_blocks = StringToBitblocks(cleartext_str);

  // Encrypt our cleartext bits
  std::vector<Block> ciphertext_blocks;
  ciphertext_blocks.reserve(cleartext_blocks.size());
  {
    GCipher cipher(key, GCipher::DIRECTION::ENCIPHER);

    for (const Block& clearBlock : cleartext_blocks) {
      ciphertext_blocks.emplace_back(cipher.Digest(clearBlock));
    }
  }

  // Decipher it again
  std::vector<Block> deciphered_blocks;
  deciphered_blocks.reserve(ciphertext_blocks.size());
  {
    GCipher cipher(key, GCipher::DIRECTION::DECIPHER);

    for (const Block& ciphBlock : ciphertext_blocks) {
      deciphered_blocks.emplace_back(cipher.Digest(ciphBlock));
    }
  }

  // Assert that the decrypted text equals the plaintext
  REQUIRE(
    cleartext_blocks ==
    deciphered_blocks
  );
}

