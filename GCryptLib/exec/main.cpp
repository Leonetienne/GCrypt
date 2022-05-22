#include <iostream>
#include <GCrypt/GWrapper.h>
#include <GCrypt/SecureBitset.h>
#include <GCrypt/Util.h>
#include <GCrypt/InitializationVector.h>
#include <GCrypt/Key.h>

using namespace Leonetienne::GCrypt;

void ExampleString() {
  std::cout << "Example on how to encrypt & decrypt a string:" << std::endl;

  // Get some string
  const std::string input = "I am a super secret message!";
  std::cout << input << std::endl;

  // Encrypt
  const std::string encrypted = GWrapper::EncryptString(input, Key::FromPassword("password1"));
  std::cout << encrypted << std::endl;

  // Decrypt
  const std::string decrypted = GWrapper::DecryptString(encrypted, Key::FromPassword("password1"));
  std::cout << decrypted << std::endl;

  return;
}

void ExampleFiles() {
  std::cout << "Example on how to encrypt & decrypt any file:" << std::endl;

  // Encrypt
  GWrapper::EncryptFile("main.cpp", "main.cpp.crypt", Key::FromPassword("password1"));

  // Decrypt
  GWrapper::DecryptFile("main.cpp.crypt", "main.cpp.clear", Key::FromPassword("password1"));

  return;
}

int main() {
  ExampleString();
  //ExampleFiles();

  return 0;
}

