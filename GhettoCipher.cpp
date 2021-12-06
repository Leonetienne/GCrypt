#include "GhettoCipher.h"
#include "Util.h"
#include <iostream>

GhettoCipher::GhettoCipher(const Block& key)
	:
	key { key }
{

	return;
}

GhettoCipher::GhettoCipher(const std::string& password)
{
	key = PasswordToKey(password);

	return;
}

GhettoCipher::~GhettoCipher()
{
	// Clear key memory
	ZeroKeyMemory();

	return;
}

void GhettoCipher::SetKey(const Block& key)
{
	ZeroKeyMemory();

	this->key = key;
	return;
}

void GhettoCipher::SetPassword(const std::string& password)
{
	ZeroKeyMemory();

	key = PasswordToKey(password);
	return;
}

Flexblock GhettoCipher::Encipher(const Flexblock& data, bool printProgress) const
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
			std::cout << "Encrypting... (Block " << i << " / " << blocks.size() << " - " << ((float)i*100 / blocks.size()) << "\%)" << std::endl;
	
		const Block& lastBlock = (i>0) ? blocks[i-1] : emptyBlock;
		blocks[i] = feistel.Encipher(blocks[i] ^ lastBlock);
	}

	// Concatenate ciphertext blocks back into a flexblock
	std::stringstream ss;
	for (Block& b : blocks)
		ss << b;

	// Return it
	return ss.str();
}

Flexblock GhettoCipher::Decipher(const Flexblock& data, bool printProgress) const
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
	Block lastBlock = emptyBlock;
	
	for (std::size_t i = 0; i < blocks.size(); i++)
	{
		// Print reports if desired. If we have > 1000 blocks, print one report every 100 blocks. Otherwise for every 10th block.
		if ((i % ((blocks.size() > 1000) ? 100 : 10) == 0) && (printProgress))
			std::cout << "Decrypting... (Block " << i << " / " << blocks.size() << " - " << ((float)i*100/ blocks.size()) << "\%)" << std::endl;

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

#pragma optimize("", off )
void GhettoCipher::ZeroKeyMemory()
{
	key.reset();
	return;
}
#pragma optimize("", on )

const Block GhettoCipher::emptyBlock;
