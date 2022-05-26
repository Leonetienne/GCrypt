#ifndef GCRYPT_GPRNG_H
#define GCRYPT_GPRNG_H

#include "GCrypt/GHash.h"
#include "GCrypt/Util.h"
#include <string.h>
#include <sstream>
#include <type_traits>

namespace Leonetienne::GCrypt {
  /** This class implements a pseudo random number generator, based on the GCrypt hash function
  */
  class GPrng {
    public:
      //! Will instanciate the prng with a seed. Seed could also be a GCrypt::Key.
      GPrng(const Block& seed);

      //! Will instanciate the GPrng with no seed. You should really seed it later.
      GPrng();

      //! Will reset and seed the prng. Seed could also be a GCrypt::Key.
      void Seed(const Block& seed);

      //! Will return a random bit.
      bool GetBit();

      //! Will return a randomized instance of any primitive.
      template <typename T>
      T GetRandom() {
        static_assert(std::is_fundamental<T>::value, "Leonetienne::GCrypt::GPrng::GetRandom() may only be used with primitive types!");

        // Pull the required amount of bits
        std::stringstream ss;
        for (std::size_t i = 0; i < sizeof(T)*8; i++) {
          ss << (GetBit() ? '1' : '0');
        }

        // Transform to bytes
        const std::string bytes = BitsToBytes(ss.str());

        // Cram bytes into type
        T t;
        memcpy(&t, bytes.data(), sizeof(T));

        // Return our randomized primitive
        return t;
      }

      //! Will return a random unsigned 32-bit integer
      std::uint32_t operator()();

      //! Will return a random block
      Block GetBlock();

    private:
      //! Will generate the next block of random bits
      void AdvanceBlock();

      GHash hasher;
      Block seed;
      std::size_t nextBit = 0;
  };
}

#endif

