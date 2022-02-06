#include "InitializationVector.h"
#include <random>
#include <sstream>

// It would be REALLY BAD if another compiler/*version would use
// a mersenne twister with different attrbitutes. It would basically mean
// that E_machine1(M,K) != E_machine2(M,K), which would make them incompatible.
// We do NOT want this to happen, so let's be VERY specific about what mersenne twister setup we want.
// This is std::mt19937, as of msvc stl.
using Prng_MT = std::mersenne_twister_engine<
	unsigned int,
	32, 624, 397, 31, 0x9908b0df, 11, 0xffffffff,
	7, 0x9d2c5680, 15,0xefc60000, 18, 1812433253
>;

GhettoCipher::InitializationVector::InitializationVector(const Block& seed)
{
	// Since an initialization vector does not have to be a secret,
	// we should be fine just using a mersenne twister seeded with
	// for example the key to fill it up to fit BLOCK_SIZE.

	// Loosely seed mersenne twister with seed
	// Here is nothing copied. Both Block::Get, and Hash<>::operator() take refs.
	Prng_MT mt = Prng_MT(std::hash<std::bitset<BLOCK_SIZE>>()(seed.Get()));
	// Now generate BLOCK_SIZE urandom bits
	std::stringstream ss;
	for (std::size_t i = 0; i < BLOCK_SIZE; i++)
		ss << (mt() % 2 ? '1' : '0');
	
	// And create a block
	iv = Block(ss.str());
}

GhettoCipher::InitializationVector::operator GhettoCipher::Block() const
{
	return iv;
}
