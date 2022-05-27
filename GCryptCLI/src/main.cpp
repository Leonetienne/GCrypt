#include "CommandlineInterface.h"
#include "Configuration.h"
#include "KeyManager.h"

int main(int argc, char* const* argv) {

  // Init cli args
  CommandlineInterface::Init(argc, argv);

  // Parse configuration
  Configuration::Parse();

  // Prepare the key
  KeyManager::PrepareKey();

  return 0;
}

