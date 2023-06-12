#include "game.h"
#include "graphics.h"
#include "config.h"
#include "Alien.h"
#include "bullet.h"
#include "Enemy.h"
#include "player.h"

//Creates a meteorite
void Game::spawnMeteorite()
{
	if (!meteorite)
	{
		meteorite = new Enemy(*this);
	}
}

//If meteorite is outside the screen -> delete
void Game::checkMeteorite()
{
	if (meteorite && !meteorite->isActive())
	{
		delete meteorite;
		meteorite = nullptr;
	}
}

//If bullets are outside the screen -> delete
void Game::checkBullet()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if ((bullets[i]) && !((bullets[i])->isActive()))
		{
			delete bullets[i];
			bullets[i] = nullptr;
		}
	}
}

//Checks Collision between the player and the meteorite
bool Game::checkCollisionPlayer_1()
{
	if (!player || !meteorite)
	{
		return false;
	}

	Disk d1 = player->getCollisionHull();
	Disk d2 = meteorite->getCollisionHull();

	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
	{
		player->drainLife(0.1f);
		return true;
	}
	else
		return false;
}

//Checks collision between the bullet and the meteorite
bool Game::checkCollisionBullet_1(bullet* shot)
{
	Disk d1 = shot->getCollisionHull();
	Disk d2 = meteorite->getCollisionHull();

	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
	{
		return true;
	}
	else
		return false;
}

//Checks Collision between the player and the alien
bool Game::checkCollisionPlayer_2()
{
	if (!player || !alien)
	{
		return false;
	}

	Disk d1 = player->getCollisionHull();
	Disk d2 = alien->getCollisionHull();

	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
	{
		player->drainLife(0.1f);
		alien->drainLife(0.1f);
		return true;
	}
	else
		return false;
}

//Checks collision between the bullet and the alien
bool Game::checkCollisionBullet_2(bullet* shot)
{
	Disk d1 = shot->getCollisionHull();
	Disk d2 = alien->getCollisionHull();

	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
	{
		alien->drainLife(0.1f);
		return true;
	}
	else
		return false;
}

void Game::updateStartScreen()
{
	graphics::setFont(std::string(ASSET_PATH) + "Death Star.otf");
	graphics::playMusic(std::string(ASSET_PATH) + "Cetus.mp3", 0.1f, true, 4000);
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	if ((graphics::getKeyState(graphics::SCANCODE_RETURN)) || (ms.button_left_pressed))
	{
		status = STATUS_PLAYING_1;
	}
}

void Game::updateLevel_1_Screen()
{
	if (!player_initialized && graphics::getGlobalTime() > 1000)
	{
		player = new Player(*this);
		player_initialized = true;
	}

	if (player)
		player->update();

	checkMeteorite();
	spawnMeteorite();

	graphics::MouseState ms;
	graphics::getMouseState(ms);
	if (ms.button_left_pressed)
	{
		graphics::playSound(std::string(ASSET_PATH) + "SHOOT005.mp3", 0.1f, false);
		shot = new bullet(*this);
		bool found = false;
		int z = 0;
		while ((z < bullets.size()) && (found == false))
		{
			if (bullets[z] == nullptr)
			{
				bullets[z] = shot;
				found = true;
			}
			z++;
		}
		if (!found)
			bullets.push_back(shot);
	}

	if (meteorite)
		meteorite->update();

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i] != nullptr)
			bullets[i]->update();
	}

	checkBullet();

	if (checkCollisionPlayer_1())
	{
		graphics::playSound(std::string(ASSET_PATH) + "explosionCrunch_000.ogg", 0.1f, false);
		delete meteorite;
		meteorite = nullptr;
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i] && meteorite)
		{
			if (checkCollisionBullet_1(bullets[i]))
			{
				graphics::playSound(std::string(ASSET_PATH) + "explosionCrunch_000.ogg", 0.1f, false);
				delete bullets[i];
				bullets[i] = nullptr;
				delete meteorite;
				meteorite = nullptr;
				kills += 1;
				score += 10;
			}
		}
	}

	if (kills == 10)
	{
		kills = 0;
		status = STATUS_PLAYING_2;
	}

	if (player->getRemainingLife() == 0)
	{
		graphics::stopMusic(0);
		graphics::playSound(std::string(ASSET_PATH) + "mixkit-sad-game-over-trombone-471.wav", 0.02, false);
		status = STATUS_END;
	}
}

void Game::updateLevel_2_Screen()
{
	if (player)
		player->update();

	if (!alien)
	{
		alien = new Alien(*this);
	}

	graphics::MouseState ms;
	graphics::getMouseState(ms);
	if (ms.button_left_pressed)
	{
		graphics::playSound(std::string(ASSET_PATH) + "SHOOT005.mp3", 0.1f, false);
		shot = new bullet(*this);
		bool found = false;
		int z = 0;
		while ((z < bullets.size()) && (found == false))
		{
			if (bullets[z] == nullptr)
			{
				bullets[z] = shot;
				found = true;
			}
			z++;
		}
		if (!found)
			bullets.push_back(shot);
	}

	if (alien)
		alien->update();

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i] != nullptr)
			bullets[i]->update();
	}

	checkBullet();

	if (checkCollisionPlayer_2())
	{
		graphics::playSound(std::string(ASSET_PATH) + "explosionCrunch_000.ogg", 0.1f, false);
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i] && alien)
		{
			if (checkCollisionBullet_2(bullets[i]))
			{
				graphics::playSound(std::string(ASSET_PATH) + "explosionCrunch_000.ogg", 0.1f, false);
				delete bullets[i];
				bullets[i] = nullptr;
			}
		}
	}

	if (alien->getRemainingLife() == 0)
	{
		score += 100;
		alien->setReinitialiseLife();
		status = STATUS_PLAYING_1;
	}

	if (player->getRemainingLife() == 0)
	{
		graphics::stopMusic(0);
		graphics::playSound(std::string(ASSET_PATH) + "mixkit-sad-game-over-trombone-471.wav", 0.02, false);
		status = STATUS_END;
	}
}

