#ifndef GCRYPTCLI_MODULE_DECRYPTION_H
#define GCRYPTCLI_MODULE_DECRYPTION_H

namespace Module {
  //! This module will decrypt supplied input
  class Decryption {
    public:
      //! Will run the module
      static void Run();

    private:
    // No instanciation! >:(
    Decryption() {};
  };
}

#endif

