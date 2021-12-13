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
	key { key }
{

	return;
}

GhettoCipher::Cipher::Cipher(const std::string& password)
{
	key = PasswordToKey(password);

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
	Block lastBlock = emptyBlock;
	
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

const GhettoCipher::Block GhettoCipher::Cipher::emptyBlock;


/*** ./../GhettoCrypt/Feistel.cpp ***/


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

GhettoCipher::Block GhettoCipher::Feistel::Encipher(const Block& data) const
{
    return Run(data, false);
}

GhettoCipher::Block GhettoCipher::Feistel::Decipher(const Block& data) const
{
    return Run(data, true);
}

GhettoCipher::Block GhettoCipher::Feistel::Run(const Block& data, bool reverseKeys) const
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

    for (std::size_t i = 0; i < m_str.size(); i += 4)
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

    // We have to double the bits!
    for (std::size_t i = 0; i < bits.size(); i += 2)
    {
        const std::string sub = bits.substr(i, 2);

             if (sub == "00") ss << "1101";
        else if (sub == "01") ss << "1000";
        else if (sub == "10") ss << "0010";
        else if (sub == "11") ss << "0111";

    }

    return Block(ss.str());
}

GhettoCipher::Halfblock GhettoCipher::Feistel::CompressionFunction(const Block& block)
{
    std::stringstream ss;
    const std::string bits = block.to_string();

    // We have to half the bits!
    for (std::size_t i = 0; i < bits.size(); i += 4)
    {
        const std::string sub = bits.substr(i, 4);

             if (sub == "0000") ss << "10";
        else if (sub == "0001") ss << "01";
        else if (sub == "0010") ss << "10";
        else if (sub == "0011") ss << "10";
        else if (sub == "0100") ss << "11";
        else if (sub == "0101") ss << "01";
        else if (sub == "0110") ss << "00";
        else if (sub == "0111") ss << "11";
        else if (sub == "1000") ss << "01";
        else if (sub == "1001") ss << "00";
        else if (sub == "1010") ss << "11";
        else if (sub == "1011") ss << "00";
        else if (sub == "1100") ss << "11";
        else if (sub == "1101") ss << "10";
        else if (sub == "1110") ss << "00";
        else if (sub == "1111") ss << "01";
    }

    return Halfblock(ss.str());
}

std::string GhettoCipher::Feistel::SBox(const std::string& in)
{
         if (in == "0000")     return "1100";
    else if (in == "0001")     return "1000";
    else if (in == "0010")     return "0001";
    else if (in == "0011")     return "0111";
    else if (in == "0100")     return "1011";
    else if (in == "0101")     return "0011";
    else if (in == "0110")     return "1101";
    else if (in == "0111")     return "1111";
    else if (in == "1000")     return "0000";
    else if (in == "1001")     return "1010";
    else if (in == "1010")     return "0100";
    else if (in == "1011")     return "1001";
    else if (in == "1100")     return "0010";
    else if (in == "1101")     return "1110";
    else if (in == "1110")     return "0101";
    else /*if (in == "1111")*/ return "0110";
}

void GhettoCipher::Feistel::GenerateRoundKeys(const Block& seedKey)
{
    // Generate round keys via output feedback modus (OFM) method

    // Clear initial key memory
    ZeroKeyMemory();
    roundKeys = Keyset();

    // Generate new keys from the seed key
    roundKeys[0] = seedKey;
    roundKeys[1] = (Shiftl(seedKey, 32) ^ roundKeys[0]);

    for (std::size_t i = 2; i < roundKeys.size(); i++)
    {
        roundKeys[i] = Shiftl(roundKeys[i - 1], i + 32) ^ roundKeys[i - 2];
    }

    return;
}

void GhettoCipher::Feistel::ZeroKeyMemory()
{
    for (Block& key : roundKeys)
        key.reset();

    return;
}


/*** ./../GhettoCrypt/GhettoCryptWrapper.cpp ***/


std::string GhettoCipher::GhettoCryptWrapper::EncryptString(const std::string& cleartext, const std::string& password)
{
	// Instanciate our cipher and supply a key
	Cipher cipher(password);

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
	Cipher cipher(password);

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
		Cipher cipher(password);

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
		Cipher cipher(password);

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

