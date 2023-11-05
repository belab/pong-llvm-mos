#pragma once

#include "Utils.h"

#include <cstdint>
#include <string.h>

struct Screen
{
	static constexpr uint16_t VIDEO_MEMORY = 0x400;

	static volatile uint8_t &display(uint8_t x, uint8_t y)
	{
		return Utils::memory(VIDEO_MEMORY + y * 40 + x);
	}

	static void clear()
	{
		__memset(reinterpret_cast<char *>(VIDEO_MEMORY), ' ', 1000);
	}
};
