#include "GCrypt/Util.h"
#include "GCrypt/GHash.h"

namespace Leonetienne::GCrypt {
  Block PasswordToKey(const std::string& in) {
    // We already have a hashing algorithm, so why not use it
    // Yeah, this won't work, because it would create an include loop. This method needs to be outsourced to a cpp file..

    const Block hashedPassword = GHash::CalculateHashsum(StringToBits(in));
    return hashedPassword;
  }
}

