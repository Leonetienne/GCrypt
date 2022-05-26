#include <iostream>
#include <GCrypt/GWrapper.h>
#include <GCrypt/Key.h>
#include <BmpPP/Bmp.h>
#include "Visualize.h"

const std::string execName = "visualize-singleblock-diffusion";

using namespace Leonetienne::GCrypt;
using namespace Leonetienne::BmpPP;
using namespace Leonetienne::Eule;

int main() {

  // Create input block
  const std::string input_str = "Hello :3";
  Block input;
  input.FromTextString(input_str);
  VisualizeBlock(input, execName+"-input.bmp");

  // Create a key
  const Key key = Key::Random();
  VisualizeBlock(key, execName+"-key.bmp");

  // Create a cipher
  GCipher cipher(key, GCipher::DIRECTION::ENCIPHER);

  // Encipher our block
  Block cipherblock = cipher.Digest(input);
  VisualizeBlock(cipherblock, execName+"-output.bmp");

  // Now flip a bit in the input
  input.FlipBit(35);
  VisualizeBlock(input, execName+"-input-flip.bmp");

  // Reset our cipher, and encipher the input with a flipped bit
  cipher.Initialize(key, GCipher::DIRECTION::ENCIPHER);

  // Encipher our block
  cipherblock = cipher.Digest(input);
  VisualizeBlock(cipherblock, execName+"-output-flip.bmp");

  return 0;
}

