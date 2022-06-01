#ifndef GCRYPTCLI_DATAOUTPUTLAYER_H
#define GCRYPTCLI_DATAOUTPUTLAYER_H

#include <iosfwd>
#include <queue>
#include <GCrypt/Block.h>
#include "Configuration.h"

using namespace Leonetienne::GCrypt;

namespace IO {

  // This class is used to read in data.
  class DataOutputLayer {
    public:
      //! Will initialize the output
      static void Init();

      //! Will destruct the output layer (like, closing file handles)
      static void Destruct();

      //! Will queue a block for writing
      static void Enqueue(const Block& block);

      //! Will attempt to write the next block
      static void WriteBlock();

      //! Indicates that no more blocks will be enqueued
      static void ReachedEOF();

      //! Returns true, if all blocks have been written, and an EOF signal as been received
      static bool IsFinished();

    private:
      //! If we are finished, and are outputting to stdout,
      //! and the user didn't specifically opt out, print a newline
      static void AddTrailingLinebreakIfRequired();

      static std::ostream* out;

      // We have to hold on to a reference to a filestream,
      // even if we're always just reading from in.
      // We still have to CLOSE the file handle afterwards!
      static std::ofstream ofs;

      // Indicates whether EOF has been reached
      static bool reachedEof;

      // Indicates whether this class has been initialized
      static bool initialized;

      // All blocks, that haven't been written yet
      static std::queue<Block> blocks;

      //! No instanciation >:(
      DataOutputLayer() {};
  };
}

#endif

