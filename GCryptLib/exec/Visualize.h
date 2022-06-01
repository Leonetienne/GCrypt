#ifndef GCRYPTEXAMPLE_VISUALIZE_H
#define GCRYPTEXAMPLE_VISUALIZE_H

#include <GCrypt/Block.h>
#include <BmpPP/Bmp.h>
#include <string>

using namespace Leonetienne::GCrypt;
using namespace Leonetienne::Eule;
using namespace Leonetienne::BmpPP;

void VisualizeBlock(const Block& block, const std::string& name) {
  BMP bmp(Vector2i(32, 16), Colormode::RGB);

  std::size_t i = 0;
  for (std::size_t x = 0; x < bmp.GetDimensions().x; x++)
  for (std::size_t y = 0; y < bmp.GetDimensions().y; y++) {
    const std::uint8_t pixel = block.GetBit(i) == false ? 255 : 0;
    bmp.SetPixel(Vector2i(x, y), pixel);

    i++;
  }

  bmp.Write(name);

  return;
}

// size.x*size.y MUST equal blocks.size() * Block::BLOCK_SIZE_BITS. That should be, by defalt blocks.size * 512.
void VisualizeBlocks(const std::vector<Block>& blocks, const Vector2i& size, const std::string& name) {

  //! A bit of error checking...
  if (size.x*size.y != blocks.size() * Block::BLOCK_SIZE_BITS) {
    throw std::invalid_argument("Supplied unfitting widht/height for visualization. Does not fit bits!");
  }

  BMP bmp(size, Colormode::RGB);

  std::size_t i = 0;
  for (std::size_t x = 0; x < bmp.GetDimensions().x; x++)
  for (std::size_t y = 0; y < bmp.GetDimensions().y; y++) {

    const std::size_t blockIndex = i / Block::BLOCK_SIZE_BITS;
    const std::size_t relBitIndex = i - blockIndex * Block::BLOCK_SIZE_BITS;

    const std::uint8_t pixel = blocks[blockIndex].GetBit(relBitIndex) == false ? 255 : 0;
    bmp.SetPixel(Vector2i(x, y), pixel);

    i++;
  }

  bmp.Write(name);

  return;
}

#endif

