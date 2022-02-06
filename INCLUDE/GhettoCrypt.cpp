/*
* BSD 2-Clause License
* 
* Copyright (c) 2021, Leon Etienne
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "GhettoCrypt.h"

/*** ./../GhettoCrypt/Cipher.cpp ***/

#include <iostream>
#include <vector>

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
		blocks[i] = feistel.Encipher(blocks[i] ^ lastBlock); // Xor last cipher block with new clear text block before E()
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

		blocks[i] = feistel.Decipher(blocks[i]) ^ lastBlock; // Decipher cipher block [i] and then xor it with the last cipher block [i-1] we've had

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


/*** ./../GhettoCrypt/Feistel.cpp ***/

#include <unordered_map>

GhettoCipher::Feistel::Feistel(const Block& key)
{
    SetKey(key);
    return;
}

GhettoCipher::Feistel::~Feistel()
{
    ZeroKeyMemory();

    return;
}

void GhettoCipher::Feistel::SetKey(const Block& key)
{
    GenerateRoundKeys(key);
    return;
}

GhettoCipher::Block GhettoCipher::Feistel::Encipher(const Block& data)
{
    return Run(data, false);
}

GhettoCipher::Block GhettoCipher::Feistel::Decipher(const Block& data)
{
    return Run(data, true);
}

GhettoCipher::Block GhettoCipher::Feistel::Run(const Block& data, bool reverseKeys)
{
    const auto splitData = FeistelSplit(data);
    GhettoCipher::Halfblock l = splitData.first;
    GhettoCipher::Halfblock r = splitData.second;

    Halfblock tmp;

    for (std::size_t i = 0; i < N_ROUNDS; i++)
    {
        // Calculate key index
        std::size_t keyIndex;
        if (reverseKeys)
            keyIndex = N_ROUNDS - i - 1;
        else
            keyIndex = i;

        // Do a feistel round
        tmp = r;
        r = l ^ F(r, roundKeys[keyIndex]);
        l = tmp;
    }

    // Block has finished de*ciphering.
    // Let's generate a new set of round keys.
    GenerateRoundKeys((Block)roundKeys.back());

    return FeistelCombine(r, l);
}

GhettoCipher::Halfblock GhettoCipher::Feistel::F(Halfblock m, const Block& key)
{
    // Made-up F function

    // Expand to full bitwidth
    Block m_expanded = ExpansionFunction(m);

    // Shift to left by 1
    m_expanded = Shiftl(m_expanded, 1);

    // Xor with key
    m_expanded ^= key;

    // Non-linearly apply subsitution boxes
    std::stringstream ss;
    const std::string m_str = m_expanded.to_string();

    for (std::size_t i = 0; i < BLOCK_SIZE; i += 4)
    {
        ss << SBox(m_str.substr(i, 4));
    }

    m_expanded = Block(ss.str());

    // Return the compressed version
    return CompressionFunction(m_expanded);
}

std::pair<GhettoCipher::Halfblock, GhettoCipher::Halfblock> GhettoCipher::Feistel::FeistelSplit(const Block& block)
{
    const std::string bits = block.to_string();

    Halfblock l(bits.substr(0, bits.size() / 2));
    Halfblock r(bits.substr(bits.size() / 2));

    return std::make_pair(l, r);
}

GhettoCipher::Block GhettoCipher::Feistel::FeistelCombine(const Halfblock& l, const Halfblock& r)
{
    return Block(l.to_string() + r.to_string());
}

GhettoCipher::Block GhettoCipher::Feistel::ExpansionFunction(const Halfblock& block)
{
    std::stringstream ss;
    const std::string bits = block.to_string();

    std::unordered_map<std::string, std::string> expansionMap;
    expansionMap["00"] = "1101";
    expansionMap["01"] = "1000";
    expansionMap["10"] = "0010";
    expansionMap["11"] = "0111";

    // We have to double the bits!
    for (std::size_t i = 0; i < HALFBLOCK_SIZE; i += 2)
    {
        const std::string sub = bits.substr(i, 2);
        ss << expansionMap[sub];
    }

    return Block(ss.str());
}

