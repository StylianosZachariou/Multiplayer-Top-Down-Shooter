#include "Level.h"
#include "Player.h"

Level::Level(sf::RenderWindow* hwnd, Input* in,int playerId)
{
	window = hwnd;
	input = in;

	maxPosition = { 5080,4236 };
	minPosition = { 0,0 };
	
	backgroundTexture.loadFromFile("res/background.png");
	background.setTexture(&backgroundTexture);
	background.setSize(maxPosition);
	background.setPosition(0, 0);

	//Set View
	view = window->getView();
	
	playerID = playerId;
	bulletManager.setMaxPos(maxPosition);

}

// Handle user input
void Level::handleInput(float dt,float gameTime)
{
	players[playerIndex]->handleInputMove(dt);
	std::vector<Bullet*> Bullets = players[playerIndex]->handleInputShoot(dt,&view,gameTime);

	for (int i = 0; i < Bullets.size(); i++)
	{
		ProjectilesSpawnUpdate update;
		update.velocity = Bullets[i]->getVelocity();
		update.type = Bullets[i]->getType();
		update.spawnPosition = Bullets[i]->getPosition();
		update.gameTime = gameTime;
		newBullets.push_back(update);
	}
}

//Update
void Level::update(float dt,float gameTime)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (i != playerIndex)
		{
			players[i]->setPosition(players[i]->predict(gameTime));
		}
		
		if (!players[i]->updateP(dt))
		{
			players[playerIndex]->gui.playerKilled(i, players[i]->getId());
		}
	}
	
	//Move View
	sf::Vector2f playerCentre = { players[playerIndex]->getPosition().x + (players[playerIndex]->getSize().x / 2),players[playerIndex]->getPosition().y + (players[playerIndex]->getSize().y / 2) };
	

	//FIX VIEW WITH DT
	if (playerCentre.x > view.getCenter().x+5)
	{
		view.move(background.moveView(view.getCenter().x, view.getSize().x, maxPosition.x) * dt, 0);
	}

	if (playerCentre.x < view.getCenter().x-5)
	{
		view.move(background.moveView(view.getCenter().x, view.getSize().x, minPosition.x) * dt, 0);
	}

	if (playerCentre.y > view.getCenter().y+5)
	{
		view.move(0, background.moveView(view.getCenter().y, view.getSize().y, maxPosition.y) * dt);
	}

	if (playerCentre.y < view.getCenter().y-5)
	{
		view.move(0, background.moveView(view.getCenter().y, view.getSize().y, minPosition.y)*dt);
	}

	//Bullets
	sf::Vector2f worldSpaceMouse = { input->getMouseX() + (view.getCenter().x - (view.getSize().x / 2)) ,input->getMouseY() + (view.getCenter().y - (view.getSize().y / 2)) };
	homingBulletPos = bulletManager.update(dt, worldSpaceMouse,playerID,gameTime);

	//Collisions
	collisions(dt);

	//Update Pllayers GUI
	players[playerIndex]->gui.updatePosition(view.getCenter(), view.getSize());

}

// Render level
void Level::render()
{
	beginDraw();
	window->setView(view);
	window->draw(background);
	levelTiles.render(window);
	for (int i = 0; i < players.size(); i++)
	{
		window->draw(*players[i]);
	}

	bulletManager.render(window);
	players[playerIndex]->gui.render(window);
	endDraw();
}

sf::Packet Level::packageUdp(float gameTime)
{
	sf::Packet packet;
	sf::Vector2f homingPos;
	if (players[playerIndex]->homingBullet == NULL)
	{
		homingPos = { -100,-100 };
	}
	else
	{
		homingPos = players[playerIndex]->homingBullet->getPosition();
	}
	packet << playerID << players[playerIndex]->getPosition().x << players[playerIndex]->getPosition().y << homingPos.x << homingPos.y <<gameTime;
	return packet;
}

