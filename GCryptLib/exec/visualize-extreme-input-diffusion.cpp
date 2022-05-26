#include <iostream>
#include <GCrypt/GWrapper.h>
#include <GCrypt/Key.h>
#include <GCrypt/Util.h>
#include <BmpPP/Bmp.h>
#include "Visualize.h"

const std::string execName = "visualize-extreme-input-diffusion";

using namespace Leonetienne::GCrypt;
using namespace Leonetienne::BmpPP;
using namespace Leonetienne::Eule;

int main() {
  // These are magic values, which work for this specific input string.
  // If you want to try another string, get the size of all blocks in bits (it gets printed),
  // and find two integer factors which factor up to it. These are your images width, and height.
  const Vector2i visualizationDimension = Vector2i(56, 64);


  // Create a key
  // The key is almost just zeores (it doesn't have an IV, so what you see is what goes in the cipher.)
  // In case you're wondering, I can tell you right now that an all-zero input with an all-zero key will result in an all-zero ciphertext.
  Key key;
  key.Reset();
  key.SetBit(199, true);
  VisualizeBlock(key, execName+"-key.bmp");

  // Create our input blocks
  // All the input blocks are just zeroes!
  std::vector<Block> input_blocks;
  input_blocks.resize(7);
  for (Block& block : input_blocks) {
    block.Reset();
  }

  std::cout << "Input size is " << input_blocks.size() * Block::BLOCK_SIZE_BITS << " bits long." << std::endl;
  VisualizeBlocks(input_blocks, visualizationDimension, execName+"-input.bmp");

  // Encrypt it
  std::vector<Block> ciphertext_blocks = GWrapper::CipherBlocks(input_blocks, key, GCipher::DIRECTION::ENCIPHER);
  VisualizeBlocks(ciphertext_blocks, visualizationDimension, execName+"-output.bmp");

  // Now flip a single bit in the input
  input_blocks[3].FlipBit(156);
  VisualizeBlocks(input_blocks, visualizationDimension, execName+"-input-flip.bmp");

  // Encrypt it again
  ciphertext_blocks = GWrapper::CipherBlocks(input_blocks, key, GCipher::DIRECTION::ENCIPHER);
  VisualizeBlocks(ciphertext_blocks, visualizationDimension, execName+"-output-flip.bmp");

  return 0;
}

