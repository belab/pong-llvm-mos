#pragma once

#include "Utils.h"
#include "Vec2.h"

#include "c64.h"

#include <cstdint>

struct Sprites
{
	static constexpr uint8_t SPRITE_ALIGNMENT = 64;
	static constexpr uint16_t SPRITE_DATA_POINTERS = 2040;

	template <bool multicolor>
	struct SpriteLine
	{
		uint8_t pixels[3];

		typedef uint_least64_t numeric_t;

		/// Pixel format converter.
		static constexpr uint8_t c(const numeric_t m)
		{
			if constexpr (multicolor)
				// 0b00ii00jj00kk00ll -> 0biijjkkll
				return uint8_t(m >> 0 & 3 << 0) |
					   uint8_t(m >> 2 & 3 << 2) |
					   uint8_t(m >> 4 & 3 << 4) |
					   uint8_t(m >> 6 & 3 << 6);
			return uint8_t(m);
		}

		/// Input bits per byte.
		constexpr static auto ibb = multicolor ? 16 : 8;

		/// Constructor.
		constexpr SpriteLine(const numeric_t line)
			: pixels{c(line >> (2 * ibb)), c(line >> ibb), c(line)}
		{
		}
	};

	template <bool multicolor>
	struct Sprite
	{
		typedef SpriteLine<multicolor> Line;
		alignas(SPRITE_ALIGNMENT) Line lines[SPRITE_ALIGNMENT / sizeof(Line)];
	};

	typedef Sprite<false> HighResSprite;
	typedef Sprite<true> MultiColorSprite;

	template <bool multicolor>
	static void activate(const uint8_t sprite_number,
						 const Sprite<multicolor> &bitmap,
						 const bool low_priority,
						 const bool double_width, const bool double_height)
	{
		Utils::memory(SPRITE_DATA_POINTERS + sprite_number) = (std::ptrdiff_t(bitmap.lines) & 0x3fff) / SPRITE_ALIGNMENT;
		Utils::setBit(VIC.spr_ena, sprite_number, true);
		Utils::setBit(VIC.spr_exp_x, sprite_number, double_width);
		Utils::setBit(VIC.spr_exp_y, sprite_number, double_height);
		Utils::setBit(VIC.spr_mcolor, sprite_number, multicolor);
		Utils::setBit(VIC.spr_bg_prio, sprite_number, low_priority);
	}

	struct Collisions
	{
		bool s0 = false, s1 = false, s2 = false, s3 = false, s4 = false, s5 = false, s6 = false, s7 = false;
	};

	static auto collisions()
	{
		const auto collisions = VIC.spr_coll;
		return Collisions{
			Utils::testBit(collisions, 0), Utils::testBit(collisions, 1), Utils::testBit(collisions, 2),
			Utils::testBit(collisions, 3), Utils::testBit(collisions, 4), Utils::testBit(collisions, 5),
			Utils::testBit(collisions, 6), Utils::testBit(collisions, 7)};
	}

	static void setPos(const uint8_t sprite_num, const Vec2<uint16_t, uint8_t>& pos)
	{
		VIC.spr_pos[sprite_num].x = pos.x;
		Utils::setBit(VIC.spr_hi_x, sprite_num, pos.x > 255);
		VIC.spr_pos[sprite_num].y = pos.y;
	}
};
