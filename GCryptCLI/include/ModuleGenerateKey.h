#ifndef GCRYPTCLI_MODULE_GENERATEKEY_H
#define GCRYPTCLI_MODULE_GENERATEKEY_H

namespace Module {
  // This module just generates a key, and outputs it.
  // Can be used to create a keyfiles.
  class GenerateKey {
    public:
      //! Will write the key to a file
      static void Run();
  };
}

#endif