GhettoCipher::Halfblock GhettoCipher::Feistel::CompressionFunction(const Block& block)
{
    std::stringstream ss;
    const std::string bits = block.to_string();

    std::unordered_map<std::string, std::string> compressionMap;
    compressionMap["0000"] = "10";
    compressionMap["0001"] = "01";
    compressionMap["0010"] = "10";
    compressionMap["0011"] = "10";
    compressionMap["0100"] = "11";
    compressionMap["0101"] = "01";
    compressionMap["0110"] = "00";
    compressionMap["0111"] = "11";
    compressionMap["1000"] = "01";
    compressionMap["1001"] = "00";
    compressionMap["1010"] = "11";
    compressionMap["1011"] = "00";
    compressionMap["1100"] = "11";
    compressionMap["1101"] = "10";
    compressionMap["1110"] = "00";
    compressionMap["1111"] = "01";

    // We have to half the bits!
    for (std::size_t i = 0; i < BLOCK_SIZE; i += 4)
    {
        const std::string sub = bits.substr(i, 4);
        ss << compressionMap[sub];
    }

    return Halfblock(ss.str());
}

std::string GhettoCipher::Feistel::SBox(const std::string& in)
{
    static std::unordered_map<std::string, std::string> subMap;
    static bool mapInitialized = false;
    if (!mapInitialized)
    {
        subMap["0000"] = "1100";
        subMap["0001"] = "1000";
        subMap["0010"] = "0001";
        subMap["0011"] = "0111";
        subMap["0100"] = "1011";
        subMap["0101"] = "0011";
        subMap["0110"] = "1101";
        subMap["0111"] = "1111";
        subMap["1000"] = "0000";
        subMap["1001"] = "1010";
        subMap["1010"] = "0100";
        subMap["1011"] = "1001";
        subMap["1100"] = "0010";
        subMap["1101"] = "1110";
        subMap["1110"] = "0101";
        subMap["1111"] = "0110";
        mapInitialized = true;
    }

    return subMap[in];
}

void GhettoCipher::Feistel::GenerateRoundKeys(const Block& seedKey)
{
    // Clear initial key memory
    ZeroKeyMemory();
    roundKeys = Keyset();

    // Derive the initial two round keys

    // Compress- substitute, and expand the seed key to form the initial and the second-initial round key
    // This action is non-linear and irreversible, and thus strenghtens security.
    Halfblock compressedSeed1 = CompressionFunction(seedKey);
    Halfblock compressedSeed2 = CompressionFunction(Shiftl(seedKey, 1)); // Shifting one key by 1 will result in a completely different compression

    // To add further confusion, let's shift seed1 by 1 aswell (after compression, but before substitution)
    // but only if the total number of bits set are a multiple of 3
    // if it is a multiple of 4, we'll shift it by 1 into the opposite direction
    const std::size_t setBits1 = compressedSeed1.count();

    if (setBits1 % 4 == 0)
        compressedSeed1 = Shiftr(compressedSeed1, 1);
    else if (setBits1 % 3 == 0)
        compressedSeed1 = Shiftl(compressedSeed1, 1);

    // Now apply substitution
    std::stringstream ssKey1;
    std::stringstream ssKey2;
    const std::string bitsKey1 = compressedSeed1.to_string();
    const std::string bitsKey2 = compressedSeed2.to_string();

    for (std::size_t i = 0; i < HALFBLOCK_SIZE; i += 4)
    {
        ssKey1 << SBox(bitsKey1.substr(i, 4));
        ssKey2 << SBox(bitsKey2.substr(i, 4));
    }

    compressedSeed1 = Halfblock(ssKey1.str());
    compressedSeed2 = Halfblock(ssKey2.str());

    // Now extrapolate them to BLOCK_SIZE (key size) again
    // Xor with the original seed key to get rid of the repititions caused by the expansion
    roundKeys[0] = ExpansionFunction(compressedSeed1) ^ seedKey;
    roundKeys[1] = ExpansionFunction(compressedSeed2) ^ seedKey;

    
    // Now derive all other round keys

    for (std::size_t i = 2; i < roundKeys.size(); i++)
    {
        // Initialize new round key with last round key
        Block newKey = roundKeys[i - 1];

        // Shift to left by how many bits are set, modulo 8
        newKey = Shiftl(newKey, newKey.count() % 8); // This action is irreversible

        // Split into two halfblocks,
        // apply F() to one halfblock with rk[i-2],
        // xor the other one with it
        // and put them back together
        auto halfkeys = FeistelSplit(newKey);
        Halfblock halfkey1 = F(halfkeys.first, roundKeys[i - 2]);
        Halfblock halfkey2 = halfkeys.second ^ halfkey1;

        roundKeys[i] = FeistelCombine(halfkey1, halfkey2);
    }

    return;
}

