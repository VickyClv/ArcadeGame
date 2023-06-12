#pragma once

#include "player.h"
#include "Enemy.h"
#include "bullet.h"
#include "Alien.h"
#include <vector>

class Game
{
	typedef enum { STATUS_START, STATUS_PLAYING_1, STATUS_PLAYING_2, STATUS_END } status_t;
	int score = 0;
	int kills = 0;
	Player* player = nullptr;
	bool player_initialized = false;
	bool debug_mode = false;
	Enemy* meteorite = nullptr;
	bullet* shot = nullptr;
	std::vector<bullet*> bullets;
	Alien* alien = nullptr;
	void spawnMeteorite();
	void checkMeteorite();
	void checkBullet();
	bool checkCollisionPlayer_1();
	bool checkCollisionBullet_1(bullet* shot);
	bool checkCollisionPlayer_2();
	bool checkCollisionBullet_2(bullet* shot);
	status_t status = STATUS_START;

	unsigned int window_width = WINDOW_WIDTH,
		window_height = WINDOW_HEIGHT;

	void updateStartScreen();
	void updateLevel_1_Screen();
	void updateLevel_2_Screen();
	void updateGameOverScreen();
	void drawStartScreen();
	void drawLevel_1_Screen();
	void drawLevel_2_Screen();
	void drawGameOverScreen();

public:
	void setDebugMode(bool d) { debug_mode = d; }
	bool getDebugMode() const { return debug_mode; }
	void update();
	void draw();
	void init();
	unsigned int getWindowWidth() { return window_width; }
	unsigned int getWindowHeight() { return window_height; }
	float window2canvasX(float x);
	float window2canvasY(float y);
	void setWindowDimentions(unsigned int w, unsigned int h) { window_width = w; window_height = h; }
	Game();
	~Game();
};
