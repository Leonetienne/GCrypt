#include <unordered_map>
#include "Feistel.h"
#include "Util.h"
#include "Config.h"

GhettoCipher::Feistel::Feistel(const Block& key)
{
    SetKey(key);
    return;
}

GhettoCipher::Feistel::~Feistel()
{
    ZeroKeyMemory();

    return;
}

void GhettoCipher::Feistel::SetKey(const Block& key)
{
    GenerateRoundKeys(key);
    return;
}

GhettoCipher::Block GhettoCipher::Feistel::Encipher(const Block& data) const
{
    return Run(data, false);
}

GhettoCipher::Block GhettoCipher::Feistel::Decipher(const Block& data) const
{
    return Run(data, true);
}

GhettoCipher::Block GhettoCipher::Feistel::Run(const Block& data, bool reverseKeys) const
{
    const auto splitData = FeistelSplit(data);
    GhettoCipher::Halfblock l = splitData.first;
    GhettoCipher::Halfblock r = splitData.second;

    Halfblock tmp;

    for (std::size_t i = 0; i < N_ROUNDS; i++)
    {
        // Calculate key index
        std::size_t keyIndex;
        if (reverseKeys)
            keyIndex = N_ROUNDS - i - 1;
        else
            keyIndex = i;

        // Do a feistel round
        tmp = r;
        r = l ^ F(r, roundKeys[keyIndex]);
        l = tmp;
    }

    return FeistelCombine(r, l);
}

GhettoCipher::Halfblock GhettoCipher::Feistel::F(Halfblock m, const Block& key)
{
    // Made-up F function

    // Expand to full bitwidth
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

std::pair<GhettoCipher::Halfblock, GhettoCipher::Halfblock> GhettoCipher::Feistel::FeistelSplit(const Block& block)
{
    const std::string bits = block.to_string();

    Halfblock l(bits.substr(0, bits.size() / 2));
    Halfblock r(bits.substr(bits.size() / 2));

    return std::make_pair(l, r);
}

GhettoCipher::Block GhettoCipher::Feistel::FeistelCombine(const Halfblock& l, const Halfblock& r)
{
    return Block(l.to_string() + r.to_string());
}

GhettoCipher::Block GhettoCipher::Feistel::ExpansionFunction(const Halfblock& block)
{
    std::stringstream ss;
    const std::string bits = block.to_string();

    std::unordered_map<std::string, std::string> expansionMap;
    expansionMap["00"] = "1101";
    expansionMap["01"] = "1000";
    expansionMap["10"] = "0010";
    expansionMap["11"] = "0111";

    // We have to double the bits!
    for (std::size_t i = 0; i < bits.size(); i += 2)
    {
        const std::string sub = bits.substr(i, 2);
        ss << expansionMap[sub];
    }

    return Block(ss.str());
}

GhettoCipher::Halfblock GhettoCipher::Feistel::CompressionFunction(const Block& block)
{
    std::stringstream ss;
    const std::string bits = block.to_string();

    std::unordered_map<std::string, std::string> compressionMap;
    compressionMap["0000"] = "10";
    compressionMap["0001"] = "01";
    compressionMap["0010"] = "10";
    compressionMap["0011"] = "10";
    compressionMap["0100"] = "11";
    compressionMap["0101"] = "01";
    compressionMap["0110"] = "00";
    compressionMap["0111"] = "11";
    compressionMap["1000"] = "01";
    compressionMap["1001"] = "00";
    compressionMap["1010"] = "11";
    compressionMap["1011"] = "00";
    compressionMap["1100"] = "11";
    compressionMap["1101"] = "10";
    compressionMap["1110"] = "00";
    compressionMap["1111"] = "01";

    // We have to half the bits!
    for (std::size_t i = 0; i < bits.size(); i += 4)
    {
        const std::string sub = bits.substr(i, 4);
        ss << compressionMap[sub];
    }

    return Halfblock(ss.str());
}

std::string GhettoCipher::Feistel::SBox(const std::string& in)
{
    static std::unordered_map<std::string, std::string> subMap;
    static bool mapInitialized = false;
    if (!mapInitialized)
    {
        subMap["0000"] = "1100";
        subMap["0001"] = "1000";
        subMap["0010"] = "0001";
        subMap["0011"] = "0111";
        subMap["0100"] = "1011";
        subMap["0101"] = "0011";
        subMap["0110"] = "1101";
        subMap["0111"] = "1111";
        subMap["1000"] = "0000";
        subMap["1001"] = "1010";
        subMap["1010"] = "0100";
        subMap["1011"] = "1001";
        subMap["1100"] = "0010";
        subMap["1101"] = "1110";
        subMap["1110"] = "0101";
        subMap["1111"] = "0110";
        mapInitialized = true;
    }

    return subMap[in];
}

void GhettoCipher::Feistel::GenerateRoundKeys(const Block& seedKey)
{
    // Generate round keys via output feedback modus (OFM) method

    // Clear initial key memory
    ZeroKeyMemory();
    roundKeys = Keyset();

    // Generate new keys from the seed key
    roundKeys[0] = seedKey;
    roundKeys[1] = (Shiftl(seedKey, 32) ^ roundKeys[0]);

    for (std::size_t i = 2; i < roundKeys.size(); i++)
    {
        roundKeys[i] = Shiftl(roundKeys[i - 1], i + 32) ^ roundKeys[i - 2];
    }

    return;
}

// These pragmas only work for MSVC and g++, as far as i know. Beware!!!
#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#elif defined __GNUG__
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
void GhettoCipher::Feistel::ZeroKeyMemory()
{
    for (Block& key : roundKeys)
        key.reset();

    return;
}
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#elif defined __GNUG__
#pragma GCC pop_options
#endif
