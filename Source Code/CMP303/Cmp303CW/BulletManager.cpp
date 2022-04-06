#include "BulletManager.h"

BulletManager::BulletManager()
{

}

BulletManager::~BulletManager()
{

}

int BulletManager::spawn(int id, BulletType bulletType, sf::Vector2f velocity, sf::Vector2f position,float gameTime)
{
	bool found = false;
	for (int i = 0; i < bullets.size(); i++)
	{
		if(!bullets[i]->isAlive())
		{ 
			Bullet* newBullet;
			newBullet = new Bullet(id, bulletType, velocity, position,gameTime);
			bullets[i] = newBullet;
			found = true;
			return i;
		}
	}

	if (!found)
	{
		Bullet* newBullet;
		newBullet = new Bullet(id, bulletType, velocity, position,gameTime);
		bullets.push_back(newBullet);
	}
	return -1;
}

sf::Vector2f BulletManager::update(float dt,sf::Vector2f mousePos, float id, float currentTime)
{
	bool homing = false;
	bool alive=true;
	int bullet;

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i] != NULL)
		{
			homing = bullets[i]->update(dt, mousePos, id, currentTime);
			if (homing)
			{
				bullet = i;
			}
			alive = checkAlive(bullets[i],id);
		}
	}

	if (!bullets.empty())
	{
		if (!bullets.back()->isAlive())
		{
			bullets.pop_back();
		}
	}

	if (homing)
	{
		return bullets[bullet]->getPosition();
	}
	else
	{
		return { -1,-1 };
	}
}

void BulletManager::render(sf::RenderWindow* window)//This function renders all alive bullets
{
	for (int i = 0; i < bullets.size(); i++) //Go through all 3 bullets
	{
		if (bullets[i] != NULL) //If bullet is alive
		{
			if (bullets[i]->isAlive())
			{
				window->draw(*bullets[i]); //render it
				
			}
		}
	}
}

bool BulletManager::checkAlive(Bullet* bullet,int id)
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

float BulletManager::getBuletSize(BulletType type)
{
	if (type == BulletType::NORMAL)
	{
		return bulletSize;
	}
	else
	{
		return rocketSize;
	}
}

void BulletManager::setMaxPos(sf::Vector2f newMaxPos)
{
	maxPos = newMaxPos;
}

bool BulletManager::collisionCheck(int id,GameObject* entity)
{
	for (int i = 0; i < bullets.size(); i++)//Go through all bullets
	{
		if (bullets[i]->isAlive())//If bullet is alive
		{
			
			if (Collision::checkBoundingBox(bullets[i], entity))//If bullet collides with entity
			{
				bullets[i]->setAlive(false);//kill it
				if (bullets[i]->getId() == id)
				{
					return true;
				}
			}
			
		}
	}
	return false;
}

int BulletManager::collisionPlayerCheck(int id,Player* player)
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

bool BulletManager::findBullet(ProjectilesSpawnUpdate psu)
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (psu.type == BulletType::NORMAL)
		{
			if (bullets[i]->getId() == psu.id && bullets[i]->getType() == psu.type && bullets[i]->getVelocity() == sf::Vector2f(psu.velocity.x * 600, psu.velocity.y * 600) && bullets[i]->getTimeSpawned() == psu.gameTime)
			{
				return true;
			}
		}
		else
		{
			if (bullets[i]->getId() == psu.id && bullets[i]->getType() == psu.type)
			{
				return true;
			}
		}
	}
	return false;
}