sf::Packet Level::packageTcp()
{
	sf::Packet packet;
	packet <<playerID<< int(newBullets.size());

	for (int i = (newBullets.size() - 1); i >= 0; i--)
	{
		packet << int(newBullets[i].type) << newBullets[i].spawnPosition.x << newBullets[i].spawnPosition.y << newBullets[i].velocity.x << newBullets[i].velocity.y << newBullets[i].gameTime;
		newBullets.pop_back();
	}

	return packet;
}

void Level::applyTcpUpdates(TCPUpdates newUpdate)
{
	for (int i = 0; i < newUpdate.hitUpdate.size(); i++)
	{
		int index;
		for (int j = 0; j < players.size(); j++)
		{
			if (players[j]->getId() == newUpdate.hitUpdate[i].id)
			{
				index = j;
			}
		}

		players[index]->healthDecrease(newUpdate.hitUpdate[i].damage);
	}
	
	for (int i = 0; i < newUpdate.psUpdates.size(); i++)
	{
		if (!bulletManager.findBullet(newUpdate.psUpdates[i]))
		{
			int index;
			for (int j = 0; j < players.size(); j++)
			{
				if (players[j]->getId() == newUpdate.psUpdates[i].id)
				{
					index = j;
				}
			}

			int pos = bulletManager.spawn(newUpdate.psUpdates[i].id, newUpdate.psUpdates[i].type, newUpdate.psUpdates[i].velocity, newUpdate.psUpdates[i].spawnPosition,newUpdate.psUpdates[i].gameTime);

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

	

	for (int i = 0; i < newUpdate.oUpdate.size(); i++)
	{
		levelTiles.changeObjectStatus(newUpdate.oUpdate[i].tileMapObjectPosition, newUpdate.oUpdate[i].objectStatus);
	}
}

void Level::applyUdpUpdates(int id, UDPUpdates newUpdate)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->getId() == id)
		{
			players[i]->addMessage({ newUpdate.ppUpdate.playerPosition, newUpdate.gameTime });
			bulletManager.addMessage({ newUpdate.hpUpdate.bulletPosition,newUpdate.gameTime }, id);
		}
	}
}

// Begins rendering to the back buffer
void Level::beginDraw()
{
	window->clear(sf::Color::White);
}

// Ends rendering to the back buffer.
void Level::endDraw()
{
	window->display();
}

void Level::collisions(float dt)
{

	//COLLISIONS
	//Check collisions with tiles
	std::vector<GameObject>* world = levelTiles.tileMap.getLevel();

	for (int i = 0; i < (int)world->size(); i++)
	{
		if ((*world)[i].isCollider())
		{
			if (Collision::checkBoundingBox(players[playerIndex], &(*world)[i]))//Collision tiles with player
			{
				players[playerIndex]->collisionResponse(&(*world)[i],dt);
				
			}

			bulletManager.collisionCheck(playerID, &(*world)[i]);//Collision tiles and  bullets
		}

	}

	//Bullets
	for (int i = 0; i < players.size(); i++)
	{
		bulletManager.collisionPlayerCheck(playerID,players[i]);
	}

}

void Level::spawnPlayer(int id, sf::Vector2f spawnPosition)
{
	Player* newPlayer = new Player();
	players.push_back(newPlayer);
	players.back()->init(id, input, &bulletManager, spawnPosition);

	if (id == playerID)
	{
		playerIndex = players.size() - 1;


		if (id == 1 || id == 3 || id == 6)
		{
			view.setCenter(5080 - (view.getSize().x / 2), view.getCenter().y);
		}

		if (id == 2 || id == 3 || id == 5)
		{
			view.setCenter(view.getCenter().x, 4236 - (view.getSize().y / 2));
		}

		if (id == 4 || id == 5)
		{
			view.setCenter(2540, view.getCenter().y);
		}

		if (id == 4 || id == 5)
		{
			view.setCenter(view.getCenter().x, 2118);
		}
	}
}

void Level::setUpGui()
{
	for (int i = 0; i < players.size(); i++)
	{
		players[playerIndex]->gui.addPlayerStatus(players[i]->getId());
	}
}
