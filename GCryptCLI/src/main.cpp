#include "CommandlineInterface.h"
#include "Configuration.h"
#include "ModulePrepareKey.h"
#include <iostream>

int main(int argc, char* const* argv) {

  // Init cli args
  CommandlineInterface::Init(argc, argv);

  // Parse configuration
  Configuration::Parse();

  // Prepare the key
  ModulePrepareKey::PrepareKey();

  std::cout << ModulePrepareKey::GetKey().ToHexString() << std::endl;


  return 0;
}

