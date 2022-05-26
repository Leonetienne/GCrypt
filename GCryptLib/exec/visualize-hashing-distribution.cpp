#include <iostream>
#include <GCrypt/GHash.h>
#include <BmpPP/Bmp.h>
#include "Visualize.h"

const std::string execName = "visualize-hashing-distribution";

using namespace Leonetienne::GCrypt;
using namespace Leonetienne::BmpPP;
using namespace Leonetienne::Eule;

void HashAndVisualize(const Block& b, const std::string filename) {
  GHash hasher;
  hasher.DigestBlock(b);
  VisualizeBlock(hasher.GetHashsum(), filename);
}

int main() {

  // Get some random input
  Block a;
  a.FromTextString("Hello, World :3");
  VisualizeBlock(a, execName+"-input-a.bmp");
  HashAndVisualize(a, execName+"-output-a.bmp");

  // Now flip a bit
  Block b = a;
  b.FlipBit(4);
  VisualizeBlock(b, execName+"-input-b.bmp");
  HashAndVisualize(b, execName+"-output-b.bmp");

  return 0;
}

