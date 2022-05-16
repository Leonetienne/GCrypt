#include <GCrypt/GhettoCryptWrapper.h>
#include <GCrypt/Flexblock.h>
#include <GCrypt/Util.h>
#include "Catch2.h"

using namespace Leonetienne::GCrypt;

// Tests that encrypting and decrypting strings using the wrapper works.
// This test will start from scratch after encryption, to ensure EVERYTHING has to be re-calculated.
TEST_CASE(__FILE__"/Encrypting and decrypting strings works", "[Wrapper]") {

    // Setup
    const std::string plaintext = "Hello, World!";
    const std::string password = "Der Affe will Zucker";

    std::string ciphertext;
    std::string decrypted;

    // Encryption
    ciphertext = GhettoCryptWrapper::EncryptString(plaintext, password);

    // Decryption
    decrypted = GhettoCryptWrapper::DecryptString(ciphertext, password);

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
      const std::string password = "Der Affe will Zucker";


      // Encryption
      GhettoCryptWrapper::EncryptFile(filename_plain, filename_encrypted, password);

      // Decryption
      GhettoCryptWrapper::DecryptFile(filename_encrypted, filename_decrypted, password);

      // Read in both the base, and the decrypted file
      const Flexblock plainfile = ReadFileToBits(filename_plain);
      const Flexblock decryptfile = ReadFileToBits(filename_decrypted);

      // Assertion (If this fails, maybe check if the image is even readable by an image viewer)
      REQUIRE(
        PadStringToLength(plainfile, decryptfile.length(), '0', false) ==
        decryptfile
      );
}

