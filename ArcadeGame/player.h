#pragma once
#include "gameobject.h"
#include "config.h"

class Player : public GameObject, public Collidable
{
	float speed = 10.0f;
	float pos_x = CANVAS_WIDTH / 2, pos_y = CANVAS_HEIGHT / 2;
	float life = 1.0f;

public:
	Player(const class Game& mygame);
	void update() override;
	void draw() override;
	void init() override;
	float getPosY() { return pos_y; }
	float getPosX() { return pos_x; }
	Disk getCollisionHull() const override;
	float getRemainingLife() const { return life; }
	void setReinitialiseLife() { life = 1.0f; }
	void drainLife(float amount) { life = std::max<float>(0.0f, life - amount); }
};
