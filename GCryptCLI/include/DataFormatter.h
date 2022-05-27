#ifndef GCRYPTCLI_DATAFORMATTER_H
#define GCRYPTCLI_DATAFORMATTER_H

#include <GCrypt/Block.h>
#include <string>
#include <vector>
#include "Configuration.h"

using namespace Leonetienne::GCrypt;

// This class has the task to format Blocks to various formats.
class DataFormatter {
  public:
    //! Will format a single block to a given iobase
    static std::string FormatBlock(
      const Block& block,
      const Configuration::IOBASE_FORMAT base
    );

    //! Will parse a string of a given iobase to a block
    static Block DecodeFormat(
      const std::string& str,
      const Configuration::IOBASE_FORMAT base
    );

    //! Will format a vector of blocks to a given iobase
    static std::string FormatBlocks(
      const std::vector<Block>& blocks,
      const Configuration::IOBASE_FORMAT base
    );

    //! Will format a string making up multiple block in a given iobase into a vector of block
    static std::vector<Block> DecodeFormatMultiblock(
      const std::string& str,
      const Configuration::IOBASE_FORMAT base
    );


  private:
    static std::string Bin2CustomBase(
      const std::string& bin,
      const std::vector<std::string>& customSet,
      const std::size_t minLen,
      const std::string& seperator = ""
    );

    static std::string CustomBase2Bin(
      const std::string& in,
      const std::vector<std::string>& customSet,
      const std::string& seperator = ""
    );
};

#endif

