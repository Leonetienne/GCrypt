#include <iostream>
#include <sstream>
#include <array>
#include <bitset>

#define BLOCK_SIZE 128
#define FEISTELBLOCK_SIZE (BLOCK_SIZE / 2)
#define N_ROUNDS 64

typedef std::bitset<BLOCK_SIZE> Block;
typedef std::bitset<FEISTELBLOCK_SIZE> Feistelblock;
typedef std::array<Feistelblock, N_ROUNDS> Keyset;

// Will convert a string to a data block
Block StringToBits(const std::string& s);

// Will convert a data block to a string
std::string BitsToString(const Block& bits);

// Split a data block into two feistel blocks (into L and R)
std::pair<Feistelblock, Feistelblock> FeistelSplit(const Block& block);

// Combine two feistel blocks (L and R) into a regular data block
Block FeistelCombine(const Feistelblock& l, const Feistelblock& r);

// Will generate a keyset from a seed-key
Keyset GenerateRoundkeys(const Feistelblock& seedKey);

// Feistel-cipher
Block Feistel(const Block& data, const Keyset& keys, bool reverseKeyOrder = false);

// Arbitrary cipher function
Feistelblock F(Feistelblock m, const Feistelblock& key);

int main()
{
    const std::string asciiMessage = "Guten Abend!";

    Block message = StringToBits(asciiMessage);

    const Feistelblock seedKey(StringToBits("Ich bin ein PASSWORT-SCHLÜSSEL!").to_string()); // StringToBits returns a bitset that's too large. We have to trim it to fit the smaller FeistelBlock bitset.
    const Keyset roundkeys = GenerateRoundkeys(seedKey);

    //std::cout << "Keys: " << std::endl;
    //for (std::size_t i = 0; i < roundkeys.size(); i++)
    //    std::cout << roundkeys[i] << std::endl;
    //std::cout << "---" << std::endl;

    std::cout << "Message ascii:   " << asciiMessage << std::endl;

    std::cout << "Message:         " << message << std::endl;

    Block ciphertext = Feistel(message, roundkeys);
    std::cout << "Ciphertext:      " << ciphertext << std::endl;

    Block decrypted = Feistel(ciphertext, roundkeys, true);
    std::cout << "Decrypted:       " << decrypted << std::endl;

    const std::string asciiDecrypted = BitsToString(decrypted);
    std::cout << "Decrypted ascii: " << asciiDecrypted << std::endl;

    return 0;
}

Block Feistel(const Block& data, const Keyset& keys, bool reverseKeyOrder)
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

    // Shift to left by 3 for every 1 in the key
    for (std::size_t i = 0; i < key.size(); i++)
        if (key[i])
            m <<= 3;

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

Keyset GenerateRoundkeys(const Feistelblock& seedKey)
{
    Keyset keys;
    
    keys[0] = seedKey;
    for (std::size_t i = 1; i < keys.size(); i++)
    {
        keys[i] = std::hash<Feistelblock>{}(keys[i-1]);
    }

    return keys;
}
