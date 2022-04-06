#include "LevelTiles.h"

LevelTiles::LevelTiles()
{
	initBlocks();
	initMap();

	srand(time(NULL));
}

LevelTiles::~LevelTiles()
{

}

void LevelTiles::render(sf::RenderWindow* window)
{
	tileMap.render(window);
}

int LevelTiles::collision(GameObject* collisionTile)
{
	if (collisionTile->isAlive())
	{
		if ((rand() % 100) < 10)
		{
			if (collisionTile->getTextureRect() == sf::IntRect(0, 96, 32, 32))
			{
				collisionTile->setTextureRect(sf::IntRect(32, 96, 32, 32));
				return 1;
			}
			else
			{
				if (collisionTile->getTextureRect() == sf::IntRect(32, 96, 32, 32))
				{
					collisionTile->setTextureRect(sf::IntRect(64, 96, 32, 32));
					return 2;
				}
				else
				{
					if (collisionTile->getTextureRect() == sf::IntRect(64, 96, 32, 32))
					{
						collisionTile->setTextureRect(sf::IntRect(96, 96, 32, 32));
						return 3;
					}
					else
					{
						if (collisionTile->getTextureRect() == sf::IntRect(96, 96, 32, 32))
						{
							collisionTile->setAlive(false);
							collisionTile->setTextureRect(sf::IntRect(0, 0, 1, 1));
							collisionTile->setCollider(false);
							return 0;
						}
						else
						{
							if (collisionTile->getTextureRect() == sf::IntRect(0, 128, 32, 32))
							{
								collisionTile->setTextureRect(sf::IntRect(32, 128, 32, 32));
								return 1;
							}
							else
							{
								if (collisionTile->getTextureRect() == sf::IntRect(32, 128, 32, 32))
								{
									collisionTile->setTextureRect(sf::IntRect(64, 128, 32, 32));
									return 2;
								}
								else
								{
									if (collisionTile->getTextureRect() == sf::IntRect(64, 128, 32, 32))
									{
										collisionTile->setTextureRect(sf::IntRect(96, 128, 32, 32));
										return 3;
									}
									else
									{
					
										collisionTile->setAlive(false);
										collisionTile->setTextureRect(sf::IntRect(0, 0, 1, 1));
										collisionTile->setCollider(false);
										return 0;
									}
								}
							}

						}
					}
				}
			}
		}
	}
	return -1;
}

void LevelTiles::changeObjectStatus(int tileMapPosition, int newStatus)
{

	std::vector<GameObject>* world = tileMap.getLevel();
	
	if (newStatus == 0)
	{
		(*world)[tileMapPosition].setAlive(false);
		(*world)[tileMapPosition].setTextureRect(sf::IntRect(0, 0, 1, 1));
		(*world)[tileMapPosition].setCollider(false);
	}
	else if (newStatus == 1)
	{
		if ((*world)[tileMapPosition].getTextureRect() == sf::IntRect(0, 96, 32, 32))
		{
			(*world)[tileMapPosition].setTextureRect(sf::IntRect(32, 96, 32, 32));
		}
		else if ((*world)[tileMapPosition].getTextureRect() == sf::IntRect(0, 128, 32, 32))
		{
			(*world)[tileMapPosition].setTextureRect(sf::IntRect(32, 128, 32, 32));
		}
	}
	else if (newStatus == 2)
	{
		if ((*world)[tileMapPosition].getTextureRect() == sf::IntRect(32, 96, 32, 32))
		{
			(*world)[tileMapPosition].setTextureRect(sf::IntRect(64, 96, 32, 32));
		}
		else if ((*world)[tileMapPosition].getTextureRect() == sf::IntRect(32, 128, 32, 32))
		{
			(*world)[tileMapPosition].setTextureRect(sf::IntRect(64, 128, 32, 32));
		}
	}
	else if (newStatus == 3)
	{
		if ((*world)[tileMapPosition].getTextureRect() == sf::IntRect(64, 96, 32, 32))
		{
			(*world)[tileMapPosition].setTextureRect(sf::IntRect(96, 96, 32, 32));
		}
		else if ((*world)[tileMapPosition].getTextureRect() == sf::IntRect(64, 128, 32, 32))
		{
			(*world)[tileMapPosition].setTextureRect(sf::IntRect(96, 128, 32, 32));
		}
	}
}

