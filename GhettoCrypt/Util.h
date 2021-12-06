#pragma once
#include <bitset>
#include <sstream>
#include <fstream>
#include "Block.h"
#include "Flexblock.h"

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
