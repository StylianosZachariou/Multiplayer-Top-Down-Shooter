#pragma once

#include<math.h>
#include<vector>
#include "Bullet.h"
#include "Framework/Collision.h"
#include "Framework/AudioManager.h"
#include "Player.h"
#include "NetworkStructures.h"

class Player;
struct PositionMessagesType;
enum class BulletType;
class Bullet;
class BulletManager
{

public:
	BulletManager();
	~BulletManager();

	int spawn(int id, BulletType bulletType,sf::Vector2f velocity,sf::Vector2f position,float gameTime);

	sf::Vector2f update(float dt, sf::Vector2f mousePos, float id, float currentTime);

	void render(sf::RenderWindow* window);
	bool checkAlive(Bullet* bullet, int id);

	float getBuletSize(BulletType type);

	void setMaxPos(sf::Vector2f newMaxPos);

	bool collisionCheck(int id, GameObject* entity);
	int collisionPlayerCheck(int id,Player* player);

	void addMessage(PositionMessagesType pmt,int id);

	Bullet* getLastBullet();
	Bullet* getBullet(int pos);

	bool findBullet(ProjectilesSpawnUpdate psu);
private:

	std::vector<Bullet*> bullets;
	float bulletSize = 32;
	float rocketSize = 50;
	sf::Vector2f maxPos;
};

