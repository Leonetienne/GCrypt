#pragma once
#include <iostream>
#include <GhettoCryptWrapper.h>
#include <SecureBitset.h>
#include <Util.h>
#include <InitializationVector.h>

using namespace GhettoCipher;

void ExampleString()
{
	std::cout << "Example on how to encrypt & decrypt a string:" << std::endl;

	// Get some string
	const std::string input = "I am a super secret message!";
	std::cout << input << std::endl;

	// Encrypt
	const std::string encrypted = GhettoCryptWrapper::EncryptString(input, "password1");
	std::cout << encrypted << std::endl;

	// Decrypt
	const std::string decrypted = GhettoCryptWrapper::DecryptString(encrypted, "password1");
	std::cout << decrypted << std::endl;

	return;
}

void ExampleFiles()
{
	std::cout << "Example on how to encrypt & decrypt any file:" << std::endl;

	// Encrypt
	GhettoCryptWrapper::EncryptFile("main.cpp", "main.cpp.crypt", "password1");

	// Decrypt
	GhettoCryptWrapper::DecryptFile("main.cpp.crypt", "main.cpp.clear", "password1");

	return;
}

int main()
{
	//ExampleString();
	//ExampleFiles();

	return 0;
}
