#pragma once
#include "gameobject.h"
#include "graphics.h"
//#include "game.h"

class Alien : public GameObject, public Collidable
{
	float pos_x, pos_y;
	float speed;
	float size;
	graphics::Brush brush;
	float life = 1.0f;
	bool upward_movement = true;
public:
	void update() override;
	void draw() override;
	void init() override;
	Alien(const class Game& mygame);
	~Alien();
	Disk getCollisionHull() const override;
	float getRemainingLife() const { return life; }
	void setReinitialiseLife() { life = 1.0f; }
	void drainLife(float amount) { life = std::max<float>(0.0f, life - amount); }
};