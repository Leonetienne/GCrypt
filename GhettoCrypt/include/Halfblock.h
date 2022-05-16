#pragma once
#include "SecureBitset.h"
#include <cstdint>
#include "Config.h"

namespace GhettoCipher
{
	constexpr std::size_t HALFBLOCK_SIZE = (BLOCK_SIZE / 2);
	typedef SecureBitset<HALFBLOCK_SIZE> Halfblock;
}
