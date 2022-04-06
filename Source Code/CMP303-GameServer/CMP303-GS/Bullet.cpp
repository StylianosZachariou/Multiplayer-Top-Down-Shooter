#include "Bullet.h"

Bullet::Bullet(int id, BulletType bulletType, sf::Vector2f velocity, sf::Vector2f position)
{
	type = bulletType;

	setId(id);
	setAlive(true);
	setSize(sf::Vector2f(32, 32));

	velocity.x *= 600;
	velocity.y *= 600;
	setPosition(position); 
	setVelocity(velocity);

	if (bulletType == BulletType::NORMAL)
	{
		damage = 1;
	}
	else
	{
		damage = 3;
	}
}

Bullet::~Bullet()
{

}

void Bullet::update(float dt, float gameTime)//This function updates the bullet
{
	//Normal
	if (isAlive())
	{
		
		if (type == BulletType::NORMAL)
		{
			move(velocity * dt);//move bullet

		}
		else//Homing
		{

			setPosition(predict(gameTime));
			if (counter <= 5)
			{
				setRotation(getRotation() + 1);
				counter += 3 * dt;
			}
		}
	}
}

int Bullet::getId()
{
	return id;
}

void Bullet::setId(int newId)
{
	id = newId;
}

BulletType Bullet::getType()
{
	return type;
}

int Bullet::getDamage()
{
	return damage;
}

void Bullet::addMessage(PositionMessagesType ppmt)
{
	homingPositionMessages.push_back(ppmt);
}

sf::Vector2f Bullet::getLastPosition()
{
	return homingPositionMessages[homingPositionMessages.size()-2].pos;
}


sf::Vector2f Bullet::predict(float currentTime)
{
	sf::Vector2f predicted = getPosition();

	if (homingPositionMessages.size() < 2)
	{
		return predicted;
	}

	const PositionMessagesType& msg0 = homingPositionMessages[homingPositionMessages.size() - 1];
	const PositionMessagesType& msg1 = homingPositionMessages[homingPositionMessages.size() - 2];

	//Linear Prediction

	float time = msg0.gameTime - msg1.gameTime;
	float speedX = (msg0.pos.x - msg1.pos.x) / time;
	float speedY = (msg0.pos.y - msg1.pos.y) / time;

	float displacementX = speedX * (currentTime  - msg0.gameTime);
	float displacementY = speedY * (currentTime  - msg0.gameTime);

	predicted = { msg1.pos.x + displacementX, msg1.pos.y + displacementY };

	if (homingPositionMessages.size() > 2)
	{
		homingPositionMessages[0] = homingPositionMessages[1];
		homingPositionMessages[1] = homingPositionMessages[2];
		homingPositionMessages.pop_back();
	}

	if (homingPredictions.size() > 2)
	{
		const PositionMessagesType& pmsg0 = homingPredictions[homingPredictions.size() - 1];
		const PositionMessagesType& pmsg1 = homingPredictions[homingPredictions.size() - 2];

		float ptime = pmsg0.gameTime - pmsg1.gameTime;
		float pspeedX = (pmsg0.pos.x - pmsg1.pos.x) / ptime;
		float pspeedY = (pmsg0.pos.y - pmsg1.pos.y) / ptime;

		float pdisplacementX = pspeedX * (currentTime -  pmsg0.gameTime);
		float pdisplacementY = pspeedY * (currentTime -  pmsg0.gameTime);

		sf::Vector2f ppredicted = { pmsg1.pos.x + pdisplacementX, pmsg1.pos.y + pdisplacementY };

		predicted.x = (ppredicted.x + predicted.x) / 2;
		predicted.y = (ppredicted.y + predicted.y) / 2;
	}

	PositionMessagesType newPrediction;
	newPrediction.pos = predicted;
	newPrediction.gameTime = currentTime;
	homingPredictions.push_back(newPrediction);

	if (homingPredictions.size() > 2)
	{
		homingPredictions[0] = homingPredictions[1];
		homingPredictions[1] = homingPredictions[2];
		homingPredictions.pop_back();
	}

	return predicted;
}
