#ifndef GCRYPT_KEY_H
#define GCRYPT_KEY_H
#include "GCrypt/Block.h"
#include <string>

namespace Leonetienne::GCrypt {

  /* This class represents encryption keys.
     You can copy them, create them from data blocks,
     or even read from files (to be implemented).
  */
  class Key : public Block {
    public:
      //! Will generate a key from a password
      static Key FromPassword(const std::string& password);

      //! Will generate a random key from actual randomness (std::random_device)
      static Key Random();

      //! Loads a keyfile
      static Key LoadFromFile(const std::string& path);

      //! Will save a keyfile
      void WriteToFile(const std::string& path) const;

      Key();
      Key(const Key& k);
      Key(const Block& b);

    private:

  };

}

#endif

