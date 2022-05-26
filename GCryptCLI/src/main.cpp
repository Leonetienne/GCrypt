#include "CommandlineInterface.h"
#include "Configuration.h"
#include "ModulePrepareKey.h"
#include "ModuleDataFormatter.h"
#include <iostream>

int main(int argc, char* const* argv) {

  // Init cli args
  CommandlineInterface::Init(argc, argv);

  // Parse configuration
  Configuration::Parse();

  // Prepare the key
  ModulePrepareKey::PrepareKey();

  std::cout
    << ModuleDataFormatter::FormatBlock(
         ModulePrepareKey::GetKey(),
         Configuration::iobaseFormat
       )
    << std::endl;


  return 0;
}

