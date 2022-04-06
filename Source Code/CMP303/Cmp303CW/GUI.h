#pragma once

#include "SFML\Graphics.hpp"
#include <vector>

class GUI
{
public:
	GUI();

	void updatePosition(sf::Vector2f centre, sf::Vector2f size);
	void updateBullet(float bulletCounter, float maxCounter);
	void updateHealth(int health);
	void playerKilled(int index,int id);
	void init(int id);
	void render(sf::RenderWindow*);
	void addPlayerStatus(int id);
private:

	sf::Texture heartTexture;
	std::vector<sf::RectangleShape> hearts;

	sf::Texture homingBulletTexture;
	sf::RectangleShape homingBullet;

	sf::Texture aliveTextures[8];
	sf::Texture deadTextures[8];
	std::vector<sf::RectangleShape> playersStatus;

	float scale = 0;
};

