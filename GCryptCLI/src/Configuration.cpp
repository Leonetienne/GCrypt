#include "Configuration.h"
#include "CommandlineInterface.h"

void Configuration::Parse() {
  DecideModule();
  DecideInputFrom();
  DecideOutputTo();
  DecideIOBaseFormat();

  return;
}

void Configuration::DecideModule() {
  if (CommandlineInterface::Get().HasParam("--encrypt")) {
    activeModule = MODULE::ENCRYPT;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--decrypt")) {
    activeModule = MODULE::DECRYPT;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--hash")) {
    activeModule = MODULE::HASH;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--generate-keyfile")) {
    activeModule = MODULE::GENERATE_KEYFILE;
    return;
  }

  throw std::runtime_error("No module option found. Is the CLI parser configuration correct?.");

  return;
}

void Configuration::DecideInputFrom() {

  if (CommandlineInterface::Get().HasParam("--intext")) {
    inputFrom = INPUT_FROM::PARAMETER;
  }
  else if (CommandlineInterface::Get().HasParam("--infile")) {
    inputFrom = INPUT_FROM::FILE;
    inputFilename = CommandlineInterface::Get()["--infile"].GetString();
  }
  else {
    inputFrom = INPUT_FROM::STDIN;
  }

  return;
}

void Configuration::DecideOutputTo() {

  // If module is "generate keyfile", we'll always write to a file.
  if (activeModule == MODULE::GENERATE_KEYFILE) {
    outputTo = OUTPUT_TO::FILE;
    outputFilename = CommandlineInterface::Get()["--generate-keyfile"].GetString();
  }

  // Else, check if we have an --ofile defined.
  if (CommandlineInterface::Get().HasParam("--ofile")) {
    outputTo = OUTPUT_TO::FILE;
    outputFilename = CommandlineInterface::Get()["--ofile"].GetString();
  }
  else {
    outputTo = OUTPUT_TO::STDOUT;
  }

  return;
}

void Configuration::DecideIOBaseFormat() {

  // Do we have any iobase specified?
  if (CommandlineInterface::Get().HasParam("--iobase-bytes")) {
    iobaseFormat = IOBASE_FORMAT::BASE_BYTES;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--iobase-2")) {
    iobaseFormat = IOBASE_FORMAT::BASE_2;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--iobase-8")) {
    iobaseFormat = IOBASE_FORMAT::BASE_8;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--iobase-10")) {
    iobaseFormat = IOBASE_FORMAT::BASE_10;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--iobase-16")) {
    iobaseFormat = IOBASE_FORMAT::BASE_16;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--iobase-64")) {
    iobaseFormat = IOBASE_FORMAT::BASE_64;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--iobase-uwu")) {
    iobaseFormat = IOBASE_FORMAT::BASE_UWU;
    return;
  }
  else if (CommandlineInterface::Get().HasParam("--iobase-ugh")) {
    iobaseFormat = IOBASE_FORMAT::BASE_UGH;
    return;
  }

  // So we have no iobase explicitly specified.. Let's default..

  // If we are encrypting or hashing,
  if (
      (activeModule == MODULE::ENCRYPT) ||
      (activeModule == MODULE::HASH)
  ) {
    // and input comes from a parameter,
    // and output goes to stdout, let's assume base-16.
    if (
        (inputFrom == INPUT_FROM::PARAMETER) &&
        (outputTo == OUTPUT_TO::STDOUT)
    ) {
      iobaseFormat = IOBASE_FORMAT::BASE_16;
      return;
    }

    // Any other case whilst encrypting, we'll assume base-bytes.
    else {
      iobaseFormat = IOBASE_FORMAT::BASE_BYTES;
      return;
    }

  }

  // Else, if we are decrypting,
  else if (activeModule == MODULE::DECRYPT) {
    // and input comes from a parameter, we'll assume base-16.
    if (inputFrom == INPUT_FROM::PARAMETER) {
      iobaseFormat = IOBASE_FORMAT::BASE_16;
      return;
    }
    // Any other case whilst decrypting, we'll assume base-bytes.
    else {
      iobaseFormat = IOBASE_FORMAT::BASE_BYTES;
      return;
    }
  }

  // Else, if we are generating a keyfile, iobase format is bytes.
  else if (activeModule == MODULE::GENERATE_KEYFILE) {
    iobaseFormat = IOBASE_FORMAT::BASE_BYTES;
    return;
  }

  // Fallback: Bytes
  else {
    iobaseFormat = IOBASE_FORMAT::BASE_BYTES;
  }

  return;
}

std::string Configuration::inputFilename;
std::string Configuration::outputFilename;
Configuration::MODULE Configuration::activeModule;
Configuration::IOBASE_FORMAT Configuration::iobaseFormat;
Configuration::INPUT_FROM Configuration::inputFrom;
Configuration::OUTPUT_TO Configuration::outputTo;

