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

/*** ./../GhettoCrypt/Flexblock.h ***/

#pragma once
#include <string>

namespace GhettoCipher
{
	//! A "bitset" of variable length
	typedef std::string Flexblock;
}

/*** ./../GhettoCrypt/Version.h ***/

#pragma once
#define GHETTOCRYPT_VERSION 0.12

/*** ./../GhettoCrypt/Config.h ***/

#pragma once
#include <cstdint>

namespace GhettoCipher
{
	constexpr std::size_t BLOCK_SIZE = 512;
	constexpr std::size_t N_ROUNDS = 64;
}

/*** ./../GhettoCrypt/SecureBitset.h ***/

#pragma once
#include <bitset>
#include <ostream>
#include <istream>

namespace GhettoCipher
{
/** Wrapper for std::bitset<T> that zeroes memory upon deletion.
* This does not include ALL methods, but the ones needed.
* 
* Just creating a specialization of std::bitset<T> does not work.
*/
template <std::size_t T>
class SecureBitset
{
public:
	explicit SecureBitset();
	explicit SecureBitset(const std::string& str);
	explicit SecureBitset(const long long int i);

	~SecureBitset();

	bool operator==(const SecureBitset<T>& other) const;
	bool operator!=(const SecureBitset<T>& other) const;
	bool operator[](const std::size_t) const;
	bool test(const std::size_t index) const;
	bool all() const;
	bool any() const;
	bool none() const;
	std::size_t count() const;
	std::size_t size() const;
	SecureBitset<T>& operator&=(const SecureBitset<T>& other);
	SecureBitset<T>& operator|=(const SecureBitset<T>& other);
	SecureBitset<T>& operator^=(const SecureBitset<T>& other);
	SecureBitset<T> operator&(const SecureBitset<T>& other);
	SecureBitset<T> operator|(const SecureBitset<T>& other);
	SecureBitset<T> operator^(const SecureBitset<T>& other);
	SecureBitset<T> operator~() const;
	SecureBitset<T>& operator<<=(const std::size_t offset);
	SecureBitset<T>& operator>>=(const std::size_t offset);
	SecureBitset<T> operator<<(const std::size_t offset) const;
	SecureBitset<T> operator>>(const std::size_t offset) const;
	SecureBitset<T>& set();
	SecureBitset<T>& set(const std::size_t index, bool value = true);
	SecureBitset<T>& reset();
	SecureBitset<T>& reset(const std::size_t index);
	SecureBitset<T>& flip();
	SecureBitset<T>& flip(const std::size_t index);
	std::string to_string() const;
	unsigned long to_ulong() const;
	unsigned long long to_ullong() const;