void LevelTiles::initBlocks()
{
	GameObject tile;

	for (int i = 0; i < 8; i++)
	{
		tile.setSize(sf::Vector2f(128, 128));
		tile.setCollisionBox(0, 0, 128, 128);
		tile.setCollider(true);
		tiles.push_back(tile);
	}

	for (int i = 0; i < 2; i++)
	{
		tile.setSize(sf::Vector2f(128, 128));
		tile.setCollisionBox(0, 0, 96, 128);
		tile.setCollider(true);
		tiles.push_back(tile);
	}

	for (int i = 0; i < 2; i++)
	{
		tile.setSize(sf::Vector2f(128, 128));
		tile.setCollisionBox(32, 32, 96, 128);
		tile.setCollider(true);
		tiles.push_back(tile);
	}

	tile.setSize(sf::Vector2f(128, 128));
	tile.setCollisionBox(32, 32, 64, 128);
	tile.setCollider(true);
	tiles.push_back(tile);

	for (int i = 0; i < 2; i++)
	{
		tile.setSize(sf::Vector2f(128, 128));
		tile.setCollisionBox(0, 0, 128, 128);
		tile.setCollider(true);
		tiles.push_back(tile);
	}

	//Blank
	tiles[0].setCollider(false);
	tiles[0].setTextureRect(sf::IntRect(0, 0, 1, 1));

	//Full Blocks
	tiles[1].setTextureRect(sf::IntRect(0, 0, 32, 32));
	tiles[2].setTextureRect(sf::IntRect(32, 0, 32, 32));
	tiles[3].setTextureRect(sf::IntRect(64, 0, 32, 32));
	tiles[4].setTextureRect(sf::IntRect(96, 0, 32, 32));
	tiles[5].setTextureRect(sf::IntRect(64, 32, 32, 32));
	tiles[6].setTextureRect(sf::IntRect(96, 32, 32, 32));
	tiles[7].setTextureRect(sf::IntRect(64, 64, 32, 32));

	//First Half Blocks
	tiles[8].setTextureRect(sf::IntRect(0, 32, 32, 32));
	tiles[9].setTextureRect(sf::IntRect(32, 32, 32, 32));

	//Second Half Blocks
	tiles[10].setTextureRect(sf::IntRect(0, 64, 32, 32));
	tiles[11].setTextureRect(sf::IntRect(32, 64, 32, 32));

	//Half Half Block
	tiles[12].setTextureRect(sf::IntRect(96, 64, 32, 32));

	//Alive Blocks
	tiles[13].setTextureRect(sf::IntRect(0, 96, 32, 32));
	tiles[13].setAlive(true);
	tiles[14].setTextureRect(sf::IntRect(0, 128, 32, 32));
	tiles[14].setAlive(true);
}

void LevelTiles::initMap()
{
	tileMap.setTileSet(tiles);

	sf::Vector2u mapSize(40, 33);

	std::vector<int> map =
	{
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 03, 07, 07, 07, 07, 07, 07, 07, 07, 05, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 14, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 14, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 12, 00, 14, 00, 00, 00, 00, 14, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 00, 00, 14, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 01, 07, 07, 07, 07, 07, 07, 07, 07, 07, 07, 05, 00, 00,
		00, 12, 00, 14, 00, 00, 00, 00, 14, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 12, 00, 14, 00, 00, 00, 00, 14, 00, 12, 00, 00, 13, 00, 00, 00, 00, 00, 00, 14, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 13, 00, 00, 13, 00, 00, 12, 00, 00,
		00,  9, 00, 00, 00, 00, 00, 00, 00, 00, 11, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 00, 00, 00, 13, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 13, 00, 00, 13, 00, 00, 12, 00, 00,
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 00, 00, 00, 00, 13, 13, 00, 00, 00, 00, 00, 00, 14, 00, 00, 14, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 00, 00, 00, 13, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 13, 00, 00, 13, 00, 00, 12, 00, 00,
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00,  8, 00, 00, 00, 00, 00, 00, 00, 00, 10, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 12, 00, 14, 00, 00, 00, 00, 14, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 13, 00, 00, 13, 00, 00, 12, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 14, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 12, 00, 14, 00, 00, 00, 00, 14, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 14, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 01, 07, 07, 07, 07, 07, 07, 07, 07, 07, 07, 06, 00, 00,
		00, 12, 00, 14, 00, 00, 00, 00, 14, 00, 12, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 12, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 04, 07, 07, 07, 07, 07, 07, 07, 07, 06, 00, 00, 00, 00, 14, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 13, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
	};

	tileMap.setTileMap(map, mapSize);
	tileMap.setPosition(sf::Vector2f(0, 0));
	tileMap.buildLevel();
}
