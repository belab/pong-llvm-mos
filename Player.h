#pragma once

#include "Joystick.h"
#include "Sprites.h"
#include "Vec2.h"

#include <cstdint>

struct Player
{
	Player(const uint8_t num, const Vec2<uint16_t, uint8_t>& start_pos)
		: player_num(num),
		  joyStick(static_cast<JoyStick::Port>(num)),
		  pos(start_pos)
	{
	}

	void updatePosition()
	{
		pos.y += joyStick.direction().y * 3;
		Sprites::setPos(player_num, pos);
	}

	void scored()
	{
		++score;
	}

	JoyStick joyStick;
	const uint8_t player_num;
	Vec2<uint16_t, uint8_t> pos;
	uint8_t score = '0';
};
