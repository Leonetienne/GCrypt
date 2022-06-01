#include <GCrypt/GWrapper.h>
#include <GCrypt/Util.h>
#include "Catch2.h"

using namespace Leonetienne::GCrypt;

// Tests that encrypting and decrypting short strings using the wrapper works.
// This test will start from scratch after encryption, to ensure EVERYTHING has to be re-calculated.
TEST_CASE(__FILE__"/Encrypting and decrypting strings works, Single block", "[Wrapper]") {

  // Setup
  const std::string plaintext = "Hello, World!";
  const Key key = Key::FromPassword("Der Affe will Zucker");

  std::string ciphertext;
  std::string decrypted;

  // Encryption
  ciphertext = GWrapper::EncryptString(plaintext, key);

  // Decryption
  decrypted = GWrapper::DecryptString(ciphertext, key);

  // Assertion
  REQUIRE(plaintext == decrypted);
}

// Tests that encrypting and decrypting very long strings using the wrapper works.
// This test will start from scratch after encryption, to ensure EVERYTHING has to be re-calculated.
TEST_CASE(__FILE__"/Encrypting and decrypting strings works, Many blocks block", "[Wrapper]") {

  // Setup

  // Read an not-multiple-of-blocksize amount of random chars, that's very large (about 200kb long string)
  srand(time(0));

  std::stringstream ss;
  const std::string charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (std::size_t i = 0; i < 198273; i++) {
    ss << charset[rand() % charset.length()];
  }

  const std::string plaintext = ss.str();
  const Key key = Key::FromPassword("Der Affe will Zucker");

  std::string ciphertext;
  std::string decrypted;

  // Encryption
  ciphertext = GWrapper::EncryptString(plaintext, key);

  // Decryption
  decrypted = GWrapper::DecryptString(ciphertext, key);

  // Assertion
  REQUIRE(plaintext == decrypted);
}

// Tests that encrypting and decrypting files using the wrapper works.
// This test will start from scratch after encryption, to ensure EVERYTHING has to be re-calculated.
TEST_CASE(__FILE__"/Encrypting and decrypting files works", "[Wrapper]") {

  // Setup
  const std::string testfile_dir = "testAssets/";

  const std::string filename_plain   = testfile_dir + "testfile.png";
  const std::string filename_encrypted = testfile_dir + "testfile.png.crypt";
  const std::string filename_decrypted = testfile_dir + "testfile.png.clear.png";
  const Key key = Key::FromPassword("Der Affe will Zucker");


  // Encryption
  GWrapper::EncryptFile(filename_plain, filename_encrypted, key);

  // Decryption
  GWrapper::DecryptFile(filename_encrypted, filename_decrypted, key);

  // Read in both the base, and the decrypted file
  const std::vector<Block> plainfile = ReadFileToBlocks(filename_plain);
  const std::vector<Block> decryptfile = ReadFileToBlocks(filename_decrypted);

  // Assertion (If this fails, maybe check if the image is even readable by an image viewer)
  REQUIRE(plainfile.size() == decryptfile.size());
  REQUIRE(plainfile == decryptfile);
}