void Game::updateGameOverScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_R))
	{
		player->setReinitialiseLife();
		status = STATUS_START;
	}
	if (graphics::getKeyState(graphics::SCANCODE_ESCAPE))
	{
		void graphics::destroyWindow();
	}
}

void Game::drawStartScreen()
{
	graphics::Brush br2;
	br2.texture = std::string(ASSET_PATH) + "hi.png";
	br2.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br2);

	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "image5.png";
	br.outline_opacity = 0.0f;

	char info[80];
	sprintf_s(info, "Click or Press ENTER to start");
	graphics::drawText(CANVAS_WIDTH / 2 - 410, CANVAS_HEIGHT / 2 + 170, 50, info, br);

	sprintf_s(info, "Move the mouse to navigate and Click to shoot");
	graphics::drawText(CANVAS_WIDTH / 2 - 250, CANVAS_HEIGHT / 2 + 200, 20, info, br);

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	graphics::drawDisk(window2canvasX(ms.cur_pos_x), window2canvasY(ms.prev_pos_y), 10, br);
}

void Game::drawLevel_1_Screen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "background2.png";
	br.outline_opacity = 0.0f;

	// draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br);

	// draw player
	if (player)
		player->draw();

	if (meteorite)
		meteorite->draw();

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i])
			bullets[i]->draw();
	}

	// UI/info layer
	if (player && debug_mode)
	{
		char info[40];
		sprintf_s(info, "(%6.2f, %6.2f)", player->getPosX(), player->getPosY());
		graphics::drawText(30, 30, 20, info, br);
	}

	char info[40];
	sprintf_s(info, "Score: %d", score);
	graphics::drawText(CANVAS_WIDTH / 2, CANVAS_HEIGHT - 450, 40, info, br);

	float player_life = player ? player->getRemainingLife() : 0.0f;

	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[1] = 0.2f;
	br.fill_secondary_color[2] = 0.2f * (1.0f - player_life) + player_life * 1.0f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100 - ((1.0f - player_life) * 120 / 2), 30, player_life * 120, 20, br);

	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100, 30, 120, 20, br);
}

void Game::drawLevel_2_Screen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "background2.png";
	br.outline_opacity = 0.0f;

	// draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br);

	// draw player
	if (player)
		player->draw();

	if (alien)
		alien->draw();

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i])
			bullets[i]->draw();
	}

	// UI/info layer
	if (player && debug_mode)
	{
		char info[40];
		sprintf_s(info, "(%6.2f, %6.2f)", player->getPosX(), player->getPosY());
		graphics::drawText(30, 30, 20, info, br);
	}

	char info[40];
	sprintf_s(info, "Score: %d", score);
	graphics::drawText(CANVAS_WIDTH / 2, CANVAS_HEIGHT - 450, 40, info, br);

	float player_life = player ? player->getRemainingLife() : 0.0f;

	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[1] = 0.2f;
	br.fill_secondary_color[2] = 0.2f * (1.0f - player_life) + player_life * 1.0f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100 - ((1.0f - player_life) * 120 / 2), 30, player_life * 120, 20, br);

	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100, 30, 120, 20, br);

	float alien_life = alien ? alien->getRemainingLife() : 0.0f;

	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f * (1.0f - alien_life) + alien_life * 0.2f;
	br.fill_secondary_color[1] = 0.2f;
	br.fill_secondary_color[2] = 0.2f * (1.0f - alien_life) + alien_life * 1.0f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100 - ((1.0f - alien_life) * 120 / 2), 80, alien_life * 120, 20, br);

	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100, 80, 120, 20, br);
}

void Game::drawGameOverScreen()
{
	graphics::Brush br;

	char info[40];
	sprintf_s(info, "GAME OVER");
	graphics::drawText(CANVAS_WIDTH / 2 - 125, CANVAS_HEIGHT / 2, 40, info, br);

	sprintf_s(info, "Press R to restart or Esc to exit");
	graphics::drawText(CANVAS_WIDTH / 2 - 215, CANVAS_HEIGHT / 2 + 30, 25, info, br);
}

void Game::update()
{
	if (status == STATUS_START)
	{
		updateStartScreen();
	}
	else if (status == STATUS_PLAYING_1)
	{
		updateLevel_1_Screen();
	}
	else if (status == STATUS_PLAYING_2)
	{
		updateLevel_2_Screen();
	}
	else
	{
		updateGameOverScreen();
	}
}

void Game::draw()
{
	if (status == STATUS_START)
	{
		drawStartScreen();
	}
	else if (status == STATUS_PLAYING_1)
	{
		drawLevel_1_Screen();
	}
	else if (status == STATUS_PLAYING_2)
	{
		drawLevel_2_Screen();
	}
	else
	{
		drawGameOverScreen();
	}
}

void Game::init()
{
	graphics::setFont(std::string(ASSET_PATH) + "Death Star.otf");
}

float Game::window2canvasX(float x)
{
	return x * CANVAS_WIDTH / (float)window_width;
}

float Game::window2canvasY(float y)
{
	return y * CANVAS_HEIGHT / (float)window_height;
}

Game::Game()
{
}

Game::~Game()
{
	if (player)
	{
		delete player;
	}
}
