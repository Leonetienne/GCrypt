#include "DataFormatter.h"
#include "Bases.h"
#include <GeneralUtility/BaseConversion.h>
#include <StringTools/StringTools.h>
#include <GCrypt/Util.h>

using namespace Leonetienne::GCrypt;
using namespace Leonetienne::StringTools;
using namespace Leonetienne::GeneralUtility;

std::string DataFormatter::FormatBlock(
    const Block& block,
    const Configuration::IOBASE_FORMAT base
) {
  switch (base) {
    case Configuration::IOBASE_FORMAT::BASE_BYTES:
      return block.ToByteString();

    case Configuration::IOBASE_FORMAT::BASE_2:
      return block.ToBinaryString();

    case Configuration::IOBASE_FORMAT::BASE_8:
      return Bin2CustomBase(
        block.ToBinaryString(),
        BASE_8,
        blockLengthByBase[base]
      );

    case Configuration::IOBASE_FORMAT::BASE_10:
      return Bin2CustomBase(
        block.ToBinaryString(),
        BASE_10,
        blockLengthByBase[base]
      );

    case Configuration::IOBASE_FORMAT::BASE_16:
      return block.ToHexString();

    case Configuration::IOBASE_FORMAT::BASE_64:
      return Bin2CustomBase(
        block.ToBinaryString(),
        BASE_64,
        blockLengthByBase[base]
      );

    case Configuration::IOBASE_FORMAT::BASE_UWU:
      return Bin2CustomBase(
        block.ToBinaryString(),
        BASE_UWU,
        blockLengthByBase[base],
        " "
      );

    case Configuration::IOBASE_FORMAT::BASE_UGH:
      return Bin2CustomBase(
        block.ToBinaryString(),
        BASE_UGH,
        blockLengthByBase[base],
        " "
      );

    default:
      throw std::invalid_argument("FormatBlock(): Iobase not found! Oh no. Anyway.");
  }
}

std::string DataFormatter::FormatBlocks(
    const std::vector<Block>& blocks,
    const Configuration::IOBASE_FORMAT base
) {
  std::stringstream ss;

  std::size_t i = 0;
  for (const Block& block : blocks) {
    ss << FormatBlock(block, base);

    // If we are dealing with a multichar base, we must append a
    // seperator (space), but only if its not the last block.
    if (
        (base == Configuration::IOBASE_FORMAT::BASE_UWU) ||
        (base == Configuration::IOBASE_FORMAT::BASE_UGH)
    ) {
      if (i++ != blocks.size()) {
        ss << " ";
      }
    }
  }

  return ss.str();
}

Block DataFormatter::DecodeFormat(
    const std::string& str,
    const Configuration::IOBASE_FORMAT base
) {

  Block b;

  switch (base) {
    case Configuration::IOBASE_FORMAT::BASE_BYTES:
      b.FromByteString(str);
      break;

    case Configuration::IOBASE_FORMAT::BASE_2:
      b.FromBinaryString(str);
      break;

    case Configuration::IOBASE_FORMAT::BASE_8:
      b.FromBinaryString(
        CustomBase2Bin(
          str,
          BASE_8
        )
      );
      break;

    case Configuration::IOBASE_FORMAT::BASE_10:
      b.FromBinaryString(
        CustomBase2Bin(
          str,
          BASE_10
        )
      );
      break;

    case Configuration::IOBASE_FORMAT::BASE_16:
      b.FromHexString(str);
      break;

    case Configuration::IOBASE_FORMAT::BASE_64:
      b.FromBinaryString(
        CustomBase2Bin(
          str,
          BASE_64
        )
      );
      break;

    case Configuration::IOBASE_FORMAT::BASE_UWU:
      b.FromBinaryString(
        CustomBase2Bin(
          str,
          BASE_UWU,
          " "
        )
      );
      break;

    case Configuration::IOBASE_FORMAT::BASE_UGH:
      b.FromBinaryString(
        CustomBase2Bin(
          str,
          BASE_UGH,
          " "
        )
      );
      break;

    default:
      throw std::invalid_argument("StringToBlock(): Iobase now found! Oh no. Anyway.");
  }

  return b;
}

