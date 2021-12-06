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

#pragma once

/*** ./../GhettoCrypt/GhettoCryptWrapper.h ***/

#pragma once
#include <string>

namespace GhettoCipher
{
	/** This class is a wrapper to make working with the GhettoCipher super easy with a python-like syntax
	*/
	class GhettoCryptWrapper
	{
	public:
		//! Will encrypt a string and return it hexadecimally encoded.
		static std::string EncryptString(const std::string& cleartext, const std::string& password);

		//! Will decrypt a hexadecimally encoded string.
		static std::string DecryptString(const std::string& ciphertext, const std::string& password);

		//! Will encrypt a file.
		//! Returns false if anything goes wrong (like, file-access).
		//! @filename_in The file to be read.
		//! @filename_out The file the encrypted version should be saved in.
		static bool EncryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password);

		//! Will decrypt a file.
		//! Returns false if anything goes wrong (like, file-access).
		//! @filename_in The file to be read.
		//! @filename_out The file the decrypted version should be saved in.
		static bool DecryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password);

	private:
		// No instanciation! >:(
		GhettoCryptWrapper();
	};
}

/*** ./../GhettoCrypt/Flexblock.h ***/

#pragma once
#include <vector>

namespace GhettoCipher
{
	//! A "bitset" of variable length
	typedef std::string Flexblock;
}

/*** ./../GhettoCrypt/Config.h ***/

#pragma once

namespace GhettoCipher
{
	constexpr int BLOCK_SIZE = 128;
	constexpr int N_ROUNDS = 64;
}

/*** ./../GhettoCrypt/Halfblock.h ***/

#pragma once
#include <bitset>

namespace GhettoCipher
{
	constexpr int HALFBLOCK_SIZE = (BLOCK_SIZE / 2);
	typedef std::bitset<HALFBLOCK_SIZE> Halfblock;
}

/*** ./../GhettoCrypt/Block.h ***/

#pragma once
#include <bitset>

namespace GhettoCipher
{
	typedef std::bitset<BLOCK_SIZE> Block;
}

/*** ./../GhettoCrypt/Util.h ***/

#pragma once
#include <bitset>
#include <sstream>
#include <fstream>

namespace GhettoCipher
{
    //! Mod-operator that works with negative values
    inline int Mod(int numerator, int denominator)
    {
        return (denominator + (numerator % denominator)) % denominator;
    }

    //! Will perform a wrapping left-bitshift on a bitset
    template <std::size_t T>
    inline std::bitset<T> Shiftl(const std::bitset<T>& bits, std::size_t amount)
    {
        std::stringstream ss;
        const std::string bitss = bits.to_string();

        for (std::size_t i = 0; i < bitss.size(); i++)
            ss << bitss[Mod((i + amount), bitss.size())];

        return std::bitset<T>(ss.str());
    }

    //! Will perform a wrapping right-bitshift on a bitset
    template <std::size_t T>
    inline std::bitset<T> Shiftr(const std::bitset<T>& bits, std::size_t amount)
    {
        std::stringstream ss;
        const std::string bitss = bits.to_string();

        for (std::size_t i = 0; i < bitss.size(); i++)
            ss << bitss[Mod((i - amount), bitss.size())];

        return std::bitset<T>(ss.str());
    }

    inline std::string PadStringToLength(const std::string& str, const std::size_t len, const char pad, const bool padLeft = true)
    {
        // Fast-reject: Already above padded length
        if (str.length() >= len)
            return str;

        std::stringstream ss;

        // Pad left:
        if (padLeft)
        {
            for (std::size_t i = 0; i < len - str.size(); i++)
                ss << pad;
            ss << str;
        }
        // Pad right:
        else
        {
            ss << str;
            for (std::size_t i = 0; i < len - str.size(); i++)
                ss << pad;
        }

        return ss.str();
    }

    //! Will convert a string to a fixed data block
    inline Block StringToBitblock(const std::string& s)
    {
        std::stringstream ss;

        for (std::size_t i = 0; i < s.size(); i++)
            ss << std::bitset<8>(s[i]);

        // Pad rest with zeores
        return Block(PadStringToLength(ss.str(), 128, '0', false));
    }

    //! Will convert a string to a flexible data block
    inline Flexblock StringToBits(const std::string& s)
    {
        std::stringstream ss;

        for (std::size_t i = 0; i < s.size(); i++)
            ss << std::bitset<8>(s[i]);

        return Flexblock(ss.str());
    }

    //! Will convert a fixed data block to a string
    inline std::string BitblockToString(const Block& bits)
    {
        std::stringstream ss;

        const std::string bitstring = bits.to_string();

        for (std::size_t i = 0; i < BLOCK_SIZE; i += 8)
        {
            ss << (char)std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
        }

        return ss.str();
    }

    //! Will convert a flexible data block to a string
    inline std::string BitsToString(const Flexblock& bits)
    {
        std::stringstream ss;

        const std::string bitstring = bits;

        for (std::size_t i = 0; i < bits.size(); i += 8)
        {
            ss << (char)std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
        }

        return ss.str();
    }

    //! Turns a fixed data block into a hex-string
    inline std::string BitblockToHexstring(const Block& b)
    {
        std::stringstream ss;
        const std::string charset = "0123456789abcdef";
        const std::string bstr = b.to_string();

        for (std::size_t i = 0; i < bstr.size(); i += 4)
            ss << charset[std::bitset<4>(bstr.substr(i, 4)).to_ulong()];

        return ss.str();
    }

