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
      static Key FromPassword(const std::string& password);
      Key();
      Key(const Key& k);
      Key(const Block& b);

    private:

  };

}

#endif

