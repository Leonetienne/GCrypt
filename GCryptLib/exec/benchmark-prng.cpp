#include <GCrypt/GPrng.h>
#include <iostream>
#include "Benchmark.h"

using namespace Leonetienne::GCrypt;

int main() {

  Benchmark(
    "generating 1.000.000 32-bit uints using prng.GetRandom<uint32_t>()",
    []() {
      GPrng prng(Key::Random());
      for (int i = 0; i < 1000000; i++) {
        prng.GetRandom<std::uint32_t>();
      }
    }
  );

  Benchmark(
    "generating 1.000.000 uint32_t using prng()",
    []() {
      GPrng prng(Key::Random());
      for (int i = 0; i < 1000000; i++) {
        prng();
      }
    }
  );

  Benchmark(
    "generating 100.000 data blocks using prng.GetBlock()",
    []() {
      GPrng prng(Key::Random());
      for (int i = 0; i < 100000; i++) {
        prng.GetBlock();
      }
    }
  );


  return 0;
}

