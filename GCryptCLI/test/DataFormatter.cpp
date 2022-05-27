#include <ModuleDataFormatter.h>
#include <GCrypt/Key.h>
#include "Catch2.h"

using namespace Leonetienne::GCrypt;

// Tests that recoding byte iobase works
TEST_CASE(__FILE__"RecodingSingleBlock-bytes", "[ModuleDataFormatter]") {

  // Setup
  Block b_initial;
  b_initial = Key::Random();

  // Exercise
  // Convert to bytes
  const std::string b_format = ModuleDataFormatter::FormatBlock(
    b_initial,
    Configuration::IOBASE_FORMAT::BASE_BYTES
  );

  // Convert back to a block
  const Block b_retrieved = ModuleDataFormatter::DecodeFormat(
    b_format,
    Configuration::IOBASE_FORMAT::BASE_BYTES
  );

  // Verify
  // Do b_initial and b_retrieved match?
  REQUIRE(b_initial == b_retrieved);
}

