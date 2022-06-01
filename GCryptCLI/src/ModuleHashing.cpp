#include "ModuleHashing.h"
#include "DataIngestionLayer.h"
#include "DataOutputLayer.h"
#include "KeyManager.h"
#include <GCrypt/GHash.h>

using namespace Module;
using namespace Leonetienne::GCrypt;

void Hashing::Run() {

  // Initialize the data ingestion layer
  IO::DataIngestionLayer::Init();

  // Initialize the data output layer
  IO::DataOutputLayer::Init();

  // Initialize a ghasher
  GHash hasher;

  // Read in new blocks, if not reached eof
  while (!IO::DataIngestionLayer::IsFinished()) {
    if (!IO::DataIngestionLayer::ReachedEOF()) {
      IO::DataIngestionLayer::ReadBlock();
    }

    // Process a block, if one is ready
    if (IO::DataIngestionLayer::IsBlockReady()) {
      const Block cleartext = IO::DataIngestionLayer::GetNextBlock();
      hasher.Digest(cleartext);
    }
  }

  // Wait until we've finished digesting all blocks
  // Enqueue that single block (the hash result) to the output layer
  IO::DataOutputLayer::Enqueue(hasher.GetHashsum());

  // Tell the data output layer that that was the last block
  IO::DataOutputLayer::ReachedEOF();

  // Dump it
  while (!IO::DataOutputLayer::IsFinished()) {
    IO::DataOutputLayer::WriteBlock();
  }

  // Destruct the data ingestion layer
  IO::DataIngestionLayer::Destruct();

  // Destruct the data output layer
  IO::DataOutputLayer::Destruct();

  return;
}
