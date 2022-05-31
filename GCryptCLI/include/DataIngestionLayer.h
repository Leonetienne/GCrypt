#ifndef GCRYPTCLI_DATAINGESTIONLAYER_H
#define GCRYPTCLI_DATAINGESTIONLAYER_H

#include <iosfwd>
#include <queue>
#include <GCrypt/Block.h>
#include "Configuration.h"

using namespace Leonetienne::GCrypt;

namespace IO {

  // This class is used to read in data.
  class DataIngestionLayer {
    public:
      //! Will initialize the ingestion
      static void Init();

      //! Will destruct the ingestion layer (like, closing file handles)
      static void Destruct();

      //! Will attempt to read a data block.
      //! Requires Init() to have been called
      static void ReadBlock();

      //! Have we read in all available blocks?
      static bool ReachedEOF();

      //! Returns true if there are blocks to be fetched (GetNextBlock())
      static bool IsBlockReady();

      //! Will return the next block in the queue
      static Block GetNextBlock();

      //! Returns true, if EOF is reached, and there are no more blocks to fetch (GetNextBlock())
      static bool IsFinished();

    private:
      static std::istream* in;

      // We have to hold on to a reference to a filestream,
      // even if we're always just reading from in.
      // We still have to CLOSE the file handle afterwards!
      static std::ifstream ifs;
      static std::istringstream iss;

      // Indicates whether EOF has been reached
      static bool reachedEof;
      // Indicates whether this class has been initialized
      static bool initialized;

      // All read blocks, that haven't been given out yet
      static std::queue<Block> blocks;

      // No instanciation! >:(
      DataIngestionLayer();
  };
}

#endif

