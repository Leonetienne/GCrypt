#ifndef GCRYPTCLI_MODULE_ENCRYPTION_H
#define GCRYPTCLI_MODULE_ENCRYPTION_H

namespace Module {
  //! This module will encrypt supplied input
  class Encryption {
    public:
      //! Will run the module
      static void Run();

    private:
    // No instanciation! >:(
    Encryption() {};
  };
}

#endif

