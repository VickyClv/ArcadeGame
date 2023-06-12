#include "bullet.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"

void bullet::update()
{
	pos_x += speed * graphics::getDeltaTime();

	if (pos_x > CANVAS_WIDTH + size)
	{
		active = false;
	}
}

void bullet::draw()
{
	brush.texture = std::string(ASSET_PATH) + "bullet1.png";
	brush.fill_opacity = 1.0f;
	graphics::drawRect(pos_x, pos_y, size, size, brush);

	if (game.getDebugMode())
	{
		graphics::Brush br;
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.3f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;
		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
	}
}

void bullet::init()
{
	speed = 0.3f;
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	pos_x = (float)ms.cur_pos_x;
	pos_y = (float)ms.cur_pos_y;
	size = 100;
	brush.outline_opacity = 0.0f;
}

bullet::bullet(const Game& mygame)
	: GameObject(mygame)
{
	init();
}
bullet::~bullet()
{
}

Disk bullet::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.31f;
	return disk;
}