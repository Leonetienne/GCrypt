#include "DataIngestionLayer.h"
#include "CommandlineInterface.h"
#include "DataFormatter.h"
#include "Bases.h"
#include <StringTools/StringTools.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace IO;
using namespace Leonetienne::StringTools;

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
  nBlocksRead = 0;

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
      case Configuration::IOBASE_FORMAT::BASE_64: {
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
        nBlocksRead++;
        break;
      }

      case Configuration::IOBASE_FORMAT::BASE_UWU:
      case Configuration::IOBASE_FORMAT::BASE_UGH: {
        // The whole of Italy doesn't have as much spaghetti as this is...

        // Hard case: Each digit is n digits long. Digits may vary in length.
        // They are seperated by spaces.
        // We have to parse them...
        std::string overshoot = ""; // this is how much we've read too much in the last iteration


        // Gets terminated by a break statement
        while (1) {
          // We'll read chunks of 64 bytes... This should be a good
          // median, to also support small multi-byte-digit sets
          std::size_t n_bytes_read = 0;
          int lastDigitPos = -1; // Should point the the space BEFORE it. Relative to chunk.
          std::size_t digitsCollected = 0;
          std::stringstream digits;
          bool foundBlock = false;

          // Remember to prepend our overshoot from the previous iteration this chunk
          std::string chunk = overshoot + ReadBytes(64, n_bytes_read);

          // We should also strip all linebreaks from the chunk, that may be a result of manual stdin input.
          chunk = StringTools::Replace(chunk, '\n', "");

          // We can't just check for completeness by n_bytes_read...
          // It can be any number of bytes, since any digit is n bytes long...

          // Parse the 64-byte chunk string we've just fetched:
          for (std::size_t i = 0; i < chunk.size(); i++) {

            // If we are near the end, and have still not found a complete block, let's load an additional chunk
            if (i == chunk.size() - 2) {
              const std::string nextChunk = ReadBytes(64, n_bytes_read);
              if (n_bytes_read != 0) {
                chunk += StringTools::Replace(nextChunk, '\n', "");
              }
            }

            // If i is on a space, or at the end of the chunk,
            // and, at least one of i, or lastDigitPos is on a space,
            if (
                (
                 (chunk[i] == ' ') ||
                 (i == chunk.size() - 1)
                ) &&
                (
                 (chunk[i] == ' ') ||
                 lastDigitPos >= 0 // This basically does the same as discribed, but safer, as its default value is -1.
                )
            ){
              digitsCollected++;

              // We have found a digit. Let's store it away...
              // We're putting them in a stringstream, to fit the format required by the data formatter...

              // We have a slight edgecase if we're currently on the last char. Then we do NOT want to read one short.
              // This is because we ususally stand on a seperator char (' '), which we do not want to extract. But in that case,
              // in which we're standing on the last char, it could be not a seperator char.
              // note: chunk[i] != ' ' can only be true if we're on the last char.
              if (chunk[i] == ' ') {
                digits <<
                  chunk.substr(
                    lastDigitPos + 1,
                    (int)i - lastDigitPos - 1
                  )
                ;
              } else {
                digits <<
                  chunk.substr(
                    lastDigitPos + 1,
                    (int)i - lastDigitPos
                  )
                ;
              }

              // Add a seperator, if its not the last
              if (digitsCollected != blockWidth) {
                digits << ' ';
              }

              lastDigitPos = i;

              // Do we have enough digits to form a block?
              if (digitsCollected == blockWidth) {
                // We've found a complete block!

                // Trim excess nullbytes off out digit string
                const std::string digitString = std::string(
                  digits.str().data(),
                  strlen(digits.str().data())
                );

                // Decode it to a block object
                const Block newBlock = DataFormatter::DecodeFormat(
                  digitString,
                  Configuration::formatIn
                );

                // Enqueue it to be processed by some module
                blocks.emplace(newBlock);
                nBlocksRead++;
                foundBlock = true;

                // Now we have to calculate how many bytes we've read TOO MANY.
                // We have to trim this current chunk to be our new overshoot.

                // If we still have more than a byte left, leave out the current seperator char
                if (i < chunk.size() - 1) {
                  overshoot = chunk.substr(i+1); // Take all bytes from the next iterator, to the end
                }
                // Else: we are on the last char: there is no overshoot
                else {
                  overshoot = "";
                }

                // Stop the for loop
                break;
              }
            }
          }

          // Exit-condition:
          // We have not found any block, not even any digit.
          if ((!foundBlock) && (digitsCollected == 0)) {
            break;
          }

          // Hard-abort: We have not finished reading a block
          if (!foundBlock) {
            throw std::runtime_error("DataIngestionLayer reached EOF whilst parsing multi-byte-digit block...");
          }
        }

        break;
      }

      default:
        throw std::invalid_argument("DataFormatter::StringToBlocks() has been passed an unknown base! No switch-case matched!");
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

std::size_t DataIngestionLayer::NBlocksRead() {
  return nBlocksRead;
}

std::istream* DataIngestionLayer::in;
std::ifstream DataIngestionLayer::ifs;
std::istringstream DataIngestionLayer::iss;
bool DataIngestionLayer::reachedEof = false;
bool DataIngestionLayer::initialized = false;
bool DataIngestionLayer::isReadingCiphertext;
std::size_t DataIngestionLayer::nBlocksRead = 0;
std::queue<Block> DataIngestionLayer::blocks;

