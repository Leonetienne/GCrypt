#include "DataIngestionLayer.h"
#include "CommandlineInterface.h"
#include "Configuration.h"
#include <iostream>
#include <cstring>

using namespace IO;

void DataIngestionLayer::Init() {

  // Set our istream
  switch (Configuration::inputFrom) {

    // Are we reading from stdin?
    case Configuration::INPUT_FROM::STDIN:

      // Redirect our istream to stdin
      in = &std::cin;
      break;

    // Are we reading from a file?
    case Configuration::INPUT_FROM::FILE:

      // Open the file
      ifs.open(
        Configuration::inputFilename,
        std::ios::in | std::ios::binary
      );

      // A little bit of error handling
      if (!ifs.good()) {
        throw std::runtime_error( "Unable to open infilestream!");
      }

      // Redirect our istream to this infilestream
      in = &ifs;
      break;

      // Are we reading from a parameter?
      case Configuration::INPUT_FROM::PARAMETER:

      // Create an instringstream with our parameter
      iss = std::istringstream(
        CommandlineInterface::Get()["--intext"].GetString()
      );

      // Redirect our istream to this instringstream
      in = &iss;

      break;
  }

  // Temporary
  std::size_t n_last_bytes_read = 0;
  char buf[64];
  do {
    memset(buf, 0, sizeof(buf));
    in->read(buf, sizeof(buf));
    n_last_bytes_read = in->gcount();
    std::cout << buf;

  } while(n_last_bytes_read == 64);


  return;
}


std::istream* DataIngestionLayer::in;
std::ifstream DataIngestionLayer::ifs;
std::istringstream DataIngestionLayer::iss;

