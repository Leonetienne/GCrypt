#include <iostream>
#include <sstream>
#include <array>
#include <bitset>

#define BLOCK_SIZE 128
#define HALFBLOCK_SIZE (BLOCK_SIZE / 2)
#define N_ROUNDS 128

typedef std::bitset<BLOCK_SIZE> Block;
typedef std::bitset<HALFBLOCK_SIZE> Halfblock;
typedef std::array<Block, N_ROUNDS> Keyset;

// Will convert a string to a data block
Block StringToBits(const std::string& s);

// Will convert a data block to a string
std::string BitsToString(const Block& bits);

// Split a data block into two half blocks (into L and R)
std::pair<Halfblock, Halfblock> FeistelSplit(const Block& block);

// Combine two half blocks (L and R) into a regular data block
Block FeistelCombine(const Halfblock& l, const Halfblock& r);

// Will generate a keyset from a seed-key
Keyset GenerateRoundkeys(const Block& seedKey);

// Feistel-cipher
Block Feistel(const Block& data, const Keyset& keys, bool reverseKeyOrder = false);

// Will expand a halfblock to a fullblock
Block ExpansionFunction(const Halfblock& block);

// Will compress a fullblock to a halfblock
Halfblock CompressionFunction(const Block& block);

// Substitutes four bits by static random others
std::string SBox(const std::string& in);

// Arbitrary cipher function
Halfblock F(Halfblock m, const Block& key);

template<std::size_t T>
std::bitset<T> Shiftl(const std::bitset<T>& bits, std::size_t amount);

template<std::size_t T>
std::bitset<T> Shiftr(const std::bitset<T>& bits, std::size_t amount);

std::string DebugPrint(const std::string& asciiMessage);

int Mod(int numerator, int denominator)
{
    return (denominator + (numerator % denominator)) % denominator;
}


int main()
{
    const std::string cipher1 = DebugPrint("Hello, World :3");
    
    std::cout << std::endl;

    const std::string cipher2 = DebugPrint("Hello, world :3");

    std::size_t numDiff = 0;
    for (std::size_t i = 0; i < cipher1.size(); i++)
        if (cipher1[i] != cipher2[i])
            numDiff++;
    std::cout << std::endl;
    std::cout << "Total difference between C1 and C2: " << numDiff << std::endl;

    return 0;
}

std::string DebugPrint(const std::string& asciiMessage)
{
    Block message = StringToBits(asciiMessage);

    const Block seedKey = StringToBits("Ich bin ein PASSWORT-SCHLÜSSEL!");
    Keyset roundkeys = GenerateRoundkeys(seedKey);

    //std::cout << "Keys: " << std::endl;
    //for (std::size_t i = 0; i < roundkeys.size(); i++)
    //    std::cout << roundkeys[i] << std::endl;
    //std::cout << "---" << std::endl;
    //exit(0);


    std::cout << "Message ascii:   " << asciiMessage << std::endl;

    std::cout << "Message:         " << message << std::endl;

    Block ciphertext = Feistel(message, roundkeys);
    std::cout << "Ciphertext:      " << ciphertext << std::endl;

    Block decrypted = Feistel(ciphertext, roundkeys, true);
    std::cout << "Decrypted:       " << decrypted << std::endl;

    const std::string asciiDecrypted = BitsToString(decrypted);
    std::cout << "Decrypted ascii: " << asciiDecrypted << std::endl;

    return ciphertext.to_string();
}

