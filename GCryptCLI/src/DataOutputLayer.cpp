#include "DataOutputLayer.h"
#include "DataFormatter.h"
#include "CommandlineInterface.h"
#include <ostream>
#include <fstream>
#include <iostream>

using namespace IO;

void DataOutputLayer::Init() {

  // Set our ostream
  switch (Configuration::outputTo) {

    // Are we writing to stdout?
    case Configuration::OUTPUT_TO::STDOUT:

      // Redirect our ostream to stdout
      out = &std::cout;
      break;

    // Are we writing to a file?
    case Configuration::OUTPUT_TO::FILE:

      // Open the file
      ofs.open(
        Configuration::outputFilename,
        std::ios::out | std::ios::binary
      );

      // A little bit of error handling
      if (!ofs.good()) {
        throw std::runtime_error("Unable to open outfilestream!");
      }

      // Redirect our ostream to this outfilestream
      out = &ofs;
      break;
  }

  initialized = true;
  reachedEof = false;

  return;

}

void DataOutputLayer::Destruct() {
  if (Configuration::outputTo == Configuration::OUTPUT_TO::FILE) {
    ofs.close();
  }

  return;
}

void DataOutputLayer::Enqueue(const Block& block) {
  blocks.emplace(block);
  return;
}

void DataOutputLayer::WriteBlock() {
  // Some error checking
  if (!initialized) {
    throw std::runtime_error("Attempted to write on uninitialized DataOutputLayer!");
  }

  // Check if we have any block to write
  // and if we should (output-buffering)
  // Basically: only output if we have anything to output, and
  //   if --buffer-output is given, only output once we have reachedEof.
  if (
      (blocks.size() > 0) &&
      (
        (!CommandlineInterface::Get().HasParam("--buffer-output")) ||
        (reachedEof)
      )
  ) {
    // Fetch the block to write
    const Block block = blocks.front();
    blocks.pop();

    // Recode it to our output format
    const std::string formattedBlock =
      DataFormatter::FormatBlock(
        block,
        Configuration::formatOut
      );

    // Dump it
    // This way we avoid zerobytes getting trimmed off...
    out->write(formattedBlock.data(), formattedBlock.length());

    // If this is not the last block, and the used iobase set
    // requires it, append a seperator
    if (
        (!IsFinished()) &&
        (
         (Configuration::formatOut == Configuration::IOBASE_FORMAT::BASE_UWU) ||
         (Configuration::formatOut == Configuration::IOBASE_FORMAT::BASE_UGH)
        )
    ) {
      *out << " ";
    }

    AddTrailingLinebreakIfRequired();
    out->flush();

  }

  return;
}

void DataOutputLayer::ReachedEOF() {
  reachedEof = true;

  // Add the trailing linebreak here aswell, as, if input is ciphertext,
  // ReachedEOF() may only be called after all blocks are already written
  AddTrailingLinebreakIfRequired();

  return;
}

bool DataOutputLayer::IsFinished() {
  return (reachedEof) && (blocks.size() == 0);
}

void DataOutputLayer::AddTrailingLinebreakIfRequired() {
  // If we are finished, and are outputting to stdout,
  // and input format is not bytes,
  // and the user didn't specifically opt out, print a newline
  if (
    (IsFinished()) &&
    (Configuration::outputTo == Configuration::OUTPUT_TO::STDOUT) &&
    (Configuration::formatOut != Configuration::IOBASE_FORMAT::BASE_BYTES) &&
    (!CommandlineInterface::Get().HasParam("--no-newline"))
  ) {
    *out << std::endl;
    out->flush();
  }

  return;
}

std::ostream* DataOutputLayer::out;
std::ofstream DataOutputLayer::ofs;
bool DataOutputLayer::reachedEof = false;
bool DataOutputLayer::initialized = false;
std::queue<Block> DataOutputLayer::blocks;

