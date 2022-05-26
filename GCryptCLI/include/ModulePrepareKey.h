#ifndef GCRYPTCLI_MODULE_PREPAREKEY_H
#define GCRYPTCLI_MODULE_PREPAREKEY_H

#include <GCrypt/Key.h>

using namespace Leonetienne::GCrypt;

// This class has the task to supply the encryption key.
class ModulePrepareKey {
  public:
    //! Will prepare the key. Be it from cli, a file, or, random, or whatever.
    static void PrepareKey();

    //! Will return the key, if prepared.
    static const Key& GetKey();

  private:
    //! Will ask for a password on stdin,
    //! hiding the input.
    static std::string PasswordPrompt();

    static Key key;
};

#endif

