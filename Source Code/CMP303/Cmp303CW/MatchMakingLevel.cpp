#include "MatchMakingLevel.h"

MatchMakingLevel::MatchMakingLevel(sf::RenderWindow* hwnd, Input* in, int id)
{
	window = hwnd;
	input = in;
	background.setSize(sf::Vector2f(window->getSize()));
	background.setFillColor(sf::Color(155, 19, 19));
	
	playerID = id;
	playerTexture.loadFromFile("res/characters.png", { 0,(32 * playerID),32,32 });
	player.setTexture(&playerTexture);
	player.setSize(sf::Vector2f(300, 300));
	player.setPosition((window->getSize().x/2) - 150, (window->getSize().y/2) - 150);

}

bool MatchMakingLevel::handleInput()
{

	if (input->isKeyDown(sf::Keyboard::Enter))
	{
		input->setKeyUp(sf::Keyboard::Enter);
		ready = !ready;
		if (!ready)
		{
			background.setFillColor(sf::Color(155, 19, 19));
		}
		else
		{
			background.setFillColor(sf::Color(0, 153, 0));
		}
		return true;
	}
}

void MatchMakingLevel::update()
{

}

void MatchMakingLevel::render()
{
	beginDraw();
	window->draw(background);
	window->draw(player);
	endDraw();
}

bool MatchMakingLevel::getReady()
{
	return ready;
}

void MatchMakingLevel::beginDraw()
{
	window->clear(sf::Color::White);
}

void MatchMakingLevel::endDraw()
{
	window->display();
}
