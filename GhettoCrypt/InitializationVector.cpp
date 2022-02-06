#include "InitializationVector.h"
#include <random>
#include <sstream>

using namespace GhettoCipher;

InitializationVector::InitializationVector(const Block& seed)
{
	// Since an initialization vector does not have to be a secret,
	// we should be fine just using a mersenne twister seeded with
	// for example the key to fill it up to fit BLOCK_SIZE.

	// Loosely seed mersenne twister with seed
	// Here is nothing copied. Both Block::Get, and Hash<>::operator() take refs.
	std::mt19937 mt = std::mt19937(std::hash<std::bitset<BLOCK_SIZE>>()(seed.Get()));

	// Now generate BLOCK_SIZE urandom bits
	std::stringstream ss;
	for (std::size_t i = 0; i < BLOCK_SIZE; i++)
		ss << (mt() % 2 ? '1' : '0');
	
	// And create a bitset
	iv = Block(ss.str());
}

InitializationVector::operator GhettoCipher::Block() const
{
	return iv;
}
