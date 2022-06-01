#include <iostream>
#include <GCrypt/Key.h>
#include <GCrypt/GPrng.h>
#include <BmpPP/Bmp.h>
#include "Benchmark.h"

const std::string execName = "visualize-prng-distribution";

using namespace Leonetienne::GCrypt;
using namespace Leonetienne::BmpPP;
using namespace Leonetienne::Eule;

int main() {

  // Black/white
  Benchmark(
    "black/white - GetBit()",
    []() {
      Key seed = Key::Random();
      BMP bmp(Vector2i(800, 800), Colormode::RGB);
      GPrng prng(seed);

      for (std::size_t x = 0; x < bmp.GetDimensions().x; x++)
      for (std::size_t y = 0; y < bmp.GetDimensions().y; y++) {
        const std::uint8_t pixel = prng.GetBit() ? 255 : 0;
        bmp.SetPixel(Vector2i(x, y), pixel);
      }

      bmp.Write(execName+"-blackwhite.bmp");
    }
  );

  // Grayscale (using GetRandom<>)
  Benchmark(
    "grayscale - GetRandom<std::uint8_t>()",
    []() {
      Key seed = Key::Random();
      BMP bmp(Vector2i(800, 800), Colormode::RGB);
      GPrng prng(seed);

      for (std::size_t x = 0; x < bmp.GetDimensions().x; x++)
      for (std::size_t y = 0; y < bmp.GetDimensions().y; y++) {
        const std::uint8_t pixel = prng.GetRandom<std::uint8_t>();
        bmp.SetPixel(Vector2i(x, y), pixel);
      }

      bmp.Write(execName+"-getrandom-grayscale.bmp");
    }
  );

  // Grayscale (using operator()) (this one returns a whole uint32)
  Benchmark(
    "grayscale - operator()",
    []() {
      Key seed = Key::Random();
      BMP bmp(Vector2i(800, 800), Colormode::RGB);
      GPrng prng(seed);

      for (std::size_t x = 0; x < bmp.GetDimensions().x; x++)
      for (std::size_t y = 0; y < bmp.GetDimensions().y; y++) {
        const std::uint8_t pixel = prng() % 256;
        bmp.SetPixel(Vector2i(x, y), pixel);
      }

      bmp.Write(execName+"-operator-grayscale.bmp");
    }
  );

  // Color (using GetRandom<>)
  Benchmark(
    "color - GetRandom<uint8_t>()",
    []() {
      Key seed = Key::Random();
      BMP bmp(Vector2i(800, 800), Colormode::RGB);
      GPrng prng(seed);

      for (std::size_t x = 0; x < bmp.GetDimensions().x; x++)
      for (std::size_t y = 0; y < bmp.GetDimensions().y; y++) {
        const std::uint8_t r = prng.GetRandom<std::uint8_t>();
        const std::uint8_t g = prng.GetRandom<std::uint8_t>();
        const std::uint8_t b = prng.GetRandom<std::uint8_t>();
        bmp.SetPixel(Vector2i(x, y), r, g, b);
      }

      bmp.Write(execName+"-getrandom-color.bmp");
    }
  );

  // Color (using operator()) (this one returns a whole uint32)
  Benchmark(
    "color - operator()",
    []() {
      Key seed = Key::Random();
      BMP bmp(Vector2i(800, 800), Colormode::RGB);
      GPrng prng(seed);

      for (std::size_t x = 0; x < bmp.GetDimensions().x; x++)
      for (std::size_t y = 0; y < bmp.GetDimensions().y; y++) {
        const std::uint8_t r = prng() % 256;
        const std::uint8_t g = prng() % 256;
        const std::uint8_t b = prng() % 256;
        bmp.SetPixel(Vector2i(x, y), r, g, b);
      }

      bmp.Write(execName+"-operator-color.bmp");
    }
  );

  // Color (using GetBlock())
  //(this one returns a derivation of the current hashsum, without using up randomness)
  Benchmark(
    "color - GetBlock()",
    []() {
      Key seed = Key::Random();
      BMP bmp(Vector2i(800, 800), Colormode::RGB);
      GPrng prng(seed);

      std::size_t bytes_written = 0;
      while (bytes_written < bmp.GetPixelbufferSize()) {
        const Block block = prng.GetBlock();

        std::size_t bytesToCopy = 0;
        if (bmp.GetPixelbufferSize() - bytes_written < Block::BLOCK_SIZE) {
          bytesToCopy = bmp.GetPixelbufferSize() - bytes_written;
        }
        else {
          bytesToCopy = Block::BLOCK_SIZE;
        }

        memcpy(
          (char*)bmp.GetPixelbuffer().data() + bytes_written,
          (char*)block.Data(),
          bytesToCopy
        );

        bytes_written += bytesToCopy;

      }

      bmp.Write(execName+"-getblock-color.bmp");
    }
  );

  return 0;
}