std::vector<Block> DataFormatter::DecodeFormatMultiblock(
  const std::string& str,
  const Configuration::IOBASE_FORMAT base
) {
  std::vector<Block> blocks;

  // A block is this many digits wide, in encoding
  const std::size_t blockWidth = blockLengthByBase[base];

  // How many blocks are we dealing with here?
  const std::size_t n_blocks = (str.length() / blockWidth) + 1;
  blocks.reserve(n_blocks);

  // Iterate over the string, and parse all blocks
  // We now have to differentiate between single-char digit sets (like hex),
  // and multi-char digit sets (like uwu):
  switch (base) {
    case Configuration::IOBASE_FORMAT::BASE_BYTES:
    case Configuration::IOBASE_FORMAT::BASE_2:
    case Configuration::IOBASE_FORMAT::BASE_8:
    case Configuration::IOBASE_FORMAT::BASE_10:
    case Configuration::IOBASE_FORMAT::BASE_16:
    case Configuration::IOBASE_FORMAT::BASE_64:
      // Easy case: Each digit is exactly one char in size.
      // We can just calculate how many bytes we should read.
      for (std::size_t i = 0; i < str.length(); i += blockWidth) {

        const std::string subs = str.substr(i, blockWidth);

        Block newBlock = DecodeFormat(
          subs,
          base
        );

        blocks.emplace_back(newBlock);
      }
      break;

    case Configuration::IOBASE_FORMAT::BASE_UWU:
    case Configuration::IOBASE_FORMAT::BASE_UGH: {
      // Hard case: Each digit is n digits long. Digits may vary in length.
      // They are seperated by spaces.
      // We have to parse them...
      std::size_t digitsPassed = 0;
      std::size_t blockStart = 0;
      for (std::size_t i = 0; i < str.length(); i++) {

        if (str[i] == ' ') {
          digitsPassed++;

          if (digitsPassed == blockWidth) {
            const std::string subs = str.substr(
              blockStart,
              i - blockStart
            );

            Block newBlock = DecodeFormat(
              subs,
              base
            );

            blocks.emplace_back(newBlock);

            digitsPassed = 0;
            blockStart = i+1;
          }
        }
      }
      // Here should never be any digits left. A formatted block should ALWAYS be full length.
      break;
    }

    default:
      throw std::invalid_argument("DataFormatter::StringToBlocks() has been passed an unknown base! No switch-case matched!");
      break;

  }

  return blocks;
}

std::string DataFormatter::Bin2CustomBase(
    const std::string& bin,
    const std::vector<std::string>& customSet,
    const std::size_t minLen,
    const std::string& seperator
) {
  std::stringstream ss;

  // Translate to custom set
  const std::vector<std::string> vec_base_custom =
    Leonetienne::GeneralUtility::BaseConversion::BaseX_2_Y<std::string, std::vector<std::string>>(
      bin,
      "01",
      customSet,
      minLen
    );

  // Convert to string
  for (std::size_t i = 0; i < vec_base_custom.size(); i++) {
    ss << vec_base_custom[i];

    if (i != vec_base_custom.size() - 1) {
      ss << seperator;
    }
  }

  return ss.str();
}

std::string DataFormatter::CustomBase2Bin(
    const std::string& in,
    const std::vector<std::string>& customSet,
    const std::string& seperator
) {
  // Split input into symbols
  const std::vector<std::string> in_symbols = StringTools::Split(in, seperator);

  // Translate to binary
  std::string binary =
    Leonetienne::GeneralUtility::BaseConversion::BaseX_2_Y<std::vector<std::string>, std::string>(
      in_symbols,
      customSet,
      std::string("01"),
      Block::BLOCK_SIZE_BITS
    );

  if (binary.length() != Block::BLOCK_SIZE_BITS) {
    throw std::invalid_argument("DataFormatter::CustomBase2Bin() received input that doesn't translate to a bitstring of length 512!");
  }

  return binary;
}

