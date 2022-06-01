#ifndef GCRYPTCLI_PROGRESSPRINTER_H
#define GCRYPTCLI_PROGRESSPRINTER_H

#include <iostream>
#include "CommandlineInterface.h"

// This class has the task to output progress to stderr, if requested
class ProgressPrinter {
  public:
    //! Will print progress to stderr, if requested, and the interval matches
    static void PrintIfAppropriate(
      const std::string& message,
      const std::size_t current,
      const std::size_t target
    );

  private:
    // No instanciation! >:(
    ProgressPrinter() {};
};

#endif

