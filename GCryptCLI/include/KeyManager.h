#ifndef GCRYPTCLI_KEYMANAGER_H
#define GCRYPTCLI_KEYMANAGER_H

#include <GCrypt/Key.h>

using namespace Leonetienne::GCrypt;

// This class has the task to prepare and supply the encryption key.
class KeyManager {
  public:
    //! Will prepare the key. Be it from cli, a file, or, random, or whatever.
    static void PrepareKey();

    //! Will return the key, if prepared.
    static const Key& GetKey();

  private:
    //! Will ask for a password on stdin,
    //! hiding the input.
    static std::string PasswordPrompt();

    //! The encryption key
    static Key key;

    // No instanciation! >:(
    KeyManager() {};
};

#endif

