#include "ModuleGenerateKey.h"
#include "DataOutputLayer.h"
#include "KeyManager.h"
#include "CommandlineInterface.h"

using namespace Leonetienne::GCrypt;
using namespace Module;

void GenerateKey::Run() {

  // Initialize the data output layer
  IO::DataOutputLayer::Init();

  // Enqueue our single block of data
  IO::DataOutputLayer::Enqueue(KeyManager::GetKey());

  // Tell the data output layer, that is has received all blocks
  IO::DataOutputLayer::ReachedEOF();

  // Tell it to write all blocks
  // (a single call should suffice, but a while-loop is the proper
  //  way to do it)
  while (!IO::DataOutputLayer::IsFinished()) {
    IO::DataOutputLayer::WriteBlock();
  }

  // Destruct the data output layer
  IO::DataOutputLayer::Destruct();

  return;
}

