#pragma once
#include <bitset>
#include "Config.h"

#define HALFBLOCK_SIZE (BLOCK_SIZE / 2)

typedef std::bitset<HALFBLOCK_SIZE> Halfblock;
