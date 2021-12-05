#include <iostream>
#include <sstream>
#include <array>
#include <bitset>

#define BLOCK_SIZE 128
#define FEISTELBLOCK_SIZE (BLOCK_SIZE / 2)
#define N_ROUNDS 8

typedef std::bitset<BLOCK_SIZE> Block;
typedef std::bitset<FEISTELBLOCK_SIZE> Feistelblock;

// Will convert a string to a data block
Block StringToBits(const std::string& s);

// Will convert a data block to a string
std::string BitsToString(const Block& bits);

// Split a data block into two feistel blocks (into L and R)
std::pair<Feistelblock, Feistelblock> FeistelSplit(const Block& block);

// Combine two feistel blocks (L and R) into a regular data block
Block FeistelCombine(const Feistelblock& l, const Feistelblock& r);

// Feistel-cipher
Block Feistel(const Block& data, const std::array<Feistelblock, 8>& keys, bool reverseKeyOrder = false);

// Arbitrary cipher function
Feistelblock F(Feistelblock m, const Feistelblock& key);

int main()
{
    const std::string asciiMessage = "Hello, World! :3";

    Block message = StringToBits(asciiMessage);

    const std::array<Feistelblock, N_ROUNDS> keys = {
        0b11101101,
        0b01110101,
        0b10111101,
        0b00010110,
        0b00000011,
        0b10110011,
        0b11011101,
        0b00111101
    };

    std::cout << "Message ascii:   " << asciiMessage << std::endl;

    std::cout << "Message:         " << message << std::endl;

    Block ciphertext = Feistel(message, keys);
    std::cout << "Ciphertext:      " << ciphertext << std::endl;

    Block decrypted = Feistel(ciphertext, keys, true);
    std::cout << "Decrypted:       " << decrypted << std::endl;

    const std::string asciiDecrypted = BitsToString(decrypted);
    std::cout << "Decrypted ascii: " << asciiDecrypted << std::endl;

    return 0;
}

Block Feistel(const Block& data, const std::array<Feistelblock, 8>& keys, bool reverseKeyOrder)
{
    const auto splitData = FeistelSplit(data);
    Feistelblock l = splitData.first;
    Feistelblock r = splitData.second;

    Feistelblock tmp;

    for (std::size_t i = 0; i < N_ROUNDS; i++)
    {
        // Calculate key index
        std::size_t keyIndex;
        if (reverseKeyOrder)
            keyIndex = N_ROUNDS - i - 1;
        else
            keyIndex = i;

        // Do a feistel round
        tmp = r;
        r = l ^ F(r, keys[keyIndex]);
        l = tmp;
    }

    return FeistelCombine(r, l);
}

Feistelblock F(Feistelblock m, const Feistelblock& key)
{
    // Made-up F function

    // Shift 5 to the left
    m <<= 5;

    // Xor with key
    return m ^ key;
}

Block StringToBits(const std::string& s)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < s.size(); i++)
        ss << std::bitset<8>(s[i]);

    // Pad rest with zeores
    for (std::size_t i = s.size() * 8; i < BLOCK_SIZE; i++)
        ss << '0';

    return Block(ss.str());
}

std::string BitsToString(const Block& bits)
{
    std::stringstream ss;

    const std::string bitstring = bits.to_string();

    for (std::size_t i = 0; i < BLOCK_SIZE; i += 8)
    {
        ss << (char)std::bitset<8>(bitstring.substr(i, i+8)).to_ulong();
    }

    return ss.str();
}

std::pair<Feistelblock, Feistelblock> FeistelSplit(const Block& block)
{
    const std::string bits = block.to_string();

    Feistelblock l(bits.substr(0, bits.size() / 2));
    Feistelblock r(bits.substr(bits.size() / 2));

    return std::make_pair(l, r);
}

Block FeistelCombine(const Feistelblock& l, const Feistelblock& r)
{
    return Block(l.to_string() + r.to_string());
}
