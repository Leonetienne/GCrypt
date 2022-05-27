#include "CommandlineInterface.h"
#include "Configuration.h"
#include "KeyManager.h"
#include "ModuleGenerateKey.h"

int main(int argc, char* const* argv) {

  // Init cli args
  CommandlineInterface::Init(argc, argv);

  // Parse configuration
  Configuration::Parse();

  // Prepare the key
  KeyManager::PrepareKey();

  // Launch our module
  switch (Configuration::activeModule) {
    case Configuration::MODULE::GENERATE_KEY:
      Module::GenerateKey::Run();
  }

  return 0;
}

