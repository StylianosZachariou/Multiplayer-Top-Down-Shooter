#include "Player.h"

Player::Player()
{

}

Player::~Player()
{
}

void Player::update(float dt,float gameTime)
{
	
	if (homingBulletCountdown < maxHomingCounter)
	{
		homingBulletCountdown += dt;
	}
	else if(homingBulletCountdown> maxHomingCounter)
	{
		homingBulletCountdown = maxHomingCounter;
	}

	if (isAlive())
	{
		setPosition(predict(gameTime));
	}
}

void Player::collisionResponse(GameObject* collider,float dt)
{
	sf::Vector2f centre;
	sf::Vector2f playerCentre;

	centre.x = collider->getPosition().x + collider->getCollisionBox().width / 2;
	centre.y = collider->getPosition().y + collider->getCollisionBox().height / 2;

	playerCentre.x = getPosition().x + getCollisionBox().width / 2;
	playerCentre.y = getPosition().y + getCollisionBox().height / 2;

	sf::Vector2f positionDiff;
	positionDiff.x = abs(centre.x - playerCentre.x);
	positionDiff.y = abs(centre.y - playerCentre.y);

	
	if (positionDiff.x >= positionDiff.y)
	{
		if ((centre.x - playerCentre.x) < 0)
		{
			move(velocity.x * dt, 0);
		}
		else
		{
			move(-velocity.x * dt, 0);
		}

	}
	
	if (positionDiff.y > positionDiff.x)
	{
		if ((centre.y - playerCentre.y) < 0)
		{
			move(0,velocity.y * dt);
		}
		else
		{
			move(0,-velocity.y * dt);
		}
	}

}

void Player::init(int playerNum, BulletManager* theBulletManager,sf::Vector2f spawnPosition)
{
	int characterTexture = 32 * playerNum;

	id = playerNum;
	
	setAlive(true);
	setSize(sf::Vector2f(128, 128));
	setCollisionBox(0,0,128,128);
	setPosition(spawnPosition);
	setVelocity(600, 600);

	bulletManager = theBulletManager;
}

void Player::healthDecrease(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		setAlive(false);
	}

}

int Player::getHealth()
{
	return health;
}

void Player::addMessage(PositionMessagesType ppmt)
{
	playerPositionMessages.push_back(ppmt);
}

sf::Vector2f Player::predict(float currentTime)
{
	sf::Vector2f predicted = getPosition();

	if (playerPositionMessages.size() < 2)
	{
		return predicted;
	}

	const PositionMessagesType& msg0 = playerPositionMessages[playerPositionMessages.size() - 1];
	const PositionMessagesType& msg1 = playerPositionMessages[playerPositionMessages.size() - 2];

	//Linear Prediction

	float time = msg0.gameTime - msg1.gameTime;
	float speedX = (msg0.pos.x - msg1.pos.x) / time;
	float speedY = (msg0.pos.y - msg1.pos.y) / time;

	float displacementX = speedX * (currentTime - msg0.gameTime);
	float displacementY = speedY * (currentTime -  msg0.gameTime);

	predicted = { msg1.pos.x + displacementX, msg1.pos.y + displacementY };

	if (playerPositionMessages.size() > 2)
	{
		playerPositionMessages[0] = playerPositionMessages[1];
		playerPositionMessages[1] = playerPositionMessages[2];
		playerPositionMessages.pop_back();
	}


	if (playerPredictions.size() > 2)
	{
		const PositionMessagesType& pmsg0 = playerPredictions[playerPredictions.size() - 1];
		const PositionMessagesType& pmsg1 = playerPredictions[playerPredictions.size() - 2];

		float ptime = pmsg0.gameTime - pmsg1.gameTime;
		float pspeedX = (pmsg0.pos.x - pmsg1.pos.x) / ptime;
		float pspeedY = (pmsg0.pos.y - pmsg1.pos.y) / ptime;

		float pdisplacementX = pspeedX * (currentTime - pmsg0.gameTime);
		float pdisplacementY = pspeedY * (currentTime - pmsg0.gameTime);

		sf::Vector2f ppredicted = { pmsg1.pos.x + pdisplacementX, pmsg1.pos.y + pdisplacementY };

		predicted.x = (ppredicted.x + predicted.x) / 2;
		predicted.y = (ppredicted.y + predicted.y) / 2;

	}

	PositionMessagesType newPrediction;
	newPrediction.pos = predicted;
	newPrediction.gameTime = currentTime;
	playerPredictions.push_back(newPrediction);

	if (playerPredictions.size() > 2)
	{
		playerPredictions[0] = playerPredictions[1];
		playerPredictions[1] = playerPredictions[2];
		playerPredictions.pop_back();
	}

	return predicted;
}

int Player::getId()
{
	return id;
}

