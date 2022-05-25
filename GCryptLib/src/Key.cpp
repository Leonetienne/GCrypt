#include "GCrypt/Key.h"
#include "GCrypt/GHash.h"
#include "GCrypt/Util.h"
#include <random>
#include <cassert>
#include <sstream>
#include <fstream>

namespace Leonetienne::GCrypt {

  Key Key::FromPassword(const std::string& password) {
    return GHash::CalculateHashsum(
      StringToBits(password)
    );
  }

  Key Key::Random() {
    // Create our truly-random rng
    std::random_device rng;
    constexpr std::size_t bitsPerCall = sizeof(std::random_device::result_type) * 8;

    // Fetch BLOCK_SIZE bits
    std::stringstream ss;
    for (std::size_t i = 0; i < Key::BLOCK_SIZE_BITS / bitsPerCall; i++) {
      ss << std::bitset<bitsPerCall>(rng());
    }

    // Verify that we actually have the correct size
    assert(ss.str().length() == Key::BLOCK_SIZE_BITS);

    // Return them as a key
    return Key(Block(ss.str()));
  }

  Key Key::LoadFromFile(const std::string& path) {
    // Read this many chars
    const std::size_t maxChars = Key::BLOCK_SIZE_BITS / 8;

    // Open ifilestream for keyfile
    std::ifstream ifs(path, std::ios::in | std::ios::binary);

    // Is the file open now? Or were there any issues...
    if (!ifs.good()) {
      throw std::runtime_error(std::string("Unable to open ifilestream for keyfile \"") + path + "\"! Aborting...");
    }

    // Read these chars to buffer
    char* ckeyfileContent = new char[maxChars];
    memset(ckeyfileContent, 0, maxChars * sizeof(char));
    ifs.read(ckeyfileContent, maxChars);
    ifs.close();

    // Convert the buffer to a bit block of key size
    std::stringstream ss;
    for (std::size_t i = 0; i < maxChars; i++)
      ss << std::bitset<8>(ckeyfileContent[i]);

    Block key(ss.str());

    // And delete the buffer
    delete[] ckeyfileContent;
    ckeyfileContent = nullptr;

    // Return it
    return key;
  }

  void Key::WriteToFile(const std::string& path) {
    // Transform key to bytes
    const std::string keybytes = BitsToBytes(ToString());

    // Create an ofilestream
    std::ofstream ofs(path, std::ios::out | std::ios::binary);

    // Write the key
    ofs.write(keybytes.data(), Key::BLOCK_SIZE_BITS / 8);

    // Close the file handle
    ofs.close();

    return;
  }

  Key::Key() : Block() {
  }

  Key::Key(const Block& b) : Block(b) {
  }

  Key::Key(const Key& k) : Block(k) {
  }
}

