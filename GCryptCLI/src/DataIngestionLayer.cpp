#include "DataIngestionLayer.h"
#include "CommandlineInterface.h"
#include "Configuration.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
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

  initialized = true;
  reachedEof = false;

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

void DataIngestionLayer::ReadBlock() {
  if (!initialized) {
    throw std::runtime_error("Attempted to read on uninitialized DataIngestionLayer!");
  }

  if (!reachedEof) {
    // Create buffer to read into
    char buf[Block::BLOCK_SIZE];
    memset(buf, 0, sizeof(buf));

    // Read
    in->read(buf, sizeof(buf));

    // Fetch how much we've read
    const std::size_t n_bytes_read = in->gcount();

    // Is this fewer bytes than we requested?
    if (n_bytes_read < sizeof(buf)) {
      // Yes: EOF reached.
      reachedEof = true;
    }

    // Construct a Block from this buf
    Block block;
    block.FromByteString(std::string(buf));

    // Enqueue it
    blocks.emplace(block);
  }

  return;
}

bool DataIngestionLayer::ReachedEOF() {
  return reachedEof;
}

bool DataIngestionLayer::IsBlockReady() {
  return blocks.size() > 0;
}

Block DataIngestionLayer::GetNextBlock() {
  if (!IsBlockReady()) {
    throw std::runtime_error("Attempted to get the next block, but there are none left!");
  }

  // Why... why not just return a T in pop()???
  const Block popped = blocks.front();
  blocks.pop();
  return popped;
}

std::istream* DataIngestionLayer::in;
std::ifstream DataIngestionLayer::ifs;
std::istringstream DataIngestionLayer::iss;
bool DataIngestionLayer::reachedEof = false;
bool DataIngestionLayer::initialized = false;
std::queue<Block> DataIngestionLayer::blocks;

