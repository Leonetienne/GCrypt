#include <iostream>
#include <GCrypt/GCryptWrapper.h>
#include <GCrypt/SecureBitset.h>
#include <GCrypt/Util.h>
#include <GCrypt/InitializationVector.h>

using namespace Leonetienne::GCrypt;

void ExampleString() {
  std::cout << "Example on how to encrypt & decrypt a string:" << std::endl;

  // Get some string
  const std::string input = "I am a super secret message!";
  std::cout << input << std::endl;

  // Encrypt
  const std::string encrypted = GCryptWrapper::EncryptString(input, "password1");
  std::cout << encrypted << std::endl;

  // Decrypt
  const std::string decrypted = GCryptWrapper::DecryptString(encrypted, "password1");
  std::cout << decrypted << std::endl;

  return;
}

void ExampleFiles() {
  std::cout << "Example on how to encrypt & decrypt any file:" << std::endl;

  // Encrypt
  GCryptWrapper::EncryptFile("main.cpp", "main.cpp.crypt", "password1");

  // Decrypt
  GCryptWrapper::DecryptFile("main.cpp.crypt", "main.cpp.clear", "password1");

  return;
}

int main() {
  ExampleString();
  //ExampleFiles();

  return 0;
}

