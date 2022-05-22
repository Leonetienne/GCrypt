#include "GCrypt/Key.h"
#include "GCrypt/GHash.h"
#include "GCrypt/Util.h"

namespace Leonetienne::GCrypt {

  Key Key::FromPassword(const std::string& password) {
    return GHash::CalculateHashsum(
      StringToBits(password)
    );
  }

  Key::Key() : Block() {
  }

  Key::Key(const Block& b) : Block(b) {
  }

  Key::Key(const Key& k) : Block(k) {
  }
}

