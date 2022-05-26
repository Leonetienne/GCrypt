#include <iostream>
#include <GCrypt/GWrapper.h>
#include <GCrypt/Key.h>
#include <GCrypt/Util.h>
#include <BmpPP/Bmp.h>
#include "Visualize.h"

const std::string execName = "visualize-multiblock-diffusion";

using namespace Leonetienne::GCrypt;
using namespace Leonetienne::BmpPP;
using namespace Leonetienne::Eule;

int main() {
  // These are magic values, which work for this specific input string.
  // If you want to try another string, get the size of all blocks in bits (it gets printed),
  // and find two integer factors which factor up to it. These are your images width, and height.
  const Vector2i visualizationDimension = Vector2i(56, 64);


  // Create a key
  const Key key = Key::Random();
  VisualizeBlock(key, execName+"-key.bmp");

  // Create our input blocks
  const std::string input = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  std::vector<Block> input_blocks = StringToBitblocks(input);
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

