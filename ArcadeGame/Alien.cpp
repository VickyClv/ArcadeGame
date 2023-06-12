#include "Alien.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"

void Alien::update()
{
	if (pos_x > CANVAS_WIDTH * 0.65f)
		pos_x -= speed * graphics::getDeltaTime();
	else
	{
		if (upward_movement == true)
		{
			pos_y += speed * graphics::getDeltaTime();
			if (pos_y > CANVAS_HEIGHT / 1.25f)	//pos_y>400
				upward_movement = false;
		}
		else
		{
			pos_y -= speed * graphics::getDeltaTime();
			if (pos_y < CANVAS_HEIGHT / 5.0f)	//pos_x<100
				upward_movement = true;
		}
	}
}

void Alien::draw()
{
	brush.texture = std::string(ASSET_PATH) + "alien_shadow.png";
	brush.fill_opacity = 0.5f;
	graphics::drawRect(pos_x - 10, pos_y, size + 30, size, brush);
	brush.texture = std::string(ASSET_PATH) + "enemy1.png";
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

void Alien::init()
{
	speed = 0.2f;
	pos_x = CANVAS_WIDTH + 1.1f * size;
	pos_y = CANVAS_HEIGHT / 2;
	size = 150;
	brush.outline_opacity = 0.0f;
}

Alien::Alien(const Game& mygame)
	: GameObject(mygame)
{
	init();
}

Alien::~Alien()
{
}

Disk Alien::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.31f;
	return disk;
}
