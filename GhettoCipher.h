#pragma once
#include "Feistel.h"
#include "Flexblock.h"

/** Class to apply a block cipher to messages of arbitrary length in a distributed manner
*/
class GhettoCipher
{
public:
	explicit GhettoCipher(const Block& key);
	explicit GhettoCipher(const std::string& password);

	GhettoCipher(const GhettoCipher& other) = delete;
	GhettoCipher(GhettoCipher&& other) noexcept = delete;
	
	~GhettoCipher();

	//! Will set the key
	void SetKey(const Block& key);

	//! Will set the key from a password
	void SetPassword(const std::string& password);

	//! Will encipher a flexblock of data
	Flexblock Encipher(const Flexblock& data, bool printProgress = false) const;

	//! Will decipher a flexblock of data
	Flexblock Decipher(const Flexblock& data, bool printProgress = false) const;

private:
	Block key;

	//! Will zero the memory used by the key
	void ZeroKeyMemory();

	// Initial value for cipher block chaining
	static const Block emptyBlock;
};