// These pragmas only work for MSVC and g++, as far as i know. Beware!!!
#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
void GhettoCipher::Feistel::ZeroKeyMemory()
{
    for (Block& key : roundKeys)
        key.reset();

    return;
}
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif


/*** ./../GhettoCrypt/GhettoCryptWrapper.cpp ***/


std::string GhettoCipher::GhettoCryptWrapper::EncryptString(const std::string& cleartext, const std::string& password)
{
	// Instanciate our cipher and supply a key
	const Block key = PasswordToKey(password);
	Cipher cipher(key);

	// Recode the ascii-string to bits
	const Flexblock cleartext_bits = StringToBits(cleartext);

	// Encrypt our cleartext bits
	const Flexblock ciphertext_bits = cipher.Encipher(cleartext_bits);

	// Recode the ciphertext bits to a hex-string
	const std::string ciphertext = BitsToHexstring(ciphertext_bits);

	// Return it
	return ciphertext;
}

std::string GhettoCipher::GhettoCryptWrapper::DecryptString(const std::string& ciphertext, const std::string& password)
{
	// Instanciate our cipher and supply a key
	const Block key = PasswordToKey(password);
	Cipher cipher(key);

	// Recode the hex-string to bits
	const Flexblock ciphertext_bits = HexstringToBits(ciphertext);

	// Decrypt the ciphertext bits
	const std::string cleartext_bits = cipher.Decipher(ciphertext_bits);

	// Recode the cleartext bits to an ascii-string
	const std::string cleartext = BitsToString(cleartext_bits);

	// Return it
	return cleartext;
}

bool GhettoCipher::GhettoCryptWrapper::EncryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport)
{
	try
	{
		// Read the file to bits
		const Flexblock cleartext_bits = ReadFileToBits(filename_in);

		// Instanciate our cipher and supply a key
		const Block key = PasswordToKey(password);
		Cipher cipher(key);

		// Encrypt our cleartext bits
		const Flexblock ciphertext_bits = cipher.Encipher(cleartext_bits, printProgressReport);

		// Write our ciphertext bits to file
		WriteBitsToFile(filename_out, ciphertext_bits);

		return true;
	}
	catch (std::runtime_error&)
	{
		return false;
	}
}

bool GhettoCipher::GhettoCryptWrapper::DecryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport)
{
	try
	{
		// Read the file to bits
		const Flexblock ciphertext_bits = ReadFileToBits(filename_in);

		// Instanciate our cipher and supply a key
		const Block key = PasswordToKey(password);
		Cipher cipher(key);

		// Decrypt the ciphertext bits
		const Flexblock cleartext_bits = cipher.Decipher(ciphertext_bits, printProgressReport);

		// Write our cleartext bits to file
		WriteBitsToFile(filename_out, cleartext_bits);

		return true;
	}
	catch (std::runtime_error&)
	{
		return false;
	}
}


/*** ./../GhettoCrypt/InitializationVector.cpp ***/

#include <iostream>

GhettoCipher::InitializationVector::InitializationVector(const Block& seed)
{
	// We'll generate our initialization vector by encrypting our seed with itself as a key
	// iv = E(M=seed, K=seed)
	iv = Feistel(seed).Encipher(seed);
}

GhettoCipher::InitializationVector::operator GhettoCipher::Block() const
{
	return iv;
}

