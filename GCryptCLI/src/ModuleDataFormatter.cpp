#include "ModuleDataFormatter.h"
#include "Bases.h"
#include <GeneralUtility/BaseConversion.h>
#include <StringTools/StringTools.h>
#include <GCrypt/Util.h>

using namespace Leonetienne::GCrypt;
using namespace Leonetienne::StringTools;
using namespace Leonetienne::GeneralUtility;


std::string ModuleDataFormatter::FormatBlock(
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
        BASE_8
      );

    case Configuration::IOBASE_FORMAT::BASE_10:
      return Bin2CustomBase(
        block.ToBinaryString(),
        BASE_10
      );

    case Configuration::IOBASE_FORMAT::BASE_16:
      return block.ToHexString();

    case Configuration::IOBASE_FORMAT::BASE_64:
      return Bin2CustomBase(
        block.ToBinaryString(),
        BASE_64
      );

    case Configuration::IOBASE_FORMAT::BASE_UWU:
      return Bin2CustomBase(
        block.ToBinaryString(),
        BASE_UWU,
        " "
      );

    case Configuration::IOBASE_FORMAT::BASE_UGH:
      return Bin2CustomBase(
        block.ToBinaryString(),
        BASE_UGH,
        " "
      );

    default:
      throw std::invalid_argument("FormatBlock(): Iobase now found! Oh no. Anyway.");
  }
}

Block ModuleDataFormatter::StringToBlock(
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


std::string ModuleDataFormatter::Bin2CustomBase(
    const std::string& bin,
    const std::vector<std::string>& customSet,
    const std::string& seperator
) {
  std::stringstream ss;

  // Translate to custom set
  const std::vector<std::string> vec_base_custom =
    Leonetienne::GeneralUtility::BaseConversion::BaseX_2_Y<std::string, std::vector<std::string>>(bin, "01", customSet);

  // Convert to string
  for (std::size_t i = 0; i < vec_base_custom.size(); i++) {
    ss << vec_base_custom[i];

    if (i != vec_base_custom.size() - 1) {
      ss << seperator;
    }
  }

  return ss.str();
}

std::string ModuleDataFormatter::CustomBase2Bin(
    const std::string& in,
    const std::vector<std::string>& customSet,
    const std::string& seperator
) {
  // Split input into symbols
  const std::vector<std::string> in_symbols = StringTools::Split(in, seperator);

  // Translate to binary
  std::string binary =
    Leonetienne::GeneralUtility::BaseConversion::BaseX_2_Y<std::vector<std::string>, std::string>(in_symbols, customSet, std::string("01"));

  // Pad to BLOCK_SIZE
  binary = PadStringToLength(binary, Block::BLOCK_SIZE_BITS, '0', false);

  // Check that our string is of size BLOCK_SIZE
  if (binary.length() != Block::BLOCK_SIZE_BITS) {
    throw std::invalid_argument("ModuleDataFormatter::CustomBase2Bin received input, which does note translate to a bitstring of size BLOCK_SIZE. Huh?");
  }

  return binary;
}

