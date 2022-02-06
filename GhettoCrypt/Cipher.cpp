#include <iostream>
#include <vector>
#include "Cipher.h"
#include "Util.h"
#include "InitializationVector.h"

GhettoCipher::Cipher::Cipher(const Block& key)
	:
	key { key },
	initializationVector(InitializationVector(key))
{

	return;
}

GhettoCipher::Cipher::Cipher(const std::string& password)
	:
	key { PasswordToKey(password) },
	initializationVector(InitializationVector(key))
{
	return;
}

GhettoCipher::Cipher::~Cipher()
{
	// Clear key memory
	ZeroKeyMemory();

	return;
}

void GhettoCipher::Cipher::SetKey(const Block& key)
{
	ZeroKeyMemory();

	this->key = key;
	return;
}

void GhettoCipher::Cipher::SetPassword(const std::string& password)
{
	ZeroKeyMemory();

	key = PasswordToKey(password);
	return;
}

GhettoCipher::Flexblock GhettoCipher::Cipher::Encipher(const Flexblock& data, bool printProgress) const
{
	// Split cleartext into blocks
	std::vector<Block> blocks;

	for (std::size_t i = 0; i < data.size(); i += BLOCK_SIZE)
		blocks.push_back(Block(
			PadStringToLength(data.substr(i, BLOCK_SIZE), BLOCK_SIZE, '0', false))
		);

	// Encrypt individual blocks using cipher block chaining
	Feistel feistel(key);

	for (std::size_t i = 0; i < blocks.size(); i++)
	{
		// Print reports if desired. If we have > 1000 blocks, print one report every 100 blocks. Otherwise for every 10th block.
		if ((i % ((blocks.size() > 1000)? 100 : 10) == 0) && (printProgress))
			std::cout << "Encrypting... (Block " << i << " / " << blocks.size() << " - " << ((float)i*100 / blocks.size()) << "%)" << std::endl;
	
		const Block& lastBlock = (i>0) ? blocks[i-1] : initializationVector;
		blocks[i] = feistel.Encipher(blocks[i] ^ lastBlock);
	}

	// Concatenate ciphertext blocks back into a flexblock
	std::stringstream ss;
	for (Block& b : blocks)
		ss << b;

	// Return it
	return ss.str();
}

GhettoCipher::Flexblock GhettoCipher::Cipher::Decipher(const Flexblock& data, bool printProgress) const
{
	// Split ciphertext into blocks
	std::vector<Block> blocks;

	for (std::size_t i = 0; i < data.size(); i += BLOCK_SIZE)
		blocks.push_back(Block(
			PadStringToLength(data.substr(i, BLOCK_SIZE), BLOCK_SIZE, '0', false))
		);

	// Decrypt individual blocks
	Feistel feistel(key);

	// We can't do this in-loop for decryption, because we are decrypting the blocks in-place.
	Block lastBlock = initializationVector;
	
	for (std::size_t i = 0; i < blocks.size(); i++)
	{
		// Print reports if desired. If we have > 1000 blocks, print one report every 100 blocks. Otherwise for every 10th block.
		if ((i % ((blocks.size() > 1000) ? 100 : 10) == 0) && (printProgress))
			std::cout << "Decrypting... (Block " << i << " / " << blocks.size() << " - " << ((float)i*100/ blocks.size()) << "%)" << std::endl;

		Block tmpCopy = blocks[i];

		blocks[i] = feistel.Decipher(blocks[i]) ^ lastBlock;

		lastBlock = std::move(tmpCopy);
	}

	// Concatenate ciphertext blocks back into a flexblock
	std::stringstream ss;
	for (Block& b : blocks)
		ss << b;

	// Return it
	return ss.str();
}

// These pragmas only work for MSVC and g++, as far as i know. Beware!!!
#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
void GhettoCipher::Cipher::ZeroKeyMemory()
{
	key.reset();
	return;
}
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif
