#pragma once
#include <bitset>
#include <sstream>
#include "Block.h"
#include "Flexblock.h"

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

//! Will convert a string to a fixed data block
inline Block StringToBitblock(const std::string& s)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < s.size(); i++)
        ss << std::bitset<8>(s[i]);

    // Pad rest with zeores
    for (std::size_t i = s.size() * 8; i < BLOCK_SIZE; i++)
        ss << '0';

    return Block(ss.str());
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

    for (std::size_t i = 0; i < hexstring.size(); i ++)
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
