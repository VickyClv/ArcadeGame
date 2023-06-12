#pragma once
#include "gameobject.h"
#include "graphics.h"

class bullet : public GameObject, public Collidable
{
	float pos_x, pos_y;
	float speed;
	float size;
	graphics::Brush brush;
	bool active = true;
public:
	void update() override;
	void draw() override;
	void init() override;
	bool isActive() { return active; }
	void setPos_X(float x) { pos_x = x; }
	void setPos_Y(float y) { pos_y = y; }
	bullet(const class Game& mygame);
	~bullet();
	Disk getCollisionHull() const override;
};