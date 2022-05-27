#include "CommandlineInterface.h"
#include "Configuration.h"
#include "ModulePrepareKey.h"

int main(int argc, char* const* argv) {

  // Init cli args
  CommandlineInterface::Init(argc, argv);

  // Parse configuration
  Configuration::Parse();

  // Prepare the key
  ModulePrepareKey::PrepareKey();

  return 0;
}

