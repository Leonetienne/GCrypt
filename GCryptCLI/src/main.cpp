#include "CommandlineInterface.h"
#include "Configuration.h"
#include "KeyManager.h"
#include "ModuleGenerateKey.h"
#include "ModuleEncryption.h"
#include "ModuleDecryption.h"

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
      break;

    case Configuration::MODULE::ENCRYPTION:
      Module::Encryption::Run();
      break;

    case Configuration::MODULE::DECRYPTION:
      Module::Decryption::Run();
      break;
  }

  return 0;
}