	std::bitset<T>& Get();
	const std::bitset<T>& Get() const;

private:
	std::bitset<T> bitset;
};

template<std::size_t T>
inline SecureBitset<T>::SecureBitset()
	:
	bitset()
{
	return;
}

template<std::size_t T>
inline SecureBitset<T>::SecureBitset(const std::string& str)
	:
	bitset(str)
{
	return;
}

template<std::size_t T>
inline SecureBitset<T>::SecureBitset(const long long int i)
	:
	bitset(i)
{
	return;
}


// Don't optimize the destructor out!!!
// These pragmas only work for MSVC and g++, as far as i know. Beware!!!
#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
template<std::size_t T>
inline SecureBitset<T>::~SecureBitset()
{
	bitset.reset();
	return;
}
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif

template<std::size_t T>
inline bool SecureBitset<T>::operator==(const SecureBitset<T>& other) const
{
	return bitset == other.bitset;
}

template<std::size_t T>
inline bool SecureBitset<T>::operator!=(const SecureBitset<T>& other) const
{
	return bitset != other.bitset;
}

template<std::size_t T>
inline bool SecureBitset<T>::operator[](const std::size_t index) const
{
	return bitset[index];
}

template<std::size_t T>
inline bool SecureBitset<T>::test(const std::size_t index) const
{
	return bitset.test(index);
}

template<std::size_t T>
inline bool SecureBitset<T>::all() const
{
	return bitset.all();
}

template<std::size_t T>
inline bool SecureBitset<T>::any() const
{
	return bitset.any();
}

template<std::size_t T>
inline bool SecureBitset<T>::none() const
{
	return bitset.none();
}

template<std::size_t T>
inline std::size_t SecureBitset<T>::count() const
{
	return bitset.count();
}

template<std::size_t T>
inline std::size_t SecureBitset<T>::size() const
{
	return bitset.count();
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::operator&=(const SecureBitset<T>& other)
{
	bitset &= other.bitset;
	return *this;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::operator|=(const SecureBitset<T>& other)
{
	bitset |= other.bitset;
	return *this;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::operator^=(const SecureBitset<T>& other)
{
	bitset ^= other.bitset;
	return *this;
}

template<std::size_t T>
inline SecureBitset<T> SecureBitset<T>::operator&(const SecureBitset<T>& other)
{
	SecureBitset bs;
	bs.bitset = bitset & other.bitset;
	return bs;
}

template<std::size_t T>
inline SecureBitset<T> SecureBitset<T>::operator|(const SecureBitset<T>& other)
{
	SecureBitset bs;
	bs.bitset = bitset | other.bitset;
	return bs;
}

template<std::size_t T>
inline SecureBitset<T> SecureBitset<T>::operator^(const SecureBitset<T>& other)
{
	SecureBitset bs;
	bs.bitset = bitset ^ other.bitset;
	return bs;
}

template<std::size_t T>
inline SecureBitset<T> SecureBitset<T>::operator~() const
{
	SecureBitset bs;
	bs.bitset = ~bitset;
	return bs;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::operator<<=(const std::size_t offset)
{
	bitset <<= offset;
	return *this;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::operator>>=(const std::size_t offset)
{
	bitset >>= offset;
	return *this;
}

template<std::size_t T>
inline SecureBitset<T> SecureBitset<T>::operator<<(const std::size_t offset) const
{
	SecureBitset bs;
	bs.bitset = bitset << offset;
	return bs;
}

template<std::size_t T>
inline SecureBitset<T> SecureBitset<T>::operator>>(const std::size_t offset) const
{
	SecureBitset bs;
	bs.bitset = bitset >> offset;
	return bs;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::set()
{
	bitset.set();
	return *this;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::set(const std::size_t index, bool value)
{
	bitset.set(index, value);
	return *this;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::reset()
{
	bitset.reset();
	return *this;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::reset(const std::size_t index)
{
	bitset.reset(index);
	return *this;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::flip()
{
	bitset.flip();
	return *this;
}

template<std::size_t T>
inline SecureBitset<T>& SecureBitset<T>::flip(const std::size_t index)
{
	bitset.flip(index);
	return *this;
}

template<std::size_t T>
inline std::string SecureBitset<T>::to_string() const
{
	return bitset.to_string();
}

template<std::size_t T>
inline unsigned long SecureBitset<T>::to_ulong() const
{
	return bitset.to_ulong();
}
template<std::size_t T>
inline unsigned long long SecureBitset<T>::to_ullong() const
{
	return bitset.to_ullong();
}

template<std::size_t T>
inline std::bitset<T>& SecureBitset<T>::Get()
{
	return bitset;
}

template<std::size_t T>
inline const std::bitset<T>& SecureBitset<T>::Get() const
{
	return bitset;
}

template <std::size_t T>
inline std::ostream& operator<<(std::ostream& ofs, const SecureBitset<T>& bs)
{
	return ofs << bs.Get();
}

template <std::size_t T>
inline std::istream& operator>>(std::istream& ifs, const SecureBitset<T>& bs)
{
	return ifs >> bs.Get();
}
}

/*** ./../GhettoCrypt/Block.h ***/

#pragma once

namespace GhettoCipher
{
	typedef SecureBitset<BLOCK_SIZE> Block;
}

/*** ./../GhettoCrypt/Util.h ***/

#pragma once
#include <bitset>
#include <sstream>
#include <fstream>

namespace GhettoCipher
{
    //! Mod-operator that works with negative values
    inline int Mod(const int numerator, const int denominator)
    {
        return (denominator + (numerator % denominator)) % denominator;
    }

    //! Will perform a wrapping left-bitshift on a bitset
    template <std::size_t T>
    inline SecureBitset<T> Shiftl(const SecureBitset<T>& bits, const std::size_t amount)
    {
        std::stringstream ss;
        const std::string bitss = bits.to_string();

        for (std::size_t i = 0; i < bitss.size(); i++)
            ss << bitss[Mod((int)(i + amount), (int)bitss.size())];

        return SecureBitset<T>(ss.str());
    }

    //! Will perform a wrapping right-bitshift on a bitset
    template <std::size_t T>
    inline SecureBitset<T> Shiftr(const SecureBitset<T>& bits, const  std::size_t amount)
    {
        std::stringstream ss;
        const std::string bitss = bits.to_string();

        for (std::size_t i = 0; i < bitss.size(); i++)
            ss << bitss[Mod((i - amount), bitss.size())];

        return SecureBitset<T>(ss.str());
    }

    //! Will pad a string to a set length with a certain character
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

    //! Will convert a string to a fixed-size data block
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

    //! Will convert a fixed-size data block to a string
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

    //! Turns a fixed-size data block into a hex-string
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


    //! Turns a hex string into a fixed-size data block
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
                value = ((std::size_t)c - '0') + 0;
            else if ((c >= 'a') && (c <= 'f'))
                // Else, it is a lowercase letter
                value = ((std::size_t)c - 'a') + 10;
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
                value = ((std::size_t)c - '0') + 0;
            else if ((c >= 'a') && (c <= 'f'))
                // Else, it is a lowercase letter
                value = ((std::size_t)c - 'a') + 10;
            else
                throw std::logic_error("non-hex string detected in HexstringToBits()");

            // Append to our bits
            ss << std::bitset<4>(value);
        }

        return ss.str();
    }

    //! Creates a key of size BLOCK_SIZE from a password of arbitrary length.
    //! Using passwords larger (in bits) than BLOCK_SIZE is not generally recommended.
    //! Note that if your password is shorter (in bits) than BLOCK_SIZE, the rest of the key will be padded with 0x0. Further round-keys will be extrapolated though.
    inline Block PasswordToKey(const std::string& in)
    {
        Block b;

        // Segment the password in segments of key-size, and xor them together.
        for (std::size_t i = 0; i < in.size(); i += BLOCK_SIZE / 8)
            b ^= StringToBitblock(
                PadStringToLength(in.substr(i, BLOCK_SIZE / 8), BLOCK_SIZE / 8, 0, false)
            );

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

/*** ./../GhettoCrypt/Halfblock.h ***/

#pragma once
#include <cstdint>

namespace GhettoCipher
{
	constexpr std::size_t HALFBLOCK_SIZE = (BLOCK_SIZE / 2);
	typedef SecureBitset<HALFBLOCK_SIZE> Halfblock;
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
		static bool EncryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport = false);

		//! Will decrypt a file.
		//! Returns false if anything goes wrong (like, file-access).
		//! @filename_in The file to be read.
		//! @filename_out The file the decrypted version should be saved in.
		static bool DecryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport = false);

	private:
		// No instanciation! >:(
		GhettoCryptWrapper();
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
