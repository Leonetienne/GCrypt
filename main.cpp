#include <iostream>
#include <sstream>
#include <array>
#include <bitset>

#define BLOCK_SIZE 16
#define FEISTELBLOCK_SIZE (BLOCK_SIZE / 2)
#define N_ROUNDS 8

typedef std::bitset<BLOCK_SIZE> Block;
typedef std::bitset<FEISTELBLOCK_SIZE> Feistelblock;

Feistelblock F(Feistelblock m, const Feistelblock& key);
std::pair<Feistelblock, Feistelblock> Feistel(Feistelblock l, Feistelblock r, const std::array<Feistelblock, 8>& keys, bool reverseKeyOrder = false);

int Mod(int numerator, int denominator)
{
    return (denominator + (numerator % denominator)) % denominator;
}

int main()
{
    Feistelblock l = 0b10101010;
    Feistelblock r = 0b10101010;
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

    std::cout << "Input:      " << l << r << std::endl;

    auto c = Feistel(l, r, keys);
    l = c.first;
    r = c.second;

    std::cout << "Ciphertext: " << l << r << std::endl;

    c = Feistel(l, r, keys, true);
    l = c.first;
    r = c.second;

    std::cout << "Decrypted:  " << l << r << std::endl;
    return 0;
}

std::pair<Feistelblock, Feistelblock> Feistel(Feistelblock l, Feistelblock r, const std::array<Feistelblock, 8>& keys, bool reverseKeyOrder)
{
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

    return std::make_pair(r, l);
}

Feistelblock F(Feistelblock m, const Feistelblock& key)
{
    // Made-up F function

    // Shift 5 to the left
    m <<= 5;

    // Xor with key
    return m ^ key;
}
