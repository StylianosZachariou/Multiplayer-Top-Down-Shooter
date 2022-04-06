#pragma once

#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "BulletManager.h"

struct PositionMessagesType
{
	sf::Vector2f pos;
	float gameTime;
};

class BulletManager;
class Bullet;

class Player: public GameObject
{
public:
	Player();
	~Player();

	void update(float dt,float gameTime) ;
	void collisionResponse(GameObject*,float dt);
	void init(int playerNum, BulletManager* theBulletManager, sf::Vector2f spawnPosition);
	void healthDecrease(int damage);
	int getHealth();

	void addMessage(PositionMessagesType ppmt);
	sf::Vector2f predict(float time);

	int getId();

	Bullet* homingBullet;
protected:

	int health = 30;

	BulletManager* bulletManager;

	float maxHomingCounter = 10;
	float normalBulletCountdown = 1;
	float homingBulletCountdown = maxHomingCounter;
	int id;
	std::vector<PositionMessagesType> playerPositionMessages;
	std::vector<PositionMessagesType> playerPredictions;
};

