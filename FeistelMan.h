#pragma once
#include "Feistel.h"
#include "Flexblock.h"

/** Class to apply a block cipher to messages of arbitrary length in a distributed manner
*/
class FeistelMan
{
public:
	explicit FeistelMan(const Block& key);
	explicit FeistelMan(const std::string& password);

	FeistelMan(const FeistelMan& other) = delete;
	FeistelMan(FeistelMan&& other) noexcept = delete;
	
	~FeistelMan();

	//! Will set the key
	void SetKey(const Block& key);

	//! Will set the key from a password
	void SetPassword(const std::string& password);

	//! Will encipher a flexblock of data
	Flexblock Encipher(const Flexblock& data) const;

	//! Will decipher a flexblock of data
	Flexblock Decipher(const Flexblock& data) const;

private:
	Block key;

	//! Will zero the memory used by the key
	void ZeroKeyMemory();

	// Initial value for cipher block chaining
	static const Block emptyBlock;
};
