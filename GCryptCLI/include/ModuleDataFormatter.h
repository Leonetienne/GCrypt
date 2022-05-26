#ifndef GCRYPTCLI_MODULE_DATAFORMATTER_H
#define GCRYPTCLI_MODULE_DATAFORMATTER_H

#include <GCrypt/Block.h>
#include <string>
#include <vector>
#include "Configuration.h"

using namespace Leonetienne::GCrypt;

// This class has the task to format Blocks to various formats.
class ModuleDataFormatter {
  public:
    static std::string FormatBlock(const Block& block, const Configuration::IOBASE_FORMAT base);
    static Block StringToBlock(const std::string& str, const Configuration::IOBASE_FORMAT base);


  private:
    static std::string Bin2CustomBase(const std::string& bin, const std::vector<std::string>& customSet, const std::string& seperator = "");
    static std::string CustomBase2Bin(const std::string& in, const std::vector<std::string>& customSet, const std::string& seperator = "");
};

#endif

