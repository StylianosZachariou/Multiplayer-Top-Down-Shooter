#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "BulletManager.h"
#include "GUI.h"

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

	bool updateP(float dt);
	void handleInputMove(float dt);
	std::vector<Bullet*> handleInputShoot(float dt, sf::View* view,float gameTime);
	void collisionResponse(GameObject*,float dt);
	void init(int playerNum, Input* in, BulletManager* theBulletManager, sf::Vector2f spawnPosition);
	void healthDecrease(int damage);
	int getHealth();


	void addMessage(PositionMessagesType ppmt);
	sf::Vector2f predict(float time);

	int getId();
	GUI gui;
	
	Bullet* homingBullet;
protected:

	int health = 30;
	sf::Texture texture;

	Animation walk;
	Animation stop;
	Animation dead;
	Animation* currentAnimation;
	
	BulletManager* bulletManager;

	float maxHomingCounter = 10;
	float normalBulletCountdown = 1;
	float homingBulletCountdown = maxHomingCounter;
	int id;

	std::vector<PositionMessagesType> playerPositionMessages;
	std::vector<PositionMessagesType> playerPredictions;
};

