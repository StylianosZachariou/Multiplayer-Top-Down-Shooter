#pragma once
#include "SFML\Graphics.hpp"
#include "Bullet.h"

enum class TcpUpdateTypes { BULLETSPAWN, OBJECT, DEATH, NOTHING };
enum class BulletType;
//UDP
struct PlayerPositionUpdate
{
	sf::Vector2f playerPosition;
};

struct HomingPositionUpdate
{
	sf::Vector2f bulletPosition;
};

//UDP UPDATES
struct UDPUpdates
{
	PlayerPositionUpdate ppUpdate;
	HomingPositionUpdate hpUpdate;
	float gameTime;
};

//TCP
struct ProjectilesSpawnUpdate
{
	int id;
	BulletType type;
	sf::Vector2f spawnPosition;
	sf::Vector2f velocity;
	float gameTime;
};

struct ObjectUpdate
{
	int tileMapObjectPosition;
	int objectStatus;
};

struct HitUpdate
{
	int id;
	int damage;
};

//TCP UPDATES
struct TCPUpdates
{
	std::vector<ProjectilesSpawnUpdate> psUpdates;
	std::vector < ObjectUpdate > oUpdate;
	std::vector < HitUpdate > hUpdate;
};