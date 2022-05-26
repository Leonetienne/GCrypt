#include <GCrypt/GWrapper.h>
#include "Benchmark.h"

using namespace Leonetienne::GCrypt;

int main() {

  Benchmark(
    "file encryption",
    []() {
      GWrapper::EncryptFile(
        "./execAssets/big-testfile.bmp",
        "./execAssets/testimage.bmp.crypt",
        Key::FromPassword("password1")
      );
    }
  );

  return 0;
}

