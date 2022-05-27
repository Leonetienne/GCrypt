#include "CommandlineInterface.h"
#include "Configuration.h"
#include "ModulePrepareKey.h"
#include "ModuleDataFormatter.h"
#include "Bases.h"
#include <iostream>

int main(int argc, char* const* argv) {

  // Init cli args
  CommandlineInterface::Init(argc, argv);

  // Parse configuration
  Configuration::Parse();

  // Prepare the key
  ModulePrepareKey::PrepareKey();

  Block block;
  block.FromTextString("Hello World :3");
  std::cout << block.ToBinaryString() << std::endl << std::endl;
  std::cout << block.ToHexString() << std::endl << std::endl;


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
          //ModulePrepareKey::GetKey(),
          block,
          Configuration::iobaseFormat
        ),
        Configuration::iobaseFormat
      ).ToTextString()
    << std::endl;

  return 0;
}

