#include <GCrypt/GWrapper.h>
#include <iostream>

using namespace Leonetienne::GCrypt;

int main() {
  std::cout << "Example on how to encrypt & decrypt any file:" << std::endl;

  // Encrypt
  GWrapper::EncryptFile(
    "./execAssets/testimage.bmp",
    "./execAssets/testimage.bmp.crypt",
    Key::FromPassword("password1")
  );

  // Decrypt
  GWrapper::DecryptFile(
    "./execAssets/testimage.bmp.crypt",
    "./execAssets/testimage.bmp.clear.bmp",
    Key::FromPassword("password1"
  ));

  return 0;
}