    //! Turns a flexible data block into a hex-string
    inline std::string BitsToHexstring(const Flexblock& b)
    {
        std::stringstream ss;
        const std::string charset = "0123456789abcdef";
        const std::string bstr = b;

        for (std::size_t i = 0; i < bstr.size(); i += 4)
            ss << charset[std::bitset<4>(bstr.substr(i, 4)).to_ulong()];

        return ss.str();
    }


    //! Turns a hex string into a fixed data block
    inline Block HexstringToBitblock(const std::string& hexstring)
    {
        std::stringstream ss;

        for (std::size_t i = 0; i < hexstring.size(); i++)
        {
            const char c = hexstring[i];

            // Get value
            std::size_t value;
            if ((c >= '0') && (c <= '9'))
                // Is it a number?
                value = (c - '0') + 0;
            else if ((c >= 'a') && (c <= 'f'))
                // Else, it is a lowercase letter
                value = (c - 'a') + 10;
            else
                throw std::logic_error("non-hex string detected in HexstringToBits()");

            // Append to our bits
            ss << std::bitset<4>(value);
        }

        return Block(ss.str());
    }

    //! Turns a hex string into a flexible data block
    inline Flexblock HexstringToBits(const std::string& hexstring)
    {
        std::stringstream ss;

        for (std::size_t i = 0; i < hexstring.size(); i++)
        {
            const char c = hexstring[i];

            // Get value
            std::size_t value;
            if ((c >= '0') && (c <= '9'))
                // Is it a number?
                value = (c - '0') + 0;
            else if ((c >= 'a') && (c <= 'f'))
                // Else, it is a lowercase letter
                value = (c - 'a') + 10;
            else
                throw std::logic_error("non-hex string detected in HexstringToBits()");

            // Append to our bits
            ss << std::bitset<4>(value);
        }

        return ss.str();
    }

    //! Creates a key of size key-size from a password of arbitrary length.
    inline Block PasswordToKey(const std::string& in)
    {
        Block b;

        // Segment the password in segments of key-size, and xor them together.
        for (std::size_t i = 0; i < in.size(); i += BLOCK_SIZE / 8)
            b ^= StringToBitblock(in.substr(i, BLOCK_SIZE / 8));

        return b;
    }

    //! Will read a file into a flexblock
    inline Flexblock ReadFileToBits(const std::string& filepath)
    {
        // Read file
        std::ifstream ifs(filepath, std::ios::binary);

        if (!ifs.good())
            throw std::runtime_error("Unable to open ifilestream!");

        std::stringstream ss;
        std::copy(
            std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(ss)
        );

        ifs.close();

        const std::string bytes = ss.str();

        // Convert bytes to bits
        return StringToBits(bytes);
    }

    //! Will save bits to a binary file
    inline void WriteBitsToFile(const std::string& filepath, const Flexblock& bits)
    {
        // Convert bits to bytes
        const std::string bytes = BitsToString(bits);

        // Write bits to file
        std::ofstream ofs(filepath, std::ios::binary);

        if (!ofs.good())
            throw std::runtime_error("Unable to open ofilestream!");

        ofs.write(bytes.data(), bytes.length());
        ofs.close();

        return;
    }
}

/*** ./../GhettoCrypt/Keyset.h ***/

#pragma once
#include <array>

namespace GhettoCipher
{
	typedef std::array<Block, N_ROUNDS> Keyset;
}

/*** ./../GhettoCrypt/Feistel.h ***/

#pragma once

namespace GhettoCipher
{
	/** Class to perform a feistel block chipher
	*/
	class Feistel
	{
	public:
		explicit Feistel(const Block& key);

		Feistel(const Feistel& other) = delete;
		Feistel(Feistel&& other) noexcept = delete;

		~Feistel();

		//! Will set the seed-key for this feistel network.
		//! Roundkeys will be derived from this.
		void SetKey(const Block& key);

		//! Will encipher a data block via the set seed-key
		Block Encipher(const Block& data) const;

		//! Will decipher a data block via the set seed-key
		Block Decipher(const Block& data) const;

	private:
		//! Will run the feistel rounds, with either regular key order or reversed key order
		Block Run(const Block& data, bool reverseKeys) const;

		//! Arbitrary cipher function
		static Halfblock F(Halfblock m, const Block& key);

		//! Split a data block into two half blocks (into L and R)
		static std::pair<Halfblock, Halfblock> FeistelSplit(const Block& block);

		//! Combine two half blocks (L and R) into a regular data block
		static Block FeistelCombine(const Halfblock& l, const Halfblock& r);

		//! Will expand a halfblock to a fullblock
		static Block ExpansionFunction(const Halfblock& block);

		//! Will compress a fullblock to a halfblock
		static Halfblock CompressionFunction(const Block& block);

		//! Substitutes four bits by static random others
		static std::string SBox(const std::string& in);

		//! Will generate a the round keys
		void GenerateRoundKeys(const Block& seedKey);

		//! Will zero the memory used by the keyset
		void ZeroKeyMemory();

		Keyset roundKeys;
	};
}

/*** ./../GhettoCrypt/Cipher.h ***/

#pragma once

namespace GhettoCipher
{
	/** Class to apply a block cipher to messages of arbitrary length in a distributed manner
	*/
	class Cipher
	{
	public:
		explicit Cipher(const Block& key);
		explicit Cipher(const std::string& password);

		Cipher(const Cipher& other) = delete;
		Cipher(Cipher&& other) noexcept = delete;

		~Cipher();

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
}
