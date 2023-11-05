#pragma once

#include <cstdint>

namespace Utils
{
	volatile uint8_t &memory(const uint16_t loc)
	{
		return *reinterpret_cast<uint8_t *>(loc);
	}

	constexpr bool testBit(uint8_t data, const uint8_t bit)
	{
		return (data & (1 << bit)) != 0;
	}

	void setBit(volatile uint8_t& byte, const uint8_t bitnum, bool val)
	{
		if (val)
		{
			byte |= (1 << bitnum);
		}
		else
		{
			byte &= (0xFF ^ (1 << bitnum));
		}
	}
}
