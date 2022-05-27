#ifndef GCRYPTCLI_MODULE_GENERATEKEYFILE_H
#define GCRYPTCLI_MODULE_GENERATEKEYFILE_H

namespace Module {
  // This class has the task to prepare and supply the encryption key.
  class GenerateKeyfile {
    public:
      //! Will write the key to a file
      static void Run();
  };
}

#endif

