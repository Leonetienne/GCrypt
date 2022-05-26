#ifndef GCRYPT_FLEXBLOCK_H
#define GCRYPT_FLEXBLOCK_H

#include <string>

namespace Leonetienne::GCrypt {
  //! A type used for conveying "bitstrings". e.g. "10101001001"
  //! These should generally not be used, as they are really really slow.
  //! The only valid usecase I can think of is when using GHash for example, because for hashing
  //! an absolute input length is required.
  //! If you need to, you can use the StringToBits() and BitsToString() functions defined in Util.h.
  typedef std::string Flexblock;
}

#endif

