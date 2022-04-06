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

	int spawn(int id, BulletType bulletType,sf::Vector2f velocity,sf::Vector2f position, float currentTime, float spawnTime);
	void update(float dt, float currentTime);

	bool checkAlive(Bullet* bullet);

	float getBuletSize(BulletType type);

	void setMaxPos(sf::Vector2f newMaxPos);

	std::pair<bool,std::vector<ProjectilesSpawnUpdate>> collisionCheck( GameObject* entity, float gameTime);
	int collisionPlayerCheck(Player* player);

	void addMessage(PositionMessagesType pmt,int id);

	Bullet* getLastBullet();
	Bullet* getBullet(int pos);
private:
	std::vector<ProjectilesSpawnUpdate> homingBurst(Bullet bullet, float gameTime);

	std::vector<Bullet*> bullets;
	float bulletSize = 32;
	float rocketSize = 50;
	sf::Vector2f maxPos;
};

