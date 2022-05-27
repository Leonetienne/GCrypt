#include "CommandlineInterface.h"
#include "Configuration.h"
#include "ModulePrepareKey.h"
#include "ModuleDataFormatter.h"
#include "Bases.h"
#include <iostream>
#include <GCrypt/Util.h>

int main(int argc, char* const* argv) {

  // Init cli args
  CommandlineInterface::Init(argc, argv);

  // Parse configuration
  Configuration::Parse();

  // Prepare the key
  ModulePrepareKey::PrepareKey();

  const std::vector<Block> textblocks = StringToBitblocks("Hansel and Gretel are a brother and sister abandoned in a forest, where they fall into the hands of a witch who lives in a house made of gingerbread, cake, and candy. The cannibalistic witch intends to fatten the children before eventually eating them, but Gretel outwits the witch and kills her. The two children then escape with their lives and return home with the witch's treasure.");
  const std::string formattedBlocks =
    ModuleDataFormatter::FormatBlocks(
      textblocks,
      Configuration::iobaseFormat
    );

  std::cout << "Formatted: " << formattedBlocks << std::endl << std::endl;

  const std::vector<Block> retrievedBlocks =
    ModuleDataFormatter::StringToBlocks(
      formattedBlocks,
      Configuration::iobaseFormat
    );

  const std::string retrievedText = BitblocksToString(retrievedBlocks);
  std::cout << "Retrieved: " << retrievedText << std::endl << std::endl;

  return 0;

  /*
  Block all1;
  for (std::size_t i = 0; i < 16; i++)
    all1[i] = -1;

  std::cout
    << ModuleDataFormatter::FormatBlock(
        all1,
         Configuration::iobaseFormat
       )
    << std::endl
    << std::endl;

  std::cout
    << ModuleDataFormatter::FormatBlock(
        all1,
         Configuration::iobaseFormat
       ).size()
    << std::endl
    << std::endl;
  */

  /*
  std::cout
    << ModuleDataFormatter::FormatBlock(
         //ModulePrepareKey::GetKey(),
        block,
         Configuration::iobaseFormat
       )
    << std::endl
    << std::endl;
    */


  /*
  const std::string customBase =
    ModuleDataFormatter::FormatBlock(
      block,
      Configuration::iobaseFormat
  );

  std::cout << "\"" << customBase << "\"" << std::endl << std::endl;

  const Block back =
    ModuleDataFormatter::StringToBlock(
      customBase,
      Configuration::iobaseFormat
  );

  std::cout << back.ToHexString() << std::endl << std::endl;
  */

  std::cout
    << ModuleDataFormatter::StringToBlock(
        ModuleDataFormatter::FormatBlock(
          ModulePrepareKey::GetKey(),
          Configuration::iobaseFormat
        ),
        Configuration::iobaseFormat
      ).ToTextString()
    << std::endl;

  return 0;
}

