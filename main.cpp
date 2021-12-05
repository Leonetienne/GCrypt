#include <iostream>
#include <sstream>
#include <array>
#include <bitset>

#define BLOCK_SIZE 16
#define FEISTELBLOCK_SIZE (BLOCK_SIZE / 2)
#define N_ROUNDS 8

typedef std::bitset<BLOCK_SIZE> Block;
typedef std::bitset<FEISTELBLOCK_SIZE> Feistelblock;



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
    Block message = 0b1010101010101010;

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

    std::cout << "Input:      " << message << std::endl;

    Block ciphertext = Feistel(message, keys);
    std::cout << "Ciphertext: " << ciphertext << std::endl;

    Block decrypted = Feistel(ciphertext, keys, true);
    std::cout << "Decrypted:  " << decrypted << std::endl;

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
