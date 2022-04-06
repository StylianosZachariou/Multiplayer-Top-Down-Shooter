#include "BulletManager.h"

BulletManager::BulletManager()
{

}

BulletManager::~BulletManager()
{

}

int BulletManager::spawn(int id, BulletType bulletType, sf::Vector2f velocity, sf::Vector2f position,float currentTime, float spawnTime)
{
	if (currentTime > spawnTime)
	{
		sf::Vector2f displacement;
		displacement.x = velocity.x * (currentTime - spawnTime);
		displacement.y = velocity.y * (currentTime - spawnTime);
		position += displacement;
	}
	
	bool found = false;
	for (int i = 0; i < bullets.size(); i++)
	{
		if(!bullets[i]->isAlive())
		{ 
			Bullet* newBullet;
			newBullet = new Bullet(id, bulletType, velocity, position);
			bullets[i] = newBullet;
			found = true;
			return i;
		}
	}

	if (!found)
	{
		Bullet* newBullet;
		newBullet = new Bullet(id, bulletType, velocity, position);
		bullets.push_back(newBullet);
	}

	

	return -1;
}

void BulletManager::update(float dt, float currentTime)
{

	int bullet=0;

	for (int i = 0; i < bullets.size(); i++)
	{
		
		bullets[i]->update(dt, currentTime);
		if (!checkAlive(bullets[i]))
		{
			bullets[i]->setAlive(false);
		}
		
	}

	if (!bullets.empty())
	{
		if (!bullets.back()->isAlive())
		{
			bullets.pop_back();
		}
	}
}

bool BulletManager::checkAlive(Bullet* bullet)
{
	bool alive = true;
	if (bullet->getPosition().x > (maxPos.x + 50))
	{
		bullet->setAlive(false);
		alive = false;
	}

	if (bullet->getPosition().y > (maxPos.y + 50))
	{
		bullet->setAlive(false);
		alive = false;
	}

	if (bullet->getPosition().x <-50)
	{
		bullet->setAlive(false); 
		alive = false;
	}

	if (bullet->getPosition().y <-50)
	{
		bullet->setAlive(false);
		alive = false;
	}

	return alive;
}

void BulletManager::setMaxPos(sf::Vector2f newMaxPos)
{
	maxPos = newMaxPos;
}

std::pair<bool, std::vector<ProjectilesSpawnUpdate>> BulletManager::collisionCheck(GameObject* entity,float gameTime)
{
	bool collision=false;
	std::vector<ProjectilesSpawnUpdate> newBullets;
	for (int i = 0; i < bullets.size(); i++)//Go through all bullets
	{
		if (bullets[i]->isAlive())//If bullet is alive
		{
			
			if (Collision::checkBoundingBox(bullets[i], entity))//If bullet collides with entity
			{
				if (bullets[i]->getType() == BulletType::HOMING)
				{
					newBullets = homingBurst(*bullets[i], gameTime);
				}
				
				bullets[i]->setAlive(false);//kill it
				collision = true;
				
			}
			
		}
	}
	return {collision,newBullets};
}

int BulletManager::collisionPlayerCheck(Player* player)
{

	for (int i = 0; i < bullets.size(); i++)//Go through all bullets
	{
		if (bullets[i]->isAlive())//If bullet is alive
		{
			if (player->getId() != bullets[i]->getId())
			{
				if (Collision::checkBoundingBox(bullets[i], player))//If bullet collides with entity
				{
					bullets[i]->setAlive(false);//kill it			
					return bullets[i]->getDamage();
				}
			}
		}
	}
	return 0;
}

void BulletManager::addMessage(PositionMessagesType pmt, int id)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getId() == id && bullets[i]->getType() == BulletType::HOMING)
		{
			bullets[i]->addMessage(pmt);
			break;
		}
	}
}


Bullet* BulletManager::getLastBullet()
{
	return bullets.back();
}

Bullet* BulletManager::getBullet(int pos)
{
	return bullets[pos];
}

std::vector<ProjectilesSpawnUpdate> BulletManager::homingBurst(Bullet bullet,float gameTime)
{
	std::vector<ProjectilesSpawnUpdate> newBullets;
	for (int i = 0; i < 18; i++)
	{
		sf::Vector2f spawnVelocity;
		float angle = 0;
		angle += (360 / 18) * 0.0174533 * i;

		spawnVelocity.x = cos(angle) - sin(angle);
		spawnVelocity.y = cos(angle) + sin(angle);
		spawn(bullet.getId(), BulletType::NORMAL, spawnVelocity, bullet.getLastPosition(), gameTime ,gameTime);

		ProjectilesSpawnUpdate newBullet;
		newBullet.gameTime = gameTime;
		newBullet.id = bullet.getId();
		newBullet.spawnPosition = bullet.getLastPosition();
		newBullet.type = BulletType::NORMAL;
		newBullet.velocity = spawnVelocity;
		newBullets.push_back(newBullet);
	}
	return newBullets;
}
