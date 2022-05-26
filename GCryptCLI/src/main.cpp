#include "CommandlineInterface.h"
#include "Configuration.h"

int main(int argc, char* const* argv) {

  // Init cli args
  CommandlineInterface::Init(argc, argv);

  // Parse configuration
  Configuration::Parse();

  return 0;
}

