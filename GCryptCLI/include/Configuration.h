#ifndef GCRYPTCLI_CONFIGURATION_H
#define GCRYPTCLI_CONFIGURATION_H

#include <string>

class Configuration {
  public:
    static enum class INPUT_FROM {
      STDIN,
      FILE,
      PARAMETER
    } inputFrom;

    static enum class OUTPUT_TO {
      STDOUT,
      FILE
    } outputTo;

    static enum class IOBASE_FORMAT {
      BASE_BYTES,
      BASE_2,
      BASE_8,
      BASE_10,
      BASE_16,
      BASE_64,
      BASE_UWU,
      BASE_UGH
    } iobaseFormat;

    static std::string inputFilename;
    static std::string outputFilename;

    static enum class MODULE {
      ENCRYPTION,
      DECRYPTION,
      HASH,
      GENERATE_KEY
    } activeModule;

    //! Will analyze the supplied cli parameters,
    //! and decide what the configuration will be.
    static void Parse();

  private:
    static void DecideInputFrom();
    static void DecideOutputTo();
    static void DecideIOBaseFormat();
    static void DecideModule();

    // No instanciation! >:(
    Configuration() {};
};

#endif

