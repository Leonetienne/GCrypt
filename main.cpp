#pragma once
#include <iostream>
#include "Util.h"
#include "FeistelMan.h"

int main()
{
	// Load file to be encrypted
	const Flexblock file = ReadFileToBits("images.jpg");
	std::cout << "Finished reading..." << std::endl;


	// Prepare cipher
	FeistelMan feistel("Password yo");


	// Encrypt
	const Flexblock ciphertext = feistel.Encipher(file, true);
	//const std::string cipherHex = BitsToHexstring(ciphertext);
	std::cout << "Finished encrypting..." << std::endl;

	// Save encrypted file
	WriteBitsToFile("encrypted.jpg", ciphertext);
	std::cout << "Finished writing..." << std::endl;


	// Decrypt
	//const Flexblock cleartextBits = feistel.Decipher(HexstringToBits(cipherHex));
	const Flexblock cleartextBits = feistel.Decipher(ciphertext, true);
	std::cout << "Finished decrypting..." << std::endl;

	
	// Save decrypted file
	WriteBitsToFile("decrypted.jpg", cleartextBits);
	std::cout << "Finished writing..." << std::endl;

	return 0;
}
