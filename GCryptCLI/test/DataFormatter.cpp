#include <DataFormatter.h>
#include <GCrypt/Key.h>
#include <GCrypt/GPrng.h>
#include "Catch2.h"

using namespace Leonetienne::GCrypt;

// Tests that recoding iobase formats works for individual blocks, with random data
TEMPLATE_TEST_CASE_SIG(
    __FILE__"Data formats can be converted, without changing in value, with indivudual blocks, with random data",
    "[DataFormatter]",
    ((Configuration::IOBASE_FORMAT formatUnderTest), formatUnderTest),
    Configuration::IOBASE_FORMAT::BASE_BYTES,
    Configuration::IOBASE_FORMAT::BASE_2,
    Configuration::IOBASE_FORMAT::BASE_8,
    Configuration::IOBASE_FORMAT::BASE_10,
    Configuration::IOBASE_FORMAT::BASE_16,
    Configuration::IOBASE_FORMAT::BASE_64,
    Configuration::IOBASE_FORMAT::BASE_UWU,
    Configuration::IOBASE_FORMAT::BASE_UGH
) {

  // Let's use a GPrng instead of Key::Random,
  // because Key::Random is rather slow (because it's using hardware events).
  // We just want randomized data for tests...

  GPrng prng(Key::Random());

  // Let's try 10 different random blocks per test
  for (std::size_t i = 0; i < 10; i++) {

    // Setup
    const Block b_initial = prng.GetBlock();

    // Exercise
    // Convert to a custom base
    const std::string b_format = DataFormatter::FormatBlock(
      b_initial,
      formatUnderTest
    );

    // Convert back to a block
    const Block b_retrieved = DataFormatter::DecodeFormat(
      b_format,
      formatUnderTest
    );

    // Verify
    // Do b_initial and b_retrieved match?
    REQUIRE(b_initial == b_retrieved);
  }
}

// Tests that recoding iobase format works
TEMPLATE_TEST_CASE_SIG(
    __FILE__"Data formats can be converted, without changing in value, with indivudual blocks, with very little data (lots of nullbytes)",
    "[DataFormatter]",
    ((Configuration::IOBASE_FORMAT formatUnderTest), formatUnderTest),
    Configuration::IOBASE_FORMAT::BASE_BYTES,
    Configuration::IOBASE_FORMAT::BASE_2,
    Configuration::IOBASE_FORMAT::BASE_8,
    Configuration::IOBASE_FORMAT::BASE_10,
    Configuration::IOBASE_FORMAT::BASE_16,
    Configuration::IOBASE_FORMAT::BASE_64,
    Configuration::IOBASE_FORMAT::BASE_UWU,
    Configuration::IOBASE_FORMAT::BASE_UGH
) {

  // Setup
  Block b_initial;
  b_initial.FromTextString("Heyu");

  // Exercise
  // Convert to a custom base
  const std::string b_format = DataFormatter::FormatBlock(
    b_initial,
    formatUnderTest
  );

  // Convert back to a block
  const Block b_retrieved = DataFormatter::DecodeFormat(
    b_format,
    formatUnderTest
  );

  // Verify
  // Do b_initial and b_retrieved match?
  REQUIRE(b_initial == b_retrieved);
}

