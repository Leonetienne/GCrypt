#include "ProgressPrinter.h"
#include "CommandlineInterface.h"
#include <iostream>

void ProgressPrinter::PrintIfAppropriate(
  const std::string& message,
  const std::size_t current,
  const std::size_t target
) {
    if (
        (CommandlineInterface::Get().HasParam("--progress")) &&
        (current % CommandlineInterface::Get()["--progress-interval"].GetInt32() == 0)
    ) {
      std::cerr
        << message
        << "... (Block "
        << current + 1
        << " / "
        << target
        << " - " << ((float)(current+1)*100 / target)
        << "%)"
        << std::endl
      ;
    }

  return;
}

