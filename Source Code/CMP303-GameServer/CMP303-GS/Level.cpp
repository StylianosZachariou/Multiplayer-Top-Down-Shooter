#include "Level.h"
#include "Player.h"

Level::Level()
{
	maxPosition = { 5080,4236 };
	minPosition = { 0,0 };
	
	bulletManager.setMaxPos(maxPosition);

}

//Update
void Level::update(float dt,float gameTime)
{
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->update(dt, gameTime);
	}
	
	bulletManager.update(dt,gameTime);

	//Collisions
	collisions(dt,gameTime);
}

void Level::applyUdpUpdates(int id, UDPUpdates newUpdate)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == id)
		{
			players[i]->addMessage({ newUpdate.ppUpdate.playerPosition,newUpdate.gameTime });
			bulletManager.addMessage({ newUpdate.hpUpdate.bulletPosition,newUpdate.gameTime }, id);
		}
	}
}

void Level::applyTcpUpdates(int id, TCPUpdates newUpdate,float gameTime)
{
	int index;
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == id)
		{
			index = i;
			break;
		}
	}

	for (int i = 0; i < newUpdate.psUpdates.size(); i++)
	{
		int pos = bulletManager.spawn(id, newUpdate.psUpdates[i].type, newUpdate.psUpdates[i].velocity, newUpdate.psUpdates[i].spawnPosition,gameTime,newUpdate.psUpdates[i].gameTime);
		if (pos > -1)
		{
			if (newUpdate.psUpdates[i].type == BulletType::HOMING)
			{
				players[index]->homingBullet = bulletManager.getBullet(pos);
			}
		}
		else
		{
			if (newUpdate.psUpdates[i].type == BulletType::HOMING)
			{
				players[index]->homingBullet = bulletManager.getLastBullet();
			}
		}
		
	}

}

void Level::spawnPlayer(int id,sf::Vector2f spawnPosition)
{
	Player* newPlayer = new Player();
	players.push_back(newPlayer);
	players.back()->init(id, &bulletManager,spawnPosition);
	
}

sf::Packet Level::packageTcp()
{
	sf::Packet packet;
	packet << int(newHit.size()) << int(newBullets.size()) << int(newChangedObject.size());

	for (int i = (newHit.size() - 1); i >= 0; i--)
	{
		packet << newHit[i].id << newHit[i].damage;
		newHit.pop_back();
	}

	for (int i = (newBullets.size() - 1); i >= 0; i--)
	{
		packet << newBullets[i].id << int(newBullets[i].type) << newBullets[i].spawnPosition.x << newBullets[i].spawnPosition.y << newBullets[i].velocity.x << newBullets[i].velocity.y<<newBullets[i].gameTime;
		newBullets.pop_back();
	}

	for (int i = (newChangedObject.size() - 1); i >= 0; i--)
	{
		packet << newChangedObject[i].tileMapObjectPosition << newChangedObject[i].objectStatus;
		newChangedObject.pop_back();
	}

	return packet;
}


sf::Packet Level::packageUdp(float gameTime)
{
	sf::Packet packet;
	packet << int(players.size());
	for (int i = 0; i < players.size(); i++)
	{
		sf::Vector2f homingPos;
		if (players[i]->homingBullet == NULL)
		{
			homingPos = { -1,-1 };
		}
		else
		{
			homingPos = players[i]->homingBullet->getPosition();
		}
		packet << players[i]->getId() << players[i]->getPosition().x << players[i]->getPosition().y << homingPos.x << homingPos.y << gameTime;
	}
	return packet;

}

void Level::collisions(float dt,float gameTime)
{

	//COLLISIONS
	//Check collisions with tiles
	std::vector<GameObject>* world = levelTiles.tileMap.getLevel();

	for (int i = 0; i < (int)world->size(); i++)
	{
		if ((*world)[i].isCollider())
		{
			for (int i = 0; i < players.size(); i++)
			{
				if (Collision::checkBoundingBox(players[i], &(*world)[i]))//Collision tiles with player
				{
					players[i]->collisionResponse(&(*world)[i], dt);
				}
			}

			std::pair<bool, std::vector<ProjectilesSpawnUpdate>> result;
			result = bulletManager.collisionCheck(&(*world)[i], gameTime);
			if (result.first)//Collision tiles and  bullets
			{

				int objectStatus = levelTiles.collision(&(*world)[i]);
				if (objectStatus != -1)
				{
					ObjectUpdate update;
					update.objectStatus = objectStatus;
					update.tileMapObjectPosition = i;
					newChangedObject.push_back(update);
				}

				for (int i = 0; i < result.second.size(); i++)
				{
					newBullets.push_back(result.second[i]);
				}
			}
			
		}

	}

	//Bullets
	for (int i = 0; i < players.size(); i++)
	{
		int damage = bulletManager.collisionPlayerCheck(players[i]);
		if (damage != 0)
		{
			players[i]->healthDecrease(damage);
			HitUpdate hit;
			hit.damage = damage;
			hit.id = players[i]->getId();
			newHit.push_back(hit);
		}
	}

}