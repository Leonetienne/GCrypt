#include <GCrypt/GWrapper.h>
#include <iostream>

using namespace Leonetienne::GCrypt;

int main() {
  std::cout << "Example on how to encrypt & decrypt strings:" << std::endl;

  const std::string cleartext = "Hello, World :3";
  std::cout << "Cleartext: " << cleartext << std::endl;

  // Encrypt
  const std::string ciphertext = GWrapper::EncryptString(
    cleartext,
    Key::FromPassword("password1")
  );

  std::cout << "Ciphertext: " << ciphertext << std::endl;

  // Decrypt
  const std::string decrypted = GWrapper::DecryptString(
    ciphertext,
    Key::FromPassword("password1")
  );

  std::cout << "Decrypted: " << decrypted << std::endl;

  return 0;
}

