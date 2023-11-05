#include "Vec2.h"
#include "Joystick.h"
#include "Player.h"
#include "Screen.h"
#include "Sprites.h"

#include "c64.h"

#include <cstdint>
#include <utility>

namespace
{
	auto frame(Player &p1, Player &p2)
	{
		struct Frame
		{
			Frame(Player &p1, Player &p2)
				: player1(p1), player2(p2)
			{
				while (VIC.rasterline != 250)
				{
				}
			}

			~Frame()
			{
				Screen::display(15, 0) = player1.score;
				Screen::display(24, 0) = player2.score;
			}

			Player &player1;
			Player &player2;
		};

		return Frame(p1, p2);
	}
}

int main()
{
	/// The ball image.
	static const Sprites::HighResSprite sBall{{0b000000000000000000000000,
											   0b000000000000000000000000,
											   0b000000000000110000000000,
											   0b000000000001100000000000,
											   0b000000000011100000000000,
											   0b000000011111111100000000,
											   0b000000101111111010000000,
											   0b000001100111110011000000,
											   0b000011111110111111100000,
											   0b000011011100011101100000,
											   0b000011001111111001100000,
											   0b000011000000000001100000,
											   0b000001100000000011000000,
											   0b000000110000000110000000,
											   0b000000011111111100000000,
											   0b000000000111110000000000,
											   0b000000000000000000000000,
											   0b000000000000000000000000,
											   0b000000000000000000000000,
											   0b000000000000000000000000,
											   0b000000000000000000000000}};

	/// The bat image.
	static const Sprites::MultiColorSprite sBat = {{0x000002200000,
													0x000002200000,
													0x000002200000,
													0x000002200000,
													0x000002200000,
													0x000002200000,
													0x000002200000,
													0x000002200000,
													0x000002200000,
													0x000002200000,
													0x000002200000,
													0x000011110000,
													0x000001100000,
													0x000003300000,
													0x000003300000,
													0x000001110000,
													0x000001110000,
													0x000011110000,
													0x000011010000,
													0x000110100000,
													0x000000000000}};

	Sprites::activate(0, sBall, true, false, false);
	Sprites::activate(1, sBat, false, false, true);
	Sprites::activate(2, sBat, false, false, true);
	VIC.bordercolor = COLOR_GRAY1;
	VIC.bgcolor0 = COLOR_BLACK;
	VIC.spr0_color = COLOR_YELLOW;
	VIC.spr1_color = COLOR_GREEN;
	VIC.spr2_color = COLOR_PURPLE;
	VIC.spr_mcolor0 = COLOR_YELLOW;
	VIC.spr_mcolor1 = COLOR_RED;
	Screen::clear();

	struct Ball
	{
		Vec2<int8_t> velocity{-1, 1};
		Vec2<uint16_t, uint8_t> pos{255 / 2, 255 / 2};
		void reset(){
			// resets position but keeps velocity
			pos = makeVec2(255 / 2, 255 / 2);
			Sprites::setPos(0, pos);
		}
		void updatePosition()
		{
			Sprites::setPos(0, pos);
		}
	};

	Ball ball;
	Player p1(1, {19, 255 / 2});
	Player p2(2, {326, 255 / 2});

	/// Game Loop
	while (true)
	{
		auto frame = ::frame(p1, p2);

		if (const auto collisions = Sprites::collisions();
			collisions.s0 && (collisions.s1 || collisions.s2))
		{
			// ball hit paddle, invert ball x velocity
			ball.velocity *= makeVec2(-1, 1);
			// "bounce" ball out of collision area
			ball.pos.x += (ball.velocity.x * 3);
		}

		// Update paddle positions
		ball.updatePosition();
		p1.updatePosition();
		p2.updatePosition();

		const auto score = [](auto& player, auto& ball)
		{
			// called when a player scores
			player.scored();
			ball.reset();
		};

		if (const auto [ball_x, ball_y] = ball.pos += ball.velocity;
			ball_y == 46 || ball_y == 234)
		{
			// ball hit the top or bottom wall, invert ball y velocity
			ball.velocity *= makeVec2(1, -1);
		}
		else if (ball_x < 22)
		{
			// ball hit left wall, player 2 scored
			score(p2, ball);
		}
		else if (ball_x > 322)
		{
			// ball hit right wall, player 1 scored
			score(p1, ball);
		}
	}
}