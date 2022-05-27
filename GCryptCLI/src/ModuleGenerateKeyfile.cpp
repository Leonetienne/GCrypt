#include "ModuleGenerateKeyfile.h"

#include "KeyManager.h"
#include "CommandlineInterface.h"

using namespace Leonetienne::GCrypt;
using namespace Module;

void GenerateKeyfile::Run() {

  KeyManager::GetKey().WriteToFile(
    CommandlineInterface::Get()["--generate-keyfile"].GetString()
  );

  return;
}

