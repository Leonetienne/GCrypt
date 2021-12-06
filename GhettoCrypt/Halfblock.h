#pragma once
#include <bitset>
#include <cstdint>
#include "Config.h"

namespace GhettoCipher
{
	constexpr std::size_t HALFBLOCK_SIZE = (BLOCK_SIZE / 2);
	typedef std::bitset<HALFBLOCK_SIZE> Halfblock;
}