Block Feistel(const Block& data, const Keyset& keys, bool reverseKeyOrder)
{
    const auto splitData = FeistelSplit(data);
    Halfblock l = splitData.first;
    Halfblock r = splitData.second;

    Halfblock tmp;

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

Halfblock F(Halfblock m, const Block& key)
{
    // Made-up F function

    // Expand to full bitwidht
    Block m_expanded = ExpansionFunction(m);

    // Shift to left by 1
    m_expanded = Shiftl(m_expanded, 1);

    // Xor with key
    m_expanded ^= key;

    // Non-linearly apply subsitution boxes
    std::stringstream ss;
    const std::string m_str = m_expanded.to_string();

    for (std::size_t i = 0; i < m_str.size(); i += 4)
    {
        ss << SBox(m_str.substr(i, 4));
    }

    m_expanded = Block(ss.str());

    // Return the compressed version
    return CompressionFunction(m_expanded);
}

Block ExpansionFunction(const Halfblock& block)
{
    std::stringstream ss;
    const std::string bits = block.to_string();

    // We have to double the bits!
    for (std::size_t i = 0; i < bits.size(); i += 2)
    {
        const std::string sub = bits.substr(i, 2);

             if (sub == "00")     ss << "1101";
        else if (sub == "01")     ss << "1000";
        else if (sub == "10")     ss << "0010";
        else /*if (sub == "11")*/ ss << "0111";

    }

    return Block(ss.str());
}

Halfblock CompressionFunction(const Block& block)
{
    std::stringstream ss;
    const std::string bits = block.to_string();

    // We have to double the bits!
    for (std::size_t i = 0; i < bits.size(); i += 4)
    {
        const std::string sub = bits.substr(i, 4);

             if (sub == "0000")     ss << "10";
        else if (sub == "0001")     ss << "01";
        else if (sub == "0010")     ss << "10";
        else if (sub == "0011")     ss << "10";
        else if (sub == "0100")     ss << "11";
        else if (sub == "0101")     ss << "01";
        else if (sub == "0110")     ss << "00";
        else if (sub == "0111")     ss << "11";
        else if (sub == "1000")     ss << "01";
        else if (sub == "1001")     ss << "00";
        else if (sub == "1010")     ss << "11";
        else if (sub == "1011")     ss << "00";
        else if (sub == "1100")     ss << "11";
        else if (sub == "1101")     ss << "10";
        else if (sub == "1110")     ss << "00";
        else /*if (sub == "1111")*/ ss << "01";
    }

    return Halfblock(ss.str());
}

std::string SBox(const std::string& in)
{
         if (in == "0000")     return "1100";
    else if (in == "0001")     return "1000";
    else if (in == "0010")     return "0001";
    else if (in == "0011")     return "0111";
    else if (in == "0100")     return "1011";
    else if (in == "0101")     return "0011";
    else if (in == "0110")     return "1101";
    else if (in == "0111")     return "1111";
    else if (in == "1000")     return "0000";
    else if (in == "1001")     return "1010";
    else if (in == "1010")     return "0100";
    else if (in == "1011")     return "1001";
    else if (in == "1100")     return "0010";
    else if (in == "1101")     return "1110";
    else if (in == "1110")     return "0101";
    else /*if (in == "1111")*/ return "0110";
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
        ss << (char)std::bitset<8>(bitstring.substr(i, 8)).to_ulong();
    }

    return ss.str();
}

std::pair<Halfblock, Halfblock> FeistelSplit(const Block& block)
{
    const std::string bits = block.to_string();

    Halfblock l(bits.substr(0, bits.size() / 2));
    Halfblock r(bits.substr(bits.size() / 2));

    return std::make_pair(l, r);
}

Block FeistelCombine(const Halfblock& l, const Halfblock& r)
{
    return Block(l.to_string() + r.to_string());
}

Keyset GenerateRoundkeys(const Block& seedKey)
{
    Keyset keys;

    keys[0] = seedKey;
    keys[1] = (Shiftl(seedKey, 32) ^ keys[0]);

    for (std::size_t i = 2; i < keys.size(); i++)
    {
        keys[i] = Shiftl(keys[i-1], i + 32) ^ keys[i-2];
    }

    return keys;
}

template <std::size_t T>
std::bitset<T> Shiftl(const std::bitset<T>& bits, std::size_t amount)
{
    std::stringstream ss;
    const std::string bitss = bits.to_string();

    for (std::size_t i = 0; i < bitss.size(); i++)
        ss << bitss[Mod((i + amount), bitss.size())];

    return std::bitset<T>(ss.str());
}

template <std::size_t T>
std::bitset<T> Shiftr(const std::bitset<T>& bits, std::size_t amount)
{
    std::stringstream ss;
    const std::string bitss = bits.to_string();

    for (std::size_t i = 0; i < bitss.size(); i++)
        ss << bitss[Mod((i - amount), bitss.size())];

    return std::bitset<T>(ss.str());
}
