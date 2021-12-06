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

    // We have to double the bits!
    for (std::size_t i = 0; i < bits.size(); i += 2)
    {
        const std::string sub = bits.substr(i, 2);

             if (sub == "00") ss << "1101";
        else if (sub == "01") ss << "1000";
        else if (sub == "10") ss << "0010";
        else if (sub == "11") ss << "0111";

    }

    return Block(ss.str());
}

GhettoCipher::Halfblock GhettoCipher::Feistel::CompressionFunction(const Block& block)
{
    std::stringstream ss;
    const std::string bits = block.to_string();

    // We have to double the bits!
    for (std::size_t i = 0; i < bits.size(); i += 4)
    {
        const std::string sub = bits.substr(i, 4);

             if (sub == "0000") ss << "10";
        else if (sub == "0001") ss << "01";
        else if (sub == "0010") ss << "10";
        else if (sub == "0011") ss << "10";
        else if (sub == "0100") ss << "11";
        else if (sub == "0101") ss << "01";
        else if (sub == "0110") ss << "00";
        else if (sub == "0111") ss << "11";
        else if (sub == "1000") ss << "01";
        else if (sub == "1001") ss << "00";
        else if (sub == "1010") ss << "11";
        else if (sub == "1011") ss << "00";
        else if (sub == "1100") ss << "11";
        else if (sub == "1101") ss << "10";
        else if (sub == "1110") ss << "00";
        else if (sub == "1111") ss << "01";
    }

    return Halfblock(ss.str());
}

std::string GhettoCipher::Feistel::SBox(const std::string& in)
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

// These pragmas only work for MSVC, as far as i know. Beware!!!
#if defined _WIN32 || defined _WIN64
#pragma optimize("", off )
#endif
void GhettoCipher::Feistel::ZeroKeyMemory()
{
    for (Block& key : roundKeys)
        key.reset();

    return;
}
#if defined _WIN32 || defined _WIN64
#pragma optimize("", on )
#endif
