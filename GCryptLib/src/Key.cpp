#include "GCrypt/Key.h"
#include "GCrypt/GHash.h"
#include "GCrypt/Util.h"
#include <random>
#include <cassert>
#include <sstream>
#include <fstream>

namespace Leonetienne::GCrypt {

  Key Key::FromPassword(const std::string& password) {
    return GHash::HashString(password);
  }

  Key Key::Random() {
    // Create our truly-random rng
    std::random_device rng;
    constexpr std::size_t bitsPerCall = sizeof(std::random_device::result_type) * 8;

    // Create a new key, and assign 16 random values
    Key key;
    for (std::size_t i = 0; i < 16; i++) {
      key[i] = rng();
    }

    // Return it
    return key;
  }

  Key Key::LoadFromFile(const std::string& path) {
    // Read this many chars
    const std::size_t maxChars = Key::BLOCK_SIZE;

    // Open ifilestream for keyfile
    std::ifstream ifs(path, std::ios::in | std::ios::binary);

    // Is the file open now? Or were there any issues...
    if (!ifs.good()) {
      throw std::runtime_error(std::string("Unable to open ifilestream for keyfile \"") + path + "\"! Aborting...");
    }

    // Create a new key, and zero it
    Key key;
    key.Reset();

    // Read into it
    ifs.read((char*)(void*)key.Data(), Key::BLOCK_SIZE);

    // Return it
    return key;
  }

  void Key::WriteToFile(const std::string& path) const {
    // Create an ofilestream
    std::ofstream ofs(path, std::ios::out | std::ios::binary);

    // Write the key
    ofs.write((char*)(void*)Data(), Key::BLOCK_SIZE);

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

