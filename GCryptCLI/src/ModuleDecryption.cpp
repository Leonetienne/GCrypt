#include "ModuleDecryption.h"
#include "DataIngestionLayer.h"
#include "DataOutputLayer.h"
#include "ProgressPrinter.h"
#include "KeyManager.h"
#include <GCrypt/GCipher.h>

using namespace Module;
using namespace Leonetienne::GCrypt;

void Decryption::Run() {

  // Initialize the data ingestion layer
  IO::DataIngestionLayer::Init();

  // Initialize the data output layer
  IO::DataOutputLayer::Init();

  // Initialize a cipher
  GCipher cipher(
    KeyManager::GetKey(),
    GCipher::DIRECTION::DECIPHER
  );

  std::size_t nBlocksDigested = 0;
  while (!IO::DataOutputLayer::IsFinished()) {
    // Read in new blocks, if not reached eof
    if (!IO::DataIngestionLayer::ReachedEOF()) {
      IO::DataIngestionLayer::ReadBlock();
    }

    // Process a block, if one is ready
    if (IO::DataIngestionLayer::IsBlockReady()) {

      // Print progress, if appropriate
      ProgressPrinter::PrintIfAppropriate(
        "Decrypting",
        nBlocksDigested,
        IO::DataIngestionLayer::NBlocksRead()
      );

      const Block cleartext = IO::DataIngestionLayer::GetNextBlock();
      const Block ciphertext = cipher.Digest(cleartext);
      nBlocksDigested++;

      // Enqueue the block for output
      IO::DataOutputLayer::Enqueue(ciphertext);
    }

    // Tell the data output layer that it received the
    // last block, if it did
    if (IO::DataIngestionLayer::IsFinished()) {
      IO::DataOutputLayer::ReachedEOF();
    }

    // Attempt to write a block
    IO::DataOutputLayer::WriteBlock();
  }

  // Destruct the data ingestion layer
  IO::DataIngestionLayer::Destruct();

  // Destruct the data output layer
  IO::DataOutputLayer::Destruct();

  return;
}
