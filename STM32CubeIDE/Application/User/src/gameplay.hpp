/*
 * gameplay.hpp
 */

#ifndef APPLICATION_USER_SRC_GAMEPLAY_HPP_
#define APPLICATION_USER_SRC_GAMEPLAY_HPP_

#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <stdint.h>

const uint8_t IS_SHOWN = 1;
const uint8_t SHOULD_SHOW = 2;
const uint8_t IS_HIDDEN = 3;
const uint8_t SHOULD_HIDE = 4;
const uint8_t SHOULD_EXPLODE = 5;

class Entity {
public:
	Entity();
	short coordinateX;
	short coordinateY;
	short width;
	short height;
	short velocityX;
	short velocityY;
	short displayStatus;
	short moveRate;
	short fireRate;
	void updateDisplayStatus(uint8_t status);
	void updateCoordinate(uint16_t x, uint16_t y);
	void updateVelocity(uint16_t x, uint16_t y);
	virtual bool update();
	virtual ~Entity();
	static bool isCollide(Entity &entity1, Entity &entity2);
};

class Ship: public Entity {
public:
	Ship();
	virtual ~Ship();
	bool update(uint8_t dt);
	int MOVE_RATE = 400;
	int FIRE_RATE = 10000;
	int VELOCITY = 10;
	void updateVelocityX(short X);
	void updateVelocityY(short Y);
	void changeShipImage();
	void updateShipHp(short change);
	bool updateBullet(uint8_t dt);
	short lives = 3;
};

const uint8_t MAX_BULLET = 10;

class Bullet: public Entity {
public:
	Bullet();
	const int MOVE_RATE = 800;
	virtual ~Bullet();
	bool update(uint8_t dt);
};

const uint8_t MAX_ENEMY = 30;
const uint8_t MAX_SHOOTING_ENEMIES = 2;

class Enemy: public Entity {
public:
	int MOVE_RATE = 600;
	int FIRE_RATE = 50000;
	Enemy();
	virtual ~Enemy();
	bool update(uint8_t dt);
	bool updateBullet(uint8_t dt);
};

class Boss: public Entity {
public:
	Boss();
	virtual ~Boss();
	void resetBoss();
	bool update(uint8_t dt);
	int MOVE_RATE = 400;
	int FIRE_RATE = 15000;
	int VELOCITY = 1;
	void updateVelocityX(short X);
	void updateVelocityY(short Y);
	void changeBossImage();
	void updateBossHp(short change);
	bool updateBullet(uint8_t dt);
	short lives = 20;
	uint32_t bossSpawnTimer = 0;
};

class Game {
public:
	uint16_t score = 0;
	static uint16_t high_score;
	Ship ship;
	Game();
	virtual ~Game();
	void update();
	void updateScore(short score);
	void updateHighScore(short score);
};

extern Game gameInstance;
extern bool shouldEndGame;
extern bool shouldStopTask;
extern Enemy enemy[MAX_ENEMY];
extern Bullet shipBullet[MAX_BULLET];
extern Bullet enemyBullet[MAX_BULLET];
extern Boss bossInstance;
extern void gameTask(void *argument);

#endif /* APPLICATION_USER_SRC_GAMEPLAY_HPP_ */
