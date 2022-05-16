#pragma once
#include <bitset>
#include <sstream>
#include <fstream>
#include <cstring>
#include "SecureBitset.h"
#include "Block.h"
#include "Flexblock.h"
#include "Config.h"
#include "Cipher.h"
#include "InitializationVector.h"

namespace Leonetienne::GCrypt {
    //! Mod-operator that works with negative values
    inline int Mod(const int numerator, const int denominator) {
        return (denominator + (numerator % denominator)) % denominator;
    }

    //! Will perform a wrapping left-bitshift on a bitset
    template <std::size_t T>
    inline SecureBitset<T> Shiftl(const SecureBitset<T>& bits, const std::size_t amount) {
        std::stringstream ss;
        const std::string bitss = bits.to_string();

        for (std::size_t i = 0; i < bitss.size(); i++) {
            ss << bitss[Mod((int)(i + amount), (int)bitss.size())];
        }

        return SecureBitset<T>(ss.str());
    }

    //! Will perform a wrapping right-bitshift on a bitset
    template <std::size_t T>
    inline SecureBitset<T> Shiftr(const SecureBitset<T>& bits, const  std::size_t amount) {
        std::stringstream ss;
        const std::string bitss = bits.to_string();

        for (std::size_t i = 0; i < bitss.size(); i++) {
            ss << bitss[Mod((i - amount), bitss.size())];
        }

        return SecureBitset<T>(ss.str());
    }

    //! Will pad a string to a set length with a certain character
    inline std::string PadStringToLength(const std::string& str, const std::size_t len, const char pad, const bool padLeft = true) {
        // Fast-reject: Already above padded length
        if (str.length() >= len) {
            return str;
        }

        std::stringstream ss;

        // Pad left:
        if (padLeft) {
            for (std::size_t i = 0; i < len - str.size(); i++) {
                ss << pad;
            }
            ss << str;
        }
        // Pad right:
        else {
            ss << str;
            for (std::size_t i = 0; i < len - str.size(); i++) {
                ss << pad;
            }
        }

        return ss.str();
    }

    //! Will convert a string to a fixed-size data block
    inline Block StringToBitblock(const std::string& s) {
        std::stringstream ss;

        for (std::size_t i = 0; i < s.size(); i++) {
            ss << std::bitset<8>(s[i]);
        }

        // Pad rest with zeores
        return Block(PadStringToLength(ss.str(), 128, '0', false));
    }

    //! Will convert a string to a flexible data block
    inline Flexblock StringToBits(const std::string& s) {
        std::stringstream ss;

        for (std::size_t i = 0; i < s.size(); i++) {
            ss << std::bitset<8>(s[i]);
        }

        return Flexblock(ss.str());
    }

    //! Will convert a fixed-size data block to a bytestring
    inline std::string BitblockToBytes(const Block& bits) {
        std::stringstream ss;

        const std::string bitstring = bits.to_string();

        for (std::size_t i = 0; i < BLOCK_SIZE; i += 8) {
            ss << (char)std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
        }

        return ss.str();
    }

    //! Will convert a fixed-size data block to a string
    //! The difference to BitblockToBytes() is, that it strips excess nullbytes
    inline std::string BitblockToString(const Block& bits) {
        // Decode to bytes
        std::string text = BitblockToBytes(bits);

        // Dümp excess nullbytes
        text.resize(strlen(text.data()));

        return text;
    }

    //! Will convert a flexible data block to a bytestring
    inline std::string BitsToBytes(const Flexblock& bits) {
        std::stringstream ss;

        const std::string bitstring = bits;

        for (std::size_t i = 0; i < bits.size(); i += 8) {
            ss << (char)std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
        }

        return ss.str();
    }

    //! Will convert a flexible data block to a string
    //! The difference to BitsToBytes() is, that it strips excess nullbytes
    inline std::string BitsToString(const Flexblock& bits) {
        // Decode to bytes
        std::string text = BitsToBytes(bits);

        // Dümp excess nullbytes
        text.resize(strlen(text.data()));

        return text;
    }

    //! Turns a fixed-size data block into a hex-string
    inline std::string BitblockToHexstring(const Block& b) {
        std::stringstream ss;
        const std::string charset = "0123456789abcdef";
        const std::string bstr = b.to_string();

        for (std::size_t i = 0; i < bstr.size(); i += 4) {
            ss << charset[std::bitset<4>(bstr.substr(i, 4)).to_ulong()];
        }

        return ss.str();
    }

