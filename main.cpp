#include <iostream>
#include <sstream>
#include <array>

#define BLOCK_SIZE 8
#define N_ROUNDS 8

std::string F(std::string m, const std::string& key);
std::string Xor(const std::string& a, const std::string& b);
std::string Shiftl(const std::string& s, std::size_t amount);
std::string Shiftr(const std::string& s, std::size_t amount);
std::pair<std::string, std::string> Feistel(std::string l, std::string r, const std::array<std::string, 8>& keys, bool reverseKeyOrder = false);

int Mod(int numerator, int denominator)
{
    return (denominator + (numerator % denominator)) % denominator;
}

int main()
{
    std::string l = "10101010";
    std::string r = "10101010";
    const std::array<std::string, 8> keys = {
        "11101101",
        "01110101",
        "10111101",
        "00010110",
        "00000011",
        "10110011",
        "11011101",
        "00111101"
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

std::pair<std::string, std::string> Feistel(std::string l, std::string r, const std::array<std::string, 8>& keys, bool reverseKeyOrder)
{
    std::string tmp;

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
        r = Xor(l, F(r, keys[keyIndex]));
        l = tmp;
    }

    return std::make_pair(r, l);
}

std::string F(std::string m, const std::string& key)
{
    // Made-up F function

    // Shift 5 to the left
    m = Shiftl(m, 5);

    // Xor with key
    return Xor(m, key);
}

std::string Shiftl(const std::string& s, std::size_t amount)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < s.size(); i++)
        ss << s[Mod((i + amount), s.size())];

    return ss.str();
}

std::string Shiftr(const std::string& s, std::size_t amount)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < s.size(); i++)
        ss << s[Mod((i - amount), s.size())];

    return ss.str();
}

std::string Xor(const std::string& a, const std::string& b)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < BLOCK_SIZE; i++)
    {
        if (a[i] xor b[i])
            ss << '1';
        else
            ss << '0';
    }

    return ss.str();
}
