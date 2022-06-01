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

      //! Returns how many blocks have been read, in total
      static std::size_t NBlocksRead();

    private:
      static std::istream* in;

      // Will read n bytes from the input.
      // If EOF is reached, it will return a string of length <= 5
      // and will set the approriate flags.
      static std::string ReadBytes(const std::size_t n, std::size_t& out_bytes_read);

      // We have to hold on to a reference to a filestream,
      // even if we're always just reading from in.
      // We still have to CLOSE the file handle afterwards!
      static std::ifstream ifs;
      static std::istringstream iss;

      // Indicates whether EOF has been reached
      static bool reachedEof;
      // Indicates whether this class has been initialized
      static bool initialized;

      // Are we reading ciphertext or regular text?
      static bool isReadingCiphertext;

      // How many blocks have been read in total
      static std::size_t nBlocksRead;

      // All read blocks, that haven't been given out yet
      static std::queue<Block> blocks;

      // No instanciation! >:(
      DataIngestionLayer();
  };
}

#endif

