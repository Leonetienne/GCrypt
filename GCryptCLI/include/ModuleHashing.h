#ifndef GCRYPTCLI_MODULE_HASHING_H
#define GCRYPTCLI_MODULE_HASHING_H

namespace Module {
  //! This module will hash supplied input
  class Hashing {
    public:
      //! Will run the module
      static void Run();

    private:
    // No instanciation! >:(
    Hashing() {};
  };
}

#endif

