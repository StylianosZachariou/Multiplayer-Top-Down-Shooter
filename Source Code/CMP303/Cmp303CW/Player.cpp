#include "Player.h"

Player::Player()
{
	//Set Up Animation
	texture.loadFromFile("res/characters.png");
	setTexture(&texture);
	id = 0;
}

Player::~Player()
{
}

bool Player::updateP(float dt)
{
	
	if (!isAlive())
	{
		currentAnimation = &dead;
		setTextureRect(currentAnimation->getCurrentFrame());

	}
	currentAnimation->animate(dt);
	setTextureRect(currentAnimation->getCurrentFrame());
	

	gui.updateBullet(homingBulletCountdown, maxHomingCounter);

	if (homingBulletCountdown < maxHomingCounter)
	{
		homingBulletCountdown += dt;
	}
	else if(homingBulletCountdown> maxHomingCounter)
	{
		homingBulletCountdown = maxHomingCounter;
	}
	return isAlive();
}

void Player::handleInputMove(float dt)
{
	if (isAlive())
	{

		currentAnimation = &stop;
		setTextureRect(currentAnimation->getCurrentFrame());
		
		if (input->isKeyDown(sf::Keyboard::W)) //move up
		{
			move(0,-velocity.y*dt);
			currentAnimation = &walk;
			setTextureRect(currentAnimation->getCurrentFrame());
		}

		if (input->isKeyDown(sf::Keyboard::S)) //move down
		{
			move(0, velocity.y * dt);
			currentAnimation = &walk;
			setTextureRect(currentAnimation->getCurrentFrame());
		}

		if (input->isKeyDown(sf::Keyboard::D)) //move right
		{
			move(velocity.x*dt, 0);
			currentAnimation = &walk;
			setTextureRect(currentAnimation->getCurrentFrame());
			currentAnimation->setFlipped(false);
		}

		if (input->isKeyDown(sf::Keyboard::A)) //move right
		{
			move(-velocity.x * dt, 0);
			currentAnimation = &walk;
			setTextureRect(currentAnimation->getCurrentFrame());
			currentAnimation->setFlipped(true);
		}
	}

}

std::vector<Bullet*> Player::handleInputShoot(float dt, sf::View* view,float gameTime)
{
	if (isAlive())
	{
		std::vector<Bullet*> bulletsSpawned;
		if (input->isMouseLDown())
		{
			setTextureRect(currentAnimation->getCurrentFrame());

			if (input->getMouseX() + (view->getCenter().x - (view->getSize().x / 2)) < getPosition().x)
			{
				currentAnimation->setFlipped(true);
			}
			else
			{

				currentAnimation->setFlipped(false);
			}

			if (normalBulletCountdown >= 0.1)
			{
				sf::Vector2f calcVelocity;
				sf::Vector2f worldSpaceMouse;

				float bulletCentre = bulletManager->getBuletSize(BulletType::NORMAL) / 2;

				sf::Vector2f playerCentre;
				playerCentre.x = -getPosition().x - getSize().x / 2;
				playerCentre.y = -getPosition().y - getSize().y / 2;


				worldSpaceMouse = { input->getMouseX() + (view->getCenter().x - (view->getSize().x / 2)) ,input->getMouseY() + (view->getCenter().y - (view->getSize().y / 2)) };

				calcVelocity.x = worldSpaceMouse.x + playerCentre.x - bulletCentre;
				calcVelocity.y = worldSpaceMouse.y + playerCentre.y - bulletCentre;

				float magn = sqrt(pow((worldSpaceMouse.x + playerCentre.x - bulletCentre), 2) + pow((worldSpaceMouse.y + playerCentre.y - bulletCentre), 2));

				calcVelocity /= magn;

				int pos = bulletManager->spawn(id, BulletType::NORMAL, calcVelocity, { getPosition().x + getSize().x / 2, getPosition().y + getSize().y / 2 },gameTime);
				if (pos > -1)
				{
					Bullet newBullet = *bulletManager->getBullet(pos);
					newBullet.setVelocity(calcVelocity);
					bulletsSpawned.push_back(&newBullet);
				}
				else
				{
					Bullet newBullet = *bulletManager->getLastBullet();
					newBullet.setVelocity(calcVelocity);
					bulletsSpawned.push_back(&newBullet);
				}
				normalBulletCountdown = 0;
			}
			else
			{
				normalBulletCountdown += dt;
			}

		}

		if (input->isMouseRDown())
		{
			setTextureRect(currentAnimation->getCurrentFrame());

			if (homingBulletCountdown >= maxHomingCounter)
			{
				if (input->getMouseX() + (view->getCenter().x - (view->getSize().x / 2)) < getPosition().x)
				{
					currentAnimation->setFlipped(true);
				}
				else
				{

					currentAnimation->setFlipped(false);
				}

				int pos = bulletManager->spawn(id, BulletType::HOMING, { 1,1 }, { getPosition().x + getSize().x / 2, getPosition().y + getSize().y / 2 },gameTime);
				if (pos > -1)
				{
					homingBullet = bulletManager->getBullet(pos);
				}
				else
				{

					homingBullet = bulletManager->getLastBullet();
				}

				homingBulletCountdown = 0;
				bulletsSpawned.push_back(bulletManager->getLastBullet());
			}
		}

		return bulletsSpawned;
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

void Player::init(int playerNum, Input* in, BulletManager* theBulletManager,sf::Vector2f spawnPosition)
{
	int characterTexture = 32 * playerNum;

	id = playerNum;

	//Animation Frames
	stop.addFrame(sf::IntRect(0, characterTexture, 32, 32));

	walk.addFrame(sf::IntRect(64, characterTexture, 32, 32));
	walk.addFrame(sf::IntRect(96, characterTexture, 32, 32));
	walk.setFrameSpeed(1.f / 5.f);

	dead.addFrame(sf::IntRect(128, characterTexture, 32, 32));

	currentAnimation = &walk;
	setTextureRect(currentAnimation->getCurrentFrame());
	
	setAlive(true);
	setSize(sf::Vector2f(128, 128));
	setCollisionBox(0,0,128,128);
	setPosition(spawnPosition);
	setInput(in);
	setVelocity(700, 700);

	gui.init(id);

	gui.updateHealth(health);
	bulletManager = theBulletManager;
}

void Player::healthDecrease(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		setAlive(false);
	}
	gui.updateHealth(health);
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

	float displacementX = speedX * (currentTime -  msg0.gameTime);
	float displacementY = speedY * (currentTime - msg0.gameTime);

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

		float pdisplacementX = pspeedX * (currentTime -pmsg0.gameTime);
		float pdisplacementY = pspeedY * (currentTime -pmsg0.gameTime);

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

	
	if (playerPredictions.size() >= 2)
	{
		if (playerPredictions.back().pos != playerPredictions[playerPredictions.size() - 2].pos)
		{
			currentAnimation = &walk;

			if (playerPredictions.back().pos.x < playerPredictions[playerPredictions.size() - 2].pos.x)
			{
				currentAnimation->setFlipped(true);
			}
			else if (playerPredictions.back().pos.x > playerPredictions[playerPredictions.size() - 2].pos.x)
			{
				currentAnimation->setFlipped(false);
			}
		}
		else
		{
			currentAnimation = &stop;
		}
	}
    
	return predicted;
}

int Player::getId()
{
	return id;
}

