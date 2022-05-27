#include "CommandlineInterface.h"
#include "Configuration.h"
#include "KeyManager.h"
#include "ModuleGenerateKeyfile.h"

int main(int argc, char* const* argv) {

  // Init cli args
  CommandlineInterface::Init(argc, argv);

  // Parse configuration
  Configuration::Parse();

  // Prepare the key
  KeyManager::PrepareKey();

  // Launch our module
  switch (Configuration::activeModule) {
    case Configuration::MODULE::GENERATE_KEYFILE:
      Module::GenerateKeyfile::Run();
  }

  return 0;
}

