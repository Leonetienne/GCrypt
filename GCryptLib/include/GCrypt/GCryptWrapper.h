#pragma once
#include <string>
#include "GCrypt/Flexblock.h"
#include "GCrypt/Block.h"
#include "GCrypt/GCipher.h"

namespace Leonetienne::GCrypt {
  /** This class is a wrapper to make working with the GhettoCipher
  *   super easy with a python-like syntax
  */
  class GCryptWrapper {
  public:
    //! Will encrypt a string and return it hexadecimally encoded.
    static std::string EncryptString(const std::string& cleartext, const std::string& password);

    //! Will decrypt a hexadecimally encoded string.
    static std::string DecryptString(const std::string& ciphertext, const std::string& password);

    //! Will encrypt a file.
    //! Returns false if anything goes wrong (like, file-access).
    //! @filename_in The file to be read.
    //! @filename_out The file the encrypted version should be saved in.
    static bool EncryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport = false);

    //! Will decrypt a file.
    //! Returns false if anything goes wrong (like, file-access).
    //! @filename_in The file to be read.
    //! @filename_out The file the decrypted version should be saved in.
    static bool DecryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport = false);

    //! Will enncrypt or decrypt an entire flexblock of binary data, given a key.
    static Flexblock CipherFlexblock(const Flexblock& data, const Block& key, const GCipher::DIRECTION direction);

  private:


    // No instanciation! >:(
    GCryptWrapper();
  };
}

