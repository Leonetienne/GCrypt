#pragma once
#include <bitset>
#include "Config.h"

namespace GhettoCipher
{
	constexpr int HALFBLOCK_SIZE = (BLOCK_SIZE / 2);
	typedef std::bitset<HALFBLOCK_SIZE> Halfblock;
}
