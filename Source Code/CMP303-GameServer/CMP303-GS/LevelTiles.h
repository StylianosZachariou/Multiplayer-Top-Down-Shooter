#pragma once
#include "Framework/TileMap.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class LevelTiles
{
public:
	LevelTiles();
	~LevelTiles();

	void render(sf::RenderWindow*);
	int collision(GameObject* collisionTile);
	void changeObjectStatus(int tileMapPosition, int newStatus);

	TileMap tileMap;
private:
	void initBlocks();
	void initMap();
	std::vector<GameObject> tiles;
};

