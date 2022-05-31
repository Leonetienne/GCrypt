#include "DataIngestionLayer.h"
#include "CommandlineInterface.h"
#include "DataFormatter.h"
#include "Bases.h"
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
        throw std::runtime_error("Unable to open infilestream!");
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

  // Derive from our the current module if we're reading ciphertext or not
  if (
    (Configuration::activeModule == Configuration::MODULE::DECRYPTION)
  ) {
    isReadingCiphertext = true;
  }
  else {
    isReadingCiphertext = false;
  }

  initialized = true;
  reachedEof = false;

  return;
}

void DataIngestionLayer::Destruct() {

  if (Configuration::inputFrom == Configuration::INPUT_FROM::FILE) {
    ifs.close();
  }

  return;
}

void DataIngestionLayer::ReadBlock() {
  if (!initialized) {
    throw std::runtime_error("Attempted to read on uninitialized DataIngestionLayer!");
  }

  if (!reachedEof) {
    // A block is this many digits wide, in encoding
    const std::size_t blockWidth = blockLengthByBase[Configuration::formatIn];

    // Iterate over the string, and parse all blocks
    // We now have to differentiate between single-char digit sets (like hex),
    // and multi-char digit sets (like uwu):
    switch (Configuration::formatIn) {
      case Configuration::IOBASE_FORMAT::BASE_BYTES:
      case Configuration::IOBASE_FORMAT::BASE_2:
      case Configuration::IOBASE_FORMAT::BASE_8:
      case Configuration::IOBASE_FORMAT::BASE_10:
      case Configuration::IOBASE_FORMAT::BASE_16:
      case Configuration::IOBASE_FORMAT::BASE_64:
        // Easy case: Each digit is exactly one char in size.
        // We can just calculate how many bytes we have to read.

        // bytesRead is always of the correct length, 0-padded.
        std::size_t n_bytes_read;
        const std::string dataRead = ReadBytes(blockWidth, n_bytes_read);

        // If we've read 0 bytes, this was the last block
        // and it's completely empty. We can abort without doing anything.
        // The ReadBytes function takes care of setting the reachedEof flag.
        if (n_bytes_read == 0) {
          return;
        }

        // If we are reading ciphertext
        // make sure we've read enough bytes to compose a block.
        if (
          (isReadingCiphertext) &&
          (n_bytes_read < blockWidth)
        ) {
          throw std::runtime_error("DataIngestionLayer::ReadBlock() read an input-data fragment that is smaller than a data block should be. Is your cipher text incomplete?");
        }

        // This should decode to a block just like this.
        Block newBlock;

        // Special-case: We are reading cleartext (no ciphertext)
        // cleartext is always base_bytes
        if (!isReadingCiphertext) {
          // When just reading cleartext-bytes, we also allow shorter strings
          // than BLOCK_SIZE. These will just get zero-padded.
          newBlock.FromTextString(dataRead);
        }
        else {
          // Else: recode to a block.
          newBlock = DataFormatter::DecodeFormat(
            dataRead,
            Configuration::formatIn
          );
        }

        blocks.emplace(newBlock);
        break;
      }

  }

  return;
}

std::string DataIngestionLayer::ReadBytes(const std::size_t n, std::size_t& out_bytes_read) {

  // Prepare a buffer to read to
  char* buf = new char[n+1];
  memset(buf, 0, (n+1) * sizeof(buf[0]));

  // Read
  in->read(buf, n * sizeof(buf[0]));

  // Fetch how much we've read
  out_bytes_read = in->gcount();

  // Is this fewer bytes than got requested?
  if (out_bytes_read < n) {
    // Yes: EOF reached.
    reachedEof = true;
  }

  // Translate buffer to a standard string
  const std::string sbuf(buf, n);
  delete[] buf;

  // Return our buffer
  return sbuf;
}

bool DataIngestionLayer::ReachedEOF() {
  return reachedEof;
}

bool DataIngestionLayer::IsBlockReady() {
  // We're not ready, if we haven't reached EOF, if we should puffer
  // the input.
  if (
      (CommandlineInterface::Get().HasParam("--puffer-input")) &&
      (!reachedEof)
  ) {
    return false;
  }

  // If we're not puffering, just return whether or not
  // we have any blocks...
  return blocks.size() > 0;
}

bool DataIngestionLayer::IsFinished() {
  return (reachedEof) && (blocks.size() == 0);
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
bool DataIngestionLayer::isReadingCiphertext;
std::queue<Block> DataIngestionLayer::blocks;

