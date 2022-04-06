#pragma once

#include <SFML/Graphics.hpp>
#include <SFML\Network.hpp>
#include "Player.h"
#include "BulletManager.h"
#include "LevelTiles.h"
#include "NetworkStructures.h"

//Level Class Responsible 
class Level {
public:

	Level();

	//Functions for "Game Loop"
	void update(float dt,float gameTime);
	void applyTcpUpdates(int id,TCPUpdates newUpdate, float gameTime);
	void applyUdpUpdates(int id,UDPUpdates);

	void spawnPlayer(int id, sf::Vector2f spawnPosition);

	sf::Packet packageTcp();
	sf::Packet packageUdp(float gameTime);

	std::vector<ProjectilesSpawnUpdate> newBullets;
	std::vector<HitUpdate> newHit;
	std::vector<ObjectUpdate> newChangedObject;

private:
	//Collisions
	void collisions(float dt,float gameTime);

	std::vector<Player*> players;
	sf::Vector2f minPosition, maxPosition;

	//Bullet Manager
	BulletManager bulletManager;

	//Tiles
	LevelTiles levelTiles;
};

