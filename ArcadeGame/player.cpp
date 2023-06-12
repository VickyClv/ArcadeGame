#include "player.h"
#include "graphics.h"
#include "game.h"

Player::Player(const Game& mygame)
	: GameObject(mygame)
{
}

void Player::update()
{
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	pos_x = (float)ms.cur_pos_x;
	pos_y = (float)ms.cur_pos_y;

	if (pos_x < 0) pos_x = 0;
	if (pos_x > CANVAS_WIDTH) pos_x = CANVAS_WIDTH;
	if (pos_y < 0) pos_y = 0;
	if (pos_y > CANVAS_HEIGHT) pos_y = CANVAS_HEIGHT;
}

void Player::draw()
{
	float glow = 0.5f + 0.5 * sinf(graphics::getGlobalTime() / 10);

	graphics::Brush br;
	br.outline_opacity = 0.0f;

	graphics::setOrientation(-90.f);
	br.fill_opacity = 0.5f;
	br.texture = std::string(ASSET_PATH) + "shadow1.png";
	graphics::drawRect(pos_x - 10, pos_y + 30 * 2, 80, 80, br);

	br.fill_opacity = 1.0f;
	br.texture = std::string(ASSET_PATH) + "ship1.png";
	graphics::drawRect(pos_x, pos_y, 80, 80, br);
	graphics::resetPose();

	br.texture = "";
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.5f + glow * 0.5f;
	br.fill_color[2] = 0.0f;
	br.fill_secondary_color[0] = 0.3f;
	br.fill_secondary_color[1] = 0.1f;
	br.fill_secondary_color[2] = 0.0f;
	br.fill_opacity = 1.0f;
	br.fill_secondary_opacity = 0.0f;
	br.gradient = true;
	graphics::drawDisk(pos_x - 30, pos_y, 20, br);
	graphics::resetPose();

	if (game.getDebugMode())
	{
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 0.3f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;
		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
	}
}

void Player::init()
{
}

Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = 55.0f;
	return disk;
}
