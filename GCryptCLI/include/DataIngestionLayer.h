#ifndef GCRYPTCLI_DATAINGESTIONLAYER_H
#define GCRYPTCLI_DATAINGESTIONLAYER_H

#include <istream>
#include <fstream>
#include <sstream>

namespace IO {

  // This class is used to read in data.
  class DataIngestionLayer {
    public:
      //! Will initialize the ingestion
      static void Init();

    private:
      static std::istream* in;

      // We have to hold on to a reference to a filestream,
      // even if we're always just reading from in.
      // We still have to CLOSE the file handle afterwards!
      static std::ifstream ifs;
      static std::istringstream iss;
  };
}

#endif

