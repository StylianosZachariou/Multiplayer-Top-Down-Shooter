#include "GUI.h"

GUI::GUI()
{

}

void GUI::updatePosition(sf::Vector2f centre, sf::Vector2f size)
{
	for (int i = 0; i < hearts.size(); i++)
	{
		hearts[i].setPosition(centre.x - (size.x / 2)+(i*20), centre.y - (size.y / 2)+10 );
	}

	for (int i = 0; i < playersStatus.size(); i++)
	{
		playersStatus[i].setPosition(centre.x - (size.x / 2) + (i * 70) + 10, centre.y - (size.y / 2) + 750);
	}

	homingBullet.setPosition((centre.x - size.x / 2) + (hearts.size() * 22)+20, centre.y - (size.y / 2) + 20);
}

void GUI::updateBullet(float bulletCounter, float maxCounter)
{
	
	homingBullet.setSize(sf::Vector2f(bulletCounter / maxCounter * 40, bulletCounter / maxCounter * 40));

}

void GUI::updateHealth(int health)
{
	std::vector<sf::RectangleShape> newHearts;
	for (int i = 0; i < health; i++)
	{
		sf::RectangleShape newHeart;
		newHeart.setSize(sf::Vector2f(40,40));
		newHeart.setPosition(128*i,0);
		newHeart.setTexture(&heartTexture);
		newHearts.push_back(newHeart);
	}
	hearts = newHearts;
}

void GUI::playerKilled(int index,int id)
{
	playersStatus[index].setTexture(&deadTextures[id]);
}

void GUI::addPlayerStatus(int id)
{
	sf::RectangleShape newStatus;
	newStatus.setSize(sf::Vector2f(40, 40));
	newStatus.setTexture(&aliveTextures[id]);
	playersStatus.push_back(newStatus);

}

void GUI::init(int id)
{
	heartTexture.loadFromFile("res/heart.png");
	homingBulletTexture.loadFromFile("res/bullets.png", {32 ,(32 * id) ,32 ,32 });
	homingBullet.setPosition(0, 128);
	homingBullet.setTexture(&homingBulletTexture);
	for (int i = 0; i < 8; i++)
	{
		aliveTextures[i].loadFromFile("res/characters.png", {0,i * 32,32,32});
	}

	for (int i = 0; i < 8; i++)
	{
		deadTextures[i].loadFromFile("res/characters.png", { 128,i * 32,32,32 });
	}
}

void GUI::render(sf::RenderWindow* window)
{
	for (int i = 0; i < hearts.size(); i++)
	{
		window->draw(hearts[i]);
	}

	for (int i = 0; i < playersStatus.size(); i++)
	{
		window->draw(playersStatus[i]);
	}

	window->draw(homingBullet);
}
