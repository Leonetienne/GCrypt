#pragma once
#include <iostream>
#include "Util.h"
#include "FeistelMan.h"

int main()
{
	FeistelMan feistel("Password yo");

	const std::string message = "I am a veeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeery long message!";

	std::cout << "Cleartext: " << message << std::endl;

	// Encrypt
	const Flexblock ciphertext = feistel.Encipher(StringToBits(message));
	const std::string cipherHex = BitsToHexstring(ciphertext);

	std::cout << "Ciphertext: " << cipherHex << std::endl;


	// Decrypt
	const Flexblock cleartextBits = feistel.Decipher(HexstringToBits(cipherHex));
	std::cout << "Decrypted:  " << BitsToString(cleartextBits) << std::endl;


	return 0;
}
