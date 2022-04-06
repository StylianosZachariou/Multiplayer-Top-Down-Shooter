#pragma once

#include <SFML/Graphics.hpp>
#include <SFML\Network.hpp>
#include "Framework/Input.h"
#include "Player.h"
#include "Background.h"
#include "BulletManager.h"
#include "LevelTiles.h"
#include "NetworkStructures.h"

//Level Class Responsible 
class Level {
public:

	Level(sf::RenderWindow* hwnd, Input* in,int playerId);

	//Functions for "Game Loop"
	void handleInput(float dt,float gameTime);
	void update(float dt,float gameTime);
	void render();

	//Networking
	sf::Packet packageUdp(float gameTime);
	sf::Packet packageTcp();

	void applyTcpUpdates(TCPUpdates);
	void applyUdpUpdates(int id, UDPUpdates);

	std::vector<ProjectilesSpawnUpdate> newBullets;
	void spawnPlayer(int id, sf::Vector2f spawnPosition);
	void setUpGui();
private:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	//Collisions
	void collisions(float dt);

	// Default variables for level class.
	sf::RenderWindow* window;
	Input* input;

	std::vector<Player*> players;
	sf::Vector2f minPosition, maxPosition;

	//Texture
	Backround background;
	sf::Texture backgroundTexture;
	sf::View view;

	//Bullet Manager
	BulletManager bulletManager;

	//Tiles
	LevelTiles levelTiles;

	//ID
	int playerID;
	int playerIndex;

	sf::Vector2f homingBulletPos;
};