    //! Turns a flexible data block into a hex-string
    inline std::string BitsToHexstring(const Flexblock& b) {
        std::stringstream ss;
        const std::string charset = "0123456789abcdef";
        const std::string bstr = b;

        for (std::size_t i = 0; i < bstr.size(); i += 4) {
            ss << charset[std::bitset<4>(bstr.substr(i, 4)).to_ulong()];
        }

        return ss.str();
    }


    //! Turns a hex string into a fixed-size data block
    inline Block HexstringToBitblock(const std::string& hexstring) {
        std::stringstream ss;

        for (std::size_t i = 0; i < hexstring.size(); i++) {
            const char c = hexstring[i];

            // Get value
            std::size_t value;
            if ((c >= '0') && (c <= '9')) {
                // Is it a number?
                value = ((std::size_t)c - '0') + 0;
            }
            else if ((c >= 'a') && (c <= 'f')) {
                // Else, it is a lowercase letter
                value = ((std::size_t)c - 'a') + 10;
            }
            else {
                throw std::logic_error("non-hex string detected in HexstringToBits()");
            }

            // Append to our bits
            ss << std::bitset<4>(value);
        }

        return Block(ss.str());
    }

    //! Turns a hex string into a flexible data block
    inline Flexblock HexstringToBits(const std::string& hexstring) {
        std::stringstream ss;

        for (std::size_t i = 0; i < hexstring.size(); i++) {
            const char c = hexstring[i];

            // Get value
            std::size_t value;
            if ((c >= '0') && (c <= '9')) {
                // Is it a number?
                value = ((std::size_t)c - '0') + 0;
            }
            else if ((c >= 'a') && (c <= 'f')) {
                // Else, it is a lowercase letter
                value = ((std::size_t)c - 'a') + 10;
            }
            else {
                throw std::logic_error("non-hex string detected in HexstringToBits()");
            }

            // Append to our bits
            ss << std::bitset<4>(value);
        }

        return ss.str();
    }

    //! Creates a key of size BLOCK_SIZE from a password of arbitrary length.
    //! Note that if your password is shorter (in bits) than BLOCK_SIZE, the rest of the key will be padded with 0 (see next line!).
    //! To provide a better initial key, (and to get rid of padding zeroes), the raw result (b) will be xor'd with an initialization vector based on b.
    //! : return b ^ iv(b)
    inline Block PasswordToKey(const std::string& in) {
        // Let's provide a nice initial value to be sure even a password of length 0 results in a proper key
        Block b = InitializationVector(StringToBitblock("3J7IipfQTDJbO8jtasz9PgWui6faPaEMOuVuAqyhB1S2CRcLw5caawewgDUEG1WN"));

        // Segment the password in segments of key-size, and xor them together.
        for (std::size_t i = 0; i < in.size(); i += BLOCK_SIZE / 8) {
            const Block fragment = StringToBitblock(
                PadStringToLength(in.substr(i, BLOCK_SIZE / 8), BLOCK_SIZE / 8, 0, false)
            );

            // To provide confusion, xor the blocks together
            // To provide diffusion, hash fragment to fragment' first
            b ^= Block(Cipher(fragment).Encipher(fragment.to_string()));
        }

        return b;
    }

    //! Will reduce a flexblock (they are of arbitrary length) to a single block.
    //! This single block should change completely, if a single bit in the input flexblock changes anywhere.
    inline Block ReductionFunction_Flexblock2Block(const Flexblock& in) {
        Block b; // No initialization vector needed here

        // Segment the input in segments of BLOCK_SIZE, and xor them together.
        for (std::size_t i = 0; i < in.size(); i += BLOCK_SIZE) {
            const Block fragment = Block(PadStringToLength(in.substr(i, BLOCK_SIZE), BLOCK_SIZE, 0, false));

            // To provide confusion, xor the blocks together
            // To provide diffusion, hash fragment to fragment' first
            b ^= Block(Cipher(fragment).Encipher(fragment.to_string()));
        }

        return b;
    }

    //! Will read a file into a flexblock
    inline Flexblock ReadFileToBits(const std::string& filepath) {
        // Read file
        std::ifstream ifs(filepath, std::ios::binary);

        if (!ifs.good()) {
            throw std::runtime_error("Unable to open ifilestream!");
        }

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
    inline void WriteBitsToFile(const std::string& filepath, const Flexblock& bits) {
        // Convert bits to bytes
        const std::string bytes = BitsToBytes(bits);

        // Write bits to file
        std::ofstream ofs(filepath, std::ios::binary);

        if (!ofs.good()) {
            throw std::runtime_error("Unable to open ofilestream!");
        }

        ofs.write(bytes.data(), bytes.length());
        ofs.close();

        return;
    }
}

